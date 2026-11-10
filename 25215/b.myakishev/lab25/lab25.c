#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); return 1; }

    pid_t pid = fork();
    if (pid == -1) { perror("fork"); return 1; }

    if (pid == 0) {
        close(fd[1]);
        char buf[1024];
        ssize_t n;
        while ((n = read(fd[0], buf, sizeof buf)) > 0) {
            for (ssize_t i = 0; i < n; i++)
                buf[i] = (char)toupper((unsigned char)buf[i]);
            if (write(STDOUT_FILENO, buf, n) != n) {
                perror("write stdout"); close(fd[0]); exit(1);
            }
        }
        if (n == -1) { perror("read"); close(fd[0]); exit(1); }
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    const char *msg = "Hello, UNIX SVR4! This is a MiXeD cAsE MeSsAgE.\n";
    size_t len = strlen(msg);
    if (write(fd[1], msg, len) != (ssize_t)len) {
        perror("write pipe");
        close(fd[1]);
        wait(NULL);
        return 1;
    }
    close(fd[1]);
    wait(NULL);
    return 0;
}