#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define ITERATIONS 10
#define MAX_STRING_LENGTH 4

typedef enum {type1, type2, type3} Type;

typedef struct data_struct {
    int int_value;
    float float_value;
    char string_value[MAX_STRING_LENGTH+1];
    Type type_value;
} Data;

typedef struct {
    long msg_type;
    Data msg_data;
} Custom_Message;

void send(int message_id) {
    // 10 messages to send
    for (int i=0; i<ITERATIONS; i++) {
        // initialize message, Data fields
        Custom_Message message;
        message.msg_type = 1;
        message.msg_data.int_value = i;
        message.msg_data.float_value = (i * 1.0f);
        strcpy(message.msg_data.string_value, ((i % 2 == 0) ? "Even" : "Odd"));
        message.msg_data.type_value = (Type) (i % 3);

        // send message to queue for receiver, print error and exit if failed
        if(msgsnd(message_id, &message, sizeof(Data), 0) == -1) {
            perror("message could not be sent");
            exit(1);
        }
    }
}

void receive(int message_id) {
    // 10 messages to receive
    for(int i=0; i<ITERATIONS; i++) {
        // initialize message
        Custom_Message message;

        // attempt message retrieval from queue, print error and exit if failed
        if(msgrcv(message_id, &message, sizeof(Data), 1, 0) == -1) {
            perror("message could not be received");
            exit(1);
        }

        // create a string to represent Type
        char *type_str;
        switch(message.msg_data.type_value) {
            case type1: type_str = "type1"; break;
            case type2: type_str = "type2"; break;
            case type3: type_str = "type3"; break;
            default: perror("message is missing enum Type"); exit(1);
        }

        // print values of the Custom_Message's Data
        printf("int: %d, float: %f, string: %s, enum: %s\n", message.msg_data.int_value, message.msg_data.float_value, message.msg_data.string_value, type_str);
    }
}

int main() {
    // create key for message queue
    key_t key = ftok(".", 'x');
    if(key == -1) {
        perror("key could not be generated");
        exit(1);
    }

    // create message queue using key, new memory segment, and enable access permissions
    int message_id = msgget(key, IPC_CREAT | 0666);
    if(message_id == -1) {
        perror("message queue identifier could not be created");
        exit(1);
    }

    // fork process for separate sender and receiver
    pid_t pid = fork();
    if(pid == -1) {
        perror("forking failed");
        exit(1);
    }
    else if(pid == 0) {
        send(message_id);
    }
    else {
        receive(message_id);

        // remove message queue, print error and exit if failed
        if(msgctl(message_id, IPC_RMID, NULL) == -1) {
            perror("message queue identifier could not be removed");
            exit(1);
        }
    }

    return 0;
}