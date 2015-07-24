/*
  Name: fxgenerales.c
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 17/04/08 14:09
  Description: Contient les fonctions principales necessaires au prog principal
*/
#include "fxgenerales.h"
#include "newprocess.h"

/*
  Fonction permettant de couper une chaine en plusieurs chaine selon un motif
*/
char** split( char* str, char c ) {
    static char* ptrch_splite[256] ;    /* 256 colonnes max */
    int current = 0;
    ptrch_splite[current++] = str;
    while( *str ) {
        if ( *str == c ) {
            *str = '\0';
            ptrch_splite[current++] = str+1;  /* on devrait vérifier si on dépasse pas 256 */
        }
        ++str;
    }
    ptrch_splite[ current ] = 0;
    return ptrch_splite;
}



/*
  Equivalent de substr() pour C
*/
DWORD strsub(const char *s, DWORD pos, DWORD len, char *szdst) {
  char *p = szdst;
  if(pos) {  
    do {
      if(*s++ == 0) {
          *p = 0;
          return (p - szdst);
      }
    } while(--pos);
  }
  do {
    if((*p = *s) == 0) break;
    p++;
    s++;
  } while(--len);
  
  *p = 0;
  return (p - szdst);
}



/*
  Permet de remplacer un caractère par un autre
*/
void remplace(char *fic, char rechercher, char remplacer) {
    char *c = fic;
    
    while(*c) {
      if(*c == rechercher) *c = remplacer;
      
      c++;
    }
}



/*
  Recupère la version de l'OS windows (API)
*/
int GetOSVersion()
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    if(!(bOsVersionInfoEx=GetVersionEx((OSVERSIONINFO *)&osvi)))
       {
       osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
       if (!GetVersionEx((OSVERSIONINFO *)&osvi) ) 
       return OS_ERROR;
       }
    if(osvi.dwMajorVersion==3 && osvi.dwMinorVersion==51)
    	return OS_WIN32_WINDOWS_NT_3_51;
    if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
    	return OS_WIN32_WINDOWS_NT_4_0;
    if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0 && 
       osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
    	return OS_WIN32_WINDOWS_95;
    if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==10 && 
       osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
    	return OS_WIN32_WINDOWS_98;
    if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==90 && 
       osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
    	return OS_WIN32_WINDOWS_Me;
    if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
    	return OS_WIN32_WINDOWS_2000;
    if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==1 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
    	return OS_WIN32_WINDOWS_XP;
    if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
    	return OS_WIN32_WINDOWS_SERVER_2003_FAMILY;
    if(osvi.dwMajorVersion==6 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
        return OS_WIN32_WINDOWS_VISTA;
}




/*
  Gère l'historisation des dossiers (renomme 5 en 4, 4 en 3,...)
*/
void Hdossier(HWND hwnd, char dossier[512]) {
     
     int i, j;
     long int histo;
     char k[2], l[2];
     histo = strtol(tab[6], (char **)NULL, 10);   // on converti la valeur du fichier de sauvegarde pour connaitre le nombre d'historisations.
     j = histo;
     i = ++histo;
     
     itoa(histo,tab[6],10);       // on rajoute +1 à la variable temporaire tab[6] pour avoir le nombre de dossier et pas l'emplacement (du au dropdownlist)
     
     /*
       Supprime le dernier dossier s'il existe
     */
     strcpy(tmp2,dossier);
     strcat(tmp2,"\\");
     strcat(tmp2,tab[6]);
     if (access(tmp2,0)==0) {
        ZeroMemory(&FileOpStr, sizeof(FileOpStr));
        FileOpStr.hwnd = hwnd;
        FileOpStr.fFlags = FOF_NOCONFIRMATION;
        FileOpStr.wFunc = FO_DELETE;
        FileOpStr.pFrom = tmp2;
        SHFileOperation(&FileOpStr);
     }
     
     /*
       Renomme les dossiers pour garder le dossier 1 comme le dernier sauvegardé
     */
     while ( i >= 2 ) {
         strcpy(tmp,dossier);    //recupere le chemin du serveur avec username
         strcat(tmp,"\\");      
         strcpy(tmp2,tmp);    
         strcat(tmp,itoa(i, k, 10));
         strcat(tmp2,itoa(j, l, 10));
         if (access(tmp2,0)==0) {  //test le nom de dossier avant renommage (si existe pas, pas de renommage)
            if (rename(tmp2, tmp )!=0) {
               MessageBox(hwnd,"Impossible de renommer le dossier pour l'historisation.\nAccès interdit.\nVerifiez que vous avez les droits d'accès suffisants sur le dossier de sauvegarde.","Arret du Logiciel de Sauvegarde",MB_ICONERROR);
               exit(-1);
            }
         }
         
         i--;
         j--;
     }
}



