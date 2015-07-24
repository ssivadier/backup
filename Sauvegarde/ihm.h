/*
  Name: ihm.h
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 12/06/08
  Description: variables necessaires a l'interface Homme-Machine
*/

#ifndef DEF_IHM
#define DEF_IHM

/*
        Les includes
*/
#ifdef WIN32
	#include <windows.h>
#endif

#include <shlobj.h>  // Pour l'API SHBrowseForFolder
#include <stdio.h>   // gestion des flux (fichiers)
#include "ressource.h"


/*
         Les fonctions
*/
LRESULT CALLBACK PropoProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);


/*
        Les definitions
*/
#define WM_TRAY (WM_USER + 1)    // declare une constante pour le systray


TC_ITEM tie;  

HGLOBAL hgMemory;
LPDLGTEMPLATE dltDlgHelp;
LPWORD lpWord;
LPWSTR lpwsWindowNameUnicode;
POINT pt;   

BROWSEINFO browser; // structure de shbrowsefolder
LPITEMIDLIST lip;   // Handle vers shbrowsefolder

NOTIFYICONDATA tnd;                // structure pour le systray
BOOL AffichageMenu = FALSE;        // gestion du menu du systray

short desactive = 0;             // flag permettant de savoir si la sauvegarde est desactivee ou pas.                                  


/*
  Declarations externes
*/
extern HINSTANCE hinst;
FILE *fichier;     
extern LPWSTR* pArgv;     // Pointeur sur arguments (argv)
extern char *argv1;        // Pointeur vers le premier parametre passé
char DestDir[MAX_PATH], AddDir[MAX_PATH] = ""; // repertoire de sauvegarde  
char ch_buff1[512] = "";
char ch_buff2[512] = "";
char ch_buff3[2000] = "";

#endif
   
