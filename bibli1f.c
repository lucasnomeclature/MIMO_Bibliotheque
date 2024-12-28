#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Constantes
#define TAILLE_COTE 10
#define TAILLE_TITRE 20
#define TAILLE_AUTEUR 20
#define MAX_DOC 100
#define TAILLE_NOM_F 100
#define TAILLE_ID 10
#define TAILLE_STANDARD 20
#define oui "Oui"
#define non "Non"

// déclaration du type global
struct document 
{
    char id_doc[TAILLE_ID];
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
    int eligible_a_lemprunt_doc ; //bool
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
void chargement();
void verif_sauvegarde();
void sauvegarde();
void quitter();

// Proc et fonctions backoffice
void wip();
void conv_maj(char chaine[]);
char *itoa(int j);

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
    printf("|   -4- Sauvegarde                    |\n");
    printf("|   -5- Chargement                    |\n");
   /* printf("|   -6- Recherche                     |\n");
    printf("|   -7- Modification                  |\n");
    printf("|   -8- Suppression                   |\n");
    printf("|   -9- tri par nom                   |\n");
    printf("|   -10- tri par age                   |\n");  */
    printf("|                                     |\n");
    printf("|   -0- Quitter                       |\n");
    printf("✧-------------------------------------✧\n");
    printf("Choix : ");

    scanf("%d",&choix_menu);

