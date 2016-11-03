#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ITER 10000000
#define NUMTHREAD 1

int kupon[6] = {1,10,23,40,42,45};
int rezultat[7] = {0,0,0,0,0,0,0};

struct lotto{
	int *vkupon;
	int *vrezultat;
	int s_pos;
	int k_pos;
};

pthread_mutex_t mtx;		/**< Mutex */

void* losuj(void *vlotto);
int main()
{
	struct lotto s_lotto[NUMTHREAD];
	int k;
	for(k = 0; k<NUMTHREAD; k++)
	{
		s_lotto[k].vkupon = kupon;
		s_lotto[k].vrezultat = rezultat;
		s_lotto[k].s_pos = k*(ITER/NUMTHREAD);
		s_lotto[k].k_pos = (k+1)*(ITER/NUMTHREAD);
	}
	pthread_t thready[NUMTHREAD];

	
	pthread_mutex_init(&mtx, NULL);	/**< Mutex initialize */

	srand(time(NULL));
	for(k = 0; k<NUMTHREAD; k++)
	{
		pthread_create(&thready[k],NULL,losuj,&s_lotto[k]);	/**< Create threads in loop */
	}

	for(k = 0; k<NUMTHREAD; k++)
		pthread_join(thready[k], NULL);		/**< Join all threads */

	printf("Ilosc powtorzen: ");
	for(k = 0; k<7;k++)
		printf("%d\t",rezultat[k]);	/**< Print rezults */

	printf("\nSuma: %d",rezultat[0]+rezultat[1]+rezultat[2]+rezultat[3]+rezultat[4]+rezultat[5]+rezultat[6]);
	printf("\n");

	pthread_mutex_destroy(&mtx);	/**< Mutex destroy */
	return 0;
}

void* losuj(void *vlotto)
{
	struct lotto *in_lotto = (struct lotto *)vlotto;
	int do_6 = 0;
	int tmp[6] = {0,0,0,0,0,0};
	int wylosowana;
	int i,j;
	int porownanie = 0;
	int ile_rownych = 0;
	int k = 0;
	int id = (int)pthread_self();
	for(k = in_lotto->s_pos; k < in_lotto->k_pos; k++)
	{
		while (do_6 < 6)
		{
			porownanie = 0;		

			wylosowana = (rand_r(&id) % 49) + 1;
			
			for(i = 0; i < 6; ++i)
			{
				if(tmp[i] == wylosowana)
				porownanie++;
			}
			if(!porownanie)
			{
				tmp[do_6] = wylosowana;
				do_6++;
			}
		}
		for(i = 0; i < 6; ++i)
		{
			for(j = 0; j <6; ++j)
			{
				if(in_lotto->vkupon[i] == tmp[j])
				{
					
					ile_rownych++;
				}
			}
		}
		pthread_mutex_lock(&mtx);
		in_lotto->vrezultat[ile_rownych] += 1;
		pthread_mutex_unlock(&mtx);
		ile_rownych = 0;
		porownanie = 0;
		do_6 = 0;
		for(i = 0; i < 6; ++i)
			tmp[i] = 0;	
	}		
}