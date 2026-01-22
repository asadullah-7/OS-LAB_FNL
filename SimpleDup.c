#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int fd = open("input.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    dup2(fd, STDOUT_FILENO); // redirect stdout to file
    close(fd);

    execlp("ls", "ls", "/", "-l", NULL);

    perror("execlp");
    return 1;
}
