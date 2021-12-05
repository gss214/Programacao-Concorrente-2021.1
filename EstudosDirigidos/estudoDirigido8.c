#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define ESQ(id) (id)
#define DIR(id)	(id+1)%N

sem_t s[N];
pthread_mutex_t lock_filosofos = PTHREAD_MUTEX_INITIALIZER; //lock 


void *filosofos (void *arg);
void pega_talher (int n);
void devolve_talher (int n);
void tentar_comer(int n);

int estado[N];

#define PENSAR 0
#define FOME 1
#define COMER 2

int main () {
	int i;
	int *id;
	//semaforo 
	for (i=0; i<N; i++) {
		sem_init(&s[i], 0, 1);
	}
	pthread_t r[N]; 

	//criacao das threads de filosofos
       for (i = 0; i < N ; i++) {
	     id = (int *) malloc(sizeof(int));
	     *id = i;
	     pthread_create(&r[i], NULL, filosofos, (void*) (id));
	}

        pthread_join(r[0],NULL);
	return 0;
}


void *filosofos (void *arg) {
	int n = *((int *) arg);
	while(1) {
		//pensar
		printf("Filosofo %d pensando ...\n", n+1);
		sleep(3);
		pega_talher(n);
		//comer
		printf("\tFilosofo %d comendo ...\n", n+1);
		sleep(3);
        printf("\tFilosofo %d acabou de comer ...\n", n+1);
		devolve_talher(n);
	} 
}

void pega_talher (int n) {
    pthread_mutex_lock(&lock_filosofos); //pega lock para mudar o estado do filosofo n para fome
        estado[n] = FOME;
       // printf("Filosofo %d está faminto\n", n+1);
        tentar_comer(n); //filosofo n vai tentar comer
    pthread_mutex_unlock(&lock_filosofos); //libera o lock
    sem_wait(&s[n]); //libera o talher  
}

void devolve_talher (int n) {
    pthread_mutex_lock(&lock_filosofos); //pega lock para mudar o estado do filosofo n para pensar
        estado[n] = PENSAR;
        //printf("Filosofo %d deixou os garfos %d e %d\n", n+1, s[ESQ(n)], s[DIR(n)]);

        //os vizinhos vao tentar comer
        tentar_comer(ESQ(n));
        tentar_comer(DIR(n));
    pthread_mutex_unlock(&lock_filosofos); //libera o lock
}

void tentar_comer(int n){
    // filosofo n come se o estado da esquerda e da direita eh diferente de comer 
    if(estado[n] == FOME && estado[ESQ(n)] != COMER && estado[DIR(n)] != COMER) {
		estado[n] = COMER;
        //printf("Filosofo %d esta comendo\n", n+1);
		sem_post(&s[n]);	
	}
}