/*
  Gère la sauvegarde des fichiers grace a l'API WIN32
*/
void sauvegarde(HWND hwnd) {
     
     strcat(dossier,"\0");
             
     if (strcmp(tab[5],"windata") == 0) {
        if (access("C:\\windata",0) != 0) {     // si le dossier n'existe pas
           strcat(fichierlog,"Aucun dossier \"Windata\" à sauvegarder\n");            
        }
        else {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)
             
             /*
               Compression des données à la volée
             */
             strcpy(param, "C:\\Program Files\\Sauvegarde\\7z.exe a -tzip -y \"");
             strcat(param,dossier);
             strcat(param ,"\\windata.zip\" \"c:\\windata\"");
             
             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Windata\" sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date); 
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Windata\"");  
             }
             free(param);
             free(output);
        }
     }
                   
     
     /*
       Sauvegarde des favoris
     */              
     if (strcmp(tab[1],"favoris") == 0) {
        /*
          Favoris Internet Explorer
        */
        strcpy(tmp,"");
        strcpy(tmp,"C:\\Documents and settings\\");
        strcat(tmp,lpszSystemInfo);
        strcat(tmp,"\\Favoris\0");
        if (access(tmp,0) != 0) {
            strcat(fichierlog,"\nAucun dossier \"Favoris\" pour Internet Explorer à sauvegarder\n");            
        }
        else {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)

             /*
               Compression des données à la volée
             */
             strcpy(param,"C:\\Program Files\\Sauvegarde\\7z.exe a -tzip -y \"");
             strcat(param,dossier);
             strcat(param ,"\\favorisIE.zip\" \"");
             strcat(param ,tmp);
             strcat(param,"\"");

             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Favoris\" pour internet explorer sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date);
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Favoris\" pour internet Explorer");  
             }
             free(param);
             free(output);
        }
        
        
        /*
          Favoris Firefox
        */
        strcpy(tmp,"");
        strcpy(tmp,"C:\\Documents and settings\\");
        strcat(tmp,lpszSystemInfo);
        strcat(tmp,"\\Application Data\\Mozilla\\Firefox\0");
        if (access(tmp,0) != 0) {
            strcat(fichierlog,"\nAucun dossier \"Favoris\" pour Firefox à sauvegarder\n");            
        }
        else {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)

            /*
              On lit le fichier de profil Firefox
            */
            int correct = 0;
            int relatif = 0;    // permet de savoir si c'est un chemin relatif ou non
            FILE *fichier;
            strcpy(tmp2,tmp);
            strcat(tmp2,"\\profiles.ini");     
            fichier = fopen(tmp2, "r");
            while ((!feof(fichier) ) && !correct) {       // tant qu'on est pas a la fin du fichier
                  fgets(buff,512,fichier);   // on recupere la ligne
                  if (strncmp(buff, "IsRelative=1", 12) == 0) relatif = 1;
                  if (strncmp(buff, "Path=", 5) == 0) correct = 1;
            }
            fclose(fichier);
            strsub(buff,5,(strlen(buff)+1),buff);   // on supprime le "Path=" dans la chaine (strlen retourne la taille sans le \0 de fin de chaine, donc: +1)
            remplace(buff,'/','\\');             // permet de remplace le / par \ 
            
            if (relatif == 1) {
               strcat(tmp, "\\");      
               strcat(tmp, buff);
            }
            else {
               strcpy(tmp, buff);      // Modifie tout le chemin de sauvegarde (profil déplacé)
            }
            remplace(tmp,'\n','\0');        // A AMELIORER : supprime le \n et remplace par fin de chaine (et si il y a plusieurs \n? = chaine tronquée)
            strcat(tmp, "\\bookmarks.html\0");
            /*
               Compression des données à la volée
            */
            strcpy(param,"C:\\Program Files\\Sauvegarde\\7z.exe a -tzip -y \"");
            strcat(param,dossier);
            strcat(param ,"\\favorisFF.zip\" \"");
            strcat(param ,tmp);
            strcat(param,"\"");

             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Favoris\" pour firefox sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date); 
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Favoris\" pour firefox");  
             }
             free(param);
             free(output);
        }
     }
                     
                     
     /*
       Sauvegarde de thunderbird
     */
     if (strcmp(tab[4],"thunderbird") == 0) {
        strcpy(tmp,"");
        strcpy(tmp,"C:\\Documents and settings\\");
        strcat(tmp,lpszSystemInfo);
        strcat(tmp,"\\Application Data\\Thunderbird\0");
        if (access(tmp,0) != 0) {
            strcat(fichierlog,"\nAucun dossier \"Thunderbird\" à sauvegarder\n");       
        }
        else {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)

            /*
              On lit le fichier de profil Thunderbird
            */
            int correct = 0;
            int relatif = 0;    // permet de savoir si c'est un chemin relatif ou non
            FILE *fichier;
            strcpy(tmp2,tmp);
            strcat(tmp2,"\\profiles.ini");     
            fichier = fopen(tmp2, "r");
            while ((!feof(fichier) ) && !correct) {       // tant qu'on est pas a la fin du fichier
                  fgets(buff,512,fichier);   // on recupere la ligne
                  if (strncmp(buff, "IsRelative=1", 12) == 0) relatif = 1;
                  if (strncmp(buff, "Path=", 5) == 0) correct = 1;
            }
            fclose(fichier);
            strsub(buff,5,(strlen(buff)+1),buff);   // on supprime le "Path=" dans la chaine (strlen retourne la taille sans le \0 de fin de chaine, donc: +1)
            remplace(buff,'/','\\');             // permet de remplace le / par \ 
            
            if (relatif == 1) {
               strcat(tmp, "\\");      
               strcat(tmp, buff);
            }
            else {
               strcpy(tmp, buff);      // Modifie tout le chemin de sauvegarde (profil déplacé, chemin absolu)
            }
            remplace(tmp,'\n','\0');         // A AMELIORER : supprime le \n et remplace par fin de chaine (et si il y a plusieurs \n? = chaine tronquée)
            /*
               Compression des données à la volée
            */
            strcpy(param,"C:\\Program Files\\Sauvegarde\\7z.exe a -tzip -y \"");
            strcat(param,dossier);
            strcat(param ,"\\thunderbird.zip\" \"");
            strcat(param ,tmp);
            strcat(param,"\"");

             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Thunderbird\" sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date); 
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Thunderbird\"");  
             }
             free(param);
             free(output);
        }
     }
                    
             
     /*
       Sauvegarde Outlook Express
     */       
     if (strcmp(tab[3],"outlookxp") == 0) {
        strcpy(tmp,"");
        strcpy(tmp,"C:\\Documents and Settings\\");
        strcat(tmp,lpszSystemInfo);
        strcat(tmp,"\\Local Settings\\Application Data\\Identities\0");
        if (access(tmp,0) != 0) {
             strcat(fichierlog,"\nAucun dossier \"Outlook Express\" à sauvegarder\n");             
        }
        else {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)

             /*
               Compression des données à la volée
             */
             strcpy(param,"C:\\Program Files\\Sauvegarde\\7z.exe a -tzip -y \"");
             strcat(param,dossier);
             strcat(param ,"\\OutlookExp.zip\" \"");
             strcat(param ,tmp);               //on met le dossier de sauvegarde
             strcat(param,"\" \"C:\\Documents and Settings\\");
             strcat(param ,lpszSystemInfo);
             strcat(param,"\\Application Data\\Microsoft\\Address Book\\*.wab\"");   // et on ajoute les fichiers de carnet d'adresse


             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Outlook Express\" sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date); 
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Outlook Express\"");  
             }
             free(param);
             free(output);
        }
     }
          
     /*          
         Sauvegarde Outlook
     */               
     if (strcmp(tab[2],"outlook") == 0) {
        strcpy(tmp,"C:\\Documents and settings\\");
        strcat(tmp,lpszSystemInfo);
        strcat(tmp,"\\Local Settings\\Application Data\\Microsoft\\Outlook\0");
        if (access(tmp,0) != 0) {
             strcat(fichierlog,"\nAucun dossier \"Outlook\" à sauvegarder\n");             
        }
        else {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)

             /*
               Compression des données à la volée
             */
             strcpy(param,"C:\\Program Files\\Sauvegarde\\7z.exe a -tzip -y \"");
             strcat(param,dossier);
             strcat(param ,"\\Outlook.zip\" \"");
             strcat(param ,tmp);
             strcat(param,"\"");

             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Outlook\" sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date); 
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Outlook\"");  
             }
             free(param);
             free(output);
        }
     }
     
     
     /*          
         Sauvegarde Dossier perso
     */               
     if (access(tab[7],0) == 0) {
             char * output;
             LPTSTR param = (LPTSTR)malloc(sizeof(char)*500+1);   // parametres d'execution (createprocess)

             /*
               Compression des données à la volée
             */
             strcpy(param,"C:\\Program Files\\Sauvegarde\\7z.exe a -xr@\"C:\\Program Files\\Sauvegarde\\exclude.ext\" -tzip -y \"");
             strcat(param,dossier);
             strcat(param ,"\\DossierPerso.zip\" \"");
             strcat(param ,tab[7]);
             strcat(param,"\"");

             if ((output = run_command(hwnd, param))) {
                strcat(fichierlog,"\n\t----------------------------------------------------------");
                strcat(fichierlog,"\n\tDossier \"Perso\" sauvegardé - ");
                time(&now);
                date = ctime(&now);
                strcat(fichierlog,date); 
                strcat(fichierlog,"\t----------------------------------------------------------\n");
                strcat(fichierlog,output);
             }  
             else { 
                strcat(fichierlog,"\nErreur lors de la sauvegarde du dossier \"Perso\"");  
             }
             free(param);
             free(output);
     }
}



