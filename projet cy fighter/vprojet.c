#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COMBATTANTS_TOTAL 20
#define TAILLE_MAX_NOM 50
#define MAX_NOM 50
#define MAX_TECHNIQUES 5
#define MAX_TOURS 100 


typedef struct {
    char nomTechnique[TAILLE_MAX_NOM];
    char type[20];
    int puissance;
    char effet[30];
    int dureeEffet;
    int recharge;
} TechniqueSpeciale;

typedef struct {
    char nom[MAX_NOM];
    int points_de_vie_max;
    int points_de_vie;
    int attaque;
    int defense;
    int agilite;
    int vitesse;
    int vitesse_accumulee; // Points de vitesse accumulés
    TechniqueSpeciale techniques[MAX_TECHNIQUES]; // Techniques spéciales
    int nb_techniques; // Nombre de techniques spéciales
    int tours_recharge[MAX_TECHNIQUES];
} Combattant;



typedef struct {
 char nom[MAX_NOM];
 Combattant combattants[5]; // Chaque équipe aura 5 combattants
 int nb_combattants;
} Equipe;

void choisir_nom_equipe(Equipe *equipe1, Equipe *equipe2) {
    char choix;
    
    printf("Voulez-vous un nom d'équipe? \nO/o:oui N/n:non\nchoix: ");
    
    // Lecture et validation de l'entrée, while(1) pour que la boucle continue jusqu'a ce qu'une condition soit remplie.
    while (1) {
        if (scanf(" %c", &choix) == 1) {
            // Vider le tampon d'entrée
            while (getchar() != '\n');
            
            // Vérifier si le choix est valide
            if (choix == 'O' || choix == 'o' || choix == 'N' || choix == 'n') {
                break;
            }
        }
        printf("Veuillez entrer O, o, N ou n: ");
    }
    
    if (choix == 'O' || choix == 'o') {
        printf("Entrez le nom de la première équipe: ");
        scanf("%s", equipe1->nom);
        while (getchar() != '\n'); // Vider le tampon après lecture
        
        printf("Entrez le nom de la deuxième équipe: ");
        scanf("%s", equipe2->nom);
        while (getchar() != '\n'); 
    } else {
        // Noms d'équipe par défaut si l'utilisateur entre n ou N
        strcpy(equipe1->nom, "Equipe 1");
        strcpy(equipe2->nom, "Equipe 2");
    }
    
    // Initialisation du nombre de combattants
    equipe1->nb_combattants = 0;
    equipe2->nb_combattants = 0;
}
void choisir_combattant(Equipe *equipe, Combattant *liste, int taille) {
    int choix;
    int dja_choisi[5] = {0}; // Initialiser le tableau pour suivre les combattants déjà choisis
    printf("Choisissez un combattant pour l'équipe %s:\n", equipe->nom);
    printf("Combattants disponibles:\n");
    for (int i = 0; i < taille; i++) {
        printf("%d. %s (PV: %d, Att: %d, Def: %d, Agi: %d, Vit: %d)\n",
               i, liste[i].nom, liste[i].points_de_vie_max, liste[i].attaque,
               liste[i].defense, liste[i].agilite, liste[i].vitesse);
    }

    for (int i = 0; i < 5; i++) { // 5 choix maximum par équipe
        printf("Choix du combattant %d pour %s : ", i + 1, equipe->nom);
        if (scanf("%d", &choix) != 1 || choix < 0 || choix >= taille) {
            printf("Erreur, veuillez entrer un numéro valide entre 0 et %d.\n", taille - 1);
            i--; // Recommence l'itération
            while (getchar() != '\n'); // Vider le tampon d'entrée
            continue;
        }

        // Vérifier si le combattant a déjà été choisi
        int deja_choisi = 0;
        for (int j = 0; j < i; j++) {
            if (dja_choisi[j] == choix) {
                deja_choisi = 1;
                break;
            }
        }

        if (deja_choisi) {
            printf("Erreur, ce combattant a déjà été choisi.\n");
            i--; // Recommence l'itération, on peut utilisr simplement une boucle if car la vérification est faite à chaque itérations.
            while (getchar() != '\n'); // Vider le tampon d'entrée
            continue;
        }

        // Ajouter le combattant à l'équipe
        dja_choisi[i] = choix; // Marquer le combattant comme choisi
        equipe->combattants[equipe->nb_combattants] = liste[choix];
        equipe->nb_combattants++;
    }
}