    switch(choix_menu)
    {
      case 1 : wip()            ;
               break            ;
      
      case 2 : saisir_new()     ;
               break            ;
      case 3 : affichage()      ;
               break            ;
      case 4 : sauvegarde()     ;
               break            ;
      case 5 : chargement()     ;
               break            ;
      /*case 6 : lance_recherche();
               break            ;
      case 7 : modification()   ;
               break            ;
      case 8 : suppression()    ;
               break            ;
      case 9 : tri_insertion_nom(tabeleves,nbeleves);
               break            ;
      case 10 : tri_insertion_age(tabeleves,nbeleves);
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
  char reponse[TAILLE_STANDARD], saisie[TAILLE_STANDARD], identifiant[TAILLE_ID];
  int result, valide = 0, staff_souhaite_saisir_doc=1;

  while ((staff_souhaite_saisir_doc!=0) && (i<MAX_DOC))
  {
    printf("------------\n");
    printf("Souhaitez vous saisir un nouveau document (0 pour terminer): ");
    result=scanf("%d", &staff_souhaite_saisir_doc);
    while (!result)
    {
        printf("Erreur, merci de saisir un entier                           : ");
        fflush(stdin);
        result=scanf("%d", &staff_souhaite_saisir_doc);
    }
    
    if (staff_souhaite_saisir_doc!=0)
    {
      strcpy(doc.id_doc, "DOC"); //L'identifiant prend le préfixe DOC afin de le différencier des identifiants des usagers notamment
      strcat(doc.id_doc,(itoa(++nbdoc))); //Au préfixe DOC s'ajoute le nombre de document existants +1 
      printf("Identifiant                                                : "); 
      printf("%s\n", doc.id_doc);
    

      printf("Côte                                                       : ");
      scanf("%21s",doc.cote_doc);
      conv_maj(doc.cote_doc);

      printf("Titre                                                      : ");
      scanf("%21s",doc.titre_doc);
      conv_maj(doc.titre_doc);

      printf("Auteur                                                     : ");
      scanf("%21s",doc.auteur_doc);
      conv_maj(doc.auteur_doc);

      while (!valide) //saisie du prix avec vérification
      {
        valide = 1;  // On part du principe que la saisie est valide
        int nb_points = 0;

        printf("Prix                                                       : ");
        scanf("%19s", saisie);

        for (int j = 0; saisie[j] != '\0'; j++) 
        {
            if (saisie[j] == ',') //On vérifie si la saisie contient des virgules, si c'est le cas elles sont remplacées par des points
            {
                saisie[j] = '.';  // On remplace la virgule par un point
                nb_points++;
            } else if (!isdigit(saisie[j]) && saisie[j] != '.') //On vérifie si la saisie contient des caractères qui ne sont ni des chiffres ni des points
            {
                valide = 0;  // Contient un caractère non autorisé
                break;
            }
        }

        if (nb_points > 1) //On vérifie s'il y a plus d'une virugle ou un point ce qui voudrait dire que le format est invalide
        {
            valide = 0; 
        }

        if (!valide) 
        {
            printf("Erreur : saisie invalide. Essayez à nouveau.\n");
        }
      }
      doc.prix_doc = atof(saisie); //on convertie en float et on le range dans le type structuré
      valide=0; //on remet valide à 0 pour le prochain passsage
    
      printf("Eligibiligté à l'emrpunt (oui / non)                       : ");
      scanf("%19s",reponse);
      conv_maj(reponse);
      if ((strcmp((reponse), "OUI"))==0) //si la réponse est oui l'égibilité à l'emprunt prend la valeur 1 (true)
      {
        doc.eligible_a_lemprunt_doc= 1;
      }
      else
      {
        doc.eligible_a_lemprunt_doc= 0;
      }

      tabdoc[i++] = doc ;
      a_sauvegarder=1 ; // Suite à l'ajout d'un doc cela signifie qu'il y a des données à sauvegarder
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
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-21s %-21s %-21s %-21s %-21s %-21s\n","ID","COTE","TITRE", "AUTEUR", "PRIX", "EMPRUNTABLE");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      printf("%-21s %-21s %-21s %-21s %-21.2f",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, doc.prix_doc); //%21.2f pour le prix car sinon pas aligné
      if  (doc.eligible_a_lemprunt_doc)
      {
        printf("%-21s\n", oui );
      }
      else
      {
        printf("%-21s\n", non);
      }
    }  
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
  }
}

void chargement()
{
  // variables
  struct document doc;
  int i=0, retour;
  FILE *f1;
  char NomFichier[TAILLE_NOM_F];

  // On vérifie si une sauvegarde doit être proposée
  verif_sauvegarde();

  printf("Nom du fichier de chargement : ");
  scanf("%s",NomFichier);
  f1=fopen(NomFichier,"r");
  while ((!feof(f1)) && (i<MAX_DOC))
  {
    retour=fscanf(f1,"%s %s %s %s %f %d\n",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, &doc.prix_doc, &doc.eligible_a_lemprunt_doc);
    
    if (retour != EOF)
    {
      conv_maj(doc.cote_doc);
      conv_maj(doc.titre_doc);
      conv_maj(doc.auteur_doc);
      tabdoc[i++] = doc ;      
    }
  }
  fclose(f1);
  nbdoc = i;  
  printf("%d documents lu(s)\n",nbdoc);
}

// ---Procédure de sauvegarde ---
void sauvegarde()
{
  // variables
  struct document doc;
  int i=0;
  FILE *f1;
  char NomFichier[TAILLE_NOM_F];

  if (nbdoc == 0)
  {
    printf("Aucun document à sauvegarder\n");
  }
  else
  {
    printf("Nom du fichier de sauvegarde : ");
    scanf("%s",NomFichier);
    f1=fopen(NomFichier,"w");
    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
        fprintf(f1, "%-15s %-15s %-15s %-15s %-15f %-15d\n",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, doc.prix_doc, doc.eligible_a_lemprunt_doc);   
    } 
     fclose(f1);
    printf("%d documents(s) sauvegardé(s)\n",nbdoc);
    // La sauvegarde a été faite => plus rien à sauvegarder
    a_sauvegarder=0 ; 
  }
} 

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

// --- Fonction de convertion équivalente à sprintf() ---
// --- entier en chaine numérique ---
char *itoa(int j)
{
char chaine[100], *retour;
int taille;
/* création d'une chaine à partir de l'entier */
sprintf(chaine,"%d",j);
/* Construction d'une chaine de la taille de l'entier */
taille = strlen(chaine);
retour = (char *) malloc(taille+1);
strcpy(retour,chaine);
return retour;
}

//---Procédure WIP pour les fonctions en cours de dev----------------
void wip()
{
    printf("Fonction en cours de développement :)\n");
}