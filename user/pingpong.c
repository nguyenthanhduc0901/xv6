#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    char byte = 'x';

    // Tạo hai pipe
    pipe(p1); // Cha gửi tới con
    pipe(p2); // Con gửi tới cha

    if (fork() == 0) { // Tiến trình con
        // Đọc từ p1 (nhận từ cha)
        read(p1[0], &byte, 1);
        printf("%d: received ping\n", getpid());

        // Ghi vào p2 (gửi tới cha)
        write(p2[1], &byte, 1);
        exit(0);
    } else { // Tiến trình cha
        // Ghi vào p1 (gửi tới con)
        write(p1[1], &byte, 1);

        // Đọc từ p2 (nhận từ con)
        read(p2[0], &byte, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}