void attaquer(Combattant *attaquant, Combattant *defenseur) { // attaquant et defenseur...

    int degats= attaquant->attaque - ((defenseur->defense)*0.01)*attaquant->attaque;//calcul des dégats en fonction de l'attaque et de la defense.
    if (degats < 0) degats = 0;//si degats <0 alors degats=0.Précaution pour ne pas avoir des dégats qui s'ajoutent aux pvs.
    defenseur->points_de_vie -= degats;//on retire aux pv actuels le nb de dégats.
    printf("%s attaque %s pour %d dégâts! Il reste %d PV à %s.\n",attaquant->nom, defenseur->nom, degats, defenseur->points_de_vie, defenseur->nom);
}
int choix_cmb(Equipe *equipedef) {//choix du combattant à attaquer.
    int choix;
    printf("Choisissez un combattant à attaquer dans l'équipe %s :\n", equipedef->nom);
    for (int i = 0; i < equipedef->nb_combattants; i++) {
        if (equipedef->combattants[i].points_de_vie > 0) { // Afficher uniquement les combattants vivants
            printf("%d. %s (PV: %d)\n", i, equipedef->combattants[i].nom, equipedef->combattants[i].points_de_vie);
        }
    }

    while (1) { // Boucle infinie jusqu'a ce qu'une condition soit remplie.
        printf("Entrez le numéro du combattant à attaquer : ");
        if (scanf("%d", &choix) == 1 && choix >= 0 && choix < equipedef->nb_combattants &&
            equipedef->combattants[choix].points_de_vie > 0) {//enetrée valide pour le choix du combattant a atatquer.
            // Entrée valide
            return choix;
        } else {
            // Entrée invalide
            printf("Erreur, veuillez entrer un numéro valide correspondant à un combattant vivant.\n");
            while (getchar() != '\n'); // Vider le tampon d'entrée
        }
    }
}

void gerer_ticks(Equipe *equipe1, Equipe *equipe2) {
    for (int i = 0; i < equipe1->nb_combattants; i++) {
        if (equipe1->combattants[i].points_de_vie > 0) {
            equipe1->combattants[i].vitesse_accumulee += equipe1->combattants[i].vitesse;
        }
    }

    for (int i = 0; i < equipe2->nb_combattants; i++) {
        if (equipe2->combattants[i].points_de_vie > 0) {
            equipe2->combattants[i].vitesse_accumulee += equipe2->combattants[i].vitesse;
        }
    }
}
int equipe_vivante(Equipe *equipe) {
    for (int i = 0; i < equipe->nb_combattants; i++) {
        if (equipe->combattants[i].points_de_vie > 0) {
            return 1; // L'équipe a encore au moins un combattant vivant
        }
    }
    return 0; // Tous les combattants sont morts
}
void afficher_barre_pv(int pv_actuels, int pv_max);
// Définition de la fonction globale
void afficher_barre_pv(int pv_actuels, int pv_max) {
    int longueur_barre = 20; // Longueur de la barre en caractères
    int remplissage = (pv_actuels * longueur_barre) / pv_max; // Proportion de PV restants
    printf("[");
    for (int i = 0; i < longueur_barre; i++) {
        if (i < remplissage) {
            printf("█"); // Partie remplie
        } else {
            printf(" "); // Partie vide
        }
    }
    printf("] %d/%d", pv_actuels, pv_max); // Afficher les PV actuels et max
}

