#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //número de usuários

#define CARTAS 20 //quantidade de cartas na mochila

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond_pombo = PTHREAD_COND_INITIALIZER; //variavel de condicao pro pombo
pthread_cond_t cond_usuario = PTHREAD_COND_INITIALIZER; //variavel de condicao pro usuario

int qnt_cartas = 0; //contador

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
    int i;

    pthread_t usuario[N];
    int *id;
    for(i = 0; i < N; i++){
         id = (int *) malloc(sizeof(int));
          *id = i;
	pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
    }
     pthread_t pombo;
     id = (int *) malloc(sizeof(int));
     *id = 0;
     pthread_create(&(pombo),NULL,f_pombo, (void*) (id));

     pthread_join(pombo,NULL);
   
}


void * f_pombo(void *arg){
  
    while(1){
        //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        pthread_mutex_lock(&lock);    

            // o pombo espera as cartas, enquanto nao for 20 ele espera
            while (qnt_cartas < CARTAS)
                pthread_cond_wait(&cond_pombo, &lock);

            //Leva as cartas para B e volta para A
            printf("O pombo partiu, partiu e nunca mais voltou!\n");
            sleep(1);

            printf("Pombo chegou ao local B e entregou as cartas!\n");
            qnt_cartas = 0;
            sleep(1);

            printf("Ele voltou sim, turu!\n\n");

            sleep(1);
            
            //Acorda os usuarios   
            pthread_cond_broadcast(&cond_usuario);

        pthread_mutex_unlock(&lock);    
    }
}

void * f_usuario(void *arg){
    
    int id = *((int *) arg);

    while(1){
        //Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme	
        pthread_mutex_lock(&lock);    
            // verifica se a mochila ta cheia
            while (qnt_cartas == CARTAS)
                pthread_cond_wait(&cond_usuario, &lock);

            //Posta sua carta na mochila do pombo
            qnt_cartas++;
    	    printf("O usuario %d escreveu uma carta       Qnt de cartas na mochila: %d\n\n", id, qnt_cartas);
           
            //Caso a mochila fique cheia, acorda o pombo
            if (qnt_cartas == CARTAS)
                pthread_cond_signal(&cond_pombo); 

        pthread_mutex_unlock(&lock);    
        sleep(1);
    }
}
