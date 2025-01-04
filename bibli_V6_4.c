#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <wchar.h> /* pour le type wchar_t */
#include <locale.h> /* pour setlocale UTF8 */

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
#define TAILLE_ELEMENTRECH 200
#define NON_TROUVE 0
#define TROUVE 1
#define NON_EMPRUNTE -1
#define EMPRUNTE 1

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
    char id[TAILLE_ID];
    wchar_t cote[TAILLE_COTE];
    wchar_t titre[TAILLE_TITRE];
    wchar_t auteur[TAILLE_AUTEUR];
    wchar_t categorie[TAILLE_CATEGORIE];
    wchar_t edition[TAILLE_EDITION];
    struct date date_premiere_publi;
    wchar_t collection[TAILLE_COLLECTION];
    wchar_t format[TAILLE_FORMAT];
    wchar_t ISBN13[TAILLE_ISBN13]; //L'ISBN10 est exprimable au format ISBN13 en rajoutant 978 au début et en recalculant le chiffre de contrôle
    float prix; 
    int eligible_a_lemprunt ; //bool
    int est_emprunte ; //bool
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
wchar_t ElementRech[TAILLE_ELEMENTRECH];


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- déclarations préliminaires ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Proc et fonctions frontoffice ----
void menu_staff();
void saisir_new();
void affichage_liste_doc();
void affichage_1_doc();
void ModifierDoc();
void SupprDoc();
void RechercheApproximative(wchar_t *ElementRech, struct document *doc);
void chargement();
void verif_sauvegarde();
void sauvegarde();
void quitter();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- déclarations préliminaires ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Proc et fonctions backoffice ----
void wip();
void conv_maj_ac_accents(wchar_t chaine[]);
void conv_maj_ss_accents(char chaine[]);
void lire_chaine_espace(wchar_t chaine[]);
void gerer_espaces_multiples(wchar_t *chaine);
void gerer_espace_avant_apres(wchar_t *ch);
char *mon_itoa(int j);
int est_bissextile(int annee);
int date_valide(int jour, int mois, int annee);
int reponse_question_fermee_en_int(wchar_t reponse[TAILLE_STANDARD]);
wchar_t *int_a_reponse_question_fermee(int reponse);


// --- Booléen indiquant si on doit faire une sauvegarde ---
int a_sauvegarder=0 ;