void charger_techniques(Combattant *liste, int nb_combattants, const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    if (!f) {
        printf("Erreur d'ouverture du fichier des techniques.\n");
        exit(1);
    }

    for (int i = 0; i < nb_combattants; i++) {
        fscanf(f, "%49[^,],%19[^,],%d,%29[^,],%d,%d\n",
               liste[i].techniques[0].nomTechnique,
               liste[i].techniques[0].type,
               &liste[i].techniques[0].puissance,
               liste[i].techniques[0].effet,
               &liste[i].techniques[0].dureeEffet,
               &liste[i].techniques[0].recharge);
        liste[i].nb_techniques = 1; // Chaque combattant a une technique
    }

    fclose(f);
}


// Fonction pour afficher l'état des équipes
void afficher_etat(Equipe *equipe1, Equipe *equipe2) {
    printf("\n=== État des équipes ===\n");

    // Afficher l'état de l'équipe 1
    printf("\nÉquipe %s :\n", equipe1->nom);
    for (int i = 0; i < equipe1->nb_combattants; i++) {
        Combattant *c = &equipe1->combattants[i];
        printf("%s [", c->nom);
        afficher_barre_pv(c->points_de_vie, c->points_de_vie_max);
        printf(", Vitesse: %3d/100]  ", c->vitesse_accumulee);

        // Ajouter un saut de ligne après deux combattants
        if ((i + 1) % 2 == 0) {
            printf("\n");
            printf("\n");
        }
    }
    printf("\n");

    // Afficher l'état de l'équipe 2
    printf("\nÉquipe %s :\n", equipe2->nom);
    for (int i = 0; i < equipe2->nb_combattants; i++) {
        Combattant *c = &equipe2->combattants[i];
        printf("%s [", c->nom);
        afficher_barre_pv(c->points_de_vie, c->points_de_vie_max);
        printf(", Vitesse: %3d/100]  ", c->vitesse_accumulee);

        // Ajouter un saut de ligne après deux combattants
        if ((i + 1) % 2 == 0) {
            printf("\n");
            printf("\n");
        }
    }
    printf("\n=========================\n");
}
void tuer_combattants(Equipe *equipe){
	for(int i=0;i<equipe->nb_combattants;i++){
		equipe->combattants[i].points_de_vie=0;
	}
printf("tous les combattants de %s sont morts!\n",equipe->nom);
}
int choix_action() {
    int choix;
    printf("1. Attaque normale\n");
    printf("2. Utiliser technique spéciale\n");
    printf("3. tuer tout les combattants\n");
    printf("Votre choix : ");
    
    while (scanf("%d", &choix) != 1 || (choix != 1 && choix != 2 && choix !=3) {
        printf("Choix invalide. Veuillez entrer 1 ou 2 ou 3 : ");
        while (getchar() != '\n'); // Vider le tampon d'entrée
    }

    return choix;
}
void utiliser_technique(Combattant *attaquant, Combattant *cible) {
    
    if (attaquant->nb_techniques == 0) {
        printf("%s n'a pas de techniques spéciales disponibles !\n", attaquant->nom);
        return;
    }

    // Afficher les techniques disponibles
    printf("\nTechniques disponibles pour %s :\n", attaquant->nom);
    for (int i = 0; i < attaquant->nb_techniques; i++) {
        TechniqueSpeciale *t = &attaquant->techniques[i];
        printf("%d. %s (Type: %s, Puissance: %d, Effet: %s, Durée: %d tours, Recharge: %d tours)\n",
               i + 1, t->nomTechnique, t->type, t->puissance, t->effet, t->dureeEffet, t->recharge);
    }

    // Choisir une technique
    int choix;
    printf("Choisissez une technique à utiliser : ");
    scanf("%d", &choix);
    if (attaquant->tours_recharge[choix - 1] > 0) {
    printf("⚠️ Cette technique est en recharge pour encore %d tour(s) !\n", attaquant->tours_recharge[choix - 1]);
    return;
}
    if (choix < 1 || choix > attaquant->nb_techniques) {
        printf("Choix invalide. Aucune technique utilisée.\n");
        return;
    }

    TechniqueSpeciale *t = &attaquant->techniques[choix - 1];

    // Affichage de l'action
    printf("\n%s utilise la technique spéciale \"%s\" sur %s !\n", 
           attaquant->nom, t->nomTechnique, cible->nom);

    // Appliquer les effets de la technique
    if (strcmp(t->type, "Attaque") == 0) {
        int degats = t->puissance - ((cible->defense) * 0.01 * t->puissance);
        if (degats < 0) degats = 0;
        cible->points_de_vie -= degats;
        printf("➤ %s subit %d dégâts. PV restants : %d\n", cible->nom, degats, cible->points_de_vie);

    } else if (strcmp(t->type, "Soins") == 0) {
        attaquant->points_de_vie += t->puissance;
        if (attaquant->points_de_vie > attaquant->points_de_vie_max)
            attaquant->points_de_vie = attaquant->points_de_vie_max;
        printf("➤ %s récupère %d PV. PV actuels : %d\n", attaquant->nom, t->puissance, attaquant->points_de_vie);

    } else if (strcmp(t->type, "Debuff") == 0) {
        printf("➤ Effet négatif prévu : %s (fonctionnalité à ajouter)\n", t->effet);

    } else if (strcmp(t->type, "Boost") == 0) {
        printf("➤ Effet positif prévu : %s (fonctionnalité à ajouter)\n", t->effet);
    
    }
    else {
        printf("⚠️ Type de technique inconnu : %s\n", t->type);
    }
    attaquant->tours_recharge[choix - 1] = t->recharge;
}
void jeu(Equipe *equipe1, Equipe *equipe2) {
    void mettre_a_jour_recharge(Equipe *equipe) {
    for (int i = 0; i < equipe->nb_combattants; i++) {
        for (int j = 0; j < equipe->combattants[i].nb_techniques; j++) {
            if (equipe->combattants[i].tours_recharge[j] > 0) {
                equipe->combattants[i].tours_recharge[j]--;
            }
        }
    }
}
    
    int tour = 1;

    printf("\n--- Début du jeu ---\n");

    while (equipe_vivante(equipe1) && equipe_vivante(equipe2)) {
        printf("\n--- Tour %d ---\n", tour);

        // Afficher l'état des équipes avant le tour
        afficher_etat(equipe1, equipe2);

        // Gérer les ticks pour les deux équipes
        gerer_ticks(equipe1, equipe2);

        // Vérifier si des combattants de l'équipe 1 peuvent attaquer
        for (int i = 0; i < equipe1->nb_combattants; i++) {
            if (equipe1->combattants[i].points_de_vie > 0 && equipe1->combattants[i].vitesse_accumulee >= 100) {
                printf("\nAction pour %s (Équipe %s) :\n", equipe1->combattants[i].nom, equipe1->nom);
                int cible = choix_cmb(equipe2);
                int action = choix_action();
if (action == 1) {
    attaquer(&equipe1->combattants[i], &equipe2->combattants[cible]);
} else if (action == 2) {
    utiliser_technique(&equipe1->combattants[i], &equipe2->combattants[cible]);
}
else if(action == 3){
	tuer_combattants(&equipe2);
}
                equipe1->combattants[i].vitesse_accumulee = 0; // Réinitialiser la vitesse accumulée

                // Vérifier si l'équipe adverse est vaincue
                if (!equipe_vivante(equipe2)) {
                    printf("\nL'équipe %s a gagné !\n", equipe1->nom);
                    afficher_etat(equipe1, equipe2);
                    return;
                }
            }
        }

        // Vérifier si des combattants de l'équipe 2 peuvent attaquer
        for (int i = 0; i < equipe2->nb_combattants; i++) {
            if (equipe2->combattants[i].points_de_vie > 0 && equipe2->combattants[i].vitesse_accumulee >= 100) {
                printf("\nAction pour %s (Équipe %s) :\n", equipe2->combattants[i].nom, equipe2->nom);
                int cible = choix_cmb(equipe1);
                int action = choix_action();
if (action == 1) {
    attaquer(&equipe1->combattants[i], &equipe2->combattants[cible]);
} else if (action == 2) {
    utiliser_technique(&equipe1->combattants[i], &equipe2->combattants[cible]);
}
                equipe2->combattants[i].vitesse_accumulee = 0; // Réinitialiser la vitesse accumulée

                // Vérifier si l'équipe adverse est vaincue
                if (!equipe_vivante(equipe1)) {
                    printf("\nL'équipe %s a gagné !\n", equipe2->nom);
                    afficher_etat(equipe1, equipe2);
                    return;
                }
            }
        }

        // Afficher l'état des équipes après le tour
        afficher_etat(equipe1, equipe2);

        tour++;
        mettre_a_jour_recharge(equipe1);
        mettre_a_jour_recharge(equipe2);
    }

    printf("\n--- Fin du jeu ---\n");
}









int main() {
    // Allocation dynamique pour la liste des combattants
    Combattant *liste = malloc(10 * sizeof(Combattant));
    if (liste == NULL) {
        printf("Erreur : allocation de mémoire échouée.\n");
        return 1;
    }

    // Lecture du fichier des combattants
    FILE *fichier = fopen("combattants.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : fichier inexistant !\n");
        free(liste); // Libérer la mémoire avant de quitter
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        fscanf(fichier, "%49[^,],%d,%d,%d,%d,%d\n", liste[i].nom, &liste[i].points_de_vie_max,&liste[i].attaque, &liste[i].defense, &liste[i].agilite, &liste[i].vitesse);liste[i].points_de_vie = liste[i].points_de_vie_max; // Initialiser les PV actuels
    }
    fclose(fichier);
    charger_techniques(liste, 10, "techniques.txt");
    for (int i = 0; i < 10; i++) {
    for (int j = 0; j < liste[i].nb_techniques; j++) {
        liste[i].tours_recharge[j] = 0;
    }
}
    Equipe e1, e2;
    char choix;
    char dernier_vainqueur[50];
    do{
        printf("\nMenu principal:\n");
        printf("1. Lancer le combat\n");
        printf("2. Quitter\n");
        printf("3. Afficher le dernier vainqueur\n");
        printf("Choix: ");
        scanf(" %c", &choix);
        while (getchar() != '\n'); // Vider le tampon après lecture

        switch (choix) {
            case '1':
             // Initialisation des équipes
    
    choisir_nom_equipe(&e1, &e2);
    choisir_combattant(&e1, liste, 10);
    choisir_combattant(&e2, liste,10 );
    
    // Affichage des équipes
    printf("\n--- Équipe %s ---\n", e1.nom);
    for (int i = 0; i < e1.nb_combattants; i++) {
        printf("%s (PV: %d, Att: %d, Def: %d, Agi: %d, Vit: %d)\n",
               e1.combattants[i].nom, e1.combattants[i].points_de_vie,
               e1.combattants[i].attaque, e1.combattants[i].defense,
               e1.combattants[i].agilite, e1.combattants[i].vitesse);
    }

    printf("\n--- Équipe %s ---\n", e2.nom);
    for (int i = 0; i < e2.nb_combattants; i++) {
        printf("%s (PV: %d, Att: %d, Def: %d, Agi: %d, Vit: %d)\n",
               e2.combattants[i].nom, e2.combattants[i].points_de_vie,
               e2.combattants[i].attaque, e2.combattants[i].defense,
               e2.combattants[i].agilite, e2.combattants[i].vitesse);
    }

                jeu(&e1, &e2);
                // Déterminer le vainqueur
                if (equipe_vivante(&e1)) {
                    strcpy(dernier_vainqueur, e1.nom);
                } else if (equipe_vivante(&e2)) {
                    strcpy(dernier_vainqueur, e2.nom);
                }

                break;
            
            
            case '2':
                printf("Au revoir !\n");
                return 0;;
            case '3':
                if (strlen(dernier_vainqueur) > 0) {
                    printf("Le dernier vainqueur était: %s\n", dernier_vainqueur);
                } else {
                    printf("Aucun combat n'a été joué.\n");
                }
                break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
                break;
        }
    }while(choix != '2');
   
   
    

    // Libérer la mémoire allouée
    free(liste);

    return 0;
}
