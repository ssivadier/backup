/*
  Name: Sauvegarde.exe
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 17/04/08 14:09
  Description: Petit logiciel de sauvegarde interne � l'acad�mie de Bordeaux.
*/

#include <windows.h> // pour la gestion des fenetres et API


/*
  Include des fichiers perso
*/
#include "ressource.h"    //   declaration des commandes

         
LPWSTR* pArgv = NULL;     // Pointeur sur arguments (argv)
int numOfArgs = 0;        // Nombre d'argument (argc)

char Errsauv[512] = "";   // buffer tampon en cas d'erreur lors de la compression de donn�es
char fichlog[100000] = "";   // buffer du fichier de log

/*
     Les definitions
*/
HINSTANCE hinst;                      

/*  On d�clare une procedure windows  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  On donne le nom de l'application  */
char szClassName[ ] = "Sauvegarde";


/*       --------------------------------------------------
         fonction WINMAIN
         --------------------------------------------------
*/   
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
    
    HACCEL hAccel; //les racourcis clavier
    HWND hwnd;               // la handle de la fenetre
    MSG messages;            // Pour la gestion des messages
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    hinst = hThisInstance;   // on r�cup�re l'instace de la fenetre pour pouvoir y ajouter des composants apr�s
    
    pArgv = CommandLineToArgvW(GetCommandLineW(), &numOfArgs);     //R�cup�re les arguments
       
       
    /* Structure de la fenetre */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;        // nom de la fentre (donn� plus haut)
    wincl.lpfnWndProc = WindowProcedure;      // Procedure de windows
    wincl.style = CS_DBLCLKS;                 // pour r�cuperer les doubles clicks
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_APPICON));    // pointe sur l'icone de ressource.h
    wincl.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_APPICON));
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);                          // curseur de base
    wincl.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);                       // pointe sur le menu
    wincl.cbClsExtra = 0;                      // Pas d'octet suppl�mentaire 
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 4);   // couleur du fond de la fenetre

    /* On enregistre la classe (si ca rate, on quitte) */
    if (!RegisterClassEx (&wincl))
        return 0;
    
    hAccel = LoadAccelerators(hThisInstance, MAKEINTRESOURCE(IDR_ACCEL)); // Charge les raccourcis clavier
    
    // on cr�� la fenetre
    hwnd = CreateWindowEx (
           0,                   
           szClassName,         // Nom de la fenetre
           "Sauvegarde",       // titre
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // type de fentre (basique ici)
           CW_USEDEFAULT,       // on laisse windows d�cider de la position horizontale
           CW_USEDEFAULT,       // on laisse windows d�cider de la position verticale
           570,                 // largeur de la fenetre
           332,                 // hauteur de la fenetre
           HWND_DESKTOP,        // la fenetre d�pend du bureau windows
           NULL,                
           hThisInstance,       // l'instance de la fonction winmain
           NULL                 
           );


    // v�rifie les messages (clavier + souris)
    while (GetMessage (&messages, NULL, 0, 0)) {
          if(!TranslateAccelerator(hwnd, hAccel, &messages)) {
                 // traduit les raccourcis clavier
                 TranslateMessage(&messages);
                 // r�parti les message des p�riph�riques
                 DispatchMessage(&messages);
           }
    }
    
    // on d�truit tout
	DestroyAcceleratorTable(hAccel);
	DestroyIcon(wincl.hIcon);
	DestroyCursor(wincl.hCursor);
	UnregisterClass(wincl.lpszClassName, hThisInstance);

    // termine l'application (vaux 0 si tout est ok)
    return messages.wParam;
}
