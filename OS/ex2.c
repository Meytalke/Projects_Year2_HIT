#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define NUM_PROCESSES 5
#define MAX_NUM 100

int semid;
struct sembuf sops[1];

void print_numbers(int process_num);
void signal_all_but_self(int process_num);

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main() {
    union semun semarg;
    semid = semget(IPC_PRIVATE, NUM_PROCESSES, 0666 | IPC_CREAT);
    if (semid == -1) {
        exit(1);
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        semarg.val = (NUM_PROCESSES - 1) - i;
        if (semctl(semid, i, SETVAL, semarg) == -1) {
            exit(1);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (fork() == 0) {
            print_numbers(i);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    if (semctl(semid, 0, IPC_RMID, semarg) == -1) {
        exit(1);
    }

    return 0;
}

void print_numbers(int process_num) {
    for (int i = process_num + 1; i <= MAX_NUM; i += NUM_PROCESSES) {
        sops[0].sem_num = process_num;
        sops[0].sem_op = -(NUM_PROCESSES - 1);
        sops[0].sem_flg = 0;
        if (semop(semid, sops, 1) == -1) {
            exit(1);
        }
        printf("%d (process number %d)\n", i, process_num);
        fflush(stdout);
        signal_all_but_self(process_num);
    }
}

void signal_all_but_self(int process_num) {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (i != process_num) {
            sops[0].sem_num = i;
            sops[0].sem_op = 1;
            sops[0].sem_flg = 0;
            if (semop(semid, sops, 1) == -1) {
                exit(1);
            }
        }
    }
}
