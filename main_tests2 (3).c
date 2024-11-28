#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ecosys.h"

//Je les ai definies ici juste pour eviter le fameux "reference indéfinie vers ... " car pour executer ce main j'ai juste modifier mon Makefile
//J'ai declarer ces variables comme externes dans mon eco_sys.h
float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;


int main(){
	srand(time(NULL));

Animal *liste_proie = NULL; //on initialise la liste des proies
Animal *liste_pred = NULL; //on initialise la liste des predateurs

int i;
for(i=0;i<20;i++){ //generer les proies et les predateurs dans les listes precedentes
	ajouter_animal(rand() % (SIZE_X - 1), rand() % (SIZE_Y - 1), rand(), &liste_proie);
	ajouter_animal(rand() % (SIZE_X - 1), rand() % (SIZE_Y - 1), rand(), &liste_pred);

}


assert(compte_animal_rec(liste_proie)==20);
assert(compte_animal_it(liste_pred)==20);

afficher_ecosys(liste_proie,liste_pred);

//juste pour tester la fonction lire
Animal *liste_pred2, *liste_proie2;
liste_pred2 = NULL;
liste_proie2 = NULL;

ecrire_ecosys("fichier.txt", liste_pred, liste_proie);
lire_ecosys("fichier.txt", &liste_pred2, &liste_proie2);

enlever_animal(&(liste_proie),liste_proie->suivant);

printf("nombre de proies est : %d\n",compte_animal_rec(liste_proie));

liberer_liste_animaux(liste_pred);
liberer_liste_animaux(liste_proie);


return 0;
}
	

