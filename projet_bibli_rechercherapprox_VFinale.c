/*--- Librairies C ---*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*--- Constantes---*/ 
#define TAILLE_COTE 10
#define TAILLE_TITRE 100
#define TAILLE_AUTEUR 50
#define MAX_DOC 100
#define TAILLE_ELEMENTRECH 200
#define NON_TROUVE 0
#define TROUVE 1

/*---déclaration du type global---*/
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

/*---Déclaration des variables globales---*/
struct document tabdoc[MAX_DOC] ;
int nbdoc=0;
char ElementRech[TAILLE_ELEMENTRECH];

/*--- déclarations préliminaires ---*/
/*--- Proc et fonctions frontoffice ---*/
void menu_staff();
void saisir_new();
void affichage();
void RechercheApproximative (char *ElementRech, struct document *doc);
void verif_sauvegarde();
void sauvegarde();
void quitter();

/*--- Proc et fonctions backoffice ---*/
void wip();
void conv_maj(char chaine[]);
void lire_chaine_espace(char chaine[]); 
void gerer_espaces_multiples(char *chaine);
void gerer_espace_avant_apres(char *ch);

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
    printf("|   -1- Rechercher                     |\n");
    printf("|   -2- Saisir un nouveau document    |\n");
    printf("|   -3- Afficher les documents        |\n");
    printf("|   -4- Rechercher un document        |\n"); 
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
      case 4 : RechercheApproximative (ElementRech, tabdoc);
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

// On vide le buffer avant les prochaines saisies
	  fflush(stdin)                          ; 

      printf("Titre                     : ");
      lire_chaine_espace(doc.titre_doc)     ;
      

      printf("Auteur                     : ");
      lire_chaine_espace(doc.auteur_doc)     ;

      printf("Prix                       : ");
      scanf("%f",&doc.prix_doc)              ;

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
      //Utilisation de la procédure gérer les espaces avant l'affichage des chaines pour empêcher l'affichage d'espace mutltiples
      gerer_espaces_multiples(doc.titre_doc); 
      gerer_espaces_multiples(doc.auteur_doc);
      //Utilisation de la procédure pour gérer les espaces avant et après la chaine
      gerer_espace_avant_apres(doc.titre_doc);
      gerer_espace_avant_apres(doc.auteur_doc); 
      printf("%-15d %-15s |%-15s| |%-15s| %-15f %-15d\n",doc.id_doc, doc.cote_doc, doc.titre_doc, doc.auteur_doc, doc.prix_doc, doc.eligible_a_lemprunt_doc);
    }  
    printf("----------------------------------------------\n");
  }
}

/*--- Procédure de recherche approximative ---*/
void RechercheApproximative(char *ElementRech, struct document *doc)
{
	/*Déclaration des variables locales*/
    int correspondance = NON_TROUVE;
    int i;
    char titre_maj[TAILLE_TITRE], auteur_maj[TAILLE_AUTEUR];

    /*---Obligée de vider le buffer, sinon il ne me laisse pas entrer une recherche---*/
    fflush(stdin);
    
    /*---On entre un élément à rechercher, que ce soit un titre ou un auteur (possibilité d'élargir si besoin)---*/
    printf("Entrez un élément à rechercher (titre ou auteur) : ");
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
        
        gerer_espaces_multiples(titre_maj);
        gerer_espace_avant_apres(titre_maj);
        conv_maj(titre_maj);

        gerer_espaces_multiples(auteur_maj);
        gerer_espace_avant_apres(auteur_maj);
        conv_maj(auteur_maj);
        
        /*---La fonction strstr me permet de vérifier s'il existe une correspondance entre la chaine auteur_maj et la sous_chaine ElementRech (notre saisie d'un élément
        à rechercher). Si c'est différent de null ça veut dire qu'il y a une correspondance entre les 2 ---*/
          if ((strstr(titre_maj, ElementRech) != NULL ) || (strstr(auteur_maj, ElementRech) != NULL))
          {
            correspondance = TROUVE;
            printf("Document trouvé : [ID : %d], Titre : %s, Auteur : %s\n",doc[i].id_doc, doc[i].titre_doc, doc[i].auteur_doc);
          }
      }
	}

    if (correspondance == NON_TROUVE)
    {
        printf("Aucun document trouvé avec l'élément saisi '%s'\n", ElementRech);
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

// --- Procédure de gestion des espaces (prise dans le poly et adaptée au projet) ---
void lire_chaine_espace(char chaine[])
{
  //Déclaration des variables locales 
  int i; 
  char lettre; 
  
  i=0; 
  while ((lettre=getchar())!='\n' && i<MAX_DOC-1)
  {
  	chaine[i++] = lettre; 
  }
  chaine[i]='\0';
  if (lettre != '\n') 
  {
    while (getchar() != '\n');
  }
}

//---Procédure de gestion des espaces multiples (prise dans le poly et adaptée au projet) ---
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
					espace_rencontre=1;
				}
			}
		}
		chaine1[j]='\0'; 
		strcpy(chaine,chaine1);
	}
}

//--- Procédure de gestion des espaces avant et après une chaine de caractère (prise dans le poly) ---
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

//---Procédure WIP pour les fonctions en cours de dev----------------
void wip()
{
    printf("Fonction en cours de développement :)\n");
}