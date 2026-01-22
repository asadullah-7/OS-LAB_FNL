#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int p1[2], p2[2], p3[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);

    // ls
    if (fork() == 0) {
        int err = open("error.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        dup2(err, STDERR_FILENO);
        dup2(p1[1], STDOUT_FILENO);

        close(err);
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        execlp("ls", "ls", "/", "-lR", NULL);
        exit(1);
    }

    // grep
    if (fork() == 0) {
        dup2(p1[0], STDIN_FILENO);
        dup2(p2[1], STDOUT_FILENO);

        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        execlp("grep", "grep", "Nov", NULL);
        exit(1);
    }

    // sort
    if (fork() == 0) {
        dup2(p2[0], STDIN_FILENO);
        dup2(p3[1], STDOUT_FILENO);

        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        execlp("sort", "sort", NULL);
        exit(1);
    }

    // less
    dup2(p3[0], STDIN_FILENO);

    close(p1[0]); close(p1[1]);
    close(p2[0]); close(p2[1]);
    close(p3[0]); close(p3[1]);

    execlp("less", "less", NULL);
    return 0;
}
