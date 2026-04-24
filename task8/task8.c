#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static volatile int g_i = 0;
static volatile int g_j = 0;
static volatile int g_k = 0;
static struct termios old_termios;

void handler_signal(int sig);
void handler_sigaction(int sig);
int rand_int(void);
void fill_matrix(int size, int matrix[size][size]);
void init_matrix(int size, int matrix[size][size]);
void mult_matrices(int size, int m1[size][size], int m2[size][size], int res[size][size]);
void disable_echo(void);
void enable_echo(void);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s (--signal || --sigaction)\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    tcgetattr(STDIN_FILENO, &old_termios);

    if (strcmp(argv[1], "--signal") == 0) {
        disable_echo();
        signal(SIGINT, handler_signal);
    } else if (strcmp(argv[1], "--sigaction") == 0) {
        disable_echo();
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = handler_sigaction;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESETHAND;
        sigaction(SIGINT, &sa, NULL);
    } else {
        fprintf(stderr, "unknown argument\n");
        return 1;
    }

    const int size = 100;
    int m1[size][size];
    int m2[size][size];
    int res[size][size];

    fill_matrix(size, m1);
    fill_matrix(size, m2);
    init_matrix(size, res);

    mult_matrices(size, m1, m2, res);

    enable_echo();
    return 0;
}

void disable_echo(void) {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void enable_echo(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

void handler_signal(int sig) {
    dprintf(STDOUT_FILENO, "%d %d\n%d %d\n%d %d\n", g_i, g_k, g_k, g_j, g_i, g_j);
    enable_echo();
    signal(SIGINT, SIG_DFL);
}

void handler_sigaction(int sig) {
    dprintf(STDOUT_FILENO, "%d %d\n%d %d\n%d %d\n", g_i, g_k, g_k, g_j, g_i, g_j);
    enable_echo();
}

int rand_int(void) {
    return rand() % 10;
}

void fill_matrix(int size, int matrix[size][size]) {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            matrix[r][c] = rand_int();
        }
    }
}

void init_matrix(int size, int matrix[size][size]) {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            matrix[r][c] = 0;
        }
    }
}

void mult_matrices(int size, int m1[size][size], int m2[size][size], int res[size][size]) {
    for (g_i = 0; g_i < size; g_i++) {
        for (g_j = 0; g_j < size; g_j++) {
            for (g_k = 0; g_k < size; g_k++) {
                res[g_i][g_j] += m1[g_i][g_k] * m2[g_k][g_j];
                usleep(500000);
            }
        }
    }
}