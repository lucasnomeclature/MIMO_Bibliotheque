#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                         ---- Constantes ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//              ---- déclaration de types globaux ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//        ---- Déclaration des variables globales ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct document tabdoc[MAX_DOC] ;
int nbdoc=0;
char ElementRech[TAILLE_ELEMENTRECH];


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- déclarations préliminaires ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Proc et fonctions frontoffice ----
void menu_staff();
void saisir_new();
void affichage_liste_doc();
void affichage_1_doc();
void ModifierDoc();
void RechercheApproximative(char *ElementRech, struct document *doc);
void chargement();
void verif_sauvegarde();
void sauvegarde();
void quitter();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- déclarations préliminaires ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Proc et fonctions backoffice ----
void wip();
void conv_maj(char chaine[]);
void lire_chaine_espace(char chaine[]);
void gerer_espaces_multiples(char *chaine);
void gerer_espace_avant_apres(char *ch);
char *mon_itoa(int j);
int est_bissextile(int annee);
int date_valide(int jour, int mois, int annee);


// --- Booléen indiquant si on doit faire une sauvegarde ---
int a_sauvegarder=0 ;


// ---------------------------Programme principal ------------------------------
int main ()
{
    menu_staff();
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                 ---- Proc et fonctions frontoffice ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••


//------------------------- Affichage du menu staff -----------------------------
void menu_staff()
{
    int choix_menu=-1 ;

  while (choix_menu != 0)
  {
    printf("✧--------MENU-------------------------✧\n");
    printf("|   -1- Saisir un nouveau document    |\n");
    printf("|   -2- Afficher les documents        |\n");
    printf("|   -3- Afficher un document          |\n");
    printf("|   -4- Sauvegarde                    |\n");
    printf("|   -5- Chargement                    |\n");
    printf("|   -6- Rechercher un document        |\n");
    printf("|   -7- Tri par titre                 |\n");
    printf("|   -8- Modifier un document          |\n");
    printf("|   -9- Supprimer un document         |\n");
    printf("|  -10- Archiver un document          |\n");
   /*
    printf("|   -10- tri par age                   |\n");  */
    printf("|                                     |\n");
    printf("|   -0- Quitter                       |\n");
    printf("✧-------------------------------------✧\n");
    printf("Choix : ");

    scanf("%d",&choix_menu);

    switch(choix_menu)
    {
      case 1 : saisir_new()            ;
               break                   ;
      
      case 2 : affichage_liste_doc()   ;
               break                   ;
               
      case 3 : affichage_1_doc()       ;
               break                   ;
               
      case 4 : sauvegarde()            ;
               break                   ;
               
      case 5 : chargement()            ;
               break                   ;
               
      case 6 : RechercheApproximative (ElementRech,tabdoc);
               break                                      ;
      
      case 7 : wip()                   ;
               break                   ;
      
      case 8 : ModifierDoc()           ;
               break                   ;
      
      case 9 : wip()                   ;
               break                   ; 
    
      case 10 : wip()                  ;
                break                  ; 
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

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Procédure de saisie d'un nouveau document ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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
    fflush(stdin);
    while (!result)
    {
        printf("Erreur, merci de saisir un entier                           : ");
        result=scanf("%d", &staff_souhaite_saisir_doc);
        fflush(stdin);
    }

    if (staff_souhaite_saisir_doc!=0)
    {
      strcpy(doc.id_doc, "DOC"); //L'identifiant prend le préfixe DOC afin de le différencier des identifiants des usagers notamment
      strcat(doc.id_doc,(mon_itoa(++nbdoc))); //Au préfixe DOC s'ajoute le nombre de document existants +1 
      printf("Identifiant                                                : "); 
      printf("%s\n", doc.id_doc);
    

      printf("Côte                                                       : ");
      lire_chaine_espace(doc.titre_doc); //sait-on jamais qu'il y ait besoin de faire un espace
      gerer_espaces_multiples(doc.cote_doc);
      gerer_espace_avant_apres(doc.cote_doc);
      conv_maj(doc.cote_doc);
      

      printf("Titre                                                      : ");
      lire_chaine_espace(doc.titre_doc);
      gerer_espaces_multiples(doc.titre_doc);
      gerer_espace_avant_apres(doc.titre_doc);
      conv_maj(doc.titre_doc);
      

      printf("Auteur                                                     : ");
      lire_chaine_espace(doc.auteur_doc);
      gerer_espaces_multiples(doc.auteur_doc);
      gerer_espace_avant_apres(doc.auteur_doc);
      conv_maj(doc.auteur_doc);

      printf("Catégorie                                                  : ");
      lire_chaine_espace(doc.categorie);
      gerer_espaces_multiples(doc.categorie);
      gerer_espace_avant_apres(doc.categorie);
      conv_maj(doc.categorie);

      printf("Edition                                                    : ");
      lire_chaine_espace(doc.edition);
      gerer_espaces_multiples(doc.edition);
      gerer_espace_avant_apres(doc.edition);
      conv_maj(doc.edition);

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
     fflush(stdin);
     

      printf("Collection                                                 : ");
      lire_chaine_espace(doc.collection);
      gerer_espaces_multiples(doc.collection);
      gerer_espace_avant_apres(doc.collection);
      conv_maj(doc.collection);

      printf("Format                                                     : ");
      lire_chaine_espace(doc.format);
      gerer_espaces_multiples(doc.format);
      gerer_espace_avant_apres(doc.format);
      conv_maj(doc.format);

      printf("ISBN13                                                     : ");
      scanf("%18s",doc.ISBN13); // je met pas la fonction lire chaine car à priori pas besoin d'espaces
      gerer_espace_avant_apres(doc.ISBN13); // par contre on gère les espaces avant après

      while (!valide) //saisie du prix avec vérification
      {
        valide = 1;  // On part du principe que la saisie est valide
        int nb_points = 0;

        printf("Prix                                                       : ");
        scanf("%19s", saisie);
        gerer_espace_avant_apres(saisie);

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
      gerer_espace_avant_apres(reponse);
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
      gerer_espace_avant_apres(reponse);
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
      gerer_espace_avant_apres(reponse);
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//             ---- Procédure d'affichage de tous les documents ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//             ----Procédure d'affichage d'un document ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                ---- Procédure de modification ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void ModifierDoc()
{
    int i, id_indice, choix_modif;
    char id[TAILLE_ID], reponse[TAILLE_STANDARD];

    printf("Saisissez l'ID du document à rechercher : ");
    scanf("%s", id);

    id_indice = -1; 
    for (int i = 0; i < nbdoc; i++) 
	{
		if (strcmp(tabdoc[i].id_doc, id)==0) 
		{
            id_indice = i;
        }
	}

    if (id_indice != -1) 
	{
        printf("Document trouvé :\n");
        affichage_1_doc(id_indice);
        
        choix_modif=-1; // On initialise à tout sauf 0 pour rentrer dans la boucle
        
        while (choix_modif!=0)
        {
        	printf("Que souhaitez-vous modifier ?\n"); 
        	printf("-1- Le titre\n"); 
        	printf("-2- L'auteur\n"); 
        	printf("-3- Le prix\n"); 
            printf("-4- La cote\n"); 
            printf("-5- La catégorie\n"); 
            printf("-6- Le format\n"); 
            printf("-7- L'édition\n"); 
            printf("-8- La collection\n"); 
            printf("-9- L'éligibilité à l'emprunt\n"); 
            printf("-10- L'éligibilité à la consultation\n");
            printf("-11- La présence en rayon\n");
        	printf("           \n");
        	printf("-0- Quitter la modification\n"); 
        	printf("Votre choix : "); 
        	scanf("%d",&choix_modif);
        	fflush(stdin);
        	
        	switch(choix_modif)
        	{
        		case 1 : 
        		 printf("Nouveau titre :"); 
        		 lire_chaine_espace(tabdoc[id_indice].titre_doc);
        		 gerer_espaces_multiples(tabdoc[id_indice].titre_doc);
                 gerer_espace_avant_apres(tabdoc[id_indice].titre_doc);
                 conv_maj(tabdoc[id_indice].titre_doc);
                 affichage_1_doc(tabdoc[id_indice]);
                 break; 
                case 2 : 
                 printf("Nouvel auteur :"); 
        		 lire_chaine_espace(tabdoc[id_indice].auteur_doc);
        		 gerer_espaces_multiples(tabdoc[id_indice].auteur_doc);
                 gerer_espace_avant_apres(tabdoc[id_indice].auteur_doc);
                 conv_maj(tabdoc[id_indice].auteur_doc);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 3 : 
                 printf("Nouveau prix :"); 
        		 scanf("%2f",&tabdoc[id_indice].prix_doc); 
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 4 : 
                 printf("Nouvelle côte :"); 
        		 scanf("%s",tabdoc[id_indice].cote_doc);
        		 gerer_espace_avant_apres(tabdoc[id_indice].cote_doc);
        		 conv_maj(tabdoc[id_indice].cote_doc);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 5 : 
                 printf("Nouvelle catégorie :"); 
        		 lire_chaine_espace(tabdoc[id_indice].categorie);
        		 gerer_espaces_multiples(tabdoc[id_indice].categorie);
                 gerer_espace_avant_apres(tabdoc[id_indice].categorie);
                 conv_maj(tabdoc[id_indice].categorie);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 6 : 
                 printf("Nouveau format :"); 
        		 lire_chaine_espace(tabdoc[id_indice].format);
        		 gerer_espaces_multiples(tabdoc[id_indice].format);
                 gerer_espace_avant_apres(tabdoc[id_indice].format);
                 conv_maj(tabdoc[id_indice].format);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 7 : 
                 printf("Nouvelle édition :"); 
                 lire_chaine_espace(tabdoc[id_indice].edition);
        		 gerer_espaces_multiples(tabdoc[id_indice].edition);
                 gerer_espace_avant_apres(tabdoc[id_indice].edition);
                 conv_maj(tabdoc[id_indice].edition);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 8 : 
                 printf("Nouvelle collection :"); 
                 lire_chaine_espace(tabdoc[id_indice].collection);
        		 gerer_espaces_multiples(tabdoc[id_indice].collection);
                 gerer_espace_avant_apres(tabdoc[id_indice].collection);
                 conv_maj(tabdoc[id_indice].collection);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 9 : 
                 printf("Le document est-il éligible à l'emprunt ? (oui/non) :");
                 scanf("%s",reponse); 
                 gerer_espace_avant_apres(reponse);
                 conv_maj(reponse);
                 if ((strcmp((reponse), "OUI"))==0) //si la réponse est oui l'égibilité à l'emprunt prend la valeur 1 (true)
                 {
                    tabdoc[id_indice].eligible_a_lemprunt_doc = 1;
                 }
                 else
                 {
                   tabdoc[id_indice].eligible_a_lemprunt_doc= 0;
                 }
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 10 : 
                 printf("Le document est-il consultable ? (oui/non) :"); 
                 scanf("%s",reponse);
				 gerer_espace_avant_apres(reponse); 
				 conv_maj(reponse);
                 if ((strcmp((reponse), "OUI"))==0) //si la réponse est oui la variable prend la valeur 1 (true)
                 {
                    tabdoc[id_indice].eligible_a_consult = 1;
                 }
                 else
                 {
                    tabdoc[id_indice].eligible_a_consult = 0;
                 }
                 affichage_1_doc(tabdoc[id_indice]);
                 break; 
                case 11 : 
                 printf("Le document est-il présent en rayon ? (oui/non) : ");
                 scanf("%s",reponse); 
                 gerer_espace_avant_apres(reponse);
                 conv_maj(reponse);
                 if ((strcmp((reponse), "OUI"))==0) //si la réponse est oui la variable prend la valeur 1 (true)
                 {
                   tabdoc[id_indice].presence_rayons = 1; 
                 }
                 else
                 {
                   tabdoc[id_indice].presence_rayons = 0; 
                 }
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 0 : printf("Modifications effectuées et enregistrées !\n");
                 break; 
                default : printf("choix erroné.\n"); 
                 break;
                
        		 
			}  
		}
    } 
	else 
	{
        printf("Aucun document trouvé avec l'ID %s.\n", id);
    }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//           ---- Procédure de recherche approximative ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void RechercheApproximative(char *ElementRech, struct document *doc)
{
	/*Déclaration des variables locales*/
    int correspondance = 0;
    int i;
    char titre_maj[TAILLE_TITRE], auteur_maj[TAILLE_AUTEUR], categorie_maj[TAILLE_CATEGORIE], edition_maj[TAILLE_EDITION], collection_maj[TAILLE_COLLECTION];

    /*---Obligée de vider le buffer, sinon il ne me laisse pas entrer une recherche---*/
    fflush(stdin);
    
    /*---On entre un élément à rechercher, que ce soit un titre ou un auteur (possibilité d'élargir si besoin)---*/
    printf("Entrez un élément à rechercher (titre, auteur, édition, collection) : ");
    fflush(stdin);
    lire_chaine_espace(ElementRech);

    /*---Cette condition me sert juste à m'assurer que l'élément saisi est bien rangé dans la variable ElementRech car sinon la procédure de recherche marche pas---*/
    if (strlen(ElementRech) == 0)
    {
        printf("Erreur : aucun élément saisi. Veuillez réessayer.\n");
    }
    else
    {
    	/*--- On rentre dans le dur : pour pouvoir bien tout comparer, il faut que l'élément recherché soit comparable par rapport à notre titre/auteur.
        j'applique donc les mêmes procédures qu'à nos titres et auteurs pour que la procédure puisse bien comparer---*/
      gerer_espaces_multiples(ElementRech);
      gerer_espace_avant_apres(ElementRech);
      conv_maj(ElementRech);

      for (i = 0; i < nbdoc; i++)
      {
       /*---Pour pas toucher aux titre et auteur de base qui ne s'écrivent pas tout en majuscule... jsp si on garde.. à tester sans---*/
        strcpy(titre_maj, doc[i].titre_doc);
        strcpy(auteur_maj, doc[i].auteur_doc);
        /*strcpy(categorie_maj, doc[i].categorie);
        strcpy(edition_maj, doc[i].edition);
        strcpy(collection_maj, doc[i].collection);*/
        
        gerer_espaces_multiples(titre_maj);
        gerer_espace_avant_apres(titre_maj);
        conv_maj(titre_maj);

        gerer_espaces_multiples(auteur_maj);
        gerer_espace_avant_apres(auteur_maj);
        conv_maj(auteur_maj);
        
        /*gerer_espaces_multiples(categorie_maj);
        gerer_espace_avant_apres(categorie_maj);
        conv_maj(categorie_maj);
        
        gerer_espaces_multiples(edition_maj);
        gerer_espace_avant_apres(edition_maj);
        conv_maj(edition_maj);
        
        gerer_espaces_multiples(collection_maj);
        gerer_espace_avant_apres(collection_maj);
        conv_maj(collection_maj);*/
        
        
        /*---La fonction strstr me permet de vérifier s'il existe une correspondance entre la chaine auteur_maj et la sous_chaine ElementRech (notre saisie d'un élément
        à rechercher). Si c'est différent de null ça veut dire qu'il y a une correspondance entre les 2 ---*/
          if ((strstr(titre_maj, ElementRech) != NULL ) || (strstr(auteur_maj, ElementRech) != NULL))
          {
            correspondance = 1;
		    printf("Document trouvé : [ID : %s], Côte : %s\n, Titre : %s\n, Auteur : ",doc[i].id_doc, doc[i].cote_doc, doc[i].titre_doc, doc[i].auteur_doc;
          }
      }
	}

    if (!correspondance)
    {
        printf("Aucun document trouvé avec l'élément saisi '%s'\n", ElementRech);
    }
} 


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                    ---- Procédure de chargement ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••                 
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
      
      //Nettoyage des espaces indésirables 
      gerer_espace_avant_apres(doc.cote_doc);
      
      gerer_espaces_multiples(doc.titre_doc);
      gerer_espace_avant_apres(doc.titre_doc);
      
      gerer_espaces_multiples(doc.auteur_doc);
      gerer_espace_avant_apres(doc.auteur_doc);
      
      tabdoc[i++] = doc ;      
    }
  }
  fclose(f1);
  nbdoc = i;  
  printf("%d documents lu(s)\n",nbdoc);
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                      ----Procédure de sauvegarde ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//         ----Procédure de vérification de la sauvegarde ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                          ---- Procédure quitter ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure de conversion en majuscules ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void conv_maj(char chaine[])
{
  int taille, i;

  taille = strlen(chaine);
  for (i=0 ; i< taille ; i++)
  {
    chaine[i] = toupper(chaine[i]);
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// --- Procédure de gestion des espaces (prise dans le poly et adaptée au projet) ---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void lire_chaine_espace(char chaine[])
{
  //Déclaration des variables locales 
  int i; 
  char lettre; 
  
  i=0; 
  while ((lettre=getchar())!='\n')
  {
  	chaine[i++] = lettre; 
  }
  chaine[i]='\0';
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//---Procédure de gestion des espaces multiples (prise dans le poly et adaptée au projet) ---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void gerer_espaces_multiples(char *chaine)
{
	//Délaration des variables locales 
	int i,j,taille,espace_rencontre; 
	char chaine1[200]; 
	
	i=0; 
	j=0; 
	espace_rencontre=0; 
	
	if(strcmp(chaine,"")!=0)
	{
		taille=strlen(chaine); 
		
		for (i=0;i<taille;i++)
		{
			if(chaine[i]!=' ')
			{
				chaine1[j++]=chaine[i]; 
				espace_rencontre=0; 
			}
			else 
			{
				if(!espace_rencontre)
				{
					chaine1[j++]=chaine[i];
				}
			}
		}
		chaine1[j]='\0'; 
		strcpy(chaine,chaine1);
	}
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//--- Procédure de gestion des espaces avant et après une chaine de caractère (prise dans le poly) ---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void gerer_espace_avant_apres(char *ch)
{
	//Déclaration des variables locales 
	char chaine1[200]; 
	int i,j,k,taille; 
	
	i=0; 
	j=0; 
	k=0; 
	
  if (strcmp(ch,"") != 0) /* test si la chaine n'est pas vide */
  {
/* --- suppression des espaces en début de chaine --- */
    while (isspace(ch[i++])); /* on passe les espaces du début */
    taille = strlen(ch);
/* recopie des caractères APRES les espaces */
    for (j=i-1 ; j<taille ; j++) chaine1[k++] = ch[j];
    chaine1[k++] = '\0' ; /* on finalise la chaine */
    strcpy(ch,chaine1) ; /* recopie la chaine traitée dans ch */
/* --- suppression des espaces en fin de chaine --- */
    i = strlen(ch);
    while (isspace(ch[--i])); /* on passe les espaces */
    ch[++i] = '\0' ; /* on finalise la chaine */
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//        --- Fonction de convertion équivalente à sprintf() ---
//               --- entier en chaine numérique ---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
char *mon_itoa(int j)
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//      ---- Fonction pour vérifier si une année est bissextile ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Procédure WIP pour les fonctions en cours de dev ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void wip()
{
    printf("Fonction en cours de développement :)\n");
}