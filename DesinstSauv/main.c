#include <windows.h>
#include <string.h>
#include "SID.h"

int main()
{
  
  HKEY hKeyGet;
  char key[512] = "";      
  char szUser[256];
  char szSID[256];
  unsigned long  nsize = 256, nsidsize= 256;
  PSID pSid = NULL;
             
  if(!GetUserName(szUser, &nsize) || (NULL == (pSid = pSidGetOwnerSid(szUser))))
            printf("Impossible de recuperer le SID");
     
  if(!GetTextualSid(pSid, szSID, &nsidsize)) 
            printf("Impossible de rendre lisible le SID");
    
  strcpy(key,szSID);
  strcat(key,"\\Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\0");
  RegDeleteKey(HKEY_USERS, key);
                             
  strcpy(key,szSID);
  strcat(key,"\\Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0");
  RegDeleteKey(HKEY_USERS, key);
                            
  strcpy(key,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\");
  strcat(key,szSID);
  strcat(key,"\\Scripts\\Logoff\\0\\0");
  RegDeleteKey(HKEY_LOCAL_MACHINE, key); 
                            
  strcpy(key,"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\");
  strcat(key,szSID);
  strcat(key,"\\Scripts\\Logoff\\0");
  RegDeleteKey(HKEY_LOCAL_MACHINE, key);
                    
  RegDeleteKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\0"); 
  RegDeleteKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0\\1");
  RegDeleteKey(HKEY_CURRENT_USER, "Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\0");
  remove("C:\\windows\\tasks\\sauvegarde.job");
                             
  return 0;
}
