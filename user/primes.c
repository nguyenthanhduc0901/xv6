#include "kernel/types.h"
#include "user/user.h"

void primes(int left_pipe) __attribute__((noreturn));

void primes(int left_pipe) {
    int prime;
    if (read(left_pipe, &prime, sizeof(prime)) == 0) {
        close(left_pipe);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p[2];
    pipe(p);

    if (fork() == 0) { // Tiến trình con
        close(p[1]);
        close(left_pipe);
        primes(p[0]);
    } else {
        int num;
        close(p[0]);
        while (read(left_pipe, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }
        close(p[1]);
        close(left_pipe);
        wait(0); // Đảm bảo tiến trình con hoàn tất
    }
    exit(0);
}

int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);
        primes(p[0]);
    } else {
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0); // Chờ tiến trình con hoàn tất
    }
    exit(0);
}
