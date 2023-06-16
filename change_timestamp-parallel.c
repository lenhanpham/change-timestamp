#include <pthread.h>
#include <utime.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 4

// Thread data structure
typedef struct {
    const char **filenames;
    size_t num_files;
} thread_data_t;

// Thread function
void *update_timestamps(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    const char **filenames = data->filenames;
    size_t num_files = data->num_files;

    // Update the timestamp of each file
    for (size_t i = 0; i < num_files; i++) {
        const char *filename = filenames[i];

        struct stat st;
        if (stat(filename, &st) == -1) {
            perror("stat");
            continue;
        }

        time_t new_time = time(NULL);

        struct utimbuf new_times;
        new_times.actime = st.st_atime;  // Keep the access time unchanged
        new_times.modtime = new_time;

        if (utime(filename, &new_times) == -1) {
            perror("utime");
            continue;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>...\n", argv[0]);
        return 1;
    }

    size_t num_files = argc - 1;
    const char **filenames = (const char **)&argv[1];

    // Create threads
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    size_t chunk_size = num_files / NUM_THREADS;

    for (size_t i = 0; i < NUM_THREADS; i++) {
        thread_data[i].filenames = &filenames[i * chunk_size];
        thread_data[i].num_files = chunk_size;
        if (i == NUM_THREADS - 1) {
            // Last chunk may be larger if num_files is not divisible by NUM_THREADS
            thread_data[i].num_files += num_files % NUM_THREADS;
        }
        pthread_create(&threads[i], NULL, update_timestamps, &thread_data[i]);
    }

    // Wait for threads to finish
    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
