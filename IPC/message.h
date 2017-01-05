//Structure d'un message
typedef struct msgContent_model {
  char dest[20];
  char seats[2];
  int pid;
} MSGCONTENT;

typedef struct msgbuf{
  long messageType;
  MSGCONTENT mess;
} MSG;
