#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // Mở thư mục hoặc tệp tại path
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // Lấy thông tin của path
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // Nếu path là một thư mục, cần duyệt nội dung bên trong
    if (st.type == T_DIR)
    {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            close(fd);
            return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        // Duyệt qua các mục trong thư mục
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue; // Bỏ qua entry rỗng

            // Bỏ qua "." và ".."
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;

            // Sao chép tên tệp vào buf để có đường dẫn đầy đủ
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            // Gọi stat() trên tệp hoặc thư mục này
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }

            // Nếu tên tệp trùng với filename, in ra đường dẫn
            if (strcmp(de.name, filename) == 0)
            {
                printf("%s\n", buf);
            }

            // Nếu là thư mục, đệ quy tìm kiếm tiếp
            if (st.type == T_DIR)
            {
                find(buf, filename);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}