/* ------------------------------------------------------------------------------------
     Charge le fichier de configuration dans l'interface graphique
   ------------------------------------------------------------------------------------
*/
void chargeconf(HWND hwnd) {
     strcpy(dossier,"C:\\Program Files\\sauvegarde\\sauv.conf");
     
     if ((access(dossier,0)==0)) {          //si la conf existe on charge les infos
        fichier = fopen(dossier, "r");
        strcpy(buffer,"");              // on re-init le buffer
                              
        while (!feof(fichier) ) {       // tant qu'on est pas a la fin du fichier
              fgets(tmp,512,fichier);   // on recupere la ligne
              strcat(buffer,tmp);       // on l'ajoute au buffer
        }
        tab = split(buffer,'\n');       // on sépare chaque ligne
                          
        SetDlgItemText(hwnd, TXT_DD, tab[0]);        // on attribue les valeurs aux dialogs   
        SetDlgItemText(hwnd, TXT_DA, tab[7]);
        if (strcmp(tab[1],"favoris") == 0) {CheckDlgButton(hwnd, CHX_FAV, BST_CHECKED);}
        if (strcmp(tab[2],"outlook") == 0) {CheckDlgButton(hwnd, CHX_OUT, BST_CHECKED);}
        if (strcmp(tab[3],"outlookxp") == 0) {CheckDlgButton(hwnd, CHX_OUTXP, BST_CHECKED);}
        if (strcmp(tab[4],"thunderbird") == 0) {CheckDlgButton(hwnd, CHX_THUN, BST_CHECKED);}
        if (strcmp(tab[5],"windata") == 0) {CheckDlgButton(hwnd, CHX_WIND, BST_CHECKED);}
        strcpy(DestDir,tab[0]);            // permet d'initialiser la valeur de destination (pour tests ultérieurs)
        long int histval = strtol(tab[6], (char **)NULL, 10);           //converti en long l'historisation
        SendMessage(cmbbox, CB_SETCURSEL,(WPARAM)histval,0);       // charge la valeur d'historisation
        fclose(fichier);             // on ferme le fichier
     }
     else {
          MessageBox(hwnd,dossier,"plpop",MB_OK);
          MessageBox(hwnd,"Impossible de lire le fichier de configuration","Erreur",MB_ICONERROR);     
     }
}


