#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //número de usuários

#define CARTAS 20 //quantidade de cartas na mochila

int qnt_cartas = 0; // contador de cartas

sem_t sem_voar;   // semaforo pra liberar o voo
sem_t sem_cartas; // semafaro de cartas
sem_t sem_pombo;  // semafaro do pombo (lock)

void *f_usuario(void *arg);
void *f_pombo(void *arg);

int main(int argc, char **argv)
{
    int i;

    pthread_t usuario[N];
    int *id;

    // inicializando os semaforos
    sem_init(&sem_cartas, 0, CARTAS);
    sem_init(&sem_voar, 0, 0);
    sem_init(&sem_pombo, 0, 1);

    for (i = 0; i < N; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&(usuario[i]), NULL, f_usuario, (void *)(id));
    }
    pthread_t pombo;
    id = (int *)malloc(sizeof(int));
    *id = 0;
    pthread_create(&(pombo), NULL, f_pombo, (void *)(id));

    pthread_join(pombo, NULL);
}

void *f_pombo(void *arg)
{

    while (1)
    {
        //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        sem_wait(&sem_voar);
        sem_wait(&sem_pombo);
        //Leva as cartas para B e volta para A
        printf("O pombo partiu, partiu e nunca mais voltou!\n");
        sleep(1);

        printf("Pombo chegou ao local B e entregou as cartas!\n");

        sleep(1);

        //Acorda os usuarios
        for (int i = 0; i < CARTAS; i++)
        {
            sem_post(&sem_cartas);
            qnt_cartas--;
        }

        printf("Ele voltou sim, turu!\n\n");

        sleep(1);

        sem_post(&sem_pombo);
    }
}

void *f_usuario(void *arg)
{

    int id = *((int *)arg);

    while (1)
    {
        sem_wait(&sem_pombo); // pega o lock do pombo

            sem_wait(&sem_cartas); // pega permissao de 1 carta
            qnt_cartas++;          // incrementa o contador de cartas
            printf("O usuario %d escreveu uma carta       Qnt de cartas na mochila: %d\n\n", id, qnt_cartas);

            //se a qnt de cartas for 20, o pombo vai entregar
            if (qnt_cartas == CARTAS)
                sem_post(&sem_voar); // da permissao para pombo voar

        sem_post(&sem_pombo); //libera o lock do pombo
        sleep(1);
    }
}
