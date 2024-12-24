#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Constantes
#define TAILLE_COTE 10
#define TAILLE_TITRE 100
#define TAILLE_AUTEUR 50
#define MAX_DOC 100

// déclaration du type global
struct document 
{
    int id_doc;
    char cote_doc[TAILLE_COTE];
    char titre_doc[TAILLE_TITRE];
    char auteur_doc[TAILLE_AUTEUR];
 /* char categorie[];
    char edition[];
    jsp date première pulication;
    char collection[];
    char format[];
    jsp ISBN-10;
    jsp ISBN-13;
 */
    float prix_doc; 
    int eligible_a_lemprunt_doc; //bool
 /*
    int eligible_a_consult //bool
    int presence_rayons; //bool
    int existe; //bool
    jsp date_deb;
    jsp date_fin;
 */
 }; 

// Déclaration des variables globales
struct document tabdoc[MAX_DOC] ;
int nbdoc=0;

// déclarations préliminaires
// Proc et fonctions frontoffice
void menu_staff();
void saisir_new();
void affichage();
void verif_sauvegarde();
void sauvegarde();
void quitter();

// Proc et fonctions backoffice
void wip();
void conv_maj(char chaine[]);

// Booléen indiquant si on doit faire une sauvegarde
int a_sauvegarder=0 ;


// --- Programme principal ----------------------------------------------
int main ()
{
    menu_staff();
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// Proc et fonctions frontoffice
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••


//--- Affichage du menu staff -------------------------------------------
void menu_staff()
{
    int choix_menu=-1 ;

  while (choix_menu != 0)
  {
    printf("✧--------MENU-------------------------✧\n");
    printf("|   -1- Rechercher                    |\n");
    printf("|   -2- Saisir un nouveau document    |\n");
    printf("|   -3- Afficher les documents        |\n");
   /* 
    printf("|   -3- Sauvegarde   |\n");
    printf("|   -4- Chargement                    |\n");
    printf("|   -5- Recherche                     |\n");
    printf("|   -6- Modification                  |\n");
    printf("|   -7- Suppression                   |\n");
    printf("|   -8- tri par nom                   |\n");
    printf("|   -9- tri par age                   |\n");  */
    printf("|                                     |\n");
    printf("|   -0- Quitter                       |\n");
    printf("✧-------------------------------------✧\n");
    printf("Choix : ");

    scanf("%d",&choix_menu);

    switch(choix_menu)
    {
      case 1 : wip()            ;
               break            ;
      
      case 2 : saisir_new()      ;
               break             ;
      case 3 : affichage()       ;
               break             ;
      /*case 3 : sauvegarde()     ;
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

// ---Procédure de saisie d'un nouveau document ---
void saisir_new() 
{
  // variables
  struct document doc;
  int i;
  i=nbdoc;

  // initialisation à la chaine vide
  //strcpy(eleve.nom,"");
  doc.id_doc=(1);

  while ((doc.id_doc!=0) && (i<MAX_DOC))
  {
    printf("------------\n");
    printf("identifiant (0 pour terminer): ");
    scanf("%d", &doc.id_doc);

    if (doc.id_doc!=0)
    {
      printf("côte                       : ");
      scanf("%s",doc.cote_doc);
      conv_maj(doc.cote_doc);

      printf("Titre                      : ");
      scanf("%s",doc.titre_doc);

      printf("Auteur                     : ");
      scanf("%s",doc.auteur_doc);

      printf("Prix                       : ");
      scanf("%f",&doc.prix_doc);

      tabdoc[i++] = doc ;
      // Ajout d'élèves => il y a des données à sauvegarder
      a_sauvegarder=1 ; 
    }
  }
  nbdoc = i; 
}

// ---Procédure d'affichage de tous les documents ---
void affichage()
{
  // variables
  struct document doc;
  int i=0;

  if (nbdoc == 0)
  {
    printf("Aucun document à afficher\n");
  }
  else
  {
    printf("----------------------------------------------\n");
    printf("%-15s %-15s %-15s %-15s %-15s\n","ID","COTE","TITRE", "AUTEUR", "PRIX", "ELIGIBLE A L'EMPRUNT");
    printf("----------------------------------------------\n");

    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      printf("%-15d %-15s %-15s %-15s %-15f %-15d\n",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, doc.prix_doc, doc.eligible_a_lemprunt_doc);
    }  
    printf("----------------------------------------------\n");
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

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// Proc et fonctions backoffice
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

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