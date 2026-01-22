#include <stdio.h>      // printf, perror ke liye
#include <unistd.h>     // fork, pipe, dup2, execlp ke liye
#include <fcntl.h>      // open flags ke liye
#include <stdlib.h>     // exit ke liye

int main() {

    // 3 pipes banaye gaye:
    // p1: ls  -> grep
    // p2: grep -> sort
    // p3: sort -> less
    int p1[2], p2[2], p3[2];

    pipe(p1);   // pehla pipe
    pipe(p2);   // doosra pipe
    pipe(p3);   // teesra pipe

    // ===================== ls PROCESS =====================
    if (fork() == 0) {  
        // child process bana jo ls command chalay ga

        // error.txt file open ki (errors store karne ke liye)
        int err = open("error.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

        // STDERR (2) ko error.txt se jor diya
        // ab ls ki errors file me jayengi
        dup2(err, STDERR_FILENO);

        // STDOUT (1) ko p1 ke write end se jor diya
        // ls ka output ab pipe me jayega
        dup2(p1[1], STDOUT_FILENO);

        // file descriptor band kar diya (dup2 ke baad zarurat nahi)
        close(err);

        // saare pipes ke unused ends band
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        // ls / -lR command execute
        execlp("ls", "ls", "/", "-lR", NULL);

        // agar execlp fail ho jaye
        exit(1);
    }

    // ===================== grep PROCESS =====================
    if (fork() == 0) {
        // naya child jo grep chalay ga

        // STDIN ko p1 ke read end se jor diya
        // grep input ls se le raha hai
        dup2(p1[0], STDIN_FILENO);

        // STDOUT ko p2 ke write end se jor diya
        // grep ka output sort ko jayega
        dup2(p2[1], STDOUT_FILENO);

        // saare pipes band jo is process ko nahi chahiye
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        // grep "Nov" command run
        execlp("grep", "grep", "Nov", NULL);

        exit(1);
    }

    // ===================== sort PROCESS =====================
    if (fork() == 0) {
        // child process jo sort chalay ga

        // STDIN ko p2 ke read end se jor diya
        // sort input grep se le raha hai
        dup2(p2[0], STDIN_FILENO);

        // STDOUT ko p3 ke write end se jor diya
        // sort ka output less ko jayega
        dup2(p3[1], STDOUT_FILENO);

        // saare unused pipe ends band
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        // sort command execute
        execlp("sort", "sort", NULL);

        exit(1);
    }

    // ===================== less PROCESS (parent) =====================

    // STDIN ko p3 ke read end se jor diya
    // less input sort se le raha hai
    dup2(p3[0], STDIN_FILENO);

    // parent me bhi saare pipes band
    close(p1[0]); close(p1[1]);
    close(p2[0]); close(p2[1]);
    close(p3[0]); close(p3[1]);

    // less command run
    execlp("less", "less", NULL);

    return 0;
}
