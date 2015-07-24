/*
  Name: newprocess.c
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 26/05/08 15:12
  Description: Contient la possibilité de demarrer une appli externe et d'avoir le retour de la sortie standard
*/

#include "newprocess.h"


char *run_command(HWND hwnd, LPTSTR cmd) {
    
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    SECURITY_ATTRIBUTES sattr;
    HANDLE readfh;
    register char *cbuff;
    
    /*
     Alloue le buffer conetnant la sortie standard
    */
    if (!(cbuff = (char *)GlobalAlloc(GMEM_FIXED, OUTPUTBUFSIZE))){
        strcpy(Errsauv,"MsgBox\"Pb lors de la sauvegarde : Manque de mémoire vive. Consultez le fichier de log dans votre répertoire de sauvegarde\",vbCritical,\"Erreur de sauvegarde\"");
        MessageBox(hwnd,Errsauv,"Erreur",MB_ICONERROR);
        return 0;
    }
    
    /*
     Initialise la structure STARTUPINFO
    */
    ZeroMemory(&sinfo, sizeof(STARTUPINFO));
    sinfo.cb = sizeof(STARTUPINFO);
    sinfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE;   
    sinfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    sinfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    sinfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    sattr.nLength = sizeof(SECURITY_ATTRIBUTES);     // les attributs necessaire pour la lecture/ecriture des sorties
    sattr.lpSecurityDescriptor = 0;
    sattr.bInheritHandle = TRUE;
    /*
      Créé un pipe contenant la sortie standard du process
    */
    if (!CreatePipe(&readfh, &sinfo.hStdOutput, &sattr, 0)) {
        strcat(fichlog,"\nImpossible de créer le pipe");
        bad1:
        GlobalFree(cbuff);
        return 0;
    }
    
    /*
     Lance l'application
    */
    if (!CreateProcess(0, cmd, 0, 0, TRUE, 0, 0, 0, &sinfo, &pinfo))
    {
        strcpy(Errsauv,"MsgBox\"Pb lors de la sauvegarde : L'application 7-zip (c:\\programme files\\sauvegarde\\7z.exe) n'a pas fonctionné correctement. Consultez votre fichier de log dans le répertoire de sauvegarde\",vbCritical,\"Erreur de sauvegarde\"");
        MessageBox(hwnd,Errsauv,"Erreur",MB_ICONERROR);
        CloseHandle(sinfo.hStdInput);
        CloseHandle(readfh);
        CloseHandle(sinfo.hStdOutput);
        goto bad1;
    }
    
    CloseHandle(sinfo.hStdInput);
    CloseHandle(sinfo.hStdOutput);

    sinfo.dwFlags = 0;
    
    
    /*
      Lecture jusqu'à OUTPUTBUFSIZE octets
    */
    while(readfh) {
        if (!ReadFile(readfh, cbuff + sinfo.dwFlags, OUTPUTBUFSIZE - sinfo.dwFlags, &pinfo.dwProcessId, 0) || !pinfo.dwProcessId) {
            // Si c'est une fin de programme ok, sinon erreur
            if (GetLastError() != ERROR_BROKEN_PIPE && pinfo.dwProcessId) {
                strcat(fichlog,"\nImpossible de récuperer la sortie standard du process");
                GlobalFree(cbuff);
                cbuff = 0;
                break;
            }
            CloseHandle(readfh);  // Close the pipe
            readfh = 0;
        }
        sinfo.dwFlags += pinfo.dwProcessId;
    }
    
    if (readfh) CloseHandle(readfh); // Si on a fini de lire on ferme le pipe
    WaitForSingleObject(pinfo.hProcess, INFINITE);        // On attend jusqu'à la fin du programme
    
    // ferme le process et les handles
    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
    
    if (cbuff) *(cbuff + sinfo.dwFlags) = 0;              // On ajoute un caractère null pour finir la chaine
    
    return cbuff;
}
