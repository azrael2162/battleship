#include <stdio.h>
#include <stdlib.h>

//les bateaux
#define C_PORTE_AVION 10
#define C_CROISEUR 20
#define C_TORPILLEUR 50
#define C_EAU 0 // Water



//constante couleur
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// constante grillle
const char line[] = "  +---+---+---+---+---+---+---+---+---+---+",
				EAU[] = "   |",
				EAU_T[] = ANSI_COLOR_BLUE " X " ANSI_COLOR_RESET "|",
				BAT[] = ANSI_COLOR_GREEN "***" ANSI_COLOR_RESET "|",
				BAT_T[] = ANSI_COLOR_RED "XXX" ANSI_COLOR_RESET "|";

typedef struct {
	/* 1 porte-avion (5 cases),
   * 1 croiseur (4 cases),
   * 1 contre torpilleur (3 cases),
   * 1 sous-marin (3 cases),
   * 1 torpilleur (2 cases),
 */
 // Cases restantes pour les bateaux
int porteAvion, croiseur, torpilleur, points;
int grille[BOARD_SIZE][BOARD_SIZE];
int grilleEnnemie[BOARD_SIZE][BOARD_SIZE];
} Plateau;


typedef struct {
	int x, y;
	char d;
} Coordonnees;

Coordonnees strToCoord(char string[], int hasDirection) {
	Coordonnees c;
	char strX[2 + 1];
	int i;

	// Protect hasDirection range
	if (hasDirection > 0) {
		hasDirection = 1;
		c.d = string[strlen(string) - 1];
	} else {
		hasDirection = 0;
	}

  // Split the string
  c.y = string[0] - 'a';
  //	printf("Debug strX- lg: %i, iMax: %i, str: %s ", (int) strlen(string), (int) strlen(string)-(1 + hasDirection), string);
  for (i = 0; i < strlen(string)-(1 + hasDirection); i++) {
    strX[i] = string[i + 1];
  }
  // Filling up (had trouble to get a clean string.)
  for (i + 1; i < strlen(strX); i++) {
    strX[i] = '\0';
  }
  //	printf("strX: %s\n", strX);

  c.x = strtol(strX, NULL, 10) - 1;

  return c;
}

/**
 * Initialise un plateau vierge
 * @return Plateau initialisé
 */
Plateau initPlateau() {
	Plateau p;
	int i, j;
	p.croiseur = B_CROISEUR;
	p.porteAvion = B_PORTE_AVION;
	p.torpilleur = B_TORPILLEUR;
	p.points = 0;

  for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			p.grille[i][j] = C_EAU;
			p.grilleEnnemie[i][j] = C_EAU;
		}
	}

	return p;
}

/**
 * Affiche une grille de jeu
 * @param g Grille à afficher
 */
void afficherGrille(int g[BOARD_SIZE][BOARD_SIZE]) {
	int i, j;

	/*
	| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|
	+---+---+---+---+---+---+---+---+---+---+
A |   |   |   |   |   |   |   |   |   |   |
	+---+---+---+---+---+---+---+---+---+---+
B |   |   |   |   |   |   |   |   |   |   |
	+---+---+---+---+---+---+---+---+---+---+
... J
	 */
	puts("\n  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|");
	puts(line);
	for (i = 0; i < BOARD_SIZE; i++) {
		printf("%c |", i + 'A');
		for (j = 0; j < BOARD_SIZE; j++) {
			if ((int) g[i][j] == C_EAU) { // Dans l'eau
				printf("%s", EAU);
			} else if (g[i][j] == C_EAU_T) { // Eau, touché
				printf("%s", EAU_T);
			} else if (// Bateau
							g[i][j] == C_PORTE_AVION ||
							g[i][j] == C_CROISEUR ||
							g[i][j] == C_CONTRE_TORPILLEUR ||
							g[i][j] == C_SOUS_MARIN ||
							g[i][j] == C_TORPILLEUR
							) {
				printf("%s", BAT);
			} else {
				printf("%s", BAT_T);
			}
		}
		printf("\n");
		puts(line);
	}
}


Plateau placeShip(Plateau p, char nom[], int size, int val) {
	int done, error, i;
	Coordonnees c;
	char pos[4 + 1], // Coordonnées
					orientation[10 + 1],
					reponse;

	afficherGrille(p.grille);
	// IF YOU WANT TO DEBUG/TEST THE GAME,
	// COMMENT FROM HERE
	printf("Pour placer un bateau, donnez l'adresse de la case de destination, puis\n"
					"son orientation (h/v). Les batiments seront positionnés sur la droite de\n"
					"la case donnée pour les placements horizontaux, et vers le base pour les \n"
					"placements verticaux. Exemple : a10v\n\n");
	printf("Veuillez placer le %s (%i cases)\n\n", nom, size);
	do {
		// Re-init vars
		error = 0;
		done = 0;
		strcpy(orientation, "horizontal");

		// Ask for coordinates
		printf("Position : ");
		scanf("%s", pos);

		c = strToCoord(pos, 1);

		//		printf("x: %i, y: %i, o: %c\n", c.x, c.y, c.d);

		if (c.x < 0 || c.y < 0 || c.x > BOARD_SIZE || c.y > BOARD_SIZE) {
			puts(" > Mauvaises coordonnées...");
			error = 1;
		} else if (c.d == 'v') {// Vérification placement des bateaux
			strcpy(orientation, "vertical");
			// Sortie de carte
			if (c.y + size > BOARD_SIZE) {
				printf(" > Vous ne pouvez pas placer votre bateau ici. Il sortirait de la carte...(y=%i)\n", c.y);
				error = 1;
			} else {
				// Chevauchements
				for (i = c.y; i < c.y + size; i++) {
					if (p.grille[i][c.x] != C_EAU) {
						puts(" > Il y a déjà un bateau ici...");
						error = 1;

						break;
					}
				}
			}
		} else if (c.x + size > BOARD_SIZE) {
			printf(" > Vous ne pouvez pas placer votre bateau ici. Il sortirait de la carte...(x=%i)\n", c.x);
			error = 1;
		} else {
			// Chevauchements
			for (i = c.x; i < c.x + size; i++) {
				if (p.grille[c.y][i] != C_EAU) {
					puts(" > Il y a déjà un bateau ici...");
					error = 1;

					break;
				}
			}
		}

		if (error == 0) {
			getchar();
			printf("Placement %s en %c:%i. Est-ce correct ? [o/N] ", orientation, c.y + 'a', c.x + 1);
			reponse = getchar();
			if (reponse == 'o' || reponse == 'O') {
				done = 1;
			}
		}
	} while (done == 0);
	// TO HERE ///////////////////////////////////////////////////////////////////
	// AND UNCOMMENT THIS LINE
	//	c = strToCoord(debugC, 1);

	// Place ship on grid
	if (c.d == 'v') {
		for (i = c.y; i < c.y + size; i++) {
			p.grille[i][c.x] = val;
		}
	} else {
		for (i = c.x; i < c.x + size; i++) {
			p.grille[c.y][i] = val;
		}
	}
	return p;
}
