//
// Created by matt-vm on 11/7/19.
//

#include <sys/shm.h>
#include <sys/signal.h>
#include "msg.h"

#define SHMSZ 1000
#define MKEY 1234
#define SKEY1 3333
#define SKEY2 9999
#define TIMEOUT 10

#define MAXUSERS 10

double getResult (){
    
    int shmid1, shmid2;

    shmid1 = shmget(SKEY1, SHMSZ, 0666);
    shmid2 = shmget(SKEY2, SHMSZ, 0666);

    pid_t *pidShm = shmat(shmid1, NULL, 0);
    double *resShm = shmat(shmid2, NULL, 0);

    for (int i = 0; i < MAXUSERS; ++i) {
        if (pidShm[i] == getpid()){
            return resShm[i];
        }
    }

    return -1;
}

void alrmHandler(){
    printf("Timeout: no entry found for your PID within specified timelimit %d\n", TIMEOUT);
    exit(EXIT_FAILURE);
}

void resetTimeout(){
    alarm(TIMEOUT);
}

int main(){

    signal(SIGALRM, &alrmHandler);
    signal(SIGUSR1, &resetTimeout);

    message_buf sbuf;
    double num;
    sbuf.mtype = 1;
    int msgid = msgget(MKEY, 0666);
    fflush(stdin);
    scanf("%lf", &num);
    fflush(stdin);
    sprintf(sbuf.mtext, "%d:%.3lf", getpid(), num);
    if (msgid == -1) {
        perror("MSGID error");
        exit(EXIT_FAILURE);
    }
    msgsnd(msgid, &sbuf, sizeof(sbuf), 0);

    alarm(TIMEOUT);


    while (1){
        if (getResult() != -1){
            printf("Result found in shm %.3lf", getResult());
            resetTimeout();
            break;
        }
    }
    
    
    


    return 0;
}

