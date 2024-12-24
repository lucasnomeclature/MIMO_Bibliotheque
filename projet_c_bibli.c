#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Constantes
//#define

// déclaration du type global
//struct x {} ; 

// Déclaration des variables globales
//struct x tabx[const] ;
//int nbeleves=0;

// déclarations préliminaires
void menu_staff();
void verif_sauvegarde();
void sauvegarde();
void quitter();
void wip();
void conv_maj(char chaine[]);

// Booléen indiquant si on doit faire une sauvegarde
int a_sauvegarder=0 ;


// --- Programme principal ----------------------------------------------
int main ()
{
    menu_staff();
}

//--- Affichage du menu staff -------------------------------------------
void menu_staff()
{
    int choix_menu=-1 ;

  while (choix_menu != 0)
  {
    printf("✧--------MENU--------✧\n");
    printf("|   -1- Rechercher   |\n");
    /*
    printf("|   -2- Affichage    |\n");
    printf("|   -3- Sauvegarde   |\n");
    printf("|   -4- Chargement   |\n");
    printf("|   -5- Recherche    |\n");
    printf("|   -6- Modification |\n");
    printf("|   -7- Suppression  |\n");
    printf("|   -8- tri par nom  |\n");
    printf("|   -9- tri par age  |\n");  */
    printf("|                    |\n");
    printf("|   -0- Quitter      |\n");
    printf("✧--------------------✧\n");
    printf("Choix : ");

    scanf("%d",&choix_menu);

    switch(choix_menu)
    {
      case 1 : wip()            ;
               break            ;
      /*
      case 2 : affichage()      ;
               break            ;
      case 3 : sauvegarde()     ;
               break            ;
      case 4 : chargement()     ;
               break            ;
      case 5 : lance_recherche();
               break            ;
      case 6 : modification()   ;
               break            ;
      case 7 : suppression()    ;
               break            ;
      case 8 : tri_insertion_nom(tabeleves,nbeleves);
               break            ;
      case 9 : tri_insertion_age(tabeleves,nbeleves);
               break            ; */
      case 0 : quitter()        ;
               break            ;
      default : printf("Choix erroné\n");
               break            ;
    }
  }
} 

// ---Procédure de sauvegarde ---
/*void sauvegarde()
{
  // variables
  struct etudiant eleve;
  int i=0;
  FILE *f1;
  char NomFichier[TAILLE_NOM_F];

  if (nbeleves == 0)
  {
    printf("Aucun élève à sauvegarder\n");
  }
  else
  {
    printf("Nom du fichier de sauvegarde : ");
    scanf("%s",NomFichier);
    f1=fopen(NomFichier,"w");
    for (i=0 ; i < nbeleves ; i++)
    {
      eleve = tabeleves[i];
      fprintf(f1,"%s %s %d\n",eleve.nom,eleve.prenom,eleve.age);
    } 
    fclose(f1);
    printf("%d élève(s) sauvegardé(s)\n",nbeleves);
    // La sauvegarde a été faite => plus rien à sauvegarder
    a_sauvegarder=0 ; 
  }
} */

// ---Procédure de vérification de la sauvegarde---
void verif_sauvegarde()
{
  char reponse[20];
  
  if (a_sauvegarder)
  {
    printf("Des données ont été modifiées !\n");
    printf("Voulez-vous effectuer une sauvegarde ? (o/n) : ");
    scanf("%s",reponse);
    conv_maj(reponse);
    if (reponse[0] == 'O')
    {
      wip();
    }
  }
}

// ---Procédure quitter---
void quitter()
{
  // On vérifie si une sauvegarde doit être proposée
  verif_sauvegarde();
  // message de fin de session
  printf("Au revoir\n");
}

// --- procédure de conversion en majuscules ---
void conv_maj(char chaine[])
{
  int taille, i;

  taille = strlen(chaine);
  for (i=0 ; i< taille ; i++)
  {
    chaine[i] = toupper(chaine[i]);
  }
}

//---Procédure WIP pour les fonctions en cours de dev----------------
void wip()
{
    printf("Fonction en cours de développement :)\n");
}