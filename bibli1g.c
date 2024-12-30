#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Constantes
#define TAILLE_COTE 10
#define TAILLE_TITRE 20
#define TAILLE_AUTEUR 20
#define TAILLE_CATEGORIE 20
#define TAILLE_EDITION 20
#define TAILLE_COLLECTION 20
#define TAILLE_FORMAT 20
#define TAILLE_ISBN13 18 //13 chiffres, 4 tirets, 1 caractère fin de ligne
#define MAX_DOC 100
#define TAILLE_NOM_F 100
#define TAILLE_ID 10
#define TAILLE_STANDARD 20
#define oui "Oui"
#define non "Non"
#define TAILLE_ELEMENTRECH 200
#define NON_TROUVE 0
#define TROUVE 1

// déclaration de types globaux
struct date
{
    int jour;
    int mois;
    int annee;
};

struct document 
{
    char id_doc[TAILLE_ID];
    char cote_doc[TAILLE_COTE];
    char titre_doc[TAILLE_TITRE];
    char auteur_doc[TAILLE_AUTEUR];
    char categorie[TAILLE_CATEGORIE];
    char edition[TAILLE_EDITION];
    struct date date_premiere_publi;
    char collection[TAILLE_COLLECTION];
    char format[TAILLE_FORMAT];
    char ISBN13[TAILLE_ISBN13]; //L'ISBN10 est exprimable au format ISBN13 en rajoutant 978 au début et en recalculant le chiffre de contrôle
    float prix_doc; 
    int eligible_a_lemprunt_doc ; //bool
    int eligible_a_consult; //bool
    int presence_rayons; //bool
    int existe; //bool
    struct date date_deb;
    struct date date_fin;
 }; 

// Déclaration des variables globales
struct document tabdoc[MAX_DOC] ;
int nbdoc=0;
char ElementRech[TAILLE_ELEMENTRECH];

// déclarations préliminaires
// Proc et fonctions frontoffice
void menu_staff();
void saisir_new();
void affichage_liste_doc();
void affichage_1_doc();
void chargement();
void verif_sauvegarde();
void sauvegarde();
void quitter();

// Proc et fonctions backoffice
void wip();
void conv_maj(char chaine[]);
char *itoa(int j);
int est_bissextile(int annee);
int date_valide(int jour, int mois, int annee);



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
    printf("|   -6- Afficher 1 doc                |\n");

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
      case 3 : affichage_liste_doc()      ;
               break            ;
      case 4 : sauvegarde()     ;
               break            ;
      case 5 : chargement()     ;
               break            ;
      case 6 : affichage_1_doc();
               break            ;
      /*case 7 : modification()   ;
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
      fflush(stdin);

      printf("Titre                                                      : ");
      scanf("%21s",doc.titre_doc);
      conv_maj(doc.titre_doc);
      fflush(stdin);

      printf("Auteur                                                     : ");
      scanf("%21s",doc.auteur_doc);
      conv_maj(doc.auteur_doc);
      fflush(stdin);

      printf("Catégorie                                                  : ");
      scanf("%21s",doc.categorie);
      conv_maj(doc.categorie);
      fflush(stdin);

      printf("Edition                                                    : ");
      scanf("%21s",doc.edition);
      conv_maj(doc.edition);
      fflush(stdin);

      while (!valide) 
      {
        printf("Date de publication (jj/mm/aaaa)                           : ");
        if (scanf("%d/%d/%d", &doc.date_premiere_publi.jour, &doc.date_premiere_publi.mois, &doc.date_premiere_publi.annee) != 3) // Vérification du format et lecture des données
        {
            printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }

        // Vérifier si la date est valide
        if (date_valide(doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee)) 
        {
            valide=1;
        } else 
        {
            printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
        }
     }
     valide=0;
     

      printf("Collection                                                 : ");
      scanf("%21s",doc.collection);
      conv_maj(doc.collection);
      fflush(stdin);

      printf("Format                                                     : ");
      scanf("%21s",doc.format);
      conv_maj(doc.format);
      fflush(stdin);

      printf("ISBN13                                                     : ");
      scanf("%18s",doc.ISBN13);
      fflush(stdin);

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
      fflush(stdin);

      printf("Eligibilité à l'emrpunt (oui / non)                        : ");
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

      printf("Eligibilité à la consultation (oui / non)                  : ");
      scanf("%19s",reponse);
      conv_maj(reponse);
      if ((strcmp((reponse), "OUI"))==0) //si la réponse est oui la variable prend la valeur 1 (true)
      {
        doc.eligible_a_consult= 1;
      }
      else
      {
        doc.eligible_a_consult= 0;
      }

      printf("Présent en rayons (oui / non)                              : ");
      scanf("%19s",reponse);
      conv_maj(reponse);
      if ((strcmp((reponse), "OUI"))==0) //si la réponse est oui la variable prend la valeur 1 (true)
      {
        doc.presence_rayons= 1;
      }
      else
      {
        doc.presence_rayons= 0;
      }

      doc.existe=1;

      //a voir si on utilise la formule qui permet de partir de la date du système
      printf("Date d'arrivée dans la bibliothèque (jj/mm/aaaa)           : ");
      scanf("%d/%d/%d",&doc.date_deb.jour,&doc.date_deb.mois,&doc.date_deb.annee);
      fflush(stdin);

      doc.date_fin.annee=0; //on initie une date de fin fausse pour ensuite pouvoir utiliser une condition et afficher NULL


      tabdoc[i++] = doc ;
      a_sauvegarder=1 ; // Suite à l'ajout d'un doc cela signifie qu'il y a des données à sauvegarder
    }
  }
  fflush(stdin);
  nbdoc = i; 
}

// ---Procédure d'affichage de tous les documents ---
void affichage_liste_doc()
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
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s\n","ID","COTE","TITRE", "AUTEUR", "DATE PUBLICATION", "ISBN13", "EMPRUNTABLE", "CONSULTABLE", "EXISTE (CHANGER)");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      printf("%-21s %-21s %-21s %-21s %02d/%02d/%-15d %-22s",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee, doc.ISBN13); //%02d/%02d/%-21d le %02d assure que le terminal affiche au format 02/09/1999 et pas 2/9/1999
      
      if  (doc.eligible_a_lemprunt_doc)
      {
        printf("%-22s", oui );
      }
      else
      {
        printf("%-22s", non);
      }

      if  (doc.eligible_a_consult)
      {
        printf("%-22s", oui );
      }
      else
      {
        printf("%-22s", non);
      }

      if  (doc.existe)
      {
        printf("%-22s\n", oui );
      }
      else
      {
        printf("%-22s\n", non);
      }
    }  
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  }
}

/*// ---Procédure d'affichage de tous les documents ---
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
    printf("%-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s\n","ID","COTE","TITRE", "AUTEUR", "CATEGORIE", "EDITION", "DATE PUBLICATION", "COLLECTION", "FORMAT", "ISBN13", "PRIX", "EMPRUNTABLE", "CONSULTABLE", "PRESENT EN RAYONS", "EXISTE (CHANGER)", "DATE DEBUT", "DATE FIN");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
