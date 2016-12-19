typedef struct shmem_model {
  char destination[21]; // 20 characters + 1 '\0'
  int nbSeats;
} SHMEM;

typedef SHMEM* pShMem;
