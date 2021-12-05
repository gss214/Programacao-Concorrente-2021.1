#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXSIZE 10000 /* maximum matrix size */

pthread_barrier_t barrier;
int size;

int matrix1[MAXSIZE][MAXSIZE];

void *Worker(void *);

/* para popular a matriz passe um numero pela linha de
comando, 

ex: ./a 2 9 

onde o 2 eh o tamanho da matriz e o 9 vai ser o elemento a
ser populado na matriz

*/

int main(int argc, char *argv[])
{
    int i, j, popula_matriz;
    size = atoi(argv[1]);
    popula_matriz = atoi(argv[2]);
    printf("arg2 = %d", popula_matriz);
    if (size > MAXSIZE)
    {
        printf("Tamanho muito grande!\n");
        return 0;
    }

    pthread_t workerid[size];

    pthread_barrier_init(&barrier, NULL, size);

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            matrix1[i][j] = popula_matriz;

    int *id;
    for (i = 0; i < size; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&workerid[i], NULL, Worker, (void *)(id));
    }

    for (i = 0; i < size; i++)
    {
        if (pthread_join(workerid[i], NULL))
        {
            printf("\n ERROR joining thread");
            exit(1);
        }
    }
    printf("Bye!\n");
}

void *Worker(void *arg)
{
    int myid = *(int *)(arg);
    int j, k;
    int sum = 0;

    int self = pthread_self();

    printf("worker %d (pthread id %d) has started\n", myid, self);

    for (j = 0; j < size; j++)
    {
        for (k = 0; k < size; k++)
        {
            sum += matrix1[j][k];
        }
    }

    pthread_barrier_wait(&barrier);

    if (myid == 0)
    {
        printf("\n ");
        for (j = 0; j < size; j++)
        {
            for (k = 0; k < size; k++)
            {
                printf("%d ", matrix1[j][k]);
            }
            printf("\n ");
        }
        printf("A soma da matriz acima é: %d\n", sum);
    }
}