//%-21d/%d/%d
    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      printf("%-21s %-21s %-21s %-21s %-21s %-21s %d/%d/%-21d %-21s %-21s %-21s %-21.2f",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, doc.categorie, doc.edition, doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee, doc.collection, doc.format, doc.ISBN13,  doc.prix_doc); //%21.2f pour le prix car sinon pas aligné
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
}*/

// ---Procédure d'affichage d'un document ---
void affichage_1_doc()
{
  // variables
  struct document doc;

  if (nbdoc == 0)
  {
    printf("Aucun document à afficher\n");
  }
  else
  {
    doc =tabdoc[nbdoc-1]; //car la première case du tableau est la case 0
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Identifiant                                   : %s\n", doc.id_doc);
    printf("Titre                                         : %s\n", doc.titre_doc);
    printf("Auteur                                        : %s\n", doc.auteur_doc);
    printf("Côte                                          : %s\n", doc.cote_doc);
    printf("Catégorie                                     : %s\n", doc.categorie);
    printf("Edition                                       : %s\n", doc.edition);
    printf("Date de publication                           : %02d/%02d/%d\n", doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee);
    printf("Collection                                    : %s\n", doc.collection);
    printf("Format                                        : %s\n", doc.format);
    printf("ISBN13                                        : %s\n", doc.ISBN13);
    printf("Prix                                          : %.2f\n", doc.prix_doc);
    printf("Empruntable                                   : ");
    if  (doc.eligible_a_lemprunt_doc)
      {
        printf("%s\n", oui );
      }
      else
      {
        printf("%-s\n", non);
      }
    printf("Consultable                                   : ");
    if  (doc.eligible_a_consult)
      {
        printf("%s\n", oui );
      }
      else
      {
        printf("%-s\n", non);
      }
    printf("Présent en rayons                             : ");
    if  (doc.presence_rayons)
      {
        printf("%s\n", oui );
      }
      else
      {
        printf("%-s\n", non);
      }
    printf("Existe (changer)                              : ");
    if  (doc.eligible_a_consult)
      {
        printf("%s\n", oui );
      }
      else
      {
        printf("%-s\n", non);
      }
    printf("Date d'entrée dans le réseau (changer)        : %02d/%02d/%d\n", doc.date_deb.jour, doc.date_deb.mois, doc.date_deb.annee);
    if (doc.date_fin.annee==0)
    {
        printf("Date de fin dans le réseau (changer)          : NA\n");
    }
    else
    {
        printf("Date de fin  dans le réseau (changer)           : %02d/%02d/%d\n", doc.date_fin.jour, doc.date_fin.mois, doc.date_fin.annee);
    }    
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");    
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

// Fonction pour vérifier si une année est bissextile
int est_bissextile(int annee)
{
    return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

// Fonction pour vérifier si une date est valide
int date_valide(int jour, int mois, int annee) 
{
    // Nombre de jours dans chaque mois
    int jours_par_mois[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
    
    if (mois < 1 || mois > 12) return 0; // Mois invalide
    if (annee < 1) return 0; // Année invalide
    
    // Vérification des jours en fonction du mois
    if (mois == 2 && est_bissextile(annee)) 
    {
        if (jour < 1 || jour > 29) return 0; // Février bissextile
    } else {
        if (jour < 1 || jour > jours_par_mois[mois - 1]) return 0; // Autres mois, [mois - 1] car la première case est 0 pas 1
    }
    return 1;
}

//---Procédure WIP pour les fonctions en cours de dev----------------
void wip()
{
    printf("Fonction en cours de développement :)\n");
}