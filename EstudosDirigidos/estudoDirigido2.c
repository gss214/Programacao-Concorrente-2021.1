#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define TRUE 1

#define NE 3  //numero de escritores
#define NL 10 //numero de leitores

pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;	// lock que controla o bd
pthread_mutex_t lock_nl = PTHREAD_MUTEX_INITIALIZER;	// lock do contador
pthread_mutex_t lock_fluxo = PTHREAD_MUTEX_INITIALIZER; // lock do fluxo de leitores

int num_leitores = 0;

void *reader(void *arg);
void *writer(void *arg);
void read_data_base();
void use_data_read();
void think_up_data();
void write_data_base();

int main()
{
	pthread_t r[NL], w[NE];
	int i;
	int *id;
	/* criando leitores */
	for (i = 0; i < NL; i++)
	{
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&r[i], NULL, reader, (void *)(id));
	}
	/* criando escritores */
	for (i = 0; i < NE; i++)
	{
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&w[i], NULL, writer, (void *)(id));
	}
	pthread_join(r[0], NULL);
	return 0;
}

void *reader(void *arg)
{
	int i = *((int *)arg);
	while (TRUE)
	{ /* repere para sempre */
		/*
            para acessar o BD, os leitor pega o lock do fluxo e verifica se é o primeiro leitor,
			se for ele adiciona 1 no contador e pega o lock do bd, assim os proximos leitores
			não precisam passar por essa verificação
        */
        pthread_mutex_lock(&lock_fluxo);
			pthread_mutex_lock(&lock_nl);
				num_leitores++;
				if (num_leitores == 1)
					pthread_mutex_lock(&lock_bd);
			pthread_mutex_unlock(&lock_nl);
        pthread_mutex_unlock(&lock_fluxo);
		
		read_data_base(i); /* acesso aos dados */

		/*
			depois que o leitor leu do bd, é decrementado 1 no numero de leitores, se for
			o ultimo leitor, ele libera o lock do bd.
        */

		pthread_mutex_lock(&lock_nl);
			num_leitores--;
			if (num_leitores == 0)
				pthread_mutex_unlock(&lock_bd);
		pthread_mutex_unlock(&lock_nl);
		use_data_read(i); /* região não crítica */
	}
	pthread_exit(0);
}

void *writer(void *arg)
{
	int i = *((int *)arg);
	while (TRUE)
	{					  /* repete para sempre */
		think_up_data(i); /* região não crítica */

		/*
            para garantir o acesso dos escritores, se algum escritor tiver esperando
			ele para o fluxo dos leitores e escreve no db
        */

		pthread_mutex_lock(&lock_fluxo);
			printf("O escritor %d está em espera, fluxo de leitores parado\n", i);
			pthread_mutex_lock(&lock_bd);
				write_data_base(i); /* atualiza os dados */
			pthread_mutex_unlock(&lock_bd);
			printf("O escritor %d terminou de escrever, fluxo de leitores iniciado\n", i);
		pthread_mutex_unlock(&lock_fluxo);
	}
	pthread_exit(0);
}

void read_data_base(int i)
{
	printf("Leitor %d está lendo os dados! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5 + 5);
}

void use_data_read(int i)
{
	printf("Leitor %d está usando os dados lidos! Número de leitores: %d\n", i, num_leitores);
	sleep(rand() % 5 + 5);
}

void think_up_data(int i)
{
	printf("Escritor %d está pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i)
{
	printf("Escritor %d está escrevendo os dados!\n", i);
	sleep(rand() % 5 + 10);
}