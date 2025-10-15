#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <stdlib.h>

void scheduler(int metadata);

int main() {

    scheduler(0);

    printf("\nend of scheduler main\n");
    
    return 0;
}

void scheduler(int metadata) {

    key_t key = ftok("f1.txt", 1);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    int shmid = shmget(key, 4096, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char* shmPointer = (char *)shmat(shmid, NULL, 0);
    if (shmPointer == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("\nReading from shared mem in scheduler:\n%s\n", shmPointer);

    char reqType[10] = "";
    strcpy(reqType, shmPointer);
    printf("\nreqType in scheduler: %s\n", reqType);

    if (strcmp(reqType, "obj. det.") == 0) {
        strcpy(shmPointer, "DLA Cluster");
        printf("\nScheduler wrote to mem: \n%s\n", shmPointer);
    } else if (strcmp(reqType, "Text gen.") == 0) {
        strcpy(shmPointer, "GPU Cluster");
        printf("\nScheduler wrote to mem: \n%s\n", shmPointer);
    }
}