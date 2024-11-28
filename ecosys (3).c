#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include "ecosys.h"
#include <string.h>



/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  Animal* na = creer_animal(x,y,energie);
  *liste_animal = ajouter_en_tete_animal(*liste_animal,na);
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    if (liste == NULL || *liste == NULL || animal == NULL) return;
    Animal **p = liste;
    while (*p != NULL) {
        if (*p == animal) {
            Animal *temp = *p;
            *p = (*p)->suivant; // Met a jour le pointeur vers le suivant
            free(temp); 
            return; // Sort de la fonction apres la suppression
        }
        p = &((*p)->suivant);
    }
}


/* A Faire. Part 1, exercice 6, question 7 */
void liberer_liste_animaux(Animal *liste) {
   Animal *p = liste;
   while(p!=NULL){
   		Animal *tmp=p;
		p=p->suivant;
		free(tmp); 
   }

}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
    if (liste_proie == NULL && liste_predateur == NULL) {
        printf("Aucun animal dans l'ecosysteme.\n");
        return; // Sortir si les deux listes sont vides
    }

    unsigned int i, j;
    char ecosys[SIZE_Y][SIZE_X]; // Notez l'ordre des dimensions
    Animal *pa = NULL;

    /* on initialise le tableau */
    for (i = 0; i < SIZE_Y; ++i) {
        for (j = 0; j < SIZE_X; ++j) {
            ecosys[i][j] = ' '; // Initialiser le tableau avec des espaces vides
        }
    }

    /* on ajoute les proies */
    pa = liste_proie;
    while (pa) {
        if (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) { // Verification des limites
            ecosys[pa->y][pa->x] = '*'; // Remplir la position avec un symbole pour les proies
        }
        pa = pa->suivant;
    }

    /* on ajoute les predateurs */
    pa = liste_predateur;
    while (pa) {
        if (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) { // Verification des limites
            if (ecosys[pa->y][pa->x] == '*') { // Proies aussi presentes
                ecosys[pa->y][pa->x] = '@'; // Remplir avec un symbole pour les predateurs
            } else {
                ecosys[pa->y][pa->x] = 'O'; // Remplir avec un symbole pour les predateurs
            }
        }
        pa = pa->suivant;
    }

    /* on affiche le tableau */
    printf("+");
    for (j = 0; j < SIZE_X; ++j) {
        printf("-");
    }
    printf("+\n");
    for (i = 0; i < SIZE_Y; ++i) {
        printf("|");
        for (j = 0; j < SIZE_X; ++j) {
            putchar(ecosys[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (j = 0; j < SIZE_X; ++j) {
        printf("-");
    }
    printf("+\n");

    // Compter le nombre d'animaux
    int nbproie = compte_animal_it(liste_proie);
    int nbpred = compte_animal_it(liste_predateur);
  
    printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}




void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

//fonction faite en TD
void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
  FILE *file = fopen(nom_fichier, "w");

  if (!file) {
    printf("Erreur lors de l'ouverture du fichier %s", nom_fichier);
    return;
  }

  fprintf(file, "<proies>\n");

  for (Animal *a = liste_proie; a != NULL; a = a->suivant) {
    fprintf(file, "x=%d y=%d dir=[%d %d] energie=%f\n", a->x, a->y, a->dir[0], a->dir[1], a->energie);
  }

  fprintf(file, "</proies>\n");

  fprintf(file, "<predateurs>\n");

  for (Animal *a = liste_predateur; a != NULL; a = a->suivant) {
    fprintf(file, "x=%d y=%d dir=[%d %d] energie=%f\n", a->x, a->y, a->dir[0], a->dir[1], a->energie);
  }

  fprintf(file, "</predateurs>\n");

  fclose(file);
}

//fonction faite en TD
void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {
  FILE *f = fopen(nom_fichier, "r");

  if (!f) {
    printf("Fichier %s n'existe pas", nom_fichier);
    return;
  }

  char tab[300];
  fgets(tab, 300, f);
  assert(strncmp(tab, "<proies>", 8) == 0);
  fgets(tab, 300, f);

  *liste_proie = NULL;
  int x, y, dir[2];
  float energie;

  while (strncmp(tab, "</proies>", 9) != 0) {
    sscanf(tab, "x=%d y=%d dir=[%d %d] energie=%f\n", &x, &y, &dir[0], &dir[1], &energie);
    Animal *new = creer_animal(x, y, energie);
    new->dir[0] = dir[0];
    new->dir[1] = dir[1];
    *liste_proie = ajouter_en_tete_animal(*liste_proie, new);
    fgets(tab, 300, f);
  }

  fgets(tab, 300, f);
  assert(strncmp(tab, "<predateurs>", 12) == 0);
  fgets(tab, 300, f);

  *liste_predateur = NULL;

  while (strncmp(tab, "</predateurs>", 13) != 0) {
    sscanf(tab, "x=%d y=%d dir=[%d %d] energie=%f\n", &x, &y, &dir[0], &dir[1], &energie);
    Animal *new = creer_animal(x, y, energie);
    new->dir[0] = dir[0];
    new->dir[1] = dir[1];
    *liste_predateur = ajouter_en_tete_animal(*liste_predateur, new);
    new->suivant = *liste_predateur;
    *liste_predateur = new;
    fgets(tab, 300, f);
  }

  fclose(f);
}


void bouger_animaux(Animal *la) {
    Animal *ap = la;
    while (ap != NULL) {
        if ((float)rand() / RAND_MAX < p_ch_dir) {
            ap->dir[0] = rand() % 3 - 1;
            ap->dir[1] = rand() % 3 - 1;
        }
        ap->x = (ap->x + ap->dir[0]) % SIZE_X;
        ap->y = (ap->y + ap->dir[1]) % SIZE_Y;
        ap = ap->suivant;
    }
}

void reproduce(Animal **liste_animal, float p_reproduce) {
    Animal *ap = (liste_animal) ? *liste_animal : NULL;

    while (ap != NULL) {
        if ((float)rand() / RAND_MAX < p_reproduce) {
            ajouter_animal(ap->x, ap->y, ap->energie / 2, liste_animal);
            ap->energie = ap->energie / 2;
        }
        ap = ap->suivant;
    }
}


void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    if (liste_proie == NULL || *liste_proie == NULL) return;  // Verification si la liste est vide

    // Bouger les proies
    bouger_animaux(*liste_proie);

    Animal *p = *liste_proie;
    Animal *precedent = NULL;

    while (p != NULL) {
        // Diminuer l'energie
        p->energie -= 1.0;
        
        // Verification des coordonnees pour eviter un acces en dehors des limites
        if (p->x >= 0 && p->x < SIZE_X && p->y >= 0 && p->y < SIZE_Y) {
            // Consommer l'herbe si elle est presente (valeur >= 0)
            if (monde[p->x][p->y] >= 0) {
                p->energie += monde[p->x][p->y];
                monde[p->x][p->y] = temps_repousse_herbe; // Reinitialiser l'herbe pour repousse
            }
        } 

        Animal *next = p->suivant;  // Sauvegarde du prochain element

        if (p->energie < 0) {  // Si l'animal doit etre supprime
            if (precedent == NULL) {
                *liste_proie = p->suivant;  // Mettre a jour l'en-tete de liste
            } else {
                precedent->suivant = p->suivant;  // Mettre a jour le lien de l'element precedent
            }
            enlever_animal(liste_proie, p);  // Liberer l'animal
        } else {
            precedent = p;  // Avance le pointeur de precedent si l'animal n'est pas supprime
        }

        p = next;  // Avance au prochain element
    }

    // Faire appel a la fonction de reproduction
    reproduce(liste_proie, p_reproduce_proie);
}



Animal *animal_en_XY(Animal *l, int x, int y){
	Animal *p=l;
	while(p!=NULL){
		if(p->x ==x && p->y==y) return p;
		p=p->suivant;
	}
	return NULL;
}

void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie){
	// Bouger les proies
    bouger_animaux(*liste_predateur);

    Animal *p = *liste_predateur;
    Animal *temp = NULL;

    while (p != NULL) {
        // Diminuer l'energie
        p->energie = p->energie - 1.0;
        
        Animal *proie = animal_en_XY(*liste_proie, p->x, p->y);
		if (proie != NULL) {
		    p->energie += proie->energie;
		    enlever_animal(liste_proie, proie);  // Supprimer la proie mangee
		}


        Animal *next = p->suivant;

    	if (p->energie < 0) {
        	enlever_animal(liste_proie, p);  // Supprime l'animal
   		 }

    	p = next;  // Avance au suivant (evite l'acces a un pointeur liberee)
}

    // Faire appel a la fonction de reproduction
    reproduce(liste_predateur, p_reproduce_predateur);
}

void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	int i,j;
    for(i = 0; i < SIZE_X; i++) {
        for(j = 0; j < SIZE_Y; j++) {
            monde[i][j]=monde[i][j]+1;
            }
        }
    }




	



