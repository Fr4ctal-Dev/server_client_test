//
// Created by matt-vm on 11/7/19.
//

#include "msg.h"
#include <sys/shm.h>
#include <fcntl.h>

#define SHMSZ 1000
#define MKEY 1234
#define SKEY1 3333
#define SKEY2 9999

double calculateResult(double num){
    //todo calculation
    return num;

}

double parsePid(char* input){
    char *strPid = strtok(input, "\0");
    return atof(strPid);
}

double parseNum(char* input){
    char *strPid = strtok(input, ":");
    char *strNum = strtok(NULL, "\0");
    return atof(strNum);
}

int main(){

    int shmid1, shmid2;
    shmid1 = shmget(SKEY1, SHMSZ, IPC_CREAT | 0666);
    shmid2 = shmget(SKEY2, SHMSZ, IPC_CREAT | 0666);
    pid_t *pidShm = shmat(shmid1, NULL, 0);
    double *resShm = shmat(shmid2, NULL, 0);

    int msgid = msgget(MKEY, IPC_CREAT | 0666);
    message_buf rbuf;

    while (1) {
    	msgrcv(msgid, &rbuf, sizeof(rbuf), 1, 0);
    	printf("%s\n", rbuf.mtext);
        fflush(stdout);
        if (strcmp("", rbuf.mtext) != 0){
            double clientNum = parseNum(rbuf.mtext);
            pid_t clientPid = parsePid(rbuf.mtext);

            pidShm[0] = clientPid;
            resShm[0] = calculateResult(clientNum);

            printf("Wrote to shm log at adresses %p, %p\n", &pidShm, &resShm);

            shmctl( shmid1, IPC_RMID, NULL);
            shmctl( shmid2, IPC_RMID, NULL);

            break;
        }
        usleep(1500);
    }

    return 0;
}