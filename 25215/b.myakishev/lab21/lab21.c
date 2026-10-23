#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t count = 0;
volatile sig_atomic_t quit = 0;

void handle_sigint(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\a", 1);
    count++;
}

void handle_sigquit(int sig) {
    (void)sig;
    quit = 1;
}

int main(void) {
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);

    while (!quit) {
        pause();
    }

    printf("\nКоличество звуковых сигналов: %d\n", (int)count);

    return 0;
}