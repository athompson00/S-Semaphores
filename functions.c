#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>

#define SHM_KEY 10282
#define SEM_KEY 11280
#define SEG_SIZE 200

union semun {
	int value;
	struct semid_ds *buffer;
	unsigned short *array;
} su;

void getcurrent() {
	FILE * f = fopen("story.txt", "r");
  	printf("So far:\n");
  	char line[200];
  	while(fgets(line, 200, f)) {
    		printf("%s", line);
  	}
  	fclose(f);
}

int main(int argc, char *argv[]) {
  	int s;
  	int gmem;
  	char *add;
  	int f;

  	if (argc > 1 && !strcmp(argv[1], "-c")) {

    		s = semget(SEM_KEY, 1, IPC_CREAT | 0644);
    		su.value = 1;
      	semctl(s, 0, SETVAL, su);

    		printf("Semaphore has been created\n");
    		gmem = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
    		add = shmat(gmem, 0, 0);
    		if (add){
      		strcpy(add, "");
      		shmdt(add);
    		}

    		printf("Shared memory has been created\n");
    		f = open("story.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
      	printf("File has been created\n");
      	close(f);
  	}

  	if (argc > 1 && !strcmp(argv[1], "-v")) {
    		getcurrent();
  	}

  	if (argc > 1 && !strcmp(argv[1], "-r")) {

    		printf("Accessing\n");

    		s = semget(SEM_KEY, 1, 0);
    		struct sembuf buff;
    		buff.sem_num = 0;
    		buff.sem_op = -1;
    		semop(s, &buff, 1);
    		getcurrent();

    		gmem = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
    		shmctl(gmem, IPC_RMID, 0);

		printf("-----\n");

    		printf("Shared memory has been removed\n");
    		remove("story.txt");
    		printf("File has been removed\n");
    		semctl(s, IPC_RMID, 0);
    		printf("Semaphore has been removed\n");
  	}

  	return 0;
}
