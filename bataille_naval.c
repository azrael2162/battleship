#include <stdio.h>
#include <stdlib.h>
#include <time.h> //fonction sdrand
#include "ncurses.h"//bibliotheque similaire a ncurses

#define MaxMatrice 5
#define MaxBateaux 3
#define oui 1
#define non 0

// 0 position off
// 1 position On

void viderBuffer(void)
{
    int c=0;
    while (c !='\n' && c!=EOF)
    {
        c=getchar();
    }
}

int lireInt (void)
{
    int x;
    int erreur=0;
    while (erreur==0)
    {
    erreur=scanf("%d",&x);
    if (erreur==0) {printf("Vous trompez pas saisissez un nombre ! RECOMMENCEZ\n");}
    viderBuffer();// ici vider buffer
    }
    return x;
}

void afficheMatrice (int  matrice[MaxMatrice][MaxMatrice],int booleen)
{
    int i,j;
    printf("   ");
    for (i=0;i<MaxMatrice;i++)
    {
        printf("%d ",i);
    }

    printf("\n   ");
    for (i=0;i<MaxMatrice*2;i++)
    {
        printf("+");
    }
    printf("\n");
    for (i=0;i<MaxMatrice;i++)
    {
        printf("%d |",i);
        for (j=0;j<MaxMatrice;j++)
        {
            //printf ("%d ",matrice[i][j]);
            if (matrice[i][j]==0) {printf("  ");}//rien
            if ((matrice[i][j]==1) && booleen) {printf("B ");}//position bateau
            if (matrice[i][j]==2) {printf("X ");}//bateau coule
            if (matrice[i][j]==3) {printf("*");}//position tir
        }
        printf("\n");
    }
    printf("\n");
}

void remplirMatriceAzero(int matrice[MaxMatrice][MaxMatrice])
{
    int i,j;
    for (i=0;i<MaxMatrice;i++)
    {
        for (j=0;j<MaxMatrice;j++)
        {
            matrice[i][j]=0;
        }
    }
}

void remplirMatriceOrdi( int matrice[MaxMatrice][MaxMatrice])
{
    int i,col,lig;
    remplirMatriceAzero(matrice);
    //contrainte eviter les memes positions!
    i=0;
    while (i<MaxBateaux)
    {
       lig = (rand() % (MaxMatrice  + 1));
       col = (rand() % (MaxMatrice  + 1));
       if (matrice[lig][col]==0)
       {
            matrice[lig][col]=1;
            i++;
       };
    }

}

void remplirMatriceJoueur( int matrice[MaxMatrice][MaxMatrice])
{

    remplirMatriceOrdi(matrice);
}

void choixCoordonnees(int *x,int *y)
{
    printf("\n entrer vos coordonnees de tir : \n");
    printf("Ligne? :");
    *x=lireInt();
    printf("\nColonne? :");
    *y=lireInt();
}


int main(void)
{
    int matriceJoueur[MaxMatrice][MaxMatrice];
    //int matriceAffichage[MaxMatrice][MaxMatrice];
    int matriceOrdi[MaxMatrice][MaxMatrice];
    int nombreBateauxJoueur = MaxBateaux;
    int nombreBateauxOrdi   = MaxBateaux;
    int x,y;
    srand(time(NULL));

    remplirMatriceOrdi(matriceOrdi);
    afficheMatrice(matriceOrdi,non);
    remplirMatriceJoueur(matriceJoueur);

    while((nombreBateauxJoueur > 0) && (nombreBateauxOrdi > 0))
    {
        //tour joueur
        printf("Grille de tir \n");
        printf("Commandant j'attends vos ordres de tir\n");
        afficheMatrice(matriceOrdi,non);
        choixCoordonnees(&x,&y);
        if (matriceOrdi[x][y]==1)
        {
            matriceOrdi[x][y]=2;//bateau coule
            nombreBateauxOrdi --;
            printf("\n T O U C H E ,il reste %d bateaux a couler\n",nombreBateauxOrdi);
        }
        else
        {
            printf("\n R A T E \n");
            matriceOrdi[x][y]=3;//pour signaler tir
        }
        afficheMatrice(matriceOrdi,non);


        getchar();
        system("clear");
        printf("Grille de tir pour l'ordi \n");
        afficheMatrice(matriceJoueur,oui);
        x = (rand() % (MaxMatrice  + 1));
        y = (rand() % (MaxMatrice  + 1));

        if (matriceJoueur[x][y]==1)
        {
            matriceJoueur[x][y]=2;
            nombreBateauxJoueur --;
            printf("\n T O U C H E ,il vous reste plus que %d bateau\n",nombreBateauxJoueur);
        }
        else
        {
            printf("\n pas de chance \n");
            matriceJoueur[x][y]=3;

        }
         afficheMatrice(matriceJoueur,oui);

        getchar();

    }
    if (nombreBateauxOrdi ==0)
    {
        printf("Vous avez gagne \n");
    }
    else
    {
        printf("Vous avez perdu\n");
    }

    return 0;
}