/* ------------------------------------------------------------------------------------
     Reconfigure le sauv.conf
   ------------------------------------------------------------------------------------
*/
void reconf(HWND hwnd, char *actif) {
     
     strcpy(tmp,"C:\\Program Files\\sauvegarde\\sauv.conf");
     fichier = fopen(tmp, "r");
     if(fichier==NULL) { //Si le fichier n'est pas lisible, on arrete tout
           fclose(fichier);
           MessageBox(hwnd,"Impossible de lire le fichier sauv.conf","Erreur",MB_ICONERROR);
           exit(-1);
     }
     
     int z =0;
     TCHAR buff[1000] = "";
                          
     while (!feof(fichier) ) {
           fgets(tmp,512,fichier);
           if (z==8) {  
                  strcat(buff,actif);
                  strcat(buff,"\n");
           } else {
                  strcat(buff,tmp);
           }
           z++;
     }
     fclose(fichier);
     
     strcpy(tmp2,"C:\\Program Files\\sauvegarde\\sauv.conf");
     fichier = fopen(tmp2, "w");
     if(fichier==NULL) { //Si le fichier n'est pas lisible, on arrete tout
           fclose(fichier);
           MessageBox(hwnd,"impossible d'ecrire dans le fichier de configuration (sauv.conf)","Erreur",MB_ICONERROR);
           exit(-1);
     }
     fwrite(buff,1,sizeof(buff),fichier);
     fclose(fichier);
     if (strcmp(actif,"0")==0) MessageBox(hwnd,"Sauvegarde desactivee","Confirmation",MB_ICONINFORMATION);
}



