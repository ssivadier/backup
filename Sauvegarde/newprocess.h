/*
  Name: newprocess.h
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 26/05/08 15:12
  Description: Definition des variables utiles au createprocess
*/

#ifndef DEF_PROCESS
#define DEF_PROCESS

/*
        Les includes
*/
#ifdef WIN32
	#include <windows.h>
#endif


/*
      Les fonctions
*/
char *run_command(HWND, LPTSTR);


/*
     Les definitions
*/
#define OUTPUTBUFSIZE 65535             // taille du buffer

extern char Errsauv[512];
extern char fichlog[100000];

#endif
