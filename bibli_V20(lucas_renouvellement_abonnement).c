#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h> /* pour setlocale UTF8 */
#include <wctype.h> /* pour towupper() */
#include <wchar.h> /* pour le type wchar_t */
#include <time.h> /* pour récupérer l'heure système */

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                         ---- Constantes ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#define TAILLE_COTE 10
#define TAILLE_TITRE 50
#define TAILLE_AUTEUR 50
#define TAILLE_CATEGORIE 50
#define TAILLE_EDITION 50
#define TAILLE_COLLECTION 50
#define TAILLE_FORMAT 50
#define TAILLE_NUM_VOIE 8
#define TAILLE_ISBN13 18 //13 chiffres, 4 tirets, 1 caractère fin de ligne
#define MAX_DOC 100
#define MAX_ABO 500
#define MAX_EMPRUNTS 5
#define TAILLE_NOM_F 100
#define TAILLE_ID 10
#define TAILLE_STANDARD 20
#define TAILLE_ELEMENTRECH 200
#define NON_TROUVE 0
#define TROUVE 1
#define NON_EMPRUNTE -1
#define EMPRUNTE 1
#define TAILLE_CH 70
#define TAILLE_VOIE 70
#define TAILLE_VILLE 150
#define TAILLE_NOM 50
#define TAILLE_PRENOM 50
#define TAILLE_EMAIL 50
#define DUREE_EMPRUNT 14


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//              ---- déclaration de types globaux ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct date
{
    int jour;
    int mois;
    int annee;
};

struct adresse
{
	char intitule_voie[TAILLE_VOIE];
	char num_voie[TAILLE_NUM_VOIE];  //char car numéro bis, ter etc
	int code_postal;
	char ville[TAILLE_VILLE]; 
};

struct emprunt
{
  char id[TAILLE_ID];
  char emp_id_abo[TAILLE_ID];
  char emp_id_doc[TAILLE_ID];
  struct date date_emprunt;
  struct date date_retour_prevue;
  struct date date_retour_effective;
  int est_en_retard; // oui ou non en calculant par rapport à la date de retour prévue..
  char nom_abo_emprunteur[TAILLE_NOM];
  char prenom_abo_emprunteur[TAILLE_PRENOM];
  char titre_doc_emprunte[TAILLE_TITRE];
  int nb_retard; 
};


struct abonne 
{
	char id_abonne[TAILLE_ID]; 
	char nom[TAILLE_NOM]; 
	char prenom[TAILLE_PRENOM]; 
	int num_tel;
	struct date date_naissance;
  struct date date_actuelle;  // Pour le calcul de renouvellement d'abonnement
  struct date date_expiration; // Pour le calcul de renouvellement d'un abonnement  
	char email[TAILLE_EMAIL];
	struct adresse adresse_postale; 
	struct date date_premier_abonnement; 
	struct date date_dernier_souscription; 
	int nb_emprunts;  
	int est_abonne; //bool
  int nb_doc_en_retard; 
};


struct document 
{
    char id[TAILLE_ID];
    char cote[TAILLE_COTE];
    char titre[TAILLE_TITRE];
    char auteur[TAILLE_AUTEUR];
    char categorie[TAILLE_CATEGORIE];
    char edition[TAILLE_EDITION];
    struct date date_premiere_publi;
    char collection[TAILLE_COLLECTION];
    char format[TAILLE_FORMAT];
    char ISBN13[TAILLE_ISBN13]; //L'ISBN10 est exprimable au format ISBN13 en rajoutant 978 au début et en recalculant le chiffre de contrôle
    float prix; 
    int eligible_a_lemprunt ; //bool
    int est_emprunte ; //bool
    int eligible_a_consult; //bool
    int presence_rayons; //bool
    int present_dans_le_fond; //bool
    struct date date_deb;
    struct date date_fin;
 }; 


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//        ---- Déclaration des variables globales ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct document tabdoc[MAX_DOC] ;
struct abonne tababo[MAX_ABO];
struct emprunt tabemp[MAX_EMPRUNTS];
int nbdoc=0;
int nbabo=0; 
int nbemp=0; //nombre total d'emprunts
int nbemp_en_cours=0; //nombre d'emprunts en cours
int id_indice = -1; 
char ElementRech[TAILLE_ELEMENTRECH];


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- déclarations préliminaires ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Proc et fonctions frontoffice ----
void menu_staff();
void saisir_nouveau_doc();
void saisir_abonne(); 
void affichage_liste_doc();
void affichage_1_doc(int indice_a_afficher);
void affichage_1_abo(int indice_abo);
void ModifierDoc();
void affichage_abonne();
void ModifierAbo(); 
void SupprDoc();
void RechercheApproximative(char *ElementRech, struct document *doc);
void RechercheAbo();
void RenouvelerAbonnement();
void chargement();
void verif_sauvegarde();
void sauvegarde();
void quitter();
void emprunt_doc ();
void afficher_liste_emprunt();
void retour_emprunt();
void statistiques ();
void afficher_emprunts_dun_usager();
void afficher_emprunts_dun_doc();
void historique_emprunt();
void sorti_du_fond();





//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- déclarations préliminaires ----
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Proc et fonctions backoffice ----
void wip();
//void conv_maj(char chaine[]);
void lire_chaine_espace(char chaine[]);
void gerer_espaces_multiples(char *chaine);
void gerer_espace_avant_apres(char *ch);
char *mon_itoa(int j);
int est_bissextile(int annee);
int date_valide(int jour, int mois, int annee);
int reponse_question_fermee_en_int(char reponse[TAILLE_STANDARD]);
char *int_a_reponse_question_fermee(int reponse);
void conv_maj_accents(char ChaineOctets[], int conserve_accents_OK);
void lire_chaine_augmentee(char *string);
void supprime_accents(char ChaineLettre[]);
void conv_souligne_en_espace(char *ch);
void conv_espace_en_souligne(char *ch);
struct abonne abonnement_expire(struct abonne abo); 
int Verif_Email(char *email); 





// --- Booléen indiquant si on doit faire une sauvegarde ---
int a_sauvegarder=0 ;


// ---------------------------Programme principal ------------------------------
int main ()
{
    /* positionnement de la table de codage */
    //setlocale (LC_ALL, "") ; /* valeur selon environnement */
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
    int ok; 

    printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                 BIENVENUE DANS LE PROGRAMME DE GESTION                   ║\n");
    printf("║                      DE VOTRE BIBLIOTHEQUE                               ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");

  while (choix_menu != 0)
  {
     printf("\n");
     printf("╔══════════════════════════════ MENU PRINCIPAL ═══════════════════════════╗\n");
     printf("║  1. Saisir un nouveau document                                          ║\n");
     printf("║  2. Afficher tous les documents                                         ║\n");
     printf("║  3. Afficher un document                                                ║\n");
     printf("║  4. Rechercher un document                                              ║\n");
     printf("║  5. Modifier un document                                                ║\n");
     printf("║  6. Sortir un document du fond                                          ║\n");
     printf("║  7. Supprimer un document                                               ║\n");
     printf("║                                                                         ║\n");
     printf("║  8. Saisir un abonné                                                    ║\n");
     printf("║  9. Afficher tous les abonnés                                           ║\n");
     printf("║ 10. Afficher un abonné                                                  ║\n");
     printf("║ 11. Modifier un abonné                                                  ║\n");
     printf("║ 12. Rechercher un abonné                                                ║\n");
     printf("║ 13. Renouveler les abonnements                                          ║\n");
     printf("║                                                                         ║\n");
     printf("║ 14. Sauvegarder                                                         ║\n");
     printf("║ 15. Charger un fichier                                                  ║\n");
     printf("║                                                                         ║\n");
     printf("║ 16. Emprunter un document                                               ║\n");
     printf("║ 17. Retourner un document                                               ║\n");
     printf("║ 18. Suivi des emprunts                                                  ║\n");
     printf("║ 19. Historique d'emprunt d'un document                                  ║\n");
     printf("║ 20. Historique d'emprunt d'un usager                                    ║\n");
     printf("║                                                                         ║\n");
     printf("║ 21. Statistiques                                                        ║\n");
     printf("║                                                                         ║\n");
     printf("║  0. Quitter                                                             ║\n");
     printf("╚═════════════════════════════════════════════════════════════════════════╝\n");
     printf("\n");
     printf("Votre choix : ");

     ok = scanf("%d",&choix_menu);

     if (!ok)
     {
       printf("Veuillez choisir parmi les fonctionnalités proposées (saisir un chiffre entre 0 et 21)\n"); 
       fflush(stdin); 
       int choix_menu=-1 ;

     }
     else if (choix_menu < 0 || choix_menu > 21)
     {
      printf("Choix erroné. Veuillez chosisir parmi les fonctionnalités proposées (saisir un chiffre entre 0 et 21)\n"); 
      choix_menu = -1; 
     }
     else
     {

       switch(choix_menu)
      { 
        case 1 : saisir_nouveau_doc()                         ;
               break                                          ;
      
        case 2 : affichage_liste_doc()                        ;
               break                                          ;
               
        case 3 : affichage_1_doc(nbdoc-1)                     ;
               break                                          ;
               
        case 4 : RechercheApproximative (ElementRech,tabdoc)  ;
               break                                          ;
               
        case 5 : ModifierDoc()                                ;
               break                                          ;
               
        case 6 : sorti_du_fond()                              ;
               break                                          ;
      
        case 7 : SupprDoc()                                   ;
               break                                          ;
      
        case 8 : saisir_abonne()                              ;  
               break                                          ;
      
        case 9 : affichage_abonne()                           ;  
               break                                          ; 
    
        case 10 : affichage_1_abo(nbabo)          ;   
                break                                         ; 

        case 11 : ModifierAbo()                               ;
                break                                         ;

        case 12 : RechercheAbo()                              ;
                break                                         ;

        case 13 : RenouvelerAbonnement()                      ;
                break                                         ;
      
        case 14 : sauvegarde()                                ;
                break                                         ;
      
        case 15 : chargement()                                ;
                break                                         ;
      
        case 16 : emprunt_doc()                               ;
                break                                         ;

        case 17 : retour_emprunt()                            ;
                break                                         ;

        case 18 : afficher_liste_emprunt()                    ;                                     
                break                                         ;

        case 19 : historique_emprunt()                        ;                                     
                break                                         ;

        case 20 : afficher_emprunts_dun_usager()              ;
                break                                         ;
      
        case 21 : statistiques ()                             ;
                break                                         ;
      /*case 7 : modification()   ;
               break            ;
      case 8 : suppression()    ;
               break            ;
      case 9 : tri_insertion_nom(tabeleves,nbeleves);
               break            ;
      case 10 : tri_insertion_age(tabeleves,nbeleves);
               break            ; */
        case 0 : quitter()                                   ;
               break                                         ;

        default : printf("Choix erroné\n")                   ;
               break                                         ;
      }
    
     }

    
  }
}



// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Procédure de saisie d'un nouveau document ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void saisir_nouveau_doc() 
{
  // variables
  struct document doc;
  int i, c, j;
  i=nbdoc;
  char reponse[TAILLE_STANDARD], saisie[TAILLE_STANDARD], identifiant[TAILLE_ID], chaine_id_temporaire[TAILLE_ID];
  int result, valide = 0, staff_souhaite_saisir_doc=1, entier_id_temporaire, dernier_id_numerique;

  while ((staff_souhaite_saisir_doc!=0) && (i<MAX_DOC))
  {
    printf("------------\n");
    printf("Souhaitez vous saisir un nouveau document (0 pour terminer): ");
    result=scanf("%d", &staff_souhaite_saisir_doc);    
    while (!result)
    {
        printf("Erreur, merci de saisir un entier                          : ");
        result=scanf("%d", &staff_souhaite_saisir_doc);
        fflush(stdin);
    }

    if (staff_souhaite_saisir_doc!=0)
    {
      if (nbdoc==0)
      {
        strcpy(doc.id,"DOC1");
      }
      else
      {
        strncpy(chaine_id_temporaire, tabdoc[nbdoc - 1].id + 3, sizeof(chaine_id_temporaire) - 1); //on extrait la partie numérique de l'id du dernier doc
        chaine_id_temporaire[sizeof(chaine_id_temporaire) - 1] = '\0'; //on sécurise la fin de chaîne

        dernier_id_numerique = atoi(chaine_id_temporaire); // on convertit la partie numérique en entier
        dernier_id_numerique++; 

        strcpy(doc.id, "DOC"); // On ajouter le préfixe pour différencier des autres types
        strcat(doc.id, mon_itoa(dernier_id_numerique)); 
        
        /*strcpy(chaine_id_temporaire,"");
        printf("nbdoc:%d\n", nbdoc);
        for (j=3; j<10 ; j++)
        {
          strcat(chaine_id_temporaire,&tabdoc[nbdoc-1].id[j]);
          printf("le caractère n° %d : %c \n", j, tabdoc[nbdoc-1].id[j]);
        }
        entier_id_temporaire=atoi(chaine_id_temporaire);
        printf("chaine temporaire %s\n", chaine_id_temporaire);
        strcpy(doc.id, "DOC");
        strcat(doc.id,(mon_itoa(entier_id_temporaire++))); */
      }
      //strcpy(doc.id, "DOC"); //L'identifiant prend le préfixe DOC afin de le différencier des identifiants des usagers notamment
      //strcat(doc.id,(mon_itoa(++nbdoc))); //Au préfixe DOC s'ajoute le nombre de document existants +1 
      printf("Identifiant                                                : "); 
      printf("%s\n", doc.id);
    
      fflush(stdin);
      printf("Côte                                                       : ");
      lire_chaine_augmentee(doc.cote);

      printf("Titre                                                      : ");
      lire_chaine_augmentee(doc.titre);

      printf("Auteur                                                     : ");
      lire_chaine_augmentee(doc.auteur);

      printf("Catégorie                                                  : ");
      lire_chaine_augmentee(doc.categorie);

      printf("Edition                                                    : ");
      lire_chaine_augmentee(doc.edition);

      while (!valide) 
      {
        doc.date_premiere_publi.jour=-1; doc.date_premiere_publi.mois=-1; doc.date_premiere_publi.annee=-1; //On met une valeur fausse, en effet après le premier passage, si la saisie est fausse, la fonction date_valide va se faire sur les valeurs rentrées lors de l'itération précédente ce qui est source d'erreur
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
            fflush(stdin);// Vider le buffer en cas d'erreur
        }
     }
     valide=0;
     fflush(stdin);
     

      printf("Collection                                                 : ");
      lire_chaine_augmentee(doc.collection);

      printf("Format                                                     : ");
      lire_chaine_augmentee(doc.format);

      while (!valide) //saisie de l'ISBN avec vérification
      {
        valide = 1;  // On part du principe que la saisie est valide
        
        printf("ISBN13                                                     : ");
        scanf("%18s", saisie); // je met pas la fonction lire chaine car à priori pas besoin d'espaces
        while ((c = getchar()) != '\n' && c != EOF);

        for (int j = 0; saisie[j] != '\0'; j++) 
        {
            if (!isdigit(saisie[j]) && saisie[j] != '-') //On vérifie si la saisie contient des caractères qui ne sont ni des chiffres ni des tirets
            {
                valide = 0;  // Contient un caractère NON autorisé
            }
        }
        if (!valide)
        { 
            printf("Erreur : saisie invalide. Essayez à nouveau (uniquement des chiffres et des tirets). \n");
        }
      } 
      strncpy(doc.ISBN13,saisie, TAILLE_ISBN13); 
      gerer_espace_avant_apres(doc.ISBN13); // par contre on gère les espaces avant après
      valide=0; //on remet valide à 0 pour le prochain passsage
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
            } 
            else if (saisie[j] == '.') //on vérifie le nombre de points préexistants
            {
                nb_points++;
            }
            else if (!isdigit(saisie[j])) //On vérifie si la saisie contient des caractères qui ne sont ne sont pas des chiffres, les points ont été testés juste avant
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
            printf("Erreur : saisie invalide. Essayez à nouveau.\n");
        }
      }
      doc.prix = atof(saisie); //on convertie en float et on le range dans le type structuré
      valide=0; //on remet valide à 0 pour le prochain passsage
      fflush(stdin);

      doc.eligible_a_lemprunt=2; //on l'initie à la valeur 2 qui est la valeur d'erreur
      while (doc.eligible_a_lemprunt==2) //tant que la variable vaut 2 (valeur d'erreur) la boucle continue
      {
        printf("Eligibilité à l'emprunt (oui / non)                        : ");
        lire_chaine_augmentee(reponse);
        doc.eligible_a_lemprunt=reponse_question_fermee_en_int(reponse); //Prendra la valeur 1,0 ou 2 en fonction de la validité de la chaine rentrée
        if (doc.eligible_a_lemprunt==2)
        {
          printf("Erreur, merci de répondre par oui ou par non.\n"); //Si la chaine rentrée est fausse un message d'erreur s'affiche
        }
      }

      doc.presence_rayons=2;
      while (doc.presence_rayons==2)
      {
        printf("Présent en rayons (oui / non)                              : ");
        lire_chaine_augmentee(reponse);
        doc.presence_rayons=reponse_question_fermee_en_int(reponse);
        if (doc.presence_rayons==2)
        {
          printf("Erreur, merci de répondre par oui ou par non.\n");
        }
      }


      if((doc.eligible_a_lemprunt==1)||(doc.presence_rayons==1))//si le doc est éligible à l'emprunt ou présent en rayon il est forcément possible de le consulter
      {
        doc.eligible_a_consult=1;
        printf("Eligibilité à la consultation (oui / non)                  : %s\n", int_a_reponse_question_fermee(doc.eligible_a_consult));
      }
      else //sinon on demande si le document est éligible à la consultation malgré le fait qu'il ne soit pas stocké en rayons
      {
        doc.eligible_a_consult=2;
        while (doc.eligible_a_consult==2)
        {
            printf("Eligibilité à la consultation (oui / non)                  : ");
            lire_chaine_augmentee(reponse);
            doc.eligible_a_consult=reponse_question_fermee_en_int(reponse);
            if (doc.eligible_a_consult==2)
            {
                 printf("Erreur, merci de répondre par oui ou par non.\n");
            }
        }
      }
      
      doc.est_emprunte=0;
      printf("En cours d'emprunt                                         : %s\n", int_a_reponse_question_fermee(doc.est_emprunte));

      doc.present_dans_le_fond=1;
      printf("Présent dans le fonds                                      : %s\n", int_a_reponse_question_fermee(doc.present_dans_le_fond));


      time_t tp,nbsec;
            struct tm hjm ;
            nbsec=time(&tp);
            hjm=*localtime(&nbsec);
            doc.date_deb.jour=hjm.tm_mday;
            doc.date_deb.mois=hjm.tm_mon+1;
            doc.date_deb.annee=hjm.tm_year+1900;
            printf("Date d'arrivée dans la bibliothèque                        : %02d/%02d/%d\n", doc.date_deb.jour, doc.date_deb.mois, doc.date_deb.annee);

      doc.date_fin.jour=0;
      doc.date_fin.mois=0;
      doc.date_fin.annee=0; //on initie une date de fin fausse pour ensuite pouvoir utiliser une condition et afficher NULL

      tabdoc[i++] = doc ;
      nbdoc++;
      a_sauvegarder=1 ; // Suite à l'ajout d'un doc cela signifie qu'il y a des données à sauvegarder
    }
  }
  fflush(stdin);
  //nbdoc = i; 
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//               ---- Procédure de saisie d'un nouvel abonné ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void saisir_abonne() 
{
  // variables
  struct abonne abo;
  int i, ok;
  i=nbabo;
  char reponse[TAILLE_STANDARD], saisie[TAILLE_STANDARD], identifiant[TAILLE_ID], tel_saisi[TAILLE_STANDARD];
  int result, valide = 0, staff_souhaite_saisir_abo=1;

  while ((staff_souhaite_saisir_abo!=0) && (i<MAX_ABO))
  {
    printf("------------\n");
    printf("Souhaitez vous saisir un nouvel abonné (0 pour terminer) : ");
    result=scanf("%d", &staff_souhaite_saisir_abo);
    fflush(stdin);
    while (!result)
    {
        printf("Erreur, merci de saisir un entier                        : ");
        result=scanf("%d", &staff_souhaite_saisir_abo);
        fflush(stdin);
    }

    if (staff_souhaite_saisir_abo!=0)
    {
      strcpy(abo.id_abonne, "ABO"); //L'identifiant prend le préfixe DOC afin de le différencier des identifiants des usagers notamment
      strcat(abo.id_abonne,(mon_itoa(++nbabo))); //Au préfixe DOC s'ajoute le nombre de document existants +1 
      printf("Identifiant                                              : "); 
      printf("%s\n", abo.id_abonne);
    

      printf("Nom                                                      : ");
      lire_chaine_augmentee(abo.nom); //sait-on jamais qu'il y ait besoin de faire un espace
    
      printf("Prénom                                                   : ");
      lire_chaine_augmentee(abo.prenom);


      while (!valide) 
      {
        abo.date_naissance.jour=-1; abo.date_naissance.mois=-1; abo.date_naissance.annee=-1; //On met une valeur fausse, en effet après le premier passage, si la saisie est fausse, la fonction date_valide va se faire sur les valeurs rentrées lors de l'itération précédente ce qui est source d'erreur
        printf("Date de naissance (jj/mm/aaaa)                           : ");
        if (scanf("%d/%d/%d", &abo.date_naissance.jour, &abo.date_naissance.mois, &abo.date_naissance.annee) != 3) // Vérification du format et lecture des données
        {
            printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }

        // Vérifier si la date est valide
        if (date_valide(abo.date_naissance.jour, abo.date_naissance.mois, abo.date_naissance.annee)) 
        {
            valide=1;
        } else 
        {
            printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }
     }
     valide=0;
     fflush(stdin);
     

    while (!valide) 
    {
      printf("Email                                                    : ");
      lire_chaine_augmentee(abo.email);  

      if (Verif_Email(abo.email)) 
     {
        valide = 1; 
     } 
     else 
     {
        printf("Email invalide. Veuillez rééssayer\n");
        fflush(stdin); 
     }
   }
    valide=0;

     
	  
	  printf("Numéro de téléphone (format 10 chiffres)                 : "); 
      lire_chaine_augmentee(tel_saisi);
      ok=strlen(tel_saisi);
      
      while (ok!=10)
     {
        printf("Erreur, merci de saisir un numéro valide                 : ");
        lire_chaine_augmentee(tel_saisi);
        ok=strlen(tel_saisi);
     } 
     abo.num_tel=atoi(tel_saisi);


      printf("Numéro de voie                                           : ");
      scanf("%s",abo.adresse_postale.num_voie); 
      fflush(stdin);
      
      printf("Intitulé de la voie                                      : ");
      lire_chaine_augmentee(abo.adresse_postale.intitule_voie);
      
      printf("Code postal                                              : ");
      ok=scanf("%d",&abo.adresse_postale.code_postal); 
      fflush(stdin);
      while (!ok)
      {
        printf("Erreur, merci de saisir un code postale valide           : ");
        ok=scanf("%d", &abo.adresse_postale.code_postal);
      }

      printf("Ville                                                    : ");
      lire_chaine_augmentee(abo.adresse_postale.ville);
      
      
      
       while (!valide) 
      {
        abo.date_premier_abonnement.jour=-1; abo.date_premier_abonnement.mois=-1; abo.date_premier_abonnement.annee=-1; //On met une valeur fausse, en effet après le premier passage, si la saisie est fausse, la fonction date_valide va se faire sur les valeurs rentrées lors de l'itération précédente ce qui est source d'erreur
        printf("Date de premier abonnement (jj/mm/aaaa)                  : ");
        if (scanf("%d/%d/%d", &abo.date_premier_abonnement.jour, &abo.date_premier_abonnement.mois, &abo.date_premier_abonnement.annee) != 3) // Vérification du format et lecture des données
        {
            printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }

        // Vérifier si la date est valide
        if (date_valide(abo.date_premier_abonnement.jour, abo.date_premier_abonnement.mois, abo.date_premier_abonnement.annee)) 
        {
            valide=1;
        } else 
        {
            printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }
     }
     valide=0;
     fflush(stdin);
     
     
       while (!valide) 
      {
        abo.date_dernier_souscription.jour=-1; abo.date_dernier_souscription.mois=-1; abo.date_dernier_souscription.annee=-1; //On met une valeur fausse, en effet après le premier passage, si la saisie est fausse, la fonction date_valide va se faire sur les valeurs rentrées lors de l'itération précédente ce qui est source d'erreur
        printf("Date du dernier abonnement (jj/mm/aaaa)                  : ");
        if (scanf("%d/%d/%d", &abo.date_dernier_souscription.jour, &abo.date_dernier_souscription.mois, &abo.date_dernier_souscription.annee) != 3) // Vérification du format et lecture des données
        {
            printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }

        // Vérifier si la date est valide
        if (date_valide(abo.date_dernier_souscription.jour, abo.date_dernier_souscription.mois, abo.date_dernier_souscription.annee)) 
        {
            valide=1;
        } else 
        {
            printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
            fflush(stdin);// Vider le buffer en cas d'erreur
        }
     }
     valide=0;
     fflush(stdin);

     abo = abonnement_expire(abo);//on vérifie si l'abonnement est expiré

     printf("Dispose d'un abonnement valide                           : %s\n", int_a_reponse_question_fermee(abo.est_abonne));
     
     abo.nb_emprunts=0;
     printf("Nombre d'emprunts                                        : %d\n", abo.nb_emprunts);
     
     abo.nb_doc_en_retard=0;
     printf("Nombre de documents en retard                            : %d\n", abo.nb_doc_en_retard);
    
      


      tababo[i++] = abo ;
      a_sauvegarder=1 ; // Suite à l'ajout d'un doc cela signifie qu'il y a des données à sauvegarder
    printf("Abonné %s %s ajouté avec succès !\n", abo.prenom, abo.nom); // Je confirme car pour l'instant on a pas l'affichage à voir si on garde
    }
  }
  fflush(stdin);
  nbabo = i; 
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
    printf("%-21s %-21s %-51s %-51s %-21s\n","ID","COTE","TITRE", "AUTEUR", "ISBN13");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      conv_maj_accents(doc.id, 0);
      conv_maj_accents(doc.cote, 0);
      conv_maj_accents(doc.titre, 0); 
      conv_maj_accents(doc.auteur, 0); 

      printf("%-21s %-21s %-51s %-51s %-21s \n",doc.id, doc.cote, doc.titre, doc.auteur, doc.ISBN13  ); 
      printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
  } 
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//             ---- Procédure d'affichage de tous les documents ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void affichage_abonne()
{
    struct abonne abo;
    int i;

    if (nbabo == 0) 
    {
        printf("Aucun abonné à afficher\n");
    }
    else 
    {
        
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("%-21s %-21s %-51s %-51s %-21s\n", "ID", "NOM", "PRENOM", "EMAIL", "NB EMPRUNTS");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (i = 0; i < nbabo; i++) 
        {
            abo = tababo[i];

            conv_maj_accents(abo.id_abonne, 0);
            conv_maj_accents(abo.nom, 0);
            conv_maj_accents(abo.prenom, 0);
            conv_maj_accents(abo.email, 0);

            printf("%-21s %-21s %-51s %-51s %d\n",abo.id_abonne, abo.nom, abo.prenom, abo.email, abo.nb_emprunts  ); 

            printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
    printf("Aucun document à afficher\n");
  }
  else
  {
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s %-21s\n","ID","COTE","TITRE", "AUTEUR", "CATEGORIE", "EDITION", "DATE PUBLICATION", "COLLECTION", "FORMAT", "ISBN13", "PRIX", "EMPRUNTABLE", "CONSULTABLE", "PRESENT EN RAYONS", "PRESENT DANS LE FONDS", "DATE DEBUT", "DATE FIN");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
//%-21d/%d/%d
    for (i=0 ; i < nbdoc ; i++)
    {
      doc = tabdoc[i];
      printf("%-21s %-21s %-21s %-21s %-21s %-21s %d/%d/%-21d %-21s %-21s %-21s %-21.2f",doc.id, doc.cote, doc.titre, doc.auteur, doc.categorie, doc.edition, doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee, doc.collection, doc.format, doc.ISBN13,  doc.prix); //%21.2f pour le prix car sinon pas aligné
      if  (doc.eligible_a_lemprunt)
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
void affichage_1_doc(int indice_a_afficher)
{
 struct document doc;

  if (nbdoc == 0)
  {
    printf("Aucun document à afficher.\n");
  }
  /*else if (document_a_afficher>=nbdoc)
  {
    printf("Le document saisi n'est pas dans la base de données.\n");
  }*/
  else
  {

    doc = tabdoc[indice_a_afficher];
    
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Identifiant                                   : %s\n", doc.id);
    printf("Titre                                         : %s\n", doc.titre);
    printf("Auteur                                        : %s\n", doc.auteur);
    printf("Côte                                          : %s\n", doc.cote);
    printf("Catégorie                                     : %s\n", doc.categorie);
    printf("Edition                                       : %s\n", doc.edition);
    printf("Date de publication                           : %02d/%02d/%d\n", doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee);
    printf("Collection                                    : %s\n", doc.collection);
    printf("Format                                        : %s\n", doc.format);
    printf("ISBN13                                        : %s\n", doc.ISBN13);
    printf("Prix                                          : %.2f\n", doc.prix);
    printf("Empruntable                                   : %s\n", int_a_reponse_question_fermee(doc.eligible_a_lemprunt));
    printf("Consultable                                   : %s\n", int_a_reponse_question_fermee(doc.eligible_a_consult));
    printf("Présent en rayons                             : %s\n", int_a_reponse_question_fermee(doc.presence_rayons));
    printf("Présent dans le fonds                         : %s\n", int_a_reponse_question_fermee(doc.present_dans_le_fond));
    printf("Date d'entrée dans le fond                    : %02d/%02d/%d\n", doc.date_deb.jour, doc.date_deb.mois, doc.date_deb.annee);
    if (doc.date_fin.annee==-1)
    {
      printf("Date de sortie du fond                        : NA\n");
    }
    else
    {
      printf("Date de sortie du fond                        : %02d/%02d/%d\n", doc.date_fin.jour, doc.date_fin.mois, doc.date_fin.annee);
    }    
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");  
     }   
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                ---- Procédure de modification ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void ModifierDoc()
{
    int i=0, choix_modif;
    int valide = 0, trouve=0;
    char id[TAILLE_ID], reponse[TAILLE_STANDARD];
    char saisie[50];
    struct document doc; 

    printf("Saisissez l'ID du document à rechercher : ");
    fflush(stdin);
    lire_chaine_augmentee(id);
    conv_maj_accents(id,0);

    id_indice = -1; 
    while ((i<nbdoc)&&(!trouve)) 
    {
      if (strcmp(tabdoc[i].id, id)==0) 
      {
        id_indice = i;
        trouve=1;
      }
      else
      {
        i++;
      }
    }

    if (id_indice != -1) 
  {
        printf("Document trouvé :\n");
        affichage_1_doc(id_indice);
        
        choix_modif=-1; // On initialise à tout sauf 0 pour rentrer dans la boucle
        
        while (choix_modif!=0)
        {
          printf("✧------------MODIFICATION------------✧\n");
          printf("|     Que souhaitez-vous modifier ?  |\n"); 
          printf("|------------------------------------|\n");
          printf("| -1- Le titre                       |\n"); 
          printf("| -2- L'auteur                       |\n"); 
          printf("| -3- Le prix                        |\n"); 
          printf("| -4- La côte                        |\n"); 
          printf("| -5- La catégorie                   |\n"); 
          printf("| -6- Le format                      |\n"); 
          printf("| -7- L'édition                      |\n"); 
          printf("| -8- La collection                  |\n"); 
          printf("| -9- L'éligibilité à l'emprunt      |\n"); 
          printf("|-10- L'éligibilité à la consultation|\n");
          printf("|-11- La présence en rayon           |\n");
          printf("|                                    |\n");
          printf("| -0- Quitter la modification.       |\n"); 
          printf("✧-------------------------------------✧\n");
          printf("Votre choix : "); 
          scanf("%d",&choix_modif);
          fflush(stdin);
          
          switch(choix_modif)
          {
            case 1 : 
             printf("Nouveau titre :"); 
             lire_chaine_espace(tabdoc[id_indice].titre);
             gerer_espaces_multiples(tabdoc[id_indice].titre);
             gerer_espace_avant_apres(tabdoc[id_indice].titre);
             conv_maj_accents(tabdoc[id_indice].titre,1);
             affichage_1_doc(id_indice);
             break; 
            
            case 2 : 
             printf("Nouvel auteur :"); 
             lire_chaine_espace(tabdoc[id_indice].auteur);
             gerer_espaces_multiples(tabdoc[id_indice].auteur);
             gerer_espace_avant_apres(tabdoc[id_indice].auteur);
             conv_maj_accents(tabdoc[id_indice].auteur,1);
             affichage_1_doc(id_indice);
             break;
            
            case 3 : 
             while (!valide) // Répéter tant que la saisie n'est pas valide
            {
              int nb_points = 0; // Compteur de points (ou virgules remplacées)
              valide = 1; // On suppose que la saisie est valide par défaut

              printf("Nouveau prix : ");
              scanf("%s", saisie); // Lecture de l'entrée utilisateur

              for (int j = 0; saisie[j] != '\0'; j++) 
             {
               if (saisie[j] == ',') // Remplace la virgule par un point
              {
                saisie[j] = '.';  
                nb_points++;
              } 
              else if (saisie[j] == '.') // Compte les points existants
              {
                nb_points++;
              } 
              else if (!isdigit(saisie[j])) // Vérifie si c'est un caractère non autorisé
              {
                valide = 0;  // La saisie est invalide
              }
            }

             if (nb_points > 1) // Plus d'un point/virgule signifie une saisie invalide
             {
               valide = 0; 
             }

             if (!valide) 
            {
              printf("Erreur : saisie invalide. Essayez à nouveau.\n");
            }
          }

           tabdoc[id_indice].prix = atof(saisie); // Convertit la saisie en float et l'assigne au prix
           affichage_1_doc(id_indice); // Affiche les détails du document mis à jour
           break;
         
          case 4 : 
           printf("Nouvelle côte :"); 
           scanf("%s",tabdoc[id_indice].cote);
           gerer_espace_avant_apres(tabdoc[id_indice].cote);
           conv_maj_accents(tabdoc[id_indice].cote,1);
           affichage_1_doc(id_indice);
           break;

          case 5 : 
           printf("Nouvelle catégorie :"); 
           lire_chaine_espace(tabdoc[id_indice].categorie);
           gerer_espaces_multiples(tabdoc[id_indice].categorie);
           gerer_espace_avant_apres(tabdoc[id_indice].categorie);
           conv_maj_accents(tabdoc[id_indice].categorie,1);
           affichage_1_doc(id_indice);
           break;
                
          case 6 : 
           printf("Nouveau format :"); 
           lire_chaine_espace(tabdoc[id_indice].format);
           gerer_espaces_multiples(tabdoc[id_indice].format);
           gerer_espace_avant_apres(tabdoc[id_indice].format);
           conv_maj_accents(tabdoc[id_indice].format,1);
           affichage_1_doc(id_indice);
           break;

          case 7 : 
           printf("Nouvelle édition :"); 
           lire_chaine_espace(tabdoc[id_indice].edition);
           gerer_espaces_multiples(tabdoc[id_indice].edition);
           gerer_espace_avant_apres(tabdoc[id_indice].edition);
           conv_maj_accents(tabdoc[id_indice].edition,1);
           affichage_1_doc(id_indice);
           break;

          case 8 : 
           printf("Nouvelle collection :"); 
           lire_chaine_espace(tabdoc[id_indice].collection);
           gerer_espaces_multiples(tabdoc[id_indice].collection);
           gerer_espace_avant_apres(tabdoc[id_indice].collection);
           conv_maj_accents(tabdoc[id_indice].collection,1);
           affichage_1_doc(id_indice);
           break;

          case 9 : 
           tabdoc[id_indice].eligible_a_lemprunt=2;

          while (tabdoc[id_indice].eligible_a_lemprunt == 2) 
         {
           printf("Le document est-il éligible à l'emprunt ? (oui / non) : ");
           lire_chaine_augmentee(reponse); 
           gerer_espace_avant_apres(reponse); 
           conv_maj_accents(reponse,1); 

           tabdoc[id_indice].eligible_a_lemprunt = reponse_question_fermee_en_int(reponse);

           if (tabdoc[id_indice].eligible_a_lemprunt == 2) 
           {
             printf("Erreur : merci de répondre par 'oui' ou 'non'.\n");
           }
         }
           affichage_1_doc(id_indice); 
           break;

        case 10 : 
         tabdoc[id_indice].eligible_a_consult=2;

         while (tabdoc[id_indice].eligible_a_consult==2)
        {
          printf("Eligibilité à la consultation (oui / non) : ");
          lire_chaine_augmentee(reponse);
          gerer_espace_avant_apres(reponse); 
          conv_maj_accents(reponse,1); 

          tabdoc[id_indice].eligible_a_consult=reponse_question_fermee_en_int(reponse);
          if (tabdoc[id_indice].eligible_a_consult==2)
          {
          printf("Erreur, merci de répondre par oui ou par non.\n");
          }
        }
          affichage_1_doc(id_indice); 
          break;

        case 11 : 
         tabdoc[id_indice].presence_rayons=2;

         while (tabdoc[id_indice].presence_rayons==2)
        {
          printf("Présent en rayons (oui / non) : ");
          lire_chaine_augmentee(reponse);

          tabdoc[id_indice].presence_rayons=reponse_question_fermee_en_int(reponse);
          if (tabdoc[id_indice].presence_rayons==2)
          {
            printf("Erreur, merci de répondre par oui ou par non.\n");
          }
        }
           affichage_1_doc(id_indice); 
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
  a_sauvegarder=1; 
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                ---- Procédure d'affichage d'un abonné ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

void affichage_1_abo(int indice_abo)
{
  struct abonne abo;

  if (nbabo == 0)
  {
    printf("Aucun document à afficher.\n");
  }
  /*else if (document_a_afficher>=nbdoc)
  {
    printf("Le document saisi n'est pas dans la base de données.\n");
  }*/
  else
  {
    abo = tababo[indice_abo];

    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Identifiant                                   : %s\n", abo.id_abonne);
    printf("Nom                                           : %s\n", abo.nom);
    printf("Prénom                                        : %s\n", abo.prenom);
    printf("Date de naissance                             : %02d/%02d/%d\n", abo.date_naissance.jour, abo.date_naissance.mois, abo.date_naissance.annee);
    printf("Adresse mail                                  : %s\n", abo.email);
    printf("Numéro de téléphone                           : %010d\n", abo.num_tel);
    printf("Numéro de voie                                : %s\n", abo.adresse_postale.num_voie);
    printf("Intitulé de voie                              : %s\n", abo.adresse_postale.intitule_voie);
    printf("Code postal                                   : %d\n", abo.adresse_postale.code_postal); 
    printf("Ville                                         : %s\n", abo.adresse_postale.ville); 
    printf("Date du premier abonnement                    : %02d/%02d/%d\n", abo.date_premier_abonnement.jour, abo.date_premier_abonnement.mois, abo.date_premier_abonnement.annee );
    printf("Date du dernier abonnement                    : %02d/%02d/%d\n", abo.date_dernier_souscription.jour, abo.date_dernier_souscription.mois, abo.date_dernier_souscription.annee);   
    printf("Abonnement valide en cours                    : %s\n", int_a_reponse_question_fermee(abo.est_abonne));
    printf("Nombre d'emprunts en cours                    : %d\n",abo.nb_emprunts);
    printf("Nombre d'emprunts en retard                   : %d\n", abo.nb_doc_en_retard);
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");    
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- Procédure de modification d'un abonné ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void ModifierAbo()
{
    int i=0, id_indice, choix_modif, ok;
    int valide = 0;
    char id[TAILLE_ID], reponse[TAILLE_STANDARD];
    char saisie[50];

    printf("Saisissez l'ID de l'abonné à rechercher : ");
    fflush(stdin);
    lire_chaine_augmentee(id);
    conv_maj_accents(id, 0);

    id_indice = -1; 

    while ((id_indice==-1)&&(i<nbabo))
    {
      if (strcmp(tababo[i].id_abonne, id)==0) 
      {
        id_indice = i;
      }
      else
      {
        i++;
      }
    }

    if (id_indice != -1) 
    {
        printf("Abonné trouvé\n");
        affichage_1_abo(id_indice);
        
        choix_modif=-1; // On initialise à tout sauf 0 pour rentrer dans la boucle
        
        while (choix_modif!=0)
        {
          printf("✧------------MODIFICATION------------✧\n");
          printf("|     Que souhaitez-vous modifier ?  |\n"); 
          printf("|------------------------------------|\n");
          printf("| -1- Le nom                         |\n"); 
          printf("| -2- Le prénom                      |\n"); 
          printf("| -3- La date de naissance           |\n"); 
          printf("| -4- L' adresse email               |\n"); 
          printf("| -5- Le numéro de téléphone         |\n"); 
          printf("| -6- L'adresse postale              |\n"); 
          printf("| -7- Date du premier abonnement     |\n"); 
          printf("| -8- Date du dernier abonnement     |\n"); 
          printf("|                                    |\n");
          printf("| -0- Quitter la modification.       |\n"); 
          printf("✧-------------------------------------✧\n");
          printf("Votre choix : "); 
          scanf("%d",&choix_modif);
          fflush(stdin);
          
          switch(choix_modif)
          {
            case 1 : 
             printf("Nouveau nom :"); 
             lire_chaine_espace(tababo[id_indice].nom);
             gerer_espaces_multiples(tababo[id_indice].nom);
             gerer_espace_avant_apres(tababo[id_indice].nom);
             conv_maj_accents(tababo[id_indice].nom,1);
             affichage_1_abo(id_indice);
             break; 
            
            case 2 : 
             printf("Nouveau prénom :"); 
             lire_chaine_espace(tababo[id_indice].prenom);
             gerer_espaces_multiples(tababo[id_indice].prenom);
             gerer_espace_avant_apres(tababo[id_indice].prenom);
             conv_maj_accents(tababo[id_indice].prenom,1);
             affichage_1_abo(id_indice);
             break;
            
            case 3 : 
             while (!valide) 
            {
              tababo[id_indice].date_naissance.jour = -1;
              tababo[id_indice].date_naissance.mois = -1;
              tababo[id_indice].date_naissance.annee = -1;

              printf("Nouvelle date de naissance (jj/mm/aaaa) : ");
             if (scanf("%d/%d/%d", &tababo[id_indice].date_naissance.jour, &tababo[id_indice].date_naissance.mois, &tababo[id_indice].date_naissance.annee) != 3) 
            {
              printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
              fflush(stdin); // Vider le buffer en cas de format incorrect
            }

            if (date_valide(tababo[id_indice].date_naissance.jour, tababo[id_indice].date_naissance.mois,tababo[id_indice].date_naissance.annee)) 
            {
              valide = 1; // La date est valide, on sort de la boucle
            } 
            else 
            {
             printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
             fflush(stdin); // Vider le buffer en cas de date invalide
            }
          }
            affichage_1_abo(id_indice);
            break;
         
          case 4 : 
           while (!valide) 
           {
             printf("Nouvelle adresse email : ");
             lire_chaine_augmentee(tababo[id_indice].email);
             gerer_espaces_multiples(tababo[id_indice].email); 
             gerer_espace_avant_apres(tababo[id_indice].email); 

            if (Verif_Email(tababo[id_indice].email)) 
            {
              printf("Nouvelle adresse email valide\n");
              valide = 1; 
            } 
            else 
           {
             printf("Email invalide. Veuillez rééssayer\n");
             fflush(stdin); 
           }
          }
           affichage_1_abo(id_indice);
           break;

          case 5 : 
           printf("Nouveau numéro de téléphone :"); 
           ok=scanf("%d",&tababo[id_indice].num_tel); 
           fflush(stdin);

           while (!ok)
          {
            printf("Erreur, merci de saisir un numéro valide : ");
            ok=scanf("%d", &tababo[id_indice].num_tel);
            fflush(stdin);
          } 
           affichage_1_abo(id_indice);
           break;
                
          case 6 : 
           printf("Nouveau numéro de voie   : ");
           scanf("%s",tababo[id_indice].adresse_postale.num_voie); 
           fflush(stdin);
      
           printf("Nouvel intitulé de voie  : ");
           lire_chaine_augmentee(tababo[id_indice].adresse_postale.intitule_voie);
           gerer_espaces_multiples(tababo[id_indice].adresse_postale.intitule_voie);
           gerer_espace_avant_apres(tababo[id_indice].adresse_postale.intitule_voie);
           conv_maj_accents(tababo[id_indice].adresse_postale.intitule_voie,1);

           printf("Nouveau code postal      : ");
           scanf("%d",&tababo[id_indice].adresse_postale.code_postal); 
           fflush(stdin);

           printf("Nouvelle ville           :");
           lire_chaine_augmentee(tababo[id_indice].adresse_postale.ville);
           gerer_espaces_multiples(tababo[id_indice].adresse_postale.ville);
           gerer_espace_avant_apres(tababo[id_indice].adresse_postale.ville);
           conv_maj_accents(tababo[id_indice].adresse_postale.ville,1);

           affichage_1_abo(id_indice);
           break;

          case 7 : 
            valide=0; 

            while (!valide) 
            {
              tababo[id_indice].date_premier_abonnement.jour = -1;
              tababo[id_indice].date_premier_abonnement.mois = -1;
              tababo[id_indice].date_premier_abonnement.annee = -1;

              printf("Nouvelle date de premier abonnement (jj/mm/aaaa) : ");
             if (scanf("%d/%d/%d", &tababo[id_indice].date_premier_abonnement.jour, &tababo[id_indice].date_premier_abonnement.mois, &tababo[id_indice].date_premier_abonnement.annee) != 3) 
            {
              printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
              fflush(stdin); // Vider le buffer en cas de format incorrect
            }

            if (date_valide(tababo[id_indice].date_premier_abonnement.jour, tababo[id_indice].date_premier_abonnement.mois,tababo[id_indice].date_premier_abonnement.annee)) 
            {
              valide = 1; // La date est valide, on sort de la boucle
            } 
            else 
            {
             printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
             fflush(stdin); // Vider le buffer en cas de date invalide
            }
          }
            affichage_1_abo(id_indice);
            break;

          case 8 : 
          valide=0; 
          
           while (!valide) 
            {
              tababo[id_indice].date_dernier_souscription.jour = -1;
              tababo[id_indice].date_dernier_souscription.mois = -1;
              tababo[id_indice].date_dernier_souscription.annee = -1;

              printf("Nouvelle date de dernier abonnement (jj/mm/aaaa) : ");
             if (scanf("%d/%d/%d", &tababo[id_indice].date_dernier_souscription.jour, &tababo[id_indice].date_dernier_souscription.mois, &tababo[id_indice].date_dernier_souscription.annee) != 3) 
            {
              printf("Erreur : Format incorrect. Veuillez saisir une date au format jj/mm/aaaa.\n");
              fflush(stdin); // Vider le buffer en cas de format incorrect
            }

            if (date_valide(tababo[id_indice].date_dernier_souscription.jour, tababo[id_indice].date_dernier_souscription.mois,tababo[id_indice].date_dernier_souscription.annee)) 
            {
              valide = 1; // La date est valide, on sort de la boucle
            } 
            else 
            {
             printf("Erreur : Date invalide. Vérifiez que le jour, le mois et l'année sont corrects.\n");
             fflush(stdin); // Vider le buffer en cas de date invalide
            }
          }
            affichage_1_abo(id_indice);
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
   printf("Aucun abonné trouvé avec l'ID %s.\n", id);
  }
  a_sauvegarder=1; 
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//          ---- Procédure de recherche d'un abonné ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

void RechercheAbo()
{
  char id[TAILLE_ID]; 
  int id_indice=-1, i, nb_retard=0;
  struct abonne abo; 
  struct emprunt emp; 
  
  if(nbabo==0)
  {
    printf("Il n'y a aucun abonnés à rechercher.\n"); 
  }
  else
  {
    printf("Saisissez l'ID de l'abonné à rechercher :"); 
    scanf("%s", id); 
    fflush(stdin); 
    
    for (i=0; i<nbabo; i++)
    {
      if(strcmp(tababo[i].id_abonne, id)==0)
      {
        id_indice=i; 
      }
    }
    
    if(id_indice!= -1)
    {
      printf("Abonné trouvé !\n"); 
      
      for(i=0; i<nbabo;i++)
      {
        abo=tababo[id_indice]; 
        
                printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("ID abonné                                     : %s\n", abo.id_abonne);
                printf("Nom                                           : %s\n", abo.nom);
                printf("Prénom                                        : %s\n", abo.prenom);
                printf("Adresse mail                                  : %s\n", abo.email);
                printf("Numéro de téléphone                           : %d\n", abo.num_tel);
                printf("Nombre d'emprunts en cours                    : %d\n", abo.nb_emprunts);
                printf("Date du premier abonnement                    : %02d/%02d/%d\n", abo.date_premier_abonnement.jour, abo.date_premier_abonnement.mois, abo.date_premier_abonnement.annee );
                printf("Date du dernier abonnement                    : %02d/%02d/%d\n", abo.date_dernier_souscription.jour, abo.date_dernier_souscription.mois, abo.date_dernier_souscription.annee);   
                printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"); 
      }
      
      if(abo.nb_emprunts==0)
      {
        printf("Aucun emprunt en cours pour cet abonné.\n");
      }
      else
      {
        for (i=0; i<nbemp; i++)
        {
          emp=tabemp[id_indice]; 
          
                     printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
                     printf("ID emprunt                                    : %s\n", emp.id);
                     printf("ID document                                   : %s\n", emp.emp_id_doc);
                     printf("Date d'emprunt                                : %02d/%02d/%d\n", emp.date_emprunt.jour, emp.date_emprunt.mois, emp.date_emprunt.annee);
                     printf("Date de retour prévue                         : %02d/%02d/%d\n", emp.date_retour_prevue.jour, emp.date_retour_prevue.mois, emp.date_retour_prevue.annee);
                     if(emp.est_en_retard)
                     {
                      printf("Document en retard ! Relancer l'abonné.\n"); 
                      emp.nb_retard++; 
           }
                     printf("Date de retour effective                      : %02d/%02d/%d\n", emp.date_retour_effective.jour, emp.date_retour_effective.mois, emp.date_retour_effective.annee);
                     printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");    
        }
        printf("Nombre de documents en retard : %d\n", emp.nb_retard); 
      }
    }
    else
    {
      printf("Aucun abonné trouvé avec l'ID %s.\n", id);
    }
    
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//       ---- Procédure de renouvellement de l'abonnement ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

void RenouvelerAbonnement()
{
  
  int i=0,indice_id, veut_renouveler;
  char reponse[TAILLE_STANDARD], id_saisi[TAILLE_ID];

  if(nbabo == 0)
  {
    printf("Il n'y a aucun abonné enregistré.\n"); 
  }
  else
  {  
    printf("Saisissez l'ID de l'abonné à rechercher : ");
    fflush(stdin);
    lire_chaine_augmentee(id_saisi);
    conv_maj_accents(id_saisi, 0);

    indice_id = -1; 

    while ((indice_id==-1)&&(i<nbabo))
    {
      if (strcmp(tababo[i].id_abonne, id_saisi)==0) 
      {
        indice_id = i;
      }
      else
      {
        i++;
      }
    }

    if (indice_id != -1) 
    {
      printf("Vérification et renouvellement des abonnements :\n"); 
      printf("\nVérification de l'abonnement de %s %s...\n", tababo[indice_id].nom, tababo[indice_id].prenom); 

      tababo[indice_id] = abonnement_expire(tababo[indice_id]);

      if (tababo[indice_id].est_abonne == 0)  
      {
        printf("L'abonnement est expiré !\n"); 

        veut_renouveler=2; 
        while (veut_renouveler==2) 
        { 
          printf("Souhaitez-vous renouveler cet abonnement (oui/non) : ");
          fflush(stdin);
          lire_chaine_augmentee(reponse);
          veut_renouveler=reponse_question_fermee_en_int(reponse);

          if(veut_renouveler==2)
          { 
            printf("Erreur, merci de répondre par oui ou par non.\n");
          }  

        } 

        if(veut_renouveler)
        { 
          printf("L'abonnement de %s %s a été renouvelé avec succès !\n",tababo[indice_id].nom, tababo[indice_id].prenom);
          tababo[indice_id].date_dernier_souscription = tababo[indice_id].date_actuelle; 
          printf("Nouvelle date de souscription :  %02d/%02d/%02d\n\n", tababo[indice_id].date_dernier_souscription.jour,tababo[indice_id].date_dernier_souscription.mois, tababo[indice_id].date_dernier_souscription.annee);
          tababo[indice_id].est_abonne=1;
        } 
        else
        {
          printf("Pas de renouvellement d'abonnement pour %s %s\n", tababo[indice_id].nom, tababo[indice_id].prenom); 
        } 
      }  
      else
      {
        printf("L'abonnement de %s %s est toujours valide.\n", tababo[indice_id].nom, tababo[indice_id].prenom);
      }    

    }  
    else
    {
      printf("L'id saisi ne semble correspondre à aucun usager du système.\n");
    }

  }

}




// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//           ---- Procédure de suppression d'un document ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*Cas d'extrême néecessité, si un doc est obsolète, un erreur s'est produite, ou un doc devient illégal.. A utiliser avec précaution*/
void SupprDoc()
{
	int id_indice_suppr, i ; 
	char id[TAILLE_ID], confirmation[20]; 
	
	printf("-------------------------------------------AVERTISSEMENT--------------------------------------------------\n");
	printf("ATTENTION, vous êtes dans le module suppression. Cette action est exceptionnelle et irréversible.\n");
	printf("Elle entraînera la suppression définitive du document et de toutes les informations associées.\n");
	printf("Si vous n'êtes pas certain de cette action, privilégiez le retrait du document du fond (module 6). Voulez-vous continuer ? (o/n) : ");
	scanf("%s",confirmation); 
	conv_maj_accents(confirmation, 0);
	if(confirmation[0] == 'O')
	{
	  printf("\nSaisissez l'ID du document à rechercher : ");
          lire_chaine_augmentee(id);
          conv_maj_accents(id, 0);

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
			
	           printf("Suppression impossible : le document %s de %s est emprunté\n",tabdoc[id_indice_suppr].titre, tabdoc[id_indice_suppr].auteur);
		}
		else 
		{
	      printf("-------------------------------------------AVERTISSEMENT--------------------------------------------------\n");

		    printf("ATTENTION, Cette action sera irréversible. Etes vous sûr de vouloir supprimer le document ? (o/n) : "); 
		    scanf("%s",confirmation);
		     conv_maj_accents(confirmation, 0); 
		        if(confirmation[0] == 'O')
			{
			   for (i=id_indice_suppr ; i<nbdoc -1 ; i++)
			   {
			     tabdoc[i]=tabdoc[i+1]; 
			   }
			   nbdoc -- ; // On met à jour le nb de documents
			   printf("Le document avec l'ID %s a été supprimé avec succès.\n", id);
			}
			else
			{
			   printf("Suppression annulée.\n"); 
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
	printf("Suppression annulée, aucun document n'a été supprimé.\n"); 
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
      conv_maj_accents(ElementRech, 1);

      for (i = 0; i < nbdoc; i++)
      {
       /*---Pour pas toucher aux titre et auteur de base qui ne s'écrivent pas tout en majuscule... jsp si on garde.. à tester sans---*/
        strcpy(titre_maj, doc[i].titre);
        strcpy(auteur_maj, doc[i].auteur);
        /*strcpy(categorie_maj, doc[i].categorie);
        strcpy(edition_maj, doc[i].edition);
        strcpy(collection_maj, doc[i].collection);*/
        
        gerer_espaces_multiples(titre_maj);
        gerer_espace_avant_apres(titre_maj);
        conv_maj_accents(titre_maj, 1);

        gerer_espaces_multiples(auteur_maj);
        gerer_espace_avant_apres(auteur_maj);
        conv_maj_accents(auteur_maj, 1);
        
        /*gerer_espaces_multiples(categorie_maj);
        gerer_espace_avant_apres(categorie_maj);
        conv_maj_accents(categorie_maj, 1);
        
        gerer_espaces_multiples(edition_maj);
        gerer_espace_avant_apres(edition_maj);
        conv_maj_accents(edition_maj, 1);
        
        gerer_espaces_multiples(collection_maj);
        gerer_espace_avant_apres(collection_maj);
        conv_maj_accents(collection_maj, 1);*/
        
        
        /*---La fonction strstr me permet de vérifier s'il existe une correspondance entre la chaine auteur_maj et la sous_chaine ElementRech (notre saisie d'un élément
        à rechercher). Si c'est différent de null ça veut dire qu'il y a une correspondance entre les 2 ---*/
          if ((strstr(titre_maj, ElementRech) != NULL ) || (strstr(auteur_maj, ElementRech) != NULL))
          {
            correspondance = 1;
		    printf("Document trouvé : [ID : %s], Côte : %s\n, Titre : %s\n, Auteur : %s\n ",doc[i].id, doc[i].cote, doc[i].titre, doc[i].auteur);
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
  struct abonne abo;
  struct emprunt emp;
  int i=0, j=0, retour, lecture_docs = 0, lecture_usagers = 0, lecture_emprunts = 0, veut_charger, usa_trouve=0;
  FILE *f1;
  char NomFichier[TAILLE_NOM_F], section[TAILLE_ID], prefixe_id[TAILLE_STANDARD], reponse[TAILLE_STANDARD];

  // On vérifie si une sauvegarde doit être proposée
  verif_sauvegarde();
  
  veut_charger=2; //on initie à la valeur d'erreur
  while (veut_charger==2) //Tant que veut_charger est à la valeur d'erreur
  {
    printf("Voulez vous charger un fichier de documents (oui/non) : ");
    fflush(stdin);
    lire_chaine_augmentee(reponse);
    veut_charger=reponse_question_fermee_en_int(reponse);
    if (veut_charger==2) //Si l'usager a saisi autre chose que oui, non ou équivalent
      {
        printf("Erreur, merci de répondre par oui ou par non.\n");
      }
  }    
  if (veut_charger) //Si la valeur de veut charger est true (oui) on lance le processus de chargement d'1 fichier document
  {
    printf("Nom du fichier de chargement des documents            : "); 
    scanf("%s",NomFichier);
  
    f1=fopen(NomFichier,"r");
    if (!f1)
    {
      printf("Erreur, impossible d'ouvrir le fichier : %s.\n", NomFichier);
    }
    else
    {
      while ((!feof(f1)) && (i<MAX_DOC))
      {
        retour=fscanf(f1, "%s %s %s %s %s %s %d/%d/%d %s %s %s %f %d %d %d %d %d %d/%d/%d %d/%d/%d\n",doc.id, doc.cote, doc.titre, doc.auteur, doc.categorie, doc.edition, &doc.date_premiere_publi.jour, &doc.date_premiere_publi.mois, &doc.date_premiere_publi.annee, doc.collection, doc.format, doc.ISBN13, &doc.prix, &doc.eligible_a_lemprunt,&doc.est_emprunte, &doc.eligible_a_consult, &doc.presence_rayons, &doc.present_dans_le_fond, &doc.date_deb.jour, &doc.date_deb.mois, &doc.date_deb.annee, &doc.date_fin.jour, &doc.date_fin.mois, &doc.date_fin.annee); 
    
        if (retour != EOF)
        {
          conv_souligne_en_espace(doc.id);
          conv_souligne_en_espace(doc.cote);
          conv_souligne_en_espace(doc.titre);
          conv_souligne_en_espace(doc.auteur);
          conv_souligne_en_espace(doc.categorie);
          conv_souligne_en_espace(doc.edition);
          conv_souligne_en_espace(doc.collection);
          conv_souligne_en_espace(doc.format);
      
          tabdoc[i++] = doc ;   
             
        }
      }
      fclose(f1);
      nbdoc = i;  
      printf("%d documents lu(s)\n",nbdoc); 
      i=0; //on remet i à 0 pour les prochains chargement (usagers, emprunts, etc...)
    }  
  }


 veut_charger=2;
  while (veut_charger==2)
  {
    printf("Voulez vous charger un fichier d'usagers (oui/non)    : ");
    fflush(stdin);
    lire_chaine_augmentee(reponse);
    veut_charger=reponse_question_fermee_en_int(reponse);
    if (veut_charger==2)
      {
        printf("Erreur, merci de répondre par oui ou par non.\n");
      }
  }  
  if (veut_charger)
  {
    printf("Nom du fichier de chargement des usagers              : ");
    scanf("%s",NomFichier);
    f1=fopen(NomFichier,"r");
    if (!f1)
    {
      printf("Erreur, impossible d'ouvrir le fichier : %s.\n", NomFichier);
    }
    else
    {
      while ((!feof(f1)) && (i<MAX_ABO))  
      {
        retour = fscanf(f1, "%s %s %s %d %d/%d/%d %s %s %s %d %s %d/%d/%d %d/%d/%d %d %d %d\n",abo.id_abonne,
        abo.nom, abo.prenom, &abo.num_tel,
        &abo.date_naissance.jour, &abo.date_naissance.mois, &abo.date_naissance.annee,
        abo.email, abo.adresse_postale.intitule_voie, abo.adresse_postale.num_voie,
        &abo.adresse_postale.code_postal, abo.adresse_postale.ville,
        &abo.date_premier_abonnement.jour, &abo.date_premier_abonnement.mois, &abo.date_premier_abonnement.annee,
        &abo.date_dernier_souscription.jour, &abo.date_dernier_souscription.mois, &abo.date_dernier_souscription.annee,
        &abo.nb_emprunts, &abo.est_abonne, &abo.nb_doc_en_retard);

        if (retour != EOF) 
        {
          conv_souligne_en_espace(abo.nom);
          conv_souligne_en_espace(abo.prenom);
          conv_souligne_en_espace(abo.email);
          conv_souligne_en_espace(abo.adresse_postale.intitule_voie);
          conv_souligne_en_espace(abo.adresse_postale.ville);

          abo.nb_doc_en_retard=0; //sera calculer lors du chargement des emprunts
          abo= abonnement_expire(abo);//on regarde si l'abonnement est valide par rapport aux dates de souscription


          tababo[i++] = abo;
        }
      }
      fclose(f1);
      nbabo = i;  
      printf("%d usager(s) lu(s)\n",nbabo); 
      i=0;
    }
  }

  if((nbdoc!=0)&&(nbabo!=0))
  {
    veut_charger=2;
    while (veut_charger==2)
    {
        printf("Voulez vous charger un fichier d'emprunts (oui/non)   : ");
        fflush(stdin);
        lire_chaine_augmentee(reponse);
        veut_charger=reponse_question_fermee_en_int(reponse);
        if (veut_charger==2)
        {
            printf("Erreur, merci de répondre par oui ou par non.\n");
        }
    }  
    if (veut_charger)
    {
        printf("Nom du fichier de chargement des emprunts             : ");
        scanf("%s",NomFichier);
        f1=fopen(NomFichier,"r");
        if (!f1)
        {
        printf("Erreur, impossible d'ouvrir le fichier : %s.\n", NomFichier);
        }
        else
        {
        while ((!feof(f1)) && (i<MAX_EMPRUNTS))  
        {
            retour = fscanf(f1, "%s %s %s %d/%d/%d %d/%d/%d %d/%d/%d %d %s %s %s\n", emp.id, emp.emp_id_abo, emp.emp_id_doc, &emp.date_emprunt.jour, &emp.date_emprunt.mois, &emp.date_emprunt.annee, &emp.date_retour_prevue.jour, &emp.date_retour_prevue.mois, &emp.date_retour_prevue.annee, &emp.date_retour_effective.jour, &emp.date_retour_effective.mois, &emp.date_retour_effective.annee, &emp.est_en_retard, emp.nom_abo_emprunteur, emp.prenom_abo_emprunteur, emp.titre_doc_emprunte);

            if (retour != EOF) 
            {
              conv_souligne_en_espace(emp.nom_abo_emprunteur);
              conv_souligne_en_espace(emp.prenom_abo_emprunteur);
              conv_souligne_en_espace(emp.titre_doc_emprunte);
                if (emp.date_retour_effective.jour==-1)//si le doc n'a pas encore été retourné on vérifie s'il est en retard ou non
                {   
                  nbemp_en_cours++; //Si le doc n'a pas encoure été retourné, l'emprunt est en cours, on incrémente donc le compteur associé

                  struct tm jma_prevue = {0};
                  time_t nb_seconde_retour_prevu, nb_seconde_now, tp;

                  jma_prevue.tm_mday=emp.date_retour_prevue.jour;
                  jma_prevue.tm_mon=emp.date_retour_prevue.mois-1;
                  jma_prevue.tm_year=emp.date_retour_prevue.annee-1900;

                  nb_seconde_retour_prevu=mktime(&jma_prevue);
                  nb_seconde_now=time(&tp);  

                  if (nb_seconde_retour_prevu<nb_seconde_now) //Si le document est en retard alors
                  {
                    emp.est_en_retard=1;
                    while ((!usa_trouve)&&(j<nbabo))
                    {
                      //DEBUG
                      printf("id abo dans struct emprunt : %s\n", emp.emp_id_abo);
                      printf("id abo ds struct abo : %s\n",tababo[j].id_abonne);
                      if (strcmp(emp.emp_id_abo,tababo[j].id_abonne)==0)
                      {
                        tababo[j].nb_doc_en_retard++;
                        usa_trouve=1;
                      }
                      else
                      {
                        j++;
                      }
                    }
                    if (!usa_trouve)
                    {
                      printf("Comportement innatendu, merci de vérifier la cohérence de vos fichiers de données.\n");
                    }
                  }
                  else
                  {
                    emp.est_en_retard=0;
                  }
                  //DEBUG
                  //printf("Date prévue : %02d/%02d/%04d\n", jma_prevue.tm_mday, jma_prevue.tm_mon + 1, jma_prevue.tm_year + 1900);
                  //printf("Secondes retour prévu : %ld, Secondes actuelles : %ld\n", nb_seconde_retour_prevu, nb_seconde_now);
                  //printf("Est en retard : %d\n", emp.est_en_retard);  
                }
                tabemp[i++] = emp;
            }
        }
        fclose(f1);
        nbemp = i;  
        printf("%d emprunt(s) lu(s)\n",nbemp); 
        i=0;
        }
    } 
  } 
  else
  {
    printf("Pour accéder au chargement d'un fichier d'emprunts vous devez avoir renseigné des documents et des usagers.\n");
  }
}



// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                      ----Procédure de sauvegarde ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void sauvegarde()
{
  // variables
  struct document doc;
  struct abonne abo;
  struct emprunt emp;
  int i=0, retour;
  FILE *f1;
  char NomFichier[TAILLE_NOM_F];

  if ((nbdoc == 0)&&(nbabo==0)&&(nbemp==0))
  {
    printf("Aucune donnée à sauvegarder\n");
  }
  else
  {
    if (nbdoc!=0)
    {
      printf("Nom du fichier de sauvegarde des documents : ");
      scanf("%s",NomFichier);
      f1=fopen(NomFichier,"w");
      for (i=0 ; i < nbdoc ; i++)//documents
      {
        doc = tabdoc[i];
        conv_espace_en_souligne(doc.cote);
        conv_espace_en_souligne(doc.titre);
        conv_espace_en_souligne(doc.auteur);
        conv_espace_en_souligne(doc.categorie);
        conv_espace_en_souligne(doc.edition);
        conv_espace_en_souligne(doc.collection);
        conv_espace_en_souligne(doc.format);

       fprintf(f1, "%s %s %s %s %s %s %d/%d/%d %s %s %s %f %d %d %d %d %d %d/%d/%d %d/%d/%d\n",doc.id, doc.cote, doc.titre, doc.auteur, doc.categorie, doc.edition, doc.date_premiere_publi.jour, doc.date_premiere_publi.mois, doc.date_premiere_publi.annee, doc.collection, doc.format, doc.ISBN13, doc.prix, doc.eligible_a_lemprunt, doc.est_emprunte, doc.eligible_a_consult, doc.presence_rayons, doc.present_dans_le_fond, doc.date_deb.jour, doc.date_deb.mois, doc.date_deb.annee, doc.date_fin.jour, doc.date_fin.mois, doc.date_fin.annee);   
      } 
      fclose(f1);
    }
    if (nbabo!=0)
    {
      printf("Nom du fichier de sauvegarde des usagers   : ");
      scanf("%s",NomFichier);
      f1=fopen(NomFichier,"w");
      for (i=0 ; i < nbabo ; i++)//usagers
      {
        abo=tababo[i];
        conv_espace_en_souligne(abo.nom);
        conv_espace_en_souligne(abo.prenom);
        conv_espace_en_souligne(abo.email);
        conv_espace_en_souligne(abo.adresse_postale.intitule_voie);
        conv_espace_en_souligne(abo.adresse_postale.ville);

        fprintf(f1, "%s %s %s %d %d/%d/%d %s %s %s %d %s %d/%d/%d %d/%d/%d %d %d %d\n", abo.id_abonne, abo.nom, abo.prenom, abo.num_tel, abo.date_naissance.jour, abo.date_naissance.mois, abo.date_naissance.annee, abo.email, abo.adresse_postale.intitule_voie, abo.adresse_postale.num_voie, abo.adresse_postale.code_postal, abo.adresse_postale.ville, abo.date_premier_abonnement.jour, abo.date_premier_abonnement.mois, abo.date_premier_abonnement.annee, abo.date_dernier_souscription.jour, abo.date_dernier_souscription.mois, abo.date_dernier_souscription.annee, abo.nb_emprunts, abo.est_abonne, abo.nb_doc_en_retard);
      }
      fclose(f1);
    }
    if (nbemp!=0)
    {
      printf("Nom du fichier de sauvegarde des emprunts  : ");
      scanf("%s",NomFichier);
      f1=fopen(NomFichier,"w");
    
      for (i=0 ; i < nbemp ; i++)
      {
        emp=tabemp[i];
        conv_espace_en_souligne(emp.nom_abo_emprunteur);
        conv_espace_en_souligne(emp.prenom_abo_emprunteur);
        conv_espace_en_souligne(emp.titre_doc_emprunte);
        fprintf(f1, "%s %s %s %d/%d/%d %d/%d/%d %d/%d/%d %d %s %s %s\n", emp.id, emp.emp_id_abo, emp.emp_id_doc, emp.date_emprunt.jour, emp.date_emprunt.mois, emp.date_emprunt.annee, emp.date_retour_prevue.jour, emp.date_retour_prevue.mois, emp.date_retour_prevue.annee, emp.date_retour_effective.jour, emp.date_retour_effective.mois, emp.date_retour_effective.annee, emp.est_en_retard, emp.nom_abo_emprunteur, emp.prenom_abo_emprunteur, emp.titre_doc_emprunte);

      }
      fclose(f1);
    }
    printf("%d documents(s) sauvegardé(s), %d usager(s) sauvegardé(s), %d emprunts sauvegardé(s).\n",nbdoc, nbabo, nbemp);
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
    fflush(stdin);
    lire_chaine_augmentee(reponse); 
    conv_maj_accents(reponse, 1);
    if (reponse_question_fermee_en_int(reponse))
    {
      sauvegarde();
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
//                          ---- Emprunt d'un doc ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void emprunt_doc ()
{
  int i=0, j=0, case_abo, case_doc, doc_dispo, id_doc_valide;
  struct emprunt emp;
  char id_abo_saisi[TAILLE_ID], id_doc_saisi[TAILLE_TITRE];

  if ((nbdoc==0)&&(nbabo==0))
  {
    printf("Il n'y a ni document, ni abonnés renseignés.\n");
  }
  else if (nbdoc==0)
  {
    printf("Il n'y a aucun document renseigné.\n");
  }
  else if (nbabo==0)
  {
    printf("Il n'y a aucun abonné renseigné.\n");
  }
  else
  {
    printf("Saisisez l'id de l'abonné associé à l'emprunt   : ");
    fflush(stdin);
    lire_chaine_augmentee(id_abo_saisi);
    conv_maj_accents(id_abo_saisi, 0);

    case_abo=-1;
    while ( (i<nbabo) && (case_abo==-1) )
    {
      if (strcmp(tababo[i].id_abonne, id_abo_saisi)==0) 
	  {
        case_abo = i;
      }
      else
      {
        i++;
      }
    }
    if (case_abo!=-1) // si l'abonné existe, on vérifie qu'il peut faire des emprunts
    {
      if (tababo[case_abo].est_abonne==0) //on vérifie si l'abonné a toujours un abonnement valide
      {
        printf("Cet usager ne dispose pas d'un abonnement valide.\n");
      }
      else if (tababo[case_abo].nb_emprunts>=MAX_EMPRUNTS) //on vérifie si l'abonné a déjà emprunté le maximum de livres
      {
        printf("Cet abonné à déjà emprunté le maximum de livres.\n");
      }
      else if (tababo[case_abo].nb_doc_en_retard!=0) //on vérifie si l'abonné a des documents à rendre en retard.
      {
        printf("Cet abonné a des documents à rendre en retard.\n");
      }
      else //si tout est bon on saisit l'id du document que l'on souhaite emprunter
      { 
        printf("Saisisez l'identifiant du document à emprunter  : ");
        lire_chaine_augmentee(id_doc_saisi);
        conv_maj_accents(id_doc_saisi, 1);

        case_doc=-1;
        id_doc_valide=0;
        doc_dispo=0; 
        while ((id_doc_valide==0)&&(j<nbdoc))
        {
            //DEBUG
            //printf("id_doc_saisi %s\n", id_doc_saisi);
            //printf("tabdoc[j].id %s\n", tabdoc[j].id);
            if (strcmp(tabdoc[j].id, id_doc_saisi)==0) 
		    {
                id_doc_valide=1;
                //DEBUG
                //printf("valide\n");
                //printf("Présent dans le fonds : %d eligible à l'emprunt : %d emprunté : %d\n", tabdoc[j].present_dans_le_fond, tabdoc[j].eligible_a_lemprunt, tabdoc[j].est_emprunte);
                if ((tabdoc[j].present_dans_le_fond!=0)&&(tabdoc[j].eligible_a_lemprunt!=0)&&(tabdoc[j].est_emprunte==0))
                    {
                        case_doc=j;
                        doc_dispo=1;
                        //DEBUG printf("exemplaire dispo\n");
                    }
            }
            j++;
        }

        //Si on cherche à partir du titre, mais pas optimal en fait, tej ?
        /*printf("Saisisez le titre du document à emprunter   : ");
        lire_chaine_augmentee(titre_saisi);
        conv_maj_accents(titre_saisi, 1);

        case_doc=-1;
        titre_valide=0;
        un_exemplaire_dispo=0; 
        while ((un_exemplaire_dispo==0)&&(j<=nbdoc))
        {
            strcpy(stockage_titre_i, tabdoc[j].titre);
            conv_maj_accents(stockage_titre_i,1);
            if (strcmp(stockage_titre_i, titre_saisi)==0) 
		    {
                titre_valide=1;
                if ((tabdoc[j].present_dans_le_fond!=0)&&(tabdoc[j].eligible_a_lemprunt!=0)&&(tabdoc[j].est_emprunte==0))
                    {
                        case_doc=j;
                        un_exemplaire_dispo=1;
                    }
            }
            j++;
        }*/
        if ((doc_dispo==0)&&(id_doc_valide==1))
        {
            printf("Ce document n'est pas disponible pour un emprunt.\n");
        }
        else if (doc_dispo!=0)
        {
            strcpy(emp.id, "EMP"); //L'identifiant prend le préfixe EMP afin de le différencier des identifiants des usagers notamment
            strcat(emp.id,(mon_itoa(nbemp+1))); //Au préfixe EMP s'ajoute le nombre d'emprunts existants +1 
            printf("Identifiant                                     : %s\n", emp.id);         

            strcpy(emp.emp_id_abo,tababo[case_abo].id_abonne);
            printf("Id de l'abonne                                  : %s\n", emp.emp_id_abo);

            strcpy(emp.emp_id_doc,tabdoc[case_doc].id);
            printf("Id du document                                  : %s\n", emp.emp_id_doc);

            time_t tp,nbsec;
            struct tm hjm ;
            nbsec=time(&tp);
            hjm=*localtime(&nbsec);
            emp.date_emprunt.jour=hjm.tm_mday;
            emp.date_emprunt.mois=hjm.tm_mon+1;
            emp.date_emprunt.annee=hjm.tm_year+1900;
            printf("Date de l'emprunt                               : %02d/%02d/%d\n", emp.date_emprunt.jour, emp.date_emprunt.mois, emp.date_emprunt.annee);

            time_t nb_secondes_TU, temps, nb_secondes_date_retour_prevue;
            struct tm heure_jour_mois ;
            nb_secondes_TU = time(&temps);
            nb_secondes_date_retour_prevue=nb_secondes_TU+(DUREE_EMPRUNT)*24*60*60;
            heure_jour_mois = *localtime(&nb_secondes_date_retour_prevue);
            emp.date_retour_prevue.jour=heure_jour_mois.tm_mday;
            emp.date_retour_prevue.mois=heure_jour_mois.tm_mon+1;
            emp.date_retour_prevue.annee=heure_jour_mois.tm_year+1900;
            printf("Retour prévu avant le                           : %02d/%02d/%d\n",emp.date_retour_prevue.jour, emp.date_retour_prevue.mois, emp.date_retour_prevue.annee); //pour vérifier que ça marche

            emp.date_retour_effective.jour=-1;
            emp.date_retour_effective.mois=-1;
            emp.date_retour_effective.annee=-1;
            //Affichage de la date de retour effective pour les tests : 
            //printf("Date de retour effective : %d/%d/%d\n", emp.date_retour_effective.jour, emp.date_retour_effective.mois, emp.date_retour_effective.annee); 

            emp.est_en_retard=0;

            strcpy(emp.nom_abo_emprunteur, tababo[case_abo].nom);
            strcpy(emp.prenom_abo_emprunteur, tababo[case_abo].prenom);
            strcpy(emp.titre_doc_emprunte, tabdoc[case_doc].titre);
            printf("Titre                                           : %s\n", emp.titre_doc_emprunte); //a tej, pr debug

            tabdoc[case_doc].est_emprunte=1;
            tababo[case_abo].nb_emprunts++;

            //pr retard on fera si la date de rendue est -1 -1 -1 et si 

            tabemp[nbemp]=emp;
            nbemp++;
            nbemp_en_cours++;

        }
        else if (id_doc_valide==0)
        {
            printf("Erreur, aucun document ne correspond à votre recherche.\n");
        }
      }
    }
    else
    printf("Erreur, aucun usager ne correspond à votre recherche.\n");
  }
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                          ---- Emprunt d'un doc ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

void historique_emprunt()
{ 


  int i; 
  char id[TAILLE_ID];

  if ((nbdoc==0)&&(nbabo==0))
  {
    printf("Il n'y a ni document, ni abonnés renseignés.\n");
  }
  else if (nbdoc==0)
  {
    printf("Il n'y a aucun document renseigné.\n");
  }
  else if (nbabo==0)
  {
    printf("Il n'y a aucun abonné renseigné.\n");
  }
  
  else 
  { 
    printf("Saisissez l'id du document dont vous souhaitez connaître l'historique d'emprunt  : ");
    fflush(stdin);
    lire_chaine_augmentee(id);
    conv_maj_accents(id, 0);

    if (nbemp == 0)
    {
       printf("Il n'y a aucun emprunt enregistré.\n");
    }
    else
    {

        printf("Historique des emprunts pour le document ID %s :\n", id);
        printf("------------------------------------------------------------------------------------------------------\n");
        
       
       
        for (i = 0; i < nbemp; i++) 
        {
            if (strcmp(tabemp[i].emp_id_doc, id) == 0) 
            {
                
                printf("\n Emprunt n°%d\n", i + 1);
                printf("Identifiant emprunt   : %s\n", tabemp[i].id);
                printf("Identifiant abonné    : %s\n", tabemp[i].emp_id_abo);
                printf("Date d'emprunt        : %02d/%02d/%02d\n", tabemp[i].date_emprunt.jour, tabemp[i].date_emprunt.mois, tabemp[i].date_emprunt.annee);
                printf("Retour prévu avant le : %02d/%02d/%02d\n", tabemp[i].date_retour_prevue.jour, tabemp[i].date_retour_prevue.mois, tabemp[i].date_retour_prevue.annee);

                if (tabemp[i].date_retour_effective.jour != -1 && tabemp[i].date_retour_effective.mois != -1 && tabemp[i].date_retour_effective.annee != -1)
                {
                    printf("Retour réel le    : %02d/%02d/%02d\n", tabemp[i].date_retour_effective.jour,tabemp[i].date_retour_effective.mois, tabemp[i].date_retour_effective.annee);
                }
                else 
                { 
                    printf("  Document en cours d'utilisation.\n");
                }

                if (tabemp[i].est_en_retard)
                { 
                    printf("Document en retard : oui\n");
                }
                else
                { 
                    printf("Document en retard : non\n");
                }

                printf("------------------------------------------------------------------------------------------------------\n");
            }
            else
            {
              printf("Aucun emprunt trouvé pour le document avec l'ID %s.\n", id);
            }

        }

    
      }

    }
    
}



// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                          ---- Emprunt d'un doc ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void afficher_liste_emprunt()
{
    // variables
  struct emprunt emp;
  int i=0;
  char message_si_pas_encore_retourne[TAILLE_STANDARD];
  strcpy(message_si_pas_encore_retourne, "NON RENDU");

  if (nbemp == 0)
  {
    printf("Aucun emprunt à afficher\n");
  }
  else
  {
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-12s %-12s %-51s %-12s %-51s %-18s %-18s %-18s %-8s\n","ID EMPRUNT","ID ABONNE","NOM ABONNE","ID DOCUMENT", "TITRE DOCUMENT", "DATE D'EMPRUNT", "DATE LIMITE RETOUR", "DATE DE RETOUR", "RETARD");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (i=0 ; i < nbemp ; i++)
    {
      emp=tabemp[i]; 
      
       
      if (emp.date_retour_effective.jour!=-1) //affichage si le doc a été rendu
      {
        printf("%-12s %-12s %-51s %-12s %-51s %02d/%02d/%-12d %02d/%02d/%-12d %02d/%02d/%-12d %-8s \n",emp.id, emp.emp_id_abo, emp.nom_abo_emprunteur, emp.emp_id_doc, emp.titre_doc_emprunte, emp.date_emprunt.jour, emp.date_emprunt.mois, emp.date_emprunt.annee, emp.date_retour_prevue.jour, emp.date_retour_prevue.mois, emp.date_retour_prevue.annee, emp.date_retour_effective.jour, emp.date_retour_effective.mois, emp.date_retour_effective.annee, int_a_reponse_question_fermee(emp.est_en_retard)); 
      }
      else //affichage si le doc n'a pas été rendu
      {
        printf("%-12s %-12s %-51s %-12s %-51s %02d/%02d/%-12d %02d/%02d/%-12d %-18s %-8s \n",emp.id, emp.emp_id_abo, emp.nom_abo_emprunteur, emp.emp_id_doc,emp.titre_doc_emprunte, emp.date_emprunt.jour, emp.date_emprunt.mois, emp.date_emprunt.annee, emp.date_retour_prevue.jour, emp.date_retour_prevue.mois, emp.date_retour_prevue.annee, message_si_pas_encore_retourne, int_a_reponse_question_fermee(emp.est_en_retard)); 
      }
      printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
  } 
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//                          ---- Emprunt d'un doc ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void retour_emprunt()
{   
    char saisie_id_doc[TAILLE_STANDARD], saisie_id_abo[TAILLE_STANDARD], reponse_cdc[TAILLE_STANDARD];
    int i=0, j=0,h=0,  saisie_id_doc_valide=0, reponse_entier=2, abonne_trouve=0, valide=0;
    struct emprunt emp;

    if (nbemp_en_cours==0) //on vérifie qu'il y a des emprunts en cours
    {
        printf("Il n'y a pas de d'emprunts en cours.\n");
    }
    else //Si c'est bien le cas on demande l'id du document à retourner
    {
        printf("Saisissez l'id du document à retourner                                                    : ");
        fflush(stdin);
        lire_chaine_augmentee(saisie_id_doc);
        conv_maj_accents(saisie_id_doc, 0);
        while ((i<nbdoc)&&(!saisie_id_doc_valide))
        { 
            if ((strcmp(saisie_id_doc,tabemp[i].emp_id_doc)==0)&&(tabemp[i].date_retour_effective.jour==-1)) //on vérifie que l'id correspond mais également que le document est actuellement en cours d'emprunt
            {
                saisie_id_doc_valide=1;
            }
            else
            {
                i++;
            }
        }
        if (!saisie_id_doc_valide) //si toute la liste a été parcourue sans succès c'est que l'id du document n'est pas associée à un emprunt
        {
            printf("Il n'y a pas d'emprunt en cours avec ce document.\n");
        }
        else
        {
            //a tej
            /*printf("Rentrez l'identifiant de l'abonné associé à l'emprunt : ");//on vérifie qu'il s'agit bien de la bonne personne qui a rapporté le document
            lire_chaine_augmentee(reponse);
            conv_maj_accents(reponse,0);
            if (strcmp(reponse, tabemp[i].emp_id_abo)==0) //si l'abonné correspond au doc emprunté alors : */
            
            while ((j<nbabo)&&(!abonne_trouve))
        { 
            if (strcmp(tabemp[i].emp_id_abo,tababo[j].id_abonne)==0) //on vérifie que l'id correspond mais également que le document est actuellement en cours d'emprunt
            {
                abonne_trouve=1;
            }
            else
            {
                j++;
            }
        }

            printf("L'abonné associé à cet emprunt est %s %s dont l'identfifiant est %s. Est-ce correct : ", tababo[j].prenom, tababo[j].nom, tababo[j].id_abonne);
            while (reponse_entier==2)
            {
                lire_chaine_augmentee(reponse_cdc);
                reponse_entier=reponse_question_fermee_en_int(reponse_cdc);
                if (reponse_entier==2)
                {
                    printf("Erreur, merci de répondre par oui ou par non                                                   :");
                }
            }
            if (reponse_entier==1)
            {
                time_t tp,nbsec;
                struct tm hjm ;
                nbsec=time(&tp);
                hjm=*localtime(&nbsec);
                tabemp[i].date_retour_effective.jour=hjm.tm_mday;
                tabemp[i].date_retour_effective.mois=hjm.tm_mon+1;
                tabemp[i].date_retour_effective.annee=hjm.tm_year+1900;

                if (tabemp[i].est_en_retard==1)
                {
                    printf("Ce document devait être rendu avant le %02d/%02d/%d, merci de faire attention la prochaine fois.\n", tabemp[i].date_retour_prevue.jour, tabemp[i].date_retour_prevue.mois, tabemp[i].date_retour_prevue.annee);
                    tababo[j].nb_doc_en_retard--;//l'abonne a un doc en retard en moins

                }
                while(!valide)
                {
                  if (strcmp(tabemp[i].emp_id_doc, tabdoc[h].id)==0)
                  {
                    valide=1;
                  }
                  else
                  {
                    h++;
                  }
                }
                if (!valide)
                {
                  printf("Erreur, document non trouvé parmi les fichiers renseignés, vérifiez la cohérence des fichiers chargés.\n");
                }
                else
                {
                  tabdoc[h].est_emprunte=0;
                }
                nbemp_en_cours--;
                printf("Le document a bien été retourné le %02d/%02d/%d, merci.\n", tabemp[i].date_retour_effective.jour, tabemp[i].date_retour_effective.mois, tabemp[i].date_retour_effective.annee);
                if (tababo[j].nb_doc_en_retard!=0)
                {
                    printf("Attention, %d de vos documents à rendre sont en retard, merci de les rapporter au plus vite.\n", tababo[j].nb_doc_en_retard);
                }
            }
            else
            {
                printf("L'identifiant renseigné ne correspond pas à l'abonné qui a réalisé l'emprunt.\n");
            }

        }
    }
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure d'afichage des statistiques ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void sorti_du_fond()
{ 
  char id_saisi[TAILLE_ID], reponse[TAILLE_STANDARD];
  int indice_arch, valide=0, i=0, rep_valide=0;

  printf("Saisissez l'ID du document à rechercher : ");
  fflush(stdin);
  lire_chaine_augmentee(id_saisi);
  conv_maj_accents(id_saisi, 1);

  indice_arch = -1; 
  while((!valide)&&(i<nbdoc))
	{
		if (strcmp(tabdoc[i].id, id_saisi)==0) 
		{
      valide=1;
      if (tabdoc[i].est_emprunte==1)
      {
        printf("Ce document est en cours d'emprunt, il n'est pas possible de changer son statut de présence dans le fond.\n");
      }
      else
      {
        tabdoc[i].present_dans_le_fond=0;
        tabdoc[i].eligible_a_lemprunt=0;
        tabdoc[i].eligible_a_consult=0;
        tabdoc[i].presence_rayons=0;

        time_t tp,nbsec;
            struct tm hjm ;
            nbsec=time(&tp);
            hjm=*localtime(&nbsec);
            tabdoc[i].date_fin.jour=hjm.tm_mday;
            tabdoc[i].date_fin.mois=hjm.tm_mon+1;
            tabdoc[i].date_fin.annee=hjm.tm_year+1900;

        printf("Le document %s, %s de %s a bien été sorti du fond.\n",tabdoc[i].id, tabdoc[i].titre, tabdoc[i].auteur);
      }
	  }
    else
    {
      i++;
    }
  }
  if (!valide)
  {
    printf("Erreur, aucun document ne correspond à l'id saisi.\n");
  }
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure d'afichage des statistiques ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void statistiques () //à compléter 
{
  printf("Nombre de documents présents dans le système  : %d\n", nbdoc);
  printf("Nombre d'usagers présents dans le système     : %d\n", nbabo);
  printf("Nombre d'emprunts en cours                    : %d\n", nbemp_en_cours);
  printf("Nombre d'emprunts totaux (passés et en cours) : %d\n", nbemp);
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure d'afichage des emprunts d'un usager ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void afficher_emprunts_dun_usager()
{
  char id_abo_saisi[TAILLE_ID], message_si_pas_encore_retourne[TAILLE_STANDARD];
  strcpy(message_si_pas_encore_retourne, "NON RENDU");
  int case_abo, au_moins_un_emp=0;
  int i, j;

  if ((nbdoc==0)&&(nbabo==0))
  {
    printf("Il n'y a ni document, ni abonnés renseignés.\n");
  }
  else if (nbdoc==0)
  {
    printf("Il n'y a aucun document renseigné.\n");
  }
  else if (nbabo==0)
  {
    printf("Il n'y a aucun usager renseigné.\n");
  }
  else
  {
    printf("Saisisez l'id de l'abonné associé à l'emprunt   : ");
    fflush(stdin);
    lire_chaine_augmentee(id_abo_saisi);
    conv_maj_accents(id_abo_saisi, 0);

    case_abo=-1;
    while ( (i<nbabo) && (case_abo==-1) )
    {
      if (strcmp(tababo[i].id_abonne, id_abo_saisi)==0) 
	    {
        case_abo = i;
      }
      else
      {
        i++;
      }
    }
    if (case_abo!=-1) // si l'abonné existe, on affiche ses emprunts du plus récent au plus ancien
    {
      if (nbemp==0)
      {
        printf("Il n'y a aucun emprunt renseigné dans le système.\n");
      }
      else
      {
        for (j=nbemp-1 ; j>=0 ; j-- )
        {
          if (strcmp(tabemp[j].emp_id_abo, id_abo_saisi)==0)
          {
            au_moins_un_emp++;
            if (au_moins_un_emp==1) // Si il y a au moins 1 emprunt, avant l'affichage de cet emprunt on affiche les titres des colonnes
            {
              printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
              printf("%-12s %-12s %-51s %-18s %-18s %-18s %-8s\n","ID EMPRUNT","ID DOCUMENT", "TITRE DOCUMENT", "DATE D'EMPRUNT", "DATE LIMITE RETOUR", "DATE DE RETOUR", "RETARD");
              printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            } 
            if (au_moins_un_emp>1)
            {
              if (tabemp[j].date_retour_effective.jour!=-1) //affichage si le doc a été rendu
              {
               printf("%-12s %-12s %-51s %02d/%02d/%-12d %02d/%02d/%-12d %02d/%02d/%-12d %-8s \n",tabemp[j].id, tabemp[j].emp_id_doc, tabemp[j].titre_doc_emprunte, tabemp[j].date_emprunt.jour, tabemp[j].date_emprunt.mois, tabemp[j].date_emprunt.annee, tabemp[j].date_retour_prevue.jour, tabemp[j].date_retour_prevue.mois, tabemp[j].date_retour_prevue.annee, tabemp[j].date_retour_effective.jour, tabemp[j].date_retour_effective.mois, tabemp[j].date_retour_effective.annee, int_a_reponse_question_fermee(tabemp[j].est_en_retard)); 
              }
              else //affichage si le doc n'a pas été rendu
              {
                printf("%-12s %-12s %-51s %02d/%02d/%-12d %02d/%02d/%-12d %-18s %-8s \n",tabemp[j].id, tabemp[j].emp_id_doc,tabemp[j].titre_doc_emprunte, tabemp[j].date_emprunt.jour, tabemp[j].date_emprunt.mois, tabemp[j].date_emprunt.annee, tabemp[j].date_retour_prevue.jour, tabemp[j].date_retour_prevue.mois, tabemp[j].date_retour_prevue.annee, message_si_pas_encore_retourne, int_a_reponse_question_fermee(tabemp[j].est_en_retard)); 
              }
            }
          }
        }
      }
      if (!au_moins_un_emp)
      {
        printf("Cet usager n'a aucun emprunt renseigné dans le système.\n");
      }
      else //on affiche le trait de séparation de fin 
      {
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
      }
    }
    else
    {
      printf("Cet usager n'est pas renseigné dans le système.\n");
    }
  }
    

}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure d'afichage des emprunts d'un document ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void afficher_emprunts_dun_doc()
{
  wip();
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// Proc et fonctions backoffice
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//            ---- procédure de conversion en majuscules ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*void conv_maj(char chaine[])
{
  int taille, i;

  taille = strlen(chaine);
  for (i=0 ; i< taille ; i++)
  {
    chaine[i] = toupper(chaine[i]);
  }
}*/


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


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//              ---  fonction qui vérifie si un abonnement est expiré---
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct abonne abonnement_expire(struct abonne abo)
{

  // la date d'expiration c'est la date de dernière souscription une année plus tard
  abo.date_expiration.jour = abo.date_dernier_souscription.jour; 
  abo.date_expiration.mois = abo.date_dernier_souscription.mois;
  abo.date_expiration.annee = abo.date_dernier_souscription.annee +1; 

  // Et du coup ensuite il faut récupérer la date actuelle 
  time_t tp,nbsec;
      struct tm hjm ;
            nbsec=time(&tp);
            hjm=*localtime(&nbsec);
            abo.date_actuelle.jour=hjm.tm_mday;
            abo.date_actuelle.mois=hjm.tm_mon+1;
            abo.date_actuelle.annee=hjm.tm_year+1900;

  //Ensuite on a juste à comparer si la date actuelle est supérieure à la date d'expriration pour pouvoir dire si l'abonnement doit être renouvelé ou non
  if(abo.date_actuelle.annee > abo.date_expiration.annee)
  {
    //DEBUG
    //printf("%d > %d\n", abo.date_actuelle.annee, abo.date_expiration.annee);

    abo.est_abonne = 0; 
  }
  else if (abo.date_actuelle.annee == abo.date_expiration.annee)
  {
    if(abo.date_actuelle.mois > abo.date_expiration.mois)
    {
      //DEBUG
      //printf("%d > %d \n", abo.date_actuelle.mois, abo.date_expiration.mois);

      abo.est_abonne = 0;
    } 
    else if(abo.date_actuelle.mois == abo.date_expiration.mois)
    {
      if(abo.date_actuelle.jour > abo.date_expiration.jour)
      { 
        //DEBUG
        //printf("%d > %d \n", abo.date_actuelle.jour, abo.date_expiration.jour);

        abo.est_abonne= 0; 
      }  

    } 

  } 
  else //sinon, c'est que l'abonnement est valide
  {
    (abo.est_abonne=1) ;
  }

  if (abo.est_abonne != 0) 
  {
    abo.est_abonne = 1; // Abonnement valide
  }

  return abo; 

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

// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Fonction pour valider les questions fermées (OUI ou NON) et les convertir en int (bool) ----
// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int reponse_question_fermee_en_int(char reponse[TAILLE_STANDARD])
{   
    conv_maj_accents(reponse, 0);
    if ((strcmp(reponse,"OUI")==0)||(strcmp(reponse,"O")==0))
    {
        return 1;
    }
    else if ((strcmp(reponse,"NON")==0)||(strcmp(reponse,"NO")==0)||(strcmp(reponse,"N")==0))
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
char *int_a_reponse_question_fermee(int reponse)
{   
    if (reponse==1)
    {
        return "OUI";
    }
    else if (reponse==0)
    {
        return "NON"; 
    }
    else 
    {
        return "OUT";
    }
}

// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Fonction pour vérifier la validité de l'email ----
// •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int Verif_Email(char* email) 
{
    int arobase = 0; 
    int point = 0; 
	int i;  

    for (i=0;email[i] != '\0';i++) 
	{
        if (email[i]=='@') 
		{
            arobase=1; 
        }
		else if (arobase && email[i]=='.') 
		{
            point = 1; 
        }
    }

   
    if (arobase && point) 
	{
        return 1; 
    } 
	else 
	{
        return 0; 
    }
}

void conv_maj_accents(char ChaineOctets[], int conserve_accents_OK)
{
/* déclarations */
int i ;
int TailleChaineOctets,TailleChaineWide ;
int nbcarUnelettreWide, NbCaracteresConvertis ;
int TailleUnCaractereWide ;
char *UnelettreOctet ;
char ChaineOctetsMajuscules[TAILLE_CH] = {0} ;
/* variables de caractères wide : 1 caractère est sur 4 octets */
wchar_t ChaineWide[TAILLE_CH] ;
wchar_t UnelettreWide[2] ; /* chaine d'un caractère wide */
/* initialisation */
ChaineWide[0]='\0';
TailleUnCaractereWide = sizeof(wchar_t) ; // calcul de la taille dutype wchar_t)
/* création de la chaine de caractères multi-bytes : 1 caractères
ASCII sur 1 octet
Les caractères accentués UTF8 sur 2 à 4 octets */
UnelettreOctet = (char *) malloc (TailleUnCaractereWide * TAILLE_CH);
/* conversion de ChaineOctets (multi-byte) en chaine de caractères
élargis (4 octets par caractères) */
NbCaracteresConvertis = mbstowcs(ChaineWide,ChaineOctets, TAILLE_CH);
TailleChaineOctets=strlen(ChaineOctets);
TailleChaineWide = wcslen(ChaineWide);
UnelettreWide[1] = '\0';
/* boucle de traitement de chaque caractère élargi (type wide_t ) */
for (i=0;i<TailleChaineWide;i++)
{
UnelettreWide[0] = towupper(ChaineWide[i]);
nbcarUnelettreWide =
wcstombs(UnelettreOctet,UnelettreWide,TailleUnCaractereWide);
if (! conserve_accents_OK)
{
supprime_accents(UnelettreOctet) ;
}
strcat(ChaineOctetsMajuscules,UnelettreOctet);
}
strcpy(ChaineOctets,ChaineOctetsMajuscules);
}

/* ------------------------------- */
/* --- suppression des accents --- */
/* ------------------------------- */
void supprime_accents(char ChaineLettre[])
{
/* ------------------------ */
/* ---- les lettres A ----- */
/* ------------------------ */
/* traitement des accents du A */
if ( (strcmp(ChaineLettre,"À")==0) || (strcmp(ChaineLettre,"Á")==0)
|| (strcmp(ChaineLettre,"Â")==0) || (strcmp(ChaineLettre,"Ã")==0) || (strcmp(ChaineLettre,"Ä")==0) || (strcmp(ChaineLettre,"Å")==0) )
{
strcpy(ChaineLettre,"A");
}
/* traitement des accents du a */
else if ( (strcmp(ChaineLettre,"à")==0) ||
(strcmp(ChaineLettre,"á")==0) || (strcmp(ChaineLettre,"â")==0) ||
(strcmp(ChaineLettre,"ã")==0) || (strcmp(ChaineLettre,"ä")==0) ||
(strcmp(ChaineLettre,"å")==0) )
{
strcpy(ChaineLettre,"a");
}
/* traitement des autres accents du A */
else if ( (strcmp(ChaineLettre,"Ā")==0) ||
(strcmp(ChaineLettre,"Ă")==0) || (strcmp(ChaineLettre,"Ą")==0) ||
(strcmp(ChaineLettre,"Ǎ")==0) || (strcmp(ChaineLettre,"Ǻ")==0))
{
strcpy(ChaineLettre,"A");
}
/* traitement des autres accents du a */
else if ( (strcmp(ChaineLettre,"ā")==0) ||
(strcmp(ChaineLettre,"ă")==0) || (strcmp(ChaineLettre,"ą")==0) ||
(strcmp(ChaineLettre,"ǎ")==0) || (strcmp(ChaineLettre,"ǻ")==0))
{
strcpy(ChaineLettre,"a");
}
/* traitement des accents du AE */
else if ( (strcmp(ChaineLettre,"Æ")==0) ||
(strcmp(ChaineLettre,"Ǽ")==0) )
{
strcpy(ChaineLettre,"AE");
}
/* traitement des accents du ae */
else if ( (strcmp(ChaineLettre,"æ")==0) ||
(strcmp(ChaineLettre,"ǽ")==0) )
{
strcpy(ChaineLettre,"ae");
}
/* ------------------------ */
/* ---- les lettres E ----- */
/* ------------------------ */
/* traitement des accents du E */
else if ( (strcmp(ChaineLettre,"È")==0) ||
(strcmp(ChaineLettre,"É")==0) || (strcmp(ChaineLettre,"Ê")==0) ||
(strcmp(ChaineLettre,"Ë")==0) )
{
strcpy(ChaineLettre,"E");
}
else if ( (strcmp(ChaineLettre,"è")==0) ||
(strcmp(ChaineLettre,"é")==0) || (strcmp(ChaineLettre,"ê")==0) ||
(strcmp(ChaineLettre,"ë")==0) )
{
strcpy(ChaineLettre,"e");
}
/* traitement des autres accents du E */
else if ( (strcmp(ChaineLettre,"Ē")==0) ||
(strcmp(ChaineLettre,"Ẽ")==0) || (strcmp(ChaineLettre,"Ė")==0) ||
(strcmp(ChaineLettre,"Ę")==0) || (strcmp(ChaineLettre,"Ě")==0) ||
(strcmp(ChaineLettre,"Ĕ")==0) )
{
strcpy(ChaineLettre,"E");
}
/* traitement des autres accents du e */
else if ( (strcmp(ChaineLettre,"ē")==0) ||
(strcmp(ChaineLettre,"ẽ")==0) || (strcmp(ChaineLettre,"ė")==0) ||
(strcmp(ChaineLettre,"ę")==0) || (strcmp(ChaineLettre,"ě")==0) ||
(strcmp(ChaineLettre,"ĕ")==0) )
{
strcpy(ChaineLettre,"e");
}
/* ------------------------ */
/* ---- les lettres I ----- */
/* ------------------------ */
/* traitement des accents du I */
else if ( (strcmp(ChaineLettre,"Ì")==0) ||
(strcmp(ChaineLettre,"Í")==0) || (strcmp(ChaineLettre,"Î")==0) ||
(strcmp(ChaineLettre,"Ï")==0) )
{
strcpy(ChaineLettre,"I");
}
/* traitement des accents du i */
else if ( (strcmp(ChaineLettre,"ì")==0) ||
(strcmp(ChaineLettre,"í")==0) || (strcmp(ChaineLettre,"î")==0) ||
(strcmp(ChaineLettre,"ï")==0) )
{
strcpy(ChaineLettre,"i");
}
/* traitement des autres accents du I */
else if ( (strcmp(ChaineLettre,"Ĩ")==0) ||
(strcmp(ChaineLettre,"Ī")==0) || (strcmp(ChaineLettre,"Ĭ")==0) ||
(strcmp(ChaineLettre,"Į")==0) || (strcmp(ChaineLettre,"İ")==0) ||
(strcmp(ChaineLettre,"Ǐ")==0) )
{
strcpy(ChaineLettre,"I");
}
/* traitement des autres accents du i */
else if ( (strcmp(ChaineLettre,"ĩ")==0) ||
(strcmp(ChaineLettre,"ī")==0) || (strcmp(ChaineLettre,"ĭ")==0) ||
(strcmp(ChaineLettre,"į")==0) || (strcmp(ChaineLettre,"ị")==0) ||
(strcmp(ChaineLettre,"ǐ")==0) || (strcmp(ChaineLettre,"ı")==0) )
{
strcpy(ChaineLettre,"i");
}
/* ------------------------ */
/* ---- les lettres O ----- */
/* ------------------------ */
/* traitement des accents du O */
else if ( (strcmp(ChaineLettre,"Ò")==0) ||
(strcmp(ChaineLettre,"Ó")==0) || (strcmp(ChaineLettre,"Ô")==0) ||
(strcmp(ChaineLettre,"Ö")==0) || (strcmp(ChaineLettre,"Õ")==0) ||
(strcmp(ChaineLettre,"Ø")==0) || (strcmp(ChaineLettre,"Ǿ")==0) ||
(strcmp(ChaineLettre,"Ǒ")==0) )
{
strcpy(ChaineLettre,"O");
}
/* traitement des accents du o */
else if ( (strcmp(ChaineLettre,"ò")==0) ||
(strcmp(ChaineLettre,"ó")==0) || (strcmp(ChaineLettre,"ô")==0) ||
(strcmp(ChaineLettre,"ö")==0) || (strcmp(ChaineLettre,"õ")==0) ||
(strcmp(ChaineLettre,"ø")==0) || (strcmp(ChaineLettre,"ǿ")==0) ||
(strcmp(ChaineLettre,"ǒ")==0) )
{
strcpy(ChaineLettre,"o");
}
/* traitement des autres accents du O */
else if ( (strcmp(ChaineLettre,"Ō")==0) ||
(strcmp(ChaineLettre,"Ŏ")==0) || (strcmp(ChaineLettre,"Ő")==0) ||
(strcmp(ChaineLettre,"Ơ")==0) )
{
strcpy(ChaineLettre,"O");
}
/* traitement des autres accents du o */
else if ( (strcmp(ChaineLettre,"ō")==0) ||
(strcmp(ChaineLettre,"ŏ")==0) || (strcmp(ChaineLettre,"ő")==0) ||
(strcmp(ChaineLettre,"ơ")==0) )
{
strcpy(ChaineLettre,"o");
}
/* traitement des accents du OE */
else if ( (strcmp(ChaineLettre,"Œ")==0) )
{
strcpy(ChaineLettre,"OE");
}
/* traitement des accents du oe */
else if ( (strcmp(ChaineLettre,"œ")==0) )
{
strcpy(ChaineLettre,"oe");
}
/* ------------------------ */
/* ---- les lettres U ----- */
/* ------------------------ */
/* traitement des accents du U */
else if ( (strcmp(ChaineLettre,"Ù")==0) ||
(strcmp(ChaineLettre,"Ú")==0) || (strcmp(ChaineLettre,"Û")==0) ||
(strcmp(ChaineLettre,"Ü")==0) )
{
strcpy(ChaineLettre,"U");
}
/* traitement des accents du U */
else if ( (strcmp(ChaineLettre,"ù")==0) ||
(strcmp(ChaineLettre,"ú")==0) || (strcmp(ChaineLettre,"û")==0) ||
(strcmp(ChaineLettre,"ü")==0) )
{
strcpy(ChaineLettre,"u");
}
/* traitement des autres accents du U */
else if ( (strcmp(ChaineLettre,"Ũ")==0) ||
(strcmp(ChaineLettre,"Ū")==0) || (strcmp(ChaineLettre,"Ŭ")==0) ||
(strcmp(ChaineLettre,"Ů")==0) || (strcmp(ChaineLettre,"Ű")==0) ||
(strcmp(ChaineLettre,"Ų")==0) || (strcmp(ChaineLettre,"Ư")==0) )
{
strcpy(ChaineLettre,"U");
}
/* traitement des autres accents du U */
else if ( (strcmp(ChaineLettre,"Ǔ")==0) ||
(strcmp(ChaineLettre,"Ǖ")==0) || (strcmp(ChaineLettre,"Ǘ")==0) ||
(strcmp(ChaineLettre,"Ǚ")==0) || (strcmp(ChaineLettre,"Ǜ")==0) )
{
strcpy(ChaineLettre,"U");
}
/* traitement des autres accents du u */
else if ( (strcmp(ChaineLettre,"ũ")==0) ||
(strcmp(ChaineLettre,"ū")==0) || (strcmp(ChaineLettre,"ŭ")==0) ||
(strcmp(ChaineLettre,"ů")==0) || (strcmp(ChaineLettre,"ű")==0) ||
(strcmp(ChaineLettre,"ų")==0) || (strcmp(ChaineLettre,"ư")==0))
{
strcpy(ChaineLettre,"u");
}
/* traitement des autres accents du u */
else if ( (strcmp(ChaineLettre,"ǔ")==0) ||
(strcmp(ChaineLettre,"ǖ")==0) || (strcmp(ChaineLettre,"ǘ")==0) ||
(strcmp(ChaineLettre,"ǚ")==0) || (strcmp(ChaineLettre,"ǜ")==0) )
{
strcpy(ChaineLettre,"u");
}
/* ------------------------ */
/* ---- les lettres Y ----- */
/* ------------------------ */
/* traitement des accents du Y */
else if ( (strcmp(ChaineLettre,"Ý")==0) ||
(strcmp(ChaineLettre,"Ÿ")==0) )
{
strcpy(ChaineLettre,"Y");
}
/* traitement des accents du y */
else if ( (strcmp(ChaineLettre,"ý")==0) ||
(strcmp(ChaineLettre,"ÿ")==0) )
{
strcpy(ChaineLettre,"y");
}
/* ------------------------ */
/* ---- les lettres C ----- */
/* ------------------------ */
/* traitement des accents du Ç */
else if ( (strcmp(ChaineLettre,"Ç")==0) ||
(strcmp(ChaineLettre,"Ć")==0) || (strcmp(ChaineLettre,"Ĉ")==0) ||
(strcmp(ChaineLettre,"Ċ")==0) || (strcmp(ChaineLettre,"Č")==0) )
{
strcpy(ChaineLettre,"C");
}
/* traitement des accents du c */
else if ( (strcmp(ChaineLettre,"ç")==0) ||
(strcmp(ChaineLettre,"ć")==0) || (strcmp(ChaineLettre,"ĉ")==0) ||
(strcmp(ChaineLettre,"ċ")==0) || (strcmp(ChaineLettre,"č")==0) )
{
strcpy(ChaineLettre,"c");
}
/* ------------------------ */
/* ---- les lettres D ----- */
/* ------------------------ */
/* traitement des accents du Ð */
else if ( (strcmp(ChaineLettre,"Ð")==0) ||
(strcmp(ChaineLettre,"Ď")==0) )
{
strcpy(ChaineLettre,"D");
}
else if ( (strcmp(ChaineLettre,"đ")==0) ||
(strcmp(ChaineLettre,"ď")==0) )
{
strcpy(ChaineLettre,"d");
}
/* ------------------------ */
/* ---- les lettres F ----- */
/* ------------------------ */
/* traitement des accents du ƒ */
else if ( (strcmp(ChaineLettre,"ƒ")==0) )
{
strcpy(ChaineLettre,"f");
}
/* ------------------------ */
/* ---- les lettres N ----- */
/* ------------------------ */
/* traitement des accents du N */
else if ( (strcmp(ChaineLettre,"Ñ")==0) ||
(strcmp(ChaineLettre,"Ń")==0) || (strcmp(ChaineLettre,"Ņ")==0) ||
(strcmp(ChaineLettre,"Ň")==0) )
{
strcpy(ChaineLettre,"N");
}
/* traitement des accents du n */
else if ( (strcmp(ChaineLettre,"ñ")==0) ||
(strcmp(ChaineLettre,"ń")==0) || (strcmp(ChaineLettre,"ņ")==0) ||
(strcmp(ChaineLettre,"ň")==0) || (strcmp(ChaineLettre,"ŉ")==0) )
{
strcpy(ChaineLettre,"n");
}
/* ------------------------ */
/* ---- les lettres G ----- */
/* ------------------------ */
/* traitement des accents du G */
else if ( (strcmp(ChaineLettre,"Ĝ")==0) ||
(strcmp(ChaineLettre,"Ğ")==0) || (strcmp(ChaineLettre,"Ġ")==0) ||
(strcmp(ChaineLettre,"Ģ")==0) )
{
strcpy(ChaineLettre,"G");
}
/* traitement des accents du g */
else if ( (strcmp(ChaineLettre,"ĝ")==0) ||
(strcmp(ChaineLettre,"ğ")==0) || (strcmp(ChaineLettre,"ġ")==0) ||
(strcmp(ChaineLettre,"ģ")==0) )
{
strcpy(ChaineLettre,"g");
}
/* ------------------------ */
/* ---- les lettres H ----- */
/* ------------------------ */
/* traitement des accents du H */
else if ( (strcmp(ChaineLettre,"Ĥ")==0) ||
(strcmp(ChaineLettre,"Ħ")==0) )
{
strcpy(ChaineLettre,"H");
}
/* traitement des accents du h */
else if ( (strcmp(ChaineLettre,"ĥ")==0) ||
(strcmp(ChaineLettre,"ħ")==0) )
{
strcpy(ChaineLettre,"h");
}
/* ------------------------ */
/* ---- les lettres IJ ----- */
/* ------------------------ */
/* traitement des accents du IJ */
else if ( (strcmp(ChaineLettre,"Ĳ")==0) )
{
strcpy(ChaineLettre,"IJ");
}
/* traitement des accents du ij */
else if ( (strcmp(ChaineLettre,"ĳ")==0) )
{
strcpy(ChaineLettre,"ij");
}
/* ------------------------ */
/* ---- les lettres J ----- */
/* ------------------------ */
/* traitement des accents du J */
else if ( (strcmp(ChaineLettre,"Ĵ")==0) )
{
strcpy(ChaineLettre,"J");
}
/* traitement des accents du j */
else if ( (strcmp(ChaineLettre,"ĵ")==0) )
{
strcpy(ChaineLettre,"j");
}
/* ------------------------ */
/* ---- les lettres K ----- */
/* ------------------------ */
/* traitement des accents du K */
else if ( (strcmp(ChaineLettre,"Ķ")==0) )
{
strcpy(ChaineLettre,"K");
}
/* traitement des accents du k */
else if ( (strcmp(ChaineLettre,"ķ")==0) )
{
strcpy(ChaineLettre,"k");
}
/* ------------------------ */
/* ---- les lettres L ----- */
/* ------------------------ */
/* traitement des accents du L */
else if ( (strcmp(ChaineLettre,"Ĺ")==0) ||
(strcmp(ChaineLettre,"Ļ")==0) || (strcmp(ChaineLettre,"Ľ")==0) ||
(strcmp(ChaineLettre,"Ŀ")==0)|| (strcmp(ChaineLettre,"Ł")==0) )
{
strcpy(ChaineLettre,"L");
}
/* traitement des accents du l */
else if ( (strcmp(ChaineLettre,"ĺ")==0) ||
(strcmp(ChaineLettre,"ļ")==0) || (strcmp(ChaineLettre,"ľ")==0) ||
(strcmp(ChaineLettre,"ŀ")==0)|| (strcmp(ChaineLettre,"ł")==0) )
{
strcpy(ChaineLettre,"l");
}
/* ------------------------ */
/* ---- les lettres R ----- */
/* ------------------------ */
/* traitement des accents du R */
else if ( (strcmp(ChaineLettre,"Ŕ")==0) ||
(strcmp(ChaineLettre,"Ŗ")==0) || (strcmp(ChaineLettre,"Ř")==0) )
{
strcpy(ChaineLettre,"R");
}
/* traitement des accents du r */
else if ( (strcmp(ChaineLettre,"ŕ")==0) ||
(strcmp(ChaineLettre,"ŗ")==0) || (strcmp(ChaineLettre,"ř")==0) )
{
strcpy(ChaineLettre,"r");
}
/* ------------------------ */
/* ---- les lettres S ----- */
/* ------------------------ */
/* traitement des accents du S */

else if ( (strcmp(ChaineLettre,"Ś")==0) ||
(strcmp(ChaineLettre,"Ŝ")==0) || (strcmp(ChaineLettre,"Ş")==0) ||
(strcmp(ChaineLettre,"Š")==0))
{
strcpy(ChaineLettre,"S");
}
/* traitement des accents du s */
else if ( (strcmp(ChaineLettre,"ś")==0) ||
(strcmp(ChaineLettre,"ŝ")==0) || (strcmp(ChaineLettre,"ş")==0) ||
(strcmp(ChaineLettre,"š")==0))
{
strcpy(ChaineLettre,"s");
}
/* traitement des accents du ß */
else if ( (strcmp(ChaineLettre,"ß")==0) )
{
strcpy(ChaineLettre,"S");
}
/* traitement des accents du ſ */
else if ( (strcmp(ChaineLettre,"ſ")==0) )
{
strcpy(ChaineLettre,"s");
}
/* ------------------------ */
/* ---- les lettres T ----- */
/* ------------------------ */
/* traitement des accents du T */
else if ( (strcmp(ChaineLettre,"Ţ")==0) ||
(strcmp(ChaineLettre,"Ť")==0) || (strcmp(ChaineLettre,"Ŧ")==0) )
{
strcpy(ChaineLettre,"T");
}
/* traitement des accents du t */
else if ( (strcmp(ChaineLettre,"ţ")==0) ||
(strcmp(ChaineLettre,"ť")==0) || (strcmp(ChaineLettre,"ŧ")==0) )
{
strcpy(ChaineLettre,"t");
}
/* ------------------------ */
/* ---- les lettres W ----- */
/* ------------------------ */
/* traitement des accents du W */
else if ( (strcmp(ChaineLettre,"Ŵ")==0) )
{
strcpy(ChaineLettre,"W");
}
/* traitement des accents du w */
else if ( (strcmp(ChaineLettre,"ŵ")==0) )
{
strcpy(ChaineLettre,"w");
}
/* ------------------------ */
/* ---- les lettres Y ----- */
/* ------------------------ */
/* traitement des accents du Y */

else if ( (strcmp(ChaineLettre,"Ŷ")==0) ||
(strcmp(ChaineLettre,"Ÿ")==0) )
{
strcpy(ChaineLettre,"Y");
}
/* traitement des accents du y */
else if ( (strcmp(ChaineLettre,"ŷ")==0) ||
(strcmp(ChaineLettre,"ÿ")==0) )
{
strcpy(ChaineLettre,"y");
}
/* ------------------------ */
/* ---- les lettres Z ----- */
/* ------------------------ */
/* traitement des accents du Z */
else if ( (strcmp(ChaineLettre,"Ź")==0) ||
(strcmp(ChaineLettre,"Ż")==0) || (strcmp(ChaineLettre,"Ž")==0) )
{
strcpy(ChaineLettre,"Z");
}
/* traitement des accents du z */
else if ( (strcmp(ChaineLettre,"ź")==0) ||
(strcmp(ChaineLettre,"ż")==0) || (strcmp(ChaineLettre,"ž")==0) )
{
strcpy(ChaineLettre,"z");
}
}

// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Procédure de lecture de chaine qui centralise les différentes procs----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void lire_chaine_augmentee(char *string)
{
  int valide=0;
  while(!valide)
  {
    fflush(stdin);
    lire_chaine_espace(string);
    gerer_espaces_multiples(string);
    gerer_espace_avant_apres(string);
    fflush(stdin);

    if (strcmp(string, "")==0) //On vérifie que l'utilisateur n'a pas juste appuyé sur la touche entrée
    {
      printf("Erreur, merci de fournir un saisie valide : ");
    }
    else
    {
      valide=1;
    }

  }
}

/* procédure outil de retrait des espaces avant et après une chaine */
void conv_souligne_en_espace(char *ch)
{
int i=0, taille ;
if (strcmp(ch,"") != 0) /* On regarde si la chaine n'est pas vide */
{
taille = strlen(ch);
for (i = 0 ; i<taille ; i++)
{
if (ch[i] == '_') ch[i] = ' ';
}
}
}

/* procédure outil de retrait des espaces avant et après une chaine */
void conv_espace_en_souligne(char *ch)
{
int i=0, taille ;
if (strcmp(ch,"") != 0) /* On regarde si la chaine n'est pas vide */
{
taille = strlen(ch);
for (i = 0 ; i<taille ; i++)
{
if (ch[i] == ' ') ch[i] = '_';
}
}
}


// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//     ---- Procédure WIP pour les fonctions en cours de dev ----
// ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void wip()
{
    printf("Fonction en cours de développement :)\n");
}