/* ------------------------------------------------------------------------------------
     Gestion de tout le processus de sauvegarde
   ------------------------------------------------------------------------------------
*/
void gestsauv(HWND hwnd) {
        
        strcpy(fichierlog,"");     //init le buffer du fichier de log
        strcpy(buffer,"");     //init le buffer temporaire
        
        
        lpszSystemInfo = tchBuffer;
        GetUserName(lpszSystemInfo, &cchBuff); // récupere le nom de l'utilisateur connecte


        /*
                       Récupère le fichier de configuration pour savoir quoi sauvegarder
        */
        strcpy(tmp,"C:\\Program Files\\sauvegarde\\sauv.conf");
        fichier = fopen(tmp, "r");
        if(fichier==NULL) { //Si le fichier n'est pas lisible, on arrete tout
                         fclose(fichier);
                         exit(-1);
        }
                     
        while (!feof(fichier) ) {
                           fgets(tmp,512,fichier);
                           strcat(buffer,tmp);
        }
        tab = split(buffer,'\n');
                     
        fclose(fichier);
                
        /*
                       Si rien n'a été coché alors on sort
        */
        if ((strcmp(tab[1],"non")==0) && (strcmp(tab[2],"non")==0) && (strcmp(tab[3],"non")==0) && (strcmp(tab[4],"non")==0) && (strcmp(tab[5],"non")==0)) exit(-1);
    
        if (strcmp(tab[8],"0")==0) exit(-1); // si la sauvegarde est desactivée
        
        /*
                       Récupère le username local et créé un dossier si pas créé
        */
        strcpy(dossier,tab[0]);      // partage                     
                     
        if (access(dossier,0)!=0) { // si le dossier sauvegarde n'est pas accessible
                        MessageBox(hwnd,"Le dossier de sauvegarde est inaccessible.","Erreur d'ecriture",MB_ICONERROR);
                        exit(-1);        
        }
        else {
                        strcat(dossier,"\\");
                        strcat(dossier,lpszSystemInfo); // devient "\\CheminSauvegardes\username"
                        if (access(dossier,0)!=0){
                           fichier = fopen(dossier,"w");   // on vérifie si on a les droits d'écriture dans le dossier
                           if (fichier==NULL) {
                                strcat(dossier," est inaccessible.\nVous ne disposez pas de droits suffisants.");
                                MessageBox(hwnd,dossier,"Erreur d'ecriture",MB_ICONERROR);
                                fclose(fichier);
                                exit(-1);     
                           }
                           fclose(fichier);
                           remove(dossier);
                           mkdir(dossier); // Création du dossier "username" dans sauvegarde
                        }
        }
                     
        /*
                       Gere le renommage des dossiers pour l'historisation. (main.h)
        */
        Hdossier(hwnd, dossier);
                     
        /*
                       Création du dossier de sauvegarde du jour
        */
        strcat(dossier,"\\1");
        if (access(dossier,0)!=0) mkdir(dossier);
    
        /*
                       Début de la sauvegarde (main.h)
        */
        sauvegarde(hwnd);
                     
        /*
                       Création du fichier de Log
        */
        strcpy(tmp,dossier);
        strcat(tmp,"\\Log.txt");
        fichier = fopen(tmp, "w");
        fwrite(fichierlog,1,sizeof(fichierlog),fichier);
        fclose(fichier);
                     
                     
        free(fichierlog);                               // on libere la memoire
        free(dossier);
        free(buffer);
        free(argv1);
        free(pArgv);
        free(DestDir);
                     
        DestroyWindow(hwnd);         // on détruit la fenetre
}
