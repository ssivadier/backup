/*
  Name: fxgenerales.h
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 17/04/08 14:09
  Description: Definition des constantes et variables necessaires au fonctions generales du prog
*/

#ifndef DEF_FXGENE
#define DEF_FXGENE

/*
        Les includes
*/
#ifdef WIN32
	#include <windows.h>
#endif

#include <time.h>    // pour l'heure de sauvegarde
#include <stdio.h>   // gestion des flux (fichiers)
#include "ressource.h"


/*
         Les fonctions
*/
char** split( char *, char);
DWORD strsub(const char *, DWORD, DWORD, char*);
void remplace(char *, char, char);
int GetOSVersion();
void Hdossier(HWND, char []);
void sauvegarde(HWND);
void chargeconf(HWND);
void reconf(HWND, char *);
void gestsauv(HWND);


/*
         Les definitions
*/
#define BUFSIZE     8192         // taille du buffer

#define OS_ERROR                            -1
#define OS_WIN32_WINDOWS_NT_3_51	        0
#define OS_WIN32_WINDOWS_NT_4_0 	        1
#define OS_WIN32_WINDOWS_95 		        2
#define OS_WIN32_WINDOWS_98			        3
#define OS_WIN32_WINDOWS_Me		            4
#define OS_WIN32_WINDOWS_2000		        5
#define OS_WIN32_WINDOWS_XP			        6
#define OS_WIN32_WINDOWS_SERVER_2003_FAMILY	7
#define OS_WIN32_WINDOWS_VISTA	            8


SHFILEOPSTRUCT FileOpStr;  // déclaration de la structure pour la gestion du bouton d'ouverture de dossier

LPTSTR lpszSystemInfo; // pointeur vers les infos systeme
DWORD cchBuff = BUFSIZE; // Nom d'ordi ou d'utilisateur
TCHAR tchBuffer[BUFSIZE]; // buffer de la taille du nom de l'ordi

FILE *fichier;         // pointeur sur un fichier

time_t now;                // structure date
char *date;                // on récupère la date

char tmp[500] = "";
char tmp2[500] = "";    // variable temporaire
char buff[500] = "";    //    ................................
char dossier[500] = "";   // variable temporaire contenant un dossier
char fichierlog[100000] = "";  // Contenu du fichier sauv.conf
char buffer[2000] = "";   // buffer temporaire
char DestDir[MAX_PATH], AddDir[MAX_PATH]; // repertoire de sauvegarde  
char** tab;

extern LPWSTR* pArgv;     // Pointeur sur arguments (argv)
char *argv1;        // Pointeur vers le premier parametre passé

#endif 
