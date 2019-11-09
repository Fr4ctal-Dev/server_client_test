//
// Created by matt-vm on 10/14/19.
//



#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>


#define KEY1 2169
#define KEY2 6921
#define MSGSZ 1024

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;