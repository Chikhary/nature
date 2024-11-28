#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000
#define max_iter 200

float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

int main(void){

srand(time(NULL));
	Animal *animal1 = NULL;
	Animal *proies = NULL;
	Animal *pred = NULL;

	int i;
	int c = 0;
	int monde[SIZE_X][SIZE_Y];
	FILE *f=fopen("Evol_Pop.txt","w");
	
	//exo4
	ajouter_animal(10,17, rand(), &animal1);
	bouger_animaux(animal1);
	printf("la position x de l'animal est %d et la position y est %d\n",animal1->x,animal1->y);
	
	reproduce(&animal1,p_reproduce_proie);
	printf("le nombre d'animaux dans la liste est : %d\n",compte_animal_it(animal1));
	
	//exo7
	
	int a,b;
	for(a=0;a<SIZE_X;a++){
		for(b=0;b<SIZE_Y;b++){
			monde[a][b]=0;
		}
	}
	
	
	//exo5
	for(i=0;i<20;i++){
		ajouter_animal(rand() % (SIZE_X - 1), rand() % (SIZE_Y - 1), rand(), &proies);
		ajouter_animal(rand() % (SIZE_X - 1), rand() % (SIZE_Y - 1), rand(), &pred);
		
	}
	
	//incrementer les cases du tableau de 1
	rafraichir_monde(monde);


    while (c < max_iter && compte_animal_it(proies)!=0) {
    	fprintf(f,"%d %d %d\n",c,compte_animal_it(proies),compte_animal_it(pred));
        rafraichir_proies(&proies, monde);
        rafraichir_predateurs(&pred,&proies);
        afficher_ecosys(proies, pred); // Afficher l'ecosysteme

        // Pause pour visualiser l'etat de l'ecosysteme
        usleep(T_WAIT);
        c++;
    }
    
    
    liberer_liste_animaux(proies);
	liberer_liste_animaux(pred);
	fclose(f);
	
 
  return 0;
}


	
