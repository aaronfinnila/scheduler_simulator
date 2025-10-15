#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

void inferenceReqHandler(char requestType[10]);

int main() {

    char requestType[10] = "";

    printf("\nGive request type (obj. det. or Text gen.)\n");
    fflush(stdout);
    fgets(requestType, sizeof(requestType), stdin);
    requestType[strcspn(requestType, "\n")] = '\0';

    inferenceReqHandler(requestType);

    printf("\nend of infreqhandler main\n");
    
    return 0;
}

void inferenceReqHandler(char request[10]) {

    char schedulingDecision[15] = "";

    printf("\nRequest: %s", request);

    key_t key = ftok("f1.txt", 1);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    int shmid = shmget(key, 4096, 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char* shmPointer = (char *)shmat(shmid, NULL, 0);
    if (shmPointer == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    if (strcmp(request, "obj. det.") != 0
    && strcmp(request, "Text gen.") != 0) {

    } else {
        strcpy(shmPointer, request);
    }

    if (strcmp(shmPointer, "DLA Cluster") == 0
    || strcmp(shmPointer, "GPU Cluster") == 0) {
        int id = fork();
        pid_t pid = getpid();
        if (id < 0) {
            perror("fork failed");
            exit(1);
        } else if (id == 0) {
            printf("\nthis is child, pid: %d\n", pid);
            // strcpy(schedulingDecision, shmPointer);
            printf("\nscheduling decision: %s\n", shmPointer);
        } else {
            printf("\nthis is parent, pid: %d\n", pid);
        }

    } else if (strcmp(shmPointer, "GPU Cluster") == 0) {

        int id = fork();
        pid_t pid = getpid();
        if (id < 0) {
            perror("fork failed");
            exit(1);
        } else if (id == 0) {
            printf("\nthis is child, pid: %d\n", pid);
        } else {
            printf("\nthis is parent, pid: %d\npid of child: %d\n", pid, id);
        }
    }
}