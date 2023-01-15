#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* pour la fonction clock */
#include <unistd.h> /* Pour la fonction usleep */

// Aluízio Augusto Rocha Pires - Projet du jeu de la vie - ECM - Langage C
// Commandes pour faire marcher le logiciel
// gcc jeudelavie.c -o jeu
// ./jeu M N K prob

void recup_arguments(int argc, char **argv, int *pt_M, int *pt_N, int *pt_K, float *pt_prob){
  //Cette fonction a pour but la récuperation des arguments donnés dans le terminal

  // Il faut au moins 4 arguments, le fichier, et les trois nombres N,M et K
  if(argc < 5){
    printf("Erreur, il n'y a pas des arguments suffisants... \n");
    printf("Il faut taper les arguments dans le format suivant: ./jeu M N K prob \n");
    printf("Où M et N sont les dimensions du tableau, K est le nombre d'intérations\n");
    printf("Et prob est la probabilité que les celules soient vivantes au début du jeu.\n");
    exit(1);
  }
  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  int K = atoi(argv[3]);
  float prob = atof(argv[4]);
 // M négatif comme condition pour passer un fichier

  // Les chiffres ne peuvent pas être négatifs
  if (N <= 0 || K <= 0 || prob <= 0){
    printf("Il faut taper les arguments dans le format suivant: ./jeu M N K prob \n");
    printf("Où M et N sont les dimensions du tableau, K est le nombre d'intérations\n");
    printf("Et prob est la probabilité que les celules soient vivantes au début du jeu.\n");
    exit(1);
  }
  // On met les valeurs sur les addresses
  *pt_M = M;
  *pt_N = N;
  *pt_K = K;
  *pt_prob = prob;
}

int row_line(int N){
  // Fonction auxiliare de la fonction d'Affichage
  // Elle imprime une ligne avec --- --- --- ...
	printf("\n");
	for(int i=0; i< N; i++){
    if(i==0)
      printf(" --- ");
    else
      printf("--- ");
  }
	printf("\n");
}

void afficher_tab(int **tab, int M, int N){
  // Fonction d'affichage des tableaux
  int i,j;
  row_line(N);
  for(i=0; i<M; i++){
    printf("|");
    for(j=0;j<N;j++){
      if(tab[i][j] == 1)
        printf(" * |");
      else
        printf("   |");
    }
    row_line(N);
  }
}

void init_tab(int **tab, int M, int N, float prob){
  // Fonction d'initialisation du jeu de la vie avec une probabilité 'prob' des celules soient vivantes

  // REMARQUE: Float est un vieux format maintenant pour C, donc, on utilise Double par défault
  // REMARQUE: Les tableaux sont des pointeurs déjà, donc il faut juste les transmettre
  // sans avoir besoin d'initialiser un autre dans la fonction
  int i,j;

  for(i=0; i<M; i++){
    for(j=0;j<N;j++){
      // Ici, on obtient un nombre aleatoire sur 100 et on compare avec la proba choisie
      if(((float)rand())/RAND_MAX*100.0f <= prob)
        tab[i][j] = 1;
      else
        tab[i][j] = 0;
    }
  }
  // Affichage du tableau initial
  afficher_tab(tab, M, N);
}

int compter_cells_vivantes(int **tab, int M, int N, int r, int c){
  // Cette fonction s'agit d'une fonction auxiliare pour compter le nb de celules vivantes voisines
	int i, j, count=0;
	for(i=r-1; i<=r+1; i++){
		for(j=c-1;j<=c+1;j++){
      // Si on sort du tableau, on fait pas la boucle et on ne compte pas la cellule elle-même
			if((i==r && j==c) || (i<0 || j<0) || (i>=M || j>=N)){
				continue;
			}
			if(tab[i][j]==1){
				count++;
			}
		}
	}
	return count;
}

void prochain_tab(int **tab, int **tab1, int M, int N){
  // Ce fonction calcule le tableau suivant au actuel
  int i, j, cells_vivantes;
  for(i=0; i<M; i++){
    for(j=0;j<N;j++){
      // En parcourant le tableau, on compte le nombre de voisins vivantes et on
      // applique les règles selon ce chiffre
      cells_vivantes = compter_cells_vivantes(tab,M,N,i,j);
      if(tab[i][j]==1 && (cells_vivantes==2 || cells_vivantes==3)){
        tab1[i][j]=1;
      }
      else if(tab[i][j]==0 && cells_vivantes==3){
        tab1[i][j]=1;
      }
      else{
        tab1[i][j]=0;
      }
    }
  }
}

int **alloc_tableau (int M, int N){
// Fonction pour allouer dynamiquement les tableaux
  int **tableau = NULL;
  tableau = malloc(M*sizeof(int*));
  for (int i=0; i<M; i++)
    tableau[i] = malloc(N*sizeof(int));
  return tableau;
}

void desalloc_tableau(int M, int **tabl){
// Fonction pour desallouer des tableaux
  for(int i=0; i<M; i++){
    free(tabl[i]);
  }
  free(tabl);
}

int main(int argc, char **argv) {
  /* Choses à faire:
      Lecture des paramètres (OK)
      allocation de la memóire pour le tableau (OK)
      initialisation du jeu de la vie (OK)
      Implementer les regles (OK)
      Boucle (OK)
      Affichage (OK)
      Libération de la memóire (OK)
   */

  // On récupere les arguments du terminal
  int M,N,K;
  float prob;
  recup_arguments(argc, argv, &M, &N, &K,&prob);

  // initialisation de la graine aleatoire
  srand(clock());

  // Allocation dynamique du tableau temporaire a être utilisé
  int **tab = alloc_tableau(M,N);

  // Allocation du tableau t+1
  int **tab1 = alloc_tableau(M,N);


  // On initialise le jeu avec des valeurs aleatoires pour chaque cellule
  printf("Tableau initial: \n");
  init_tab(tab, M, N, prob);
  sleep(1.5);
  system("cls");

  // Le pointeur auxiliare pour inverser les tableaux
  int **tmp_pt = NULL;

  // La boucle for du jeu selon la valeur du K
  for(int x = 1; x<K+1; x++){
    // Ici, on calcule le prochain tableau et on l'affiche
    printf("Tableau nb %d: \n", x);
    prochain_tab(tab,tab1,M,N);

    // Inversion des addresses
    tmp_pt = tab;
    tab = tab1;
    tab1 = tmp_pt;

    // Comme les tableaux sont inversés maintenant, on peut afficher l'ancien tab qui a été
    // mis à jour
    afficher_tab(tab,M,N);
    sleep(1.5);
    system("cls");
    tmp_pt = NULL;
  }

  // Maintenant, on désalloue la memóire
  desalloc_tableau(M,tab);
  desalloc_tableau(M,tab1);

  return 0;
}
