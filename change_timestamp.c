#include <utime.h> 
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    struct stat st;
    if (stat(filename, &st) == -1) {
        perror("stat");
        return 1;
    }

    time_t new_time = time(NULL);

    struct utimbuf new_times;
    new_times.actime = st.st_atime;  // Keep the access time unchanged
    new_times.modtime = new_time;

    if (utime(filename, &new_times) == -1) {
        perror("utime");
        return 1;
    }

    return 0;
}
