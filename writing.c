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

int main (int argc, char *argv[]) {
	int f;
	char userin[200];
	char *add;
	int s;
	int gmem;

	s = semget(SEM_KEY, 1, 0);
	struct sembuf buffer;
	buffer.sem_num = 0;
	buffer.sem_op = -1;

	printf("Trying\n");

	semop(s, &buffer, 1);
	gmem = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);

  f = open("story.txt", O_WRONLY | O_APPEND);
	add = shmat(gmem, 0, 0);

  printf("last addition: %s\n", add);
	printf("your addition: ");

  fgets(userin, 200, stdin);

  strcpy(add, userin);

  write(f, userin, strlen(userin));
  	shmdt(add);
  	buffer.sem_op = 1;
  	semop(s, &buffer, 1);

  close(f);
  	return 0;
}