// ---------------------------Programme principal ------------------------------
int main ()
{
    /* positionnement de la table de codage */
    //setlocale (LC_ALL, L"") ; /* valeur selon environnement */
    setlocale (LC_ALL, "fr_FR.UTF-8") ; /* Valeur a UTF-8 */
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
    wprintf(L"✧--------MENU-------------------------✧\n");
    wprintf(L"|   -1- Saisir un nouveau document    |\n");
    wprintf(L"|   -2- Afficher tous les documents   |\n");
    wprintf(L"|   -3- Afficher un document          |\n");
    wprintf(L"|   -4- Rechercher un document        |\n");
    wprintf(L"|   -5- Modifier un document          |\n");
    wprintf(L"|   -6- Archiver un document          |\n");
    wprintf(L"|   -7- Supprimer un document         |\n");
    wprintf(L"|                                     |\n");
    wprintf(L"|   -8- Sauvegarder                   |\n");
    wprintf(L"|   -9- Charger un fichier            |\n");
    wprintf(L"|                                     |\n");
    wprintf(L"|  -10- Emprunt d'un document         |\n");
    wprintf(L"|  -11- Retour d'un document          |\n"); 
    wprintf(L"|  -12- Suivi des emprunts            |\n"); 
    wprintf(L"|                                     |\n"); 
    wprintf(L"|  -13- Statistiques                  |\n"); 
   /*
    wprintf(L"|   -10- tri par age                   |\n");  */
    wprintf(L"|                                     |\n");
    wprintf(L"|   -0- Quitter                       |\n");
    wprintf(L"✧-------------------------------------✧\n");
    wprintf(L"Choix : ");

    wscanf(L"%d",&choix_menu);

    switch(choix_menu)
    {
      case 1 : saisir_new()                               ;
               break                                      ;
      
      case 2 : affichage_liste_doc()                      ;
               break                                      ;
               
      case 3 : affichage_1_doc()                          ;
               break                                      ;
               
      case 4 : RechercheApproximative (ElementRech,tabdoc);
               break                                      ;
               
      case 5 : wip()                              ;
               break                                      ;
               
      case 6 : wip()                                      ;
               break                                      ;
      
      case 7 : SupprDoc()                                 ;
               break                                      ;
      
      case 8 : sauvegarde()                               ;
               break                                      ;
      
      case 9 : chargement()                               ;
               break                                      ; 
    
      case 10 : wip()                                     ;
                break                                     ; 
      
      case 11 : wip()                                     ;
                break                                     ;
      
      case 12 : wip()                                     ;
                break                                     ;
      
      case 13 : wip()                                     ;
                break                                     ;
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
      default : wprintf(L"Choix erroné\n");
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
  wchar_t reponse[TAILLE_STANDARD], saisie[TAILLE_STANDARD], identifiant[TAILLE_ID];
  int result, valide = 0, staff_souhaite_saisir_doc=1;

  while ((staff_souhaite_saisir_doc!=0) && (i<MAX_DOC))
  {
    wprintf(L"------------\n");
    wprintf(L"Souhaitez vous saisir un nouveau document (0 pour terminer): ");
    result=wscanf(L"%d", &staff_souhaite_saisir_doc);
    fflush(stdin);
    while (!result)
    {
        wprintf(L"Erreur, merci de saisir un entier                          : ");
        result=wscanf(L"%d", &staff_souhaite_saisir_doc);
        fflush(stdin);
    }

    if (staff_souhaite_saisir_doc!=0)
    {
      strcpy(doc.id, "DOC"); //L'identifiant prend le préfixe DOC afin de le différencier des identifiants des usagers notamment
      strcat(doc.id,(mon_itoa(++nbdoc))); //Au préfixe DOC s'ajoute le nombre de document existants +1 
      printf("Identifiant                                                : "); 
      printf("%s\n", doc.id);
    

      wprintf(L"Côte                                                       : ");
      lire_chaine_espace(doc.cote); //sait-on jamais qu'il y ait besoin de faire un espace
      gerer_espaces_multiples(doc.cote);
      gerer_espace_avant_apres(doc.cote);
      conv_maj_ac_accents(doc.cote);
      

      wprintf(L"Titre                                                      : ");
      lire_chaine_espace(doc.titre);
      gerer_espaces_multiples(doc.titre);
      gerer_espace_avant_apres(doc.titre);
      conv_maj_ac_accents(doc.titre);
      

      wprintf(L"Auteur                                                     : ");
      lire_chaine_espace(doc.auteur);
      gerer_espaces_multiples(doc.auteur);
      gerer_espace_avant_apres(doc.auteur);
      conv_maj_ac_accents(doc.auteur);

      wprintf(L"Catégorie                                                  : ");
      lire_chaine_espace(doc.categorie);
      gerer_espaces_multiples(doc.categorie);
      gerer_espace_avant_apres(doc.categorie);
      conv_maj_ac_accents(doc.categorie);

      wprintf(L"Edition                                                    : ");
      lire_chaine_espace(doc.edition);
      gerer_espaces_multiples(doc.edition);
      gerer_espace_avant_apres(doc.edition);
      conv_maj_ac_accents(doc.edition);

      while (!valide) 
      {
        doc.date_premiere_publi.jour=-1; doc.date_premiere_publi.mois=-1; doc.date_premiere_publi.annee=-1; //On met une valeur fausse, en effet après le premier passage, si la saisie est fausse, la fonction date_valide va se faire sur les valeurs rentrées lors de l'itération précédente ce qui est source d'erreur
        wprintf(L"Date de publication (jj/mm/aaaa)                           : ");
        if (wscanf(L"%d/%d/%d", &doc.date_premiere_publi.jour, &doc.date_premiere_publi.mois, &doc.date_premiere_publi.annee) != 3) // Vérification du format et lecture des données
        {
            wprintf(L"Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }

        // Vérifier si la date est valide
        if (date_valide(doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee)) 
        {
            valide=1;
        } else 
        {
            wprintf(L"Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }
     }
     valide=0;
     fflush(stdin);
     

      wprintf(L"Collection                                                 : ");
      lire_chaine_espace(doc.collection);
      gerer_espaces_multiples(doc.collection);
      gerer_espace_avant_apres(doc.collection);
      conv_maj_ac_accents(doc.collection);

      wprintf(L"Format                                                     : ");
      lire_chaine_espace(doc.format);
      gerer_espaces_multiples(doc.format);
      gerer_espace_avant_apres(doc.format);
      conv_maj_ac_accents(doc.format);

      while (!valide) //saisie de l'ISBN avec vérification
      {
        valide = 1;  // On part du principe que la saisie est valide
        
        wprintf(L"ISBN13                                                     : ");
        wscanf(L"%18s", saisie); // je met pas la fonction lire chaine car à priori pas besoin d'espaces
        fflush(stdin);

        for (int j = 0; saisie[j] != '\0'; j++) 
        {
            if (!iswdigit(saisie[j]) && saisie[j] != '-') //On vérifie si la saisie contient des caractères qui ne sont ni des chiffres ni des tirets
            {
                valide = 0;  // Contient un caractère NON autorisé
            }
        }
        if (!valide)
        { 
            wprintf(L"Erreur : saisie invalide. Essayez à nouveau (uniquement des chiffres et des tirets). \n");
        }
      } 
      wcscpy(doc.ISBN13,saisie); 
      gerer_espace_avant_apres(doc.ISBN13); // par contre on gère les espaces avant après
      valide=0; //on remet valide à 0 pour le prochain passsage
      fflush(stdin);

      /*while (!valide) //saisie du prix avec vérification
      {
        valide = 1;  // On part du principe que la saisie est valide
        int nb_points = 0;

        wprintf(L"Prix                                                       : ");
        wscanf(L"%19s", saisie);

        for (int j = 0; saisie[j] != '\0'; j++) 
        {
            if (saisie[j] == ',') //On vérifie si la saisie contient des virgules, si c'est le cas elles sont remplacées par des points
            {
                saisie[j] = '.';  // On remplace la virgule par un point
                nb_points++;
            } 
            else if (saisie[j] == '.') //on vérifie le nombre de points préexistants
            {
                nb_points++;
            }
            else if (!iswdigit(saisie[j])) //On vérifie si la saisie contient des caractères qui ne sont ne sont pas des chiffres, les points ont été testés juste avant
            {
                valide = 0;  // Contient un caractère NON autorisé
            }
        }
        if (nb_points > 1) //On vérifie s'il y a plus d'une virugle ou un point ce qui voudrait dire que le format est invalide
        {
            valide = 0; 
        }

        if (!valide) 
        {
            wprintf(L"Erreur : saisie invalide. Essayez à nouveau.\n");
        }
      }
      doc.prix = atof(saisie); //on convertie en float et on le range dans le type structuré
      valide=0; //on remet valide à 0 pour le prochain passsage
      fflush(stdin); */

      while (!valide) // saisie du prix avec vérification
      {
        valide = 1; // On part du principe que la saisie est valide
        int prix;

        wprintf(L"Prix (ex: 123.45)                                          : ");
    
        if (wscanf(L"%f", &prix) != 1) // Si la saisie échoue (caractères invalides)
        {
          valide = 0; // Saisie invalide
          wprintf(L"Erreur : format invalide. Essayez à nouveau.\n");
          fflush(stdin);
        }
        else if (prix < 0) // Vérifier que le prix est positif
        {
          valide = 0;
          wprintf(L"Erreur : le prix ne peut pas être négatif. Essayez à nouveau.\n");
        }

        if (valide) 
        {
          doc.prix = prix; // Stocker le prix dans la structure si tout est valide
        }
      }
      valide = 0; // Réinitialiser pour d'autres saisies

      doc.eligible_a_lemprunt=2; //on l'initie à la valeur 2 qui est la valeur d'erreur
      while (doc.eligible_a_lemprunt==2) //tant que la variable vaut 2 (valeur d'erreur) la boucle continue
      {
        wprintf(L"Eligibilité à l'emprunt (oui / non)                        : ");
        lire_chaine_espace(reponse);
        doc.eligible_a_lemprunt=reponse_question_fermee_en_int(reponse); //Prendra la valeur 1,0 ou 2 en fonction de la validité de la chaine rentrée
        if (doc.eligible_a_lemprunt==2)
        {
          wprintf(L"Erreur, merci de répondre par oui ou par non.\n"); //Si la chaine rentrée est fausse un message d'erreur s'affiche
        }
      }

      doc.eligible_a_consult=2;
      while (doc.eligible_a_consult==2)
      {
        wprintf(L"Eligibilité à la consultation (oui / non)                  : ");
        lire_chaine_espace(reponse);
        doc.eligible_a_consult=reponse_question_fermee_en_int(reponse);
        if (doc.eligible_a_consult==2)
        {
          wprintf(L"Erreur, merci de répondre par oui ou par non.\n");
        }
      }

      doc.presence_rayons=2;
      while (doc.presence_rayons==2)
      {
        wprintf(L"Présent en rayons (oui / non)                              : ");
        lire_chaine_espace(reponse);
        doc.presence_rayons=reponse_question_fermee_en_int(reponse);
        if (doc.presence_rayons==2)
        {
          wprintf(L"Erreur, merci de répondre par oui ou par non.\n");
        }
      }

      doc.existe=1;

      //a voir si on utilise la formule qui permet de partir de la date du système
      while (!valide) 
      {
        doc.date_deb.jour=-1; doc.date_deb.mois=-1; doc.date_deb.annee=-1; //On met une valeur fausse, en effet après le premier passage, si la saisie est fausse, la fonction date_valide va se faire sur les valeurs rentrées lors de l'itération précédente ce qui est source d'erreur
        wprintf(L"Date d'arrivée dans la bibliothèque (jj/mm/aaaa)           : ");
        if (wscanf(L"%d/%d/%d", &doc.date_deb.jour, &doc.date_deb.mois, &doc.date_deb.annee) != 3) // Vérification du format et lecture des données
        {
            wprintf(L"Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }

        // Vérifier si la date est valide
        if (date_valide(doc.date_deb.jour, doc.date_deb.mois, doc.date_deb.annee)) 
        {
            valide=1;
        } else 
        {
            wprintf(L"Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }
     }
     valide=0;
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
    wprintf(L"Aucun document à afficher\n");
  }
  else
  {
    wprintf(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    wprintf(L"%-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s\n",L"ID",L"COTE",L"TITRE", L"AUTEUR", L"DATE PUBLICATION", L"ISBN13", L"EMPRUNTABLE", L"CONSULTABLE", L"EXISTE (CHANGER)");
    wprintf(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      wprintf(L"%-21s %-21s %-21s %-21s %02d/%02d/%-15d %-22s %-22s %-22s %-22s\n",doc.id, doc.cote, doc.titre, doc.auteur, doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee, doc.ISBN13, int_a_reponse_question_fermee(doc.eligible_a_lemprunt), int_a_reponse_question_fermee(doc.eligible_a_consult), int_a_reponse_question_fermee(doc.presence_rayons)); //%02d/%02d/%-21d le %02d assure que le terminal affiche au format 02/09/1999 et pas 2/9/1999, la fonction int_a_reponse_question_fermee affiche OUI, NON ou OUT (erreur) enfonction de la valeur du int (bool)
      wprintf(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
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
    wprintf(L"Aucun document à afficher\n");
  }
  else
  {
    wprintf(L"----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    wprintf(L"%-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s\n",L"ID",L"COTE",L"TITRE", L"AUTEUR", L"CATEGORIE", L"EDITION", L"DATE PUBLICATION", L"COLLECTION", L"FORMAT", L"ISBN13", L"PRIX", L"EMPRUNTABLE", L"CONSULTABLE", L"PRESENT EN RAYONS", L"EXISTE (CHANGER)", L"DATE DEBUT", L"DATE FIN");
    wprintf(L"----------------------------------------------------------------------------------------------------------------------------------------------------\n");
//%-21d/%d/%d
    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      wprintf(L"%-21s %-21s %-21s %-21s %-21s %-21s %d/%d/%-21d %-21s %-21s %-21s %-21.2f",doc.id, doc.cote, doc.titre, doc.auteur, doc.categorie, doc.edition, doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee, doc.collection, doc.format, doc.ISBN13,  doc.prix); //%21.2f pour le prix car sinon pas aligné
      if  (doc.eligible_a_lemprunt)
      {
        wprintf(L"%-21s\n", oui );
      }
      else
      {
        wprintf(L"%-21s\n", non);
      }
    }  
    wprintf(L"----------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
    wprintf(L"Aucun document à afficher\n");
  }
  else
  {
    doc =tabdoc[nbdoc-1]; //car la première case du tableau est la case 0
    wprintf(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    wprintf(L"Identifiant                                   : %s\n", doc.id);
    wprintf(L"Titre                                         : %s\n", doc.titre);
    wprintf(L"Auteur                                        : %s\n", doc.auteur);
    wprintf(L"Côte                                          : %s\n", doc.cote);
    wprintf(L"Catégorie                                     : %s\n", doc.categorie);
    wprintf(L"Edition                                       : %s\n", doc.edition);
    wprintf(L"Date de publication                           : %02d/%02d/%d\n", doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee);
    wprintf(L"Collection                                    : %s\n", doc.collection);
    wprintf(L"Format                                        : %s\n", doc.format);
    wprintf(L"ISBN13                                        : %s\n", doc.ISBN13);
    wprintf(L"Prix                                          : %.2f\n", doc.prix);
    wprintf(L"Empruntable                                   : %s\n", int_a_reponse_question_fermee(doc.est_emprunte));
    wprintf(L"Consultable                                   : %s\n", int_a_reponse_question_fermee(doc.eligible_a_lemprunt));
    wprintf(L"Présent en rayons                             : %s\n", int_a_reponse_question_fermee(doc.presence_rayons));
    wprintf(L"Existe (changer)                              : %s\n", int_a_reponse_question_fermee(doc.existe));
    wprintf(L"Date d'entrée dans le réseau (changer)        : %02d/%02d/%d\n", doc.date_deb.jour, doc.date_deb.mois, doc.date_deb.annee);
    if (doc.date_fin.annee==0)
    {
        wprintf(L"Date de fin dans le réseau (changer)          : NA\n");
    }
    else
    {
        wprintf(L"Date de fin  dans le réseau (changer)           : %02d/%02d/%d\n", doc.date_fin.jour, doc.date_fin.mois, doc.date_fin.annee);
    }    
    wprintf(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");    
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                ---- Procédure de modification ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*void ModifierDoc()
{
    int i, id_indice, choix_modif;
    wchar_t id[TAILLE_ID], reponse[TAILLE_STANDARD];

    wprintf(L"Saisissez l'ID du document à rechercher : ");
    wscanf(L"%s", id);

    id_indice = -1; 
    for (i = 0; i < nbdoc; i++) 
	{
		if (wcscmp(tabdoc[i].id, id)==0) 
		{
            id_indice = i;
        }
	}

    if (id_indice != -1) 
	{
        wprintf(L"Document trouvé :\n");
        affichage_1_doc(id_indice);
        
        choix_modif=-1; // On initialise à tout sauf 0 pour rentrer dans la boucle
        
        while (choix_modif!=0)
        {
        	wprintf(L"Que souhaitez-vous modifier ?\n"); 
        	wprintf(L"-1- Le titre\n"); 
        	wprintf(L"-2- L'auteur\n"); 
        	wprintf(L"-3- Le prix\n"); 
            wprintf(L"-4- La cote\n"); 
            wprintf(L"-5- La catégorie\n"); 
            wprintf(L"-6- Le format\n"); 
            wprintf(L"-7- L'édition\n"); 
            wprintf(L"-8- La collection\n"); 
            wprintf(L"-9- L'éligibilité à l'emprunt\n"); 
            wprintf(L"-10- L'éligibilité à la consultation\n");
            wprintf(L"-11- La présence en rayon\n");
        	wprintf(L"           \n");
        	wprintf(L"-0- Quitter la modification\n"); 
        	wprintf(L"Votre choix : "); 
        	wscanf(L"%d",&choix_modif);
        	fflush(stdin);
        	
        	switch(choix_modif)
        	{
        		case 1 : 
        		 wprintf(L"Nouveau titre :"); 
        		 lire_chaine_espace(tabdoc[id_indice].titre);
        		 gerer_espaces_multiples(tabdoc[id_indice].titre);
                 gerer_espace_avant_apres(tabdoc[id_indice].titre);
                 conv_maj_ac_accents(tabdoc[id_indice].titre);
                 affichage_1_doc(tabdoc[id_indice]);
                 break; 
                case 2 : 
                 wprintf(L"Nouvel auteur :"); 
        		 lire_chaine_espace(tabdoc[id_indice].auteur);
        		 gerer_espaces_multiples(tabdoc[id_indice].auteur);
                 gerer_espace_avant_apres(tabdoc[id_indice].auteur);
                 conv_maj_ac_accents(tabdoc[id_indice].auteur);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 3 : 
                 wprintf(L"Nouveau prix :"); 
        		 wscanf(L"%2f",&tabdoc[id_indice].prix); 
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 4 : 
                 wprintf(L"Nouvelle côte :"); 
        		 wscanf(L"%s",tabdoc[id_indice].cote);
        		 gerer_espace_avant_apres(tabdoc[id_indice].cote);
        		 conv_maj_ac_accents(tabdoc[id_indice].cote);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 5 : 
                 wprintf(L"Nouvelle catégorie :"); 
        		 lire_chaine_espace(tabdoc[id_indice].categorie);
        		 gerer_espaces_multiples(tabdoc[id_indice].categorie);
                 gerer_espace_avant_apres(tabdoc[id_indice].categorie);
                 conv_maj_ac_accents(tabdoc[id_indice].categorie);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 6 : 
                 wprintf(L"Nouveau format :"); 
        		 lire_chaine_espace(tabdoc[id_indice].format);
        		 gerer_espaces_multiples(tabdoc[id_indice].format);
                 gerer_espace_avant_apres(tabdoc[id_indice].format);
                 conv_maj_ac_accents(tabdoc[id_indice].format);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 7 : 
                 wprintf(L"Nouvelle édition :"); 
                 lire_chaine_espace(tabdoc[id_indice].edition);
        		 gerer_espaces_multiples(tabdoc[id_indice].edition);
                 gerer_espace_avant_apres(tabdoc[id_indice].edition);
                 conv_maj_ac_accents(tabdoc[id_indice].edition);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 8 : 
                 wprintf(L"Nouvelle collection :"); 
                 lire_chaine_espace(tabdoc[id_indice].collection);
        		 gerer_espaces_multiples(tabdoc[id_indice].collection);
                 gerer_espace_avant_apres(tabdoc[id_indice].collection);
                 conv_maj_ac_accents(tabdoc[id_indice].collection);
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 9 : 
                 wprintf(L"Le document est-il éligible à l'emprunt ? (oui/non) :");
                 wscanf(L"%s",reponse); 
                 gerer_espace_avant_apres(reponse);
                 conv_maj_ac_accents(reponse);
                 if ((wcscmp((reponse), L"OUI"))==0) //si la réponse est oui l'égibilité à l'emprunt prend la valeur 1 (true)
                 {
                    tabdoc[id_indice].eligible_a_lemprunt = 1;
                 }
                 else
                 {
                   tabdoc[id_indice].eligible_a_lemprunt= 0;
                 }
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 10 : 
                 wprintf(L"Le document est-il consultable ? (oui/non) :"); 
                 wscanf(L"%s",reponse);
				 gerer_espace_avant_apres(reponse); 
				 conv_maj_ac_accents(reponse);
                 if ((wcscmp((reponse), L"OUI"))==0) //si la réponse est oui la variable prend la valeur 1 (true)
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
                 wprintf(L"Le document est-il présent en rayon ? (oui/non) : ");
                 wscanf(L"%s",reponse); 
                 gerer_espace_avant_apres(reponse);
                 conv_maj_ac_accents(reponse);
                 if ((wcscmp((reponse), L"OUI"))==0) //si la réponse est oui la variable prend la valeur 1 (true)
                 {
                   tabdoc[id_indice].presence_rayons = 1; 
                 }
                 else
                 {
                   tabdoc[id_indice].presence_rayons = 0; 
                 }
                 affichage_1_doc(tabdoc[id_indice]);
                 break;
                case 0 : wprintf(L"Modifications effectuées et enregistrées !\n");
                 break; 
                default : wprintf(L"choix erroné.\n"); 
                 break;
                
        		 
			}  
		}
    } 
	else 
	{
        wprintf(L"Aucun document trouvé avec l'ID %s.\n", id);
    }
}*/

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//           ---- Procédure de suppression d'un document ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*Cas d'extrême néecessité, si un doc est obsolète, un erreur s'est produite, ou un doc devient illégal.. A utiliser avec précaution*/
void SupprDoc()
{
	int id_indice_suppr, i ; 
	char id[TAILLE_ID], confirmation[20]; 
	
	wprintf(L"-------------------------------------------AVERTISSEMENT--------------------------------------------------\n");
	wprintf(L"ATTENTION, vous êtes dans le module suppression. Cette action est très rare et irréversible\n");
	wprintf(L"Elle entraînera la perte définitive du document et de toutes les informations associées\n");
	wprintf(L"Si vous n'êtes pas certain de cette action, privilégiez le module d'archivage. Voulez-vous continuer ? (o/n) : ");
	scanf("%s",confirmation); 
	conv_maj_ss_accents(confirmation);
	if(confirmation[0] == 'O')
	{
	  wprintf(L"\nSaisissez l'ID du document à rechercher : ");
          scanf("%s", id);

         id_indice_suppr = -1; 
          for (i = 0; i < nbdoc; i++) 
	  {
		if (strcmp(tabdoc[i].id, id)==0) 
		{
                  id_indice_suppr = i;
                }
	  }

	if (id_indice_suppr != -1) 
	{
		if (tabdoc[id_indice_suppr].est_emprunte==EMPRUNTE)
		{
			
	           wprintf(L"Suppression impossible : le document %s de %s est emprunté\n",tabdoc[id_indice_suppr].titre, tabdoc[id_indice_suppr].auteur);
		}
		else 
		{
		    wprintf(L"-------------------------------------------AVERTISSEMENT---------------------------------------------\n");
		    wprintf(L"ATTENTION, Cette action sera irréversible. Etes vous sûr de vouloir supprimer le document ? : (o/n)"); 
		    scanf("%s",confirmation);
		     conv_maj_ss_accents(confirmation); 
		        if(confirmation[0] == 'O')
			{
			   for (i=id_indice_suppr ; i<nbdoc -1 ; i++)
			   {
			     tabdoc[i]=tabdoc[i+1]; 
			   }
			   nbdoc -- ; // On met à jour le nb de documents
			   printf("Le document avec l'ID %s a été supprimé avec succès\n", id);
			}
			else
			{
			   wprintf(L"Suppression annulée\n"); 
			}	
		}
    }
    else 
    {
       printf("Suppression impossible : aucun document trouvé avec l'ID %s.\n", id);
    }
   }
     else 
     {
	wprintf(L"suppression annulée, aucun document n'a été supprimé'\n"); 
     }	
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//           ---- Procédure de recherche approximative ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void RechercheApproximative(wchar_t *ElementRech, struct document *doc)
{
	/*Déclaration des variables locales*/
    int correspondance = 0;
    int i;
    wchar_t titre_maj[TAILLE_TITRE], auteur_maj[TAILLE_AUTEUR], categorie_maj[TAILLE_CATEGORIE], edition_maj[TAILLE_EDITION], collection_maj[TAILLE_COLLECTION];

    /*---Obligée de vider le buffer, sinon il ne me laisse pas entrer une recherche---*/
    fflush(stdin);
    
    /*---On entre un élément à rechercher, que ce soit un titre ou un auteur (possibilité d'élargir si besoin)---*/
    wprintf(L"Entrez un élément à rechercher (titre, auteur, édition, collection) : ");
    fflush(stdin);
    lire_chaine_espace(ElementRech);

    /*---Cette condition me sert juste à m'assurer que l'élément saisi est bien rangé dans la variable ElementRech car sinon la procédure de recherche marche pas---*/
    if (wcslen(ElementRech) == 0)
    {
        wprintf(L"Erreur : aucun élément saisi. Veuillez réessayer.\n");
    }
    else
    {
    	/*--- On rentre dans le dur : pour pouvoir bien tout comparer, il faut que l'élément recherché soit comparable par rapport à notre titre/auteur.
        j'applique donc les mêmes procédures qu'à nos titres et auteurs pour que la procédure puisse bien comparer---*/
      gerer_espaces_multiples(ElementRech);
      gerer_espace_avant_apres(ElementRech);
      conv_maj_ac_accents(ElementRech);

      for (i = 0; i < nbdoc; i++)
      {
       /*---Pour pas toucher aux titre et auteur de base qui ne s'écrivent pas tout en majuscule... jsp si on garde.. à tester sans---*/
        wcscpy(titre_maj, doc[i].titre);
        wcscpy(auteur_maj, doc[i].auteur);
        /*wcscpy(categorie_maj, doc[i].categorie);
        wcscpy(edition_maj, doc[i].edition);
        wcscpy(collection_maj, doc[i].collection);*/
        
        gerer_espaces_multiples(titre_maj);
        gerer_espace_avant_apres(titre_maj);
        conv_maj_ac_accents(titre_maj);

        gerer_espaces_multiples(auteur_maj);
        gerer_espace_avant_apres(auteur_maj);
        conv_maj_ac_accents(auteur_maj);
        
        /*gerer_espaces_multiples(categorie_maj);
        gerer_espace_avant_apres(categorie_maj);
        conv_maj_ac_accents(categorie_maj);
        
        gerer_espaces_multiples(edition_maj);
        gerer_espace_avant_apres(edition_maj);
        conv_maj_ac_accents(edition_maj);
        
        gerer_espaces_multiples(collection_maj);
        gerer_espace_avant_apres(collection_maj);
        conv_maj_ac_accents(collection_maj);*/
        
        
        /*---La fonction wcsstr me permet de vérifier s'il existe une correspondance entre la chaine auteur_maj et la sous_chaine ElementRech (notre saisie d'un élément
        à rechercher). Si c'est différent de null ça veut dire qu'il y a une correspondance entre les 2 ---*/
          if ((wcsstr(titre_maj, ElementRech) != NULL ) || (wcsstr(auteur_maj, ElementRech) != NULL))
          {
            correspondance = 1;
		    wprintf(L"Document trouvé : [ID : %s], Côte : %s\n, Titre : %s\n, Auteur : ",doc[i].id, doc[i].cote, doc[i].titre, doc[i].auteur);
          }
      }
	}

    if (!correspondance)
    {
        wprintf(L"Aucun document trouvé avec l'élément saisi '%s'\n", ElementRech);
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
    retour=fwscanf(f1,L"%s %s %s %s %f %d\n",doc.id, doc.cote, doc.titre, doc.auteur, &doc.prix, &doc.eligible_a_lemprunt);
    
    if (retour != EOF)
    {
      conv_maj_ac_accents(doc.cote);
      conv_maj_ac_accents(doc.titre);
      conv_maj_ac_accents(doc.auteur);
      
      //Nettoyage des espaces indésirables 
      gerer_espace_avant_apres(doc.cote);
      
      gerer_espaces_multiples(doc.titre);
      gerer_espace_avant_apres(doc.titre);
      
      gerer_espaces_multiples(doc.auteur);
      gerer_espace_avant_apres(doc.auteur);
      
      tabdoc[i++] = doc ;      
    }
  }
  fclose(f1);
  nbdoc = i;  
  wprintf(L"%d documents lu(s)\n",nbdoc);
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
    wprintf(L"Aucun document à sauvegarder\n");
  }
  else
  {
    wprintf(L"Nom du fichier de sauvegarde : ");
    scanf("%s",NomFichier);
    f1=fopen(NomFichier,"w");
    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
        fwprintf(f1, L"%-15s %-15s %-15s %-15s %-15f %-15d\n",doc.id, doc.cote, doc.titre, doc.auteur, doc.prix, doc.eligible_a_lemprunt);   
    } 
     fclose(f1);
    wprintf(L"%d documents(s) sauvegardé(s)\n",nbdoc);
    // La sauvegarde a été faite => plus rien à sauvegarder
    a_sauvegarder=0 ; 
  }
} 


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//         ----Procédure de vérification de la sauvegarde ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void verif_sauvegarde()
{
  wchar_t reponse[20];
  
  if (a_sauvegarder)
  {
    wprintf(L"Des données ont été modifiées !\n");
    wprintf(L"Voulez-vous effectuer une sauvegarde ? (o/n) : ");
    wscanf(L"%s",reponse);
    conv_maj_ac_accents(reponse);
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
  wprintf(L"Au revoir\n");
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// Proc et fonctions backoffice
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure de conversion en majuscules avec acent ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void conv_maj_ac_accents(wchar_t chaine[])
{
  int taille, i;

  taille = wcslen(chaine);
  for (i=0 ; i< taille ; i++)
  {
    chaine[i] = towupper(chaine[i]);
  }
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure de conversion en majuscules sans acent ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void conv_maj_ss_accents(char chaine[])
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
void lire_chaine_espace(wchar_t chaine[])
{
  //Déclaration des variables locales 
  int i; 
  wchar_t lettre; 
  
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
void gerer_espaces_multiples(wchar_t *chaine)
{
	//Délaration des variables locales 
	int i,j,taille,espace_rencontre; 
	wchar_t chaine1[200]; 
	
	i=0; 
	j=0; 
	espace_rencontre=0; 
	
	if(wcscmp(chaine,L"")!=0)
	{
		taille=wcslen(chaine); 
		
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
		wcscpy(chaine,chaine1);
	}
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//--- Procédure de gestion des espaces avant et après une chaine de caractère (prise dans le poly) ---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void gerer_espace_avant_apres(wchar_t *ch)
{
	//Déclaration des variables locales 
	wchar_t chaine1[200]; 
	int i,j,k,taille; 
	
	i=0; 
	j=0; 
	k=0; 
	
  if (wcscmp(ch,L"") != 0) /* test si la chaine n'est pas vide */
  {
/* --- suppression des espaces en début de chaine --- */
    while (isspace(ch[i++])); /* on passe les espaces du début */
    taille = wcslen(ch);
/* recopie des caractères APRES les espaces */
    for (j=i-1 ; j<taille ; j++) chaine1[k++] = ch[j];
    chaine1[k++] = '\0' ; /* on finalise la chaine */
    wcscpy(ch,chaine1) ; /* recopie la chaine traitée dans ch */
/* --- suppression des espaces en fin de chaine --- */
    i = wcslen(ch);
    while (isspace(ch[--i])); /* on passe les espaces */
    ch[++i] = '\0' ; /* on finalise la chaine */
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//        --- Fonction de convertion équivalente à swprintf() ---
//               --- entier en chaine numérique ---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
char *mon_itoa(int j)
{
char chaine[100], *retour;
int taille;
printf(chaine,"%d",j); //création d'une chaine à partir de l'entier 
taille = strlen(chaine); //Construction d'une chaine de la taille de l'entier 
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

// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Fonction pour valider les questions fermées (OUI ou NON) et les convertir en int (bool) ----
// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int reponse_question_fermee_en_int(wchar_t reponse[TAILLE_STANDARD])
{   
    gerer_espace_avant_apres(reponse);
    conv_maj_ac_accents(reponse);
    if ((wcscmp(reponse,L"OUI")==0)||(wcscmp(reponse,L"O")==0))
    {
        return 1;
    }
    else if ((wcscmp(reponse,L"NON")==0)||(wcscmp(reponse,L"NO")==0)||(wcscmp(reponse,L"N")==0))
    {
        return 0; 
    }
    else 
    {
        return 2;
    }
}

// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Fonction pour convertir les int (bool) en réponses de questions fermées ----
// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
wchar_t *int_a_reponse_question_fermee(int reponse)
{   
    if (reponse==1)
    {
        return L"OUI";
    }
    else if (reponse==2)
    {
        return L"NON"; 
    }
    else 
    {
        return L"OUT";
    }
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Procédure WIP pour les fonctions en cours de dev ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void wip()
{
    wprintf(L"Fonction en cours de développement :)\n");
}
