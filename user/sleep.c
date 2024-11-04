#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }

  int ticks = atoi(argv[1]); // Chuyển đổi chuỗi thành số nguyên
  sleep(ticks); // Gọi hàm sleep trong xv6
  exit(0);
}
