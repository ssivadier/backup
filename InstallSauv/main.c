#include <windows.h>
#include <string.h>
#include "SID.h"


// Fonction de création des clés concernées dans le registre
void RegSetKey(HKEY szSection, CHAR szPath[], CHAR szName[], CHAR szValue[])
{
    HKEY hKeySet;
    DWORD dwData;
    int nError;
    
    
    // Création de la clé ou ouverture si existante
    nError = RegCreateKeyEx((HKEY)szSection, szPath,
    0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeySet, &dwData);
    if (nError == ERROR_SUCCESS) printf("cle: %s\\%s Cree avec succes\n", szPath, szName);
  
    // Définition du nom, du type et de la valeur de la clé
    RegSetValueEx(hKeySet, szName, 0, REG_SZ, (LPBYTE) szValue, strlen(szValue));
    
    // Fermeture de la clé
    RegCloseKey(hKeySet);
    return;
}




int main(int argc,char *argv[] )
{
    char key[512]; 
    char szUser[256];
    char szSID[256];
    unsigned long  nsize = 256, nsidsize= 256;
    PSID pSid = NULL;
 
 
    if(!GetUserName(szUser, &nsize) ||
            (NULL == (pSid = pSidGetOwnerSid(szUser))))
        printf("Impossible de recuperer le SID");
 
    if(!GetTextualSid(pSid, szSID, &nsidsize)) {
        printf("Impossible de rendre lisible le SID");
    }
    
        
    strcpy(key,szSID);
    strcat(key,"\\Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\0");
    RegSetKey(HKEY_USERS, key, "Script", "C:\\Program Files\\sauvegarde\\sauvegarde.exe");
    RegSetKey(HKEY_USERS, key, "Parameters", "");
    RegSetKey(HKEY_USERS, key, "ExecTime", "hex(b):00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00");    
                                     
    strcpy(key,szSID);
    strcat(key,"\\Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0");
    RegSetKey(HKEY_USERS, key, "GPO-ID", "LocalGPO");       
    RegSetKey(HKEY_USERS, key, "SOM-ID", "Local");       
    RegSetKey(HKEY_USERS, key, "FileSysPath", "C:\\WINDOWS\\System32\\GroupPolicy\\User"); 
    RegSetKey(HKEY_USERS, key, "DisplayName", "Stratégie de groupe locale");
    RegSetKey(HKEY_USERS, key, "GPOName", "Stratégie de groupe locale");
                            
        
    strcpy(key,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\");
    strcat(key,szSID);
    strcat(key,"\\Scripts\\Logoff\\0\\0");
    RegSetKey(HKEY_LOCAL_MACHINE, key, "Script", "C:\\Program Files\\sauvegarde\\sauvegarde.exe");              
    RegSetKey(HKEY_LOCAL_MACHINE, key, "Parameters", "");
    RegSetKey(HKEY_LOCAL_MACHINE, key, "ExecTime", "hex(b):00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00");
 
                            
    strcpy(key,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\");
    strcat(key,szSID);
    strcat(key,"\\Scripts\\Logoff\\0");
    RegSetKey(HKEY_LOCAL_MACHINE, key, "GPO-ID", "LocalGPO");       
    RegSetKey(HKEY_LOCAL_MACHINE, key, "SOM-ID", "Local");       
    RegSetKey(HKEY_LOCAL_MACHINE, key, "FileSysPath", "C:\\WINDOWS\\System32\\GroupPolicy\\User"); 
    RegSetKey(HKEY_LOCAL_MACHINE, key, "DisplayName", "Stratégie de groupe locale");
    RegSetKey(HKEY_LOCAL_MACHINE, key, "GPOName", "Stratégie de groupe locale");
                    
                    
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\0", "Script", "C:\\Program Files\\sauvegarde\\sauvegarde.exe");
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\0", "Parameters", "");       
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\0", "ExecTime", "hex(b):00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00");     
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0", "GPO-ID", "LocalGPO");       
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0", "SOM-ID", "Local");       
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0", "FileSysPath", "C:\\WINDOWS\\System32\\GroupPolicy\\User"); 
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0", "DisplayName", "Stratégie de groupe locale");
    RegSetKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0", "GPOName", "Stratégie de groupe locale");
    
    /*
      Permet d'eviter l'arret du script au bout de 10 minutes
    */
    HKEY hKeySet;
    DWORD dwData;
    int nError;
    const BYTE waitgpo = 0;
    
    // Création de la clé ou ouverture si existante
    nError = RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
    0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeySet, &dwData);
    if (nError == ERROR_SUCCESS) printf("cle: %s\\%s Cree avec succes\n", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "MaxGPOScriptWait");
  
    // Définition du nom, du type et de la valeur de la clé
    RegSetValueEx(hKeySet, "MaxGPOScriptWait", 0, REG_DWORD, &waitgpo, sizeof(int));
    
    // Fermeture de la clé
    RegCloseKey(hKeySet);
    
     /*
      Permet d'eviter l'arret du script au bout de 10 minutes
    */    
    // Création de la clé ou ouverture si existante
    nError = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
    0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeySet, &dwData);
    if (nError == ERROR_SUCCESS) printf("cle: %s\\%s Cree avec succes\n", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "MaxGPOScriptWait");
  
    // Définition du nom, du type et de la valeur de la clé
    RegSetValueEx(hKeySet, "MaxGPOScriptWait", 0, REG_DWORD, &waitgpo, sizeof(int));
    
    // Fermeture de la clé
    RegCloseKey(hKeySet);
    	
    return 0;
}

