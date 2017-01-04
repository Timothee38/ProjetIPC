typedef struct shmem_model {
  char destination[21]; // 20 characters + 1 '\0'
  int nbSeats;
  int pid;
} SHMEM;

typedef SHMEM* pShMem;

