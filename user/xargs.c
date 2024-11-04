#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char *args[MAXARG];
    int i;

    // Kiểm tra nếu không có lệnh nào được cung cấp
    if (argc < 2) {
        fprintf(2, "Usage: xargs [-n 1] <command> [args...]\n");
        exit(1);
    }

    // Kiểm tra tùy chọn -n 1 (không bắt buộc trong bài này nhưng đảm bảo truyền một đối số mỗi lần)
    if (strcmp(argv[1], "-n") == 0 && atoi(argv[2]) == 1) {
        i = 3; // Bỏ qua tùy chọn -n 1
    } else {
        i = 1; // Bắt đầu từ đối số đầu tiên của lệnh
    }

    // Sao chép các đối số ban đầu từ dòng lệnh xargs vào args
    int base_argc = 0;
    for (; i < argc && base_argc < MAXARG - 1; i++) {
        args[base_argc++] = argv[i];
    }

    // Buffer để đọc đầu vào chuẩn
    char buf[512];
    int n;

    // Đọc và xử lý từng dòng từ đầu vào chuẩn
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        int line_start = 0;
        for (int j = 0; j < n; j++) {
            if (buf[j] == '\n') {
                // Kết thúc của dòng hiện tại
                buf[j] = 0;

                // Gán dòng hiện tại làm đối số
                args[base_argc] = &buf[line_start];
                args[base_argc + 1] = 0;

                // Thực thi lệnh với một đối số duy nhất
                if (fork() == 0) {
                    exec(args[0], args);
                    // In lỗi nếu exec thất bại
                    fprintf(2, "exec %s failed\n", args[0]);
                    exit(1);
                } else {
                    wait(0); // Chờ tiến trình con hoàn tất
                }

                // Đặt lại cho đối số tiếp theo
                line_start = j + 1;
            }
        }
    }

    // Xử lý lỗi đọc
    if (n < 0) {
        fprintf(2, "xargs: read error\n");
        exit(1);
    }

    exit(0);
}
