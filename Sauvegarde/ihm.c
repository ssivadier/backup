/*
  Name: ihm.c
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 12/06/08
  Description: Interface Homme-Machine
*/

#include "ihm.h"

/*
  Gestion des commandes du "A Propos"
*/                     
LRESULT CALLBACK PropoProc(HWND hwDlg, UINT message, WPARAM wParam, LPARAM lParam){
        HWND hwGroup,hwButtonOk,hwLabelInfo,hwLabelInfo2, hwLabelIcon;
        HWND hwButtonSite;
        HANDLE hwImage;
        
        switch(message){
            case WM_INITDIALOG :
                 hwGroup = CreateWindow("button","Sauvegarde",WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                                                    10,10,400,180,hwDlg,NULL,hinst,NULL);
                 hwButtonOk = CreateWindow("button","OK",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                                                     20,155,80,25,hwDlg,(HMENU)111,hinst,NULL);
                 hwLabelInfo = CreateWindow("static","Scol-Téléservices",WS_CHILD | WS_VISIBLE,
                                                                     85,50,300,30,hwDlg,NULL,hinst,NULL);
                 hwLabelInfo2 = CreateWindow("static","DirectionInformatique - Académie de Bordeaux\n\nVersion : 3.2",WS_CHILD | WS_VISIBLE,
                                                                     85,80,300,70,hwDlg,NULL,hinst,NULL);
                 hwButtonSite = CreateWindow("button","http://www.ac-bordeaux.fr",WS_VISIBLE | WS_CHILD  | BS_PUSHBUTTON,
                                                                     180,155,200,25,hwDlg,(HMENU)1111,hinst,NULL);                          
                 HFONT PoliceVerdana;
                 HFONT PoliceVerdana2;
                 
                 PoliceVerdana = CreateFont(17,0,0,0,0,1,0,0,0,0,0,0,0,"verdana");
                 PoliceVerdana2 = CreateFont(15,0,0,0,0,0,0,0,0,0,0,0,0,"verdana");
                 SendMessage(hwGroup,WM_SETFONT,(long)PoliceVerdana2,0);
                 SendMessage(hwButtonOk,WM_SETFONT,(long)PoliceVerdana2,0);
                 SendMessage(hwLabelInfo,WM_SETFONT,(long)PoliceVerdana,0);
                 SendMessage(hwLabelInfo2,WM_SETFONT,(long)PoliceVerdana2,0);
                 SendMessage(hwButtonSite,WM_SETFONT,(long)PoliceVerdana2,0);
                 
                 // on recupere le HWND du bouton
                 hwLabelIcon = CreateWindow("static","",WS_CHILD | WS_VISIBLE | SS_ICON,15,55,80,25,hwDlg,(HMENU)111,hinst,NULL);
                 
                 // on charge l'image
                 hwImage = LoadImage(hinst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 0, 0, 0);
                 // et on l'applique sur le bouton
                 SendMessage(hwLabelIcon, STM_SETICON, (WPARAM)(HANDLE)hwImage, 0);
                 
                 break;
            case WM_COMMAND :
                 if(HIWORD(wParam) == BN_CLICKED){
                                   switch(LOWORD(wParam)){
                                       case 111 :
                                            EndDialog(hwDlg,0);
                                       break;
                                       case 1111 : ShellExecute(NULL,"open","http://www.ac-bordeaux.fr",0,NULL,SW_MAXIMIZE);
                                       break;
                                   }
                 }
                 break;            
            case WM_CLOSE :
                 EndDialog(hwDlg,0);
                 break;
        }
        return 0;
}






/* ---------------------------------------------------------------------
                     Traitement des messages utilisateur
------------------------------------------------------------------------
*/

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)      // Selon le message
    {                  
        
        case WM_CREATE:
              
              argv1 = malloc(18);    // on alloue la mémoire pour les arguments passés
              wcstombs(argv1, pArgv[1],18);   // on convertit le wide char en char
              
              
              strcpy(ch_buff1,"C:\\Program Files\\sauvegarde"); // on vérifie si le dossier SAUV_CRIA existe
              if (access(ch_buff1,0)) {
                 MessageBox(hwnd,"Le dossier C:\\Program Files\\sauvegarde\\ n'est pas créé.\nImpossible de démarrer la sauvegarde.","Arret",MB_ICONERROR);
                 exit(-1);
              }
              
              
              strcpy(ch_buff1,"C:\\Program Files\\sauvegarde\\sauv.conf");
              
              if ((access(ch_buff1,0)==0) && (strcmp(argv1,"-min") != 0)) {   //test si le fichier de config existe et s'il n'y a pas de parametre "-conf"
                 gestsauv(hwnd);         // début de la sauvegarde (main.h)
              }
              else {                        // sauvegarde.exe -min
                  /*
                                   Création des onglets
                  */
                  htabs = CreateWindowEx(0 , WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE, 0, 0, 570, 240, hwnd, NULL, hinst, NULL);
                  tie.mask = TCIF_TEXT;
    
                  tie.pszText = " A sauvegarder ";
                  TabCtrl_InsertItem(htabs, 1, &tie);
                  tie.pszText = " Destination ";
                  TabCtrl_InsertItem(htabs, 2, &tie);
                         
                         
                  /*
                           Création des groupbox et des autres boutons
                  */
                  groupbox1 = CreateWindow("button","Autorisations de sauvegarde", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,20,30,260,200,hwnd,0,hinst, NULL);
                  chx_all = CreateWindow("button","Tout Sauvegarder?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,30,52,230,30,hwnd,(HMENU)CHX_ALL,hinst, NULL);
                  chx_thunder = CreateWindow("button","Sauvegarde Thunderbird?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,30,80,230,30,hwnd,(HMENU)CHX_THUN,hinst, NULL);
                  chx_outlook = CreateWindow("button","Sauvegarde Outlook?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,30,105,230,30,hwnd,(HMENU)CHX_OUT,hinst, NULL);
                  chx_outlookexp = CreateWindow("button","Sauvegarde Outlook Express?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,30,130,230,30,hwnd,(HMENU)CHX_OUTXP,hinst, NULL);
                  chx_favoris = CreateWindow("button","Sauvegarde Favoris?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,30,155,230,30,hwnd,(HMENU)CHX_FAV,hinst, NULL);
                  chx_windata = CreateWindow("button","Sauvegarde Windata?", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,30,180,230,30,hwnd,(HMENU)CHX_WIND,hinst, NULL);
                         
                  groupbox2 = CreateWindow("button","Dossier de Sauvegarde", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,20,30,260,200,hwnd,0,hinst, NULL);
                  static1 = CreateWindow("static","Saisissez le chemin ci-dessous\nou cliquez sur parcourir", WS_CHILD | WS_VISIBLE ,30,65,240,40,hwnd,0,hinst, NULL);
                  dossdest = CreateWindow("edit","", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,30,100,240,20,hwnd,(HMENU)TXT_DD,hinst, NULL);
                  parcourir = CreateWindow("button","Parcourir", WS_CHILD | WS_VISIBLE ,30,123,80,25,hwnd,(HMENU)BTN_PARCOUR,hinst, NULL);
                  static2 = CreateWindow("static","Nombre de jours d'historisation : ", WS_CHILD | WS_VISIBLE ,30,170,240,20,hwnd,0,hinst, NULL);
                  cmbbox = CreateWindow("COMBOBOX","Parcourir", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST ,30,195,60,255,hwnd,0,hinst, NULL);
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"1");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"2");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"3");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"4");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"5");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"6");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"7");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"8");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"9");
                  SendMessage(cmbbox, CB_ADDSTRING, 0,(LPARAM)"10");  
                  SendMessage(cmbbox, CB_SETCURSEL,0,0);
                         
                  groupbox3 = CreateWindow("button","dossier supplementaire", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,290,30,260,100,hwnd,0,hinst, NULL);
                  dossajout = CreateWindow("edit","", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,300,55,240,20,hwnd,(HMENU)TXT_DA,hinst, NULL);
                  parcourir2 = CreateWindow("button","Parcourir", WS_CHILD | WS_VISIBLE ,300,78,80,25,hwnd,(HMENU)BTN_PARCOUR2,hinst, NULL);
                         
                  sauvconf = CreateWindow("button","OK", WS_CHILD | WS_VISIBLE ,190,245,90,28,hwnd,(HMENU)CM_SAUV,hinst, NULL);
                  reinit = CreateWindow("button","Réinitialiser", WS_CHILD | WS_VISIBLE ,300,245,90,28,hwnd,(HMENU)CM_INIT,hinst, NULL);
           
                  /*
                           On change la police
                  */
                  HFONT Police;
                 
                  Police = CreateFont(15,0,0,0,0,0,0,0,0,0,0,0,0,"tahoma");
                  SendMessage(groupbox1,WM_SETFONT,(long)Police,0);
                  SendMessage(groupbox2,WM_SETFONT,(long)Police,0);
                  SendMessage(groupbox3,WM_SETFONT,(long)Police,0);
                  SendMessage(static1,WM_SETFONT,(long)Police,0);
                  SendMessage(static2,WM_SETFONT,(long)Police,0);
                  SendMessage(chx_all,WM_SETFONT,(long)Police,0);
                  SendMessage(chx_thunder,WM_SETFONT,(long)Police,0);
                  SendMessage(chx_outlook,WM_SETFONT,(long)Police,0);
                  SendMessage(chx_outlookexp,WM_SETFONT,(long)Police,0);
                  SendMessage(chx_favoris,WM_SETFONT,(long)Police,0);
                  SendMessage(chx_windata,WM_SETFONT,(long)Police,0);
                  SendMessage(cmbbox,WM_SETFONT,(long)Police,0);
                  SendMessage(dossdest,WM_SETFONT,(long)Police,0);
                  SendMessage(dossajout,WM_SETFONT,(long)Police,0);
                  SendMessage(parcourir,WM_SETFONT,(long)Police,0);
                  SendMessage(parcourir2,WM_SETFONT,(long)Police,0);
                  SendMessage(sauvconf,WM_SETFONT,(long)Police,0);
                  SendMessage(reinit,WM_SETFONT,(long)Police,0);
                  SendMessage(htabs,WM_SETFONT,(long)Police,0);
                 
                         
                  /*
                           On affiche (ou cache) les dialogs par défaut
                  */
                  ShowWindow(groupbox1,SW_SHOW);
                  ShowWindow(chx_all,SW_SHOW);
                  ShowWindow(chx_thunder,SW_SHOW);
                  ShowWindow(chx_outlook,SW_SHOW);
                  ShowWindow(chx_outlookexp,SW_SHOW);
                  ShowWindow(chx_favoris,SW_SHOW);
                  ShowWindow(chx_windata,SW_SHOW);
                  ShowWindow(groupbox3,SW_SHOW);
                  ShowWindow(dossajout,SW_SHOW);                 
                  ShowWindow(parcourir2,SW_SHOW);
                  ShowWindow(groupbox2,SW_HIDE);
                  ShowWindow(parcourir,SW_HIDE);
                  ShowWindow(dossdest,SW_HIDE);
                  ShowWindow(cmbbox,SW_HIDE);
                  ShowWindow(static1,SW_HIDE);
                  ShowWindow(static2,SW_HIDE);
                  
                  /*
                            Permet de réinitialiser la sauvegarde normalement (apres desactivation)
                  */
                  reconf(hwnd,"1"); // reconfigure le sauv.conf (main.h)  
                      
                  /* 
                            Si le fichier de configuration existe deja, alors on charge la conf (directement à l'ouverture)
                  */
                  chargeconf(hwnd);
                  
                  /*
                            Gère l'iconification (systray)
                  */
                  tnd.cbSize = sizeof(NOTIFYICONDATA);
                  tnd.hWnd = hwnd;
                  tnd.uID = IDI_APPICON;
                  tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
                  tnd.uCallbackMessage = WM_TRAY;
                  tnd.hIcon = LoadIcon(hinst,MAKEINTRESOURCE(IDI_APPICON));
                  strcpy(tnd.szTip,"Sauvegarde");
                  Shell_NotifyIcon(NIM_ADD,&tnd);
              }      
            break;
                 
                 
                 
                 
                 
        case WM_COMMAND:     //Message correspondant à une action sur un composant (bouton, combo…)

            switch(LOWORD(wParam)) {
                                   
                 case CM_QUITTER:
	                  Shell_NotifyIcon(NIM_DELETE, &tnd); // on supprime l'icone du systray
                      DestroyWindow(hwnd);
                 break;
                 
                 case CM_SHOW:
                      ShowWindow(hwnd,1); 
                 break;
                 
                 /*
                   Desactive temporairement la sauvegarde (pour la prise en main a distance)
                 */
                 case CM_INACTIVE: 
                      if (desactive == 0) {                    
                         reconf(hwnd,"0"); // reconfigure le sauv.conf (main.h)
                         desactive = 1;
                      } else {
                         MessageBox(hwnd,"La sauvegarde a deja ete desactivee","Impossible",MB_ICONINFORMATION);
                      }
                 break;
                 
                 case CM_APROPOS:                               
                      hgMemory = GlobalAlloc(GPTR,512);
                      if(!hgMemory) break;
                      dltDlgHelp = (LPDLGTEMPLATE) hgMemory;
                               
                      dltDlgHelp->style = WS_CAPTION | WS_POPUP | DS_MODALFRAME | WS_SYSMENU;
                      dltDlgHelp->cx = 210;
                      dltDlgHelp->cy = 100;
                      dltDlgHelp->x = GetSystemMetrics(SM_CXSCREEN)/6;
                      dltDlgHelp->y = GetSystemMetrics(SM_CYSCREEN)/6;
                      lpWord = (LPWORD) (dltDlgHelp+1);
                      lpwsWindowNameUnicode = (LPWSTR) (lpWord+2);         
                      MultiByteToWideChar(CP_ACP,0,"A propos de ",-1,lpwsWindowNameUnicode,128);
                      DialogBoxIndirect(hinst,dltDlgHelp,0,(DLGPROC)PropoProc);
                 break;
                 
                 case CHX_ALL:
                      if (IsDlgButtonChecked(hwnd, CHX_ALL) == BST_CHECKED) {
                          CheckDlgButton(hwnd, CHX_FAV, BST_CHECKED);
                          CheckDlgButton(hwnd, CHX_OUT, BST_CHECKED);
                          CheckDlgButton(hwnd, CHX_OUTXP, BST_CHECKED);
                          CheckDlgButton(hwnd, CHX_THUN, BST_CHECKED);
                          CheckDlgButton(hwnd, CHX_WIND, BST_CHECKED);   
                      }
                      else {
                          CheckDlgButton(hwnd, CHX_FAV, BST_UNCHECKED);
                          CheckDlgButton(hwnd, CHX_OUT, BST_UNCHECKED);
                          CheckDlgButton(hwnd, CHX_OUTXP, BST_UNCHECKED);
                          CheckDlgButton(hwnd, CHX_THUN, BST_UNCHECKED);
                          CheckDlgButton(hwnd, CHX_WIND, BST_UNCHECKED);
                      }
                 break;
                 
                 case BTN_PARCOUR:
                        browser.hwndOwner = hwnd;
                        browser.pidlRoot = 0;
                        browser.lpfn = 0;
                        browser.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE; //permet l'option de création d'un ch_buff1
                        browser.lParam = 0;
                        browser.pszDisplayName = DestDir;  //résultat ici
                        browser.lpszTitle = "dossier de destination";
                        lip = SHBrowseForFolder(&browser);
                        SHGetPathFromIDList(lip, DestDir);
                        SetDlgItemText(hwnd, TXT_DD, DestDir);        // on donne le chemin a la textbox 
                        if (!strcmp(DestDir,"")) MessageBox(hwnd,"Aucun dossier de destination selectionné","Attention",MB_ICONWARNING);                   
                 break;
                 
                 case BTN_PARCOUR2:
                        browser.hwndOwner = hwnd;
                        browser.pidlRoot = 0;
                        browser.lpfn = 0;
                        browser.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE; //permet l'option de création d'un ch_buff1
                        browser.lParam = 0;
                        browser.pszDisplayName = DestDir;  //résultat ici
                        browser.lpszTitle = "dossier de destination";
                        lip = SHBrowseForFolder(&browser);
                        SHGetPathFromIDList(lip, DestDir);
                        SetDlgItemText(hwnd, TXT_DA, DestDir);        // on donne le chemin a la textbox 
                        if (!strcmp(DestDir,"")) MessageBox(hwnd,"Aucun dossier selectionné","Attention",MB_ICONWARNING);                   
                 break;
                 
                 case CM_INIT:
                      CheckDlgButton(hwnd, CHX_ALL, BST_UNCHECKED);
                      CheckDlgButton(hwnd, CHX_FAV, BST_UNCHECKED);
                      CheckDlgButton(hwnd, CHX_OUT, BST_UNCHECKED);
                      CheckDlgButton(hwnd, CHX_OUTXP, BST_UNCHECKED);
                      CheckDlgButton(hwnd, CHX_THUN, BST_UNCHECKED);
                      CheckDlgButton(hwnd, CHX_WIND, BST_UNCHECKED);
                      SetDlgItemText(hwnd, TXT_DD, "");
                      SetDlgItemText(hwnd, TXT_DA, "");
                      SendMessage(cmbbox, CB_SETCURSEL,0,0);                     
                 break;
                 
                 case CM_SAUV:
                      
                      GetWindowText(dossdest,DestDir,300);    // DestDir récupère la valeur du champ txt (TXT_DD)
                      GetWindowText(dossajout,AddDir,300);    // AddDir récupère la valeur du champ txt (TXT_DA)
                      
                      if (strcmp(DestDir,"")) {
                          char fav[20], out[20], outxp[20], thun[20], wind[20], histo[2];
                          strcpy(fav, "\nnon\n");
                          strcpy(out, "non\n");
                          strcpy(outxp, "non\n");
                          strcpy(thun, "non\n");
                          strcpy(wind, "non\n");
                          
                          if(IsDlgButtonChecked(hwnd, CHX_FAV) == BST_CHECKED) strcpy(fav, "\nfavoris\n");
                          if(IsDlgButtonChecked(hwnd, CHX_OUT) == BST_CHECKED) strcpy(out, "outlook\n");
                          if(IsDlgButtonChecked(hwnd, CHX_OUTXP) == BST_CHECKED) strcpy(outxp, "outlookxp\n");
                          if(IsDlgButtonChecked(hwnd, CHX_THUN) == BST_CHECKED) strcpy(thun, "thunderbird\n");    
                          if(IsDlgButtonChecked(hwnd, CHX_WIND) == BST_CHECKED) strcpy(wind, "windata\n");    
                          
                          
                          if((IsDlgButtonChecked(hwnd, CHX_FAV) == BST_UNCHECKED)            //si aucune case n'est coché, on prompte
                          && (IsDlgButtonChecked(hwnd, CHX_OUT) == BST_UNCHECKED)
                          && (IsDlgButtonChecked(hwnd, CHX_OUTXP) == BST_UNCHECKED)
                          && (IsDlgButtonChecked(hwnd, CHX_THUN) == BST_UNCHECKED)
                          && (IsDlgButtonChecked(hwnd, CHX_WIND) == BST_UNCHECKED))
                             if (MessageBox(hwnd,"Aucune case n'a ete cochée.\n\nEtes-vous certain de ne rien vouloir sauvegarder?\n","Attention",MB_YESNO) == IDNO) break;
                          
                          /*
                            Création du fichier de configuration
                          */
                          int combo = SendMessage(cmbbox, CB_GETCURSEL, 0,0);
                          itoa(combo,histo,10);
                          strcpy(ch_buff3,DestDir);
                          strcat(ch_buff3,fav);
                          strcat(ch_buff3,out);
                          strcat(ch_buff3,outxp);
                          strcat(ch_buff3,thun);
                          strcat(ch_buff3,wind);
                          strcat(ch_buff3,histo);
                          strcat(ch_buff3,"\n");
                          strcat(ch_buff3,AddDir);
                          strcat(ch_buff3,"\n1\n");       // 1 = sauvegarde active (modifié par systray)
                          strcpy(ch_buff2,"C:\\Program Files\\sauvegarde\\sauv.conf");
                          fichier = fopen(ch_buff2, "w");
                          if(fichier==NULL) { //Si le fichier n'est pas lisible, on arrete tout
                                     fclose(fichier);
                                     exit(-1);
                          }
                          fwrite(ch_buff3,1,sizeof(ch_buff3),fichier);
                          fclose(fichier);
                                             
                          MessageBox(hwnd,"Configuration Sauvegardée","Sauvegarde",MB_ICONINFORMATION);
                          
                          ShowWindow(hwnd,0);
                      }
                      else {
                          MessageBox(hwnd,"Aucun dossier de destination selectionné","Erreur",MB_ICONERROR);
                          exit(-1);
                      }
                 break;
                 
                 case CM_OUVRIR:
                     chargeconf(hwnd);
                 break;
                 
            }
            break;
            
// A chaque fois qu'il y a un changement, on vérifie que ce n'est pas l'onglet qui a été cliqué, sinon on simule son utilisation (show/hide)
        case WM_NOTIFY:
            {
            LPNMHDR pnmhdr = (LPNMHDR)lParam;
            if(pnmhdr->code == TCN_SELCHANGE && TabCtrl_GetCurSel(htabs) == 0)
              {
                 ShowWindow(groupbox1,SW_SHOW);
                 ShowWindow(chx_all,SW_SHOW);
                 ShowWindow(chx_thunder,SW_SHOW);
                 ShowWindow(chx_outlook,SW_SHOW);
                 ShowWindow(chx_outlookexp,SW_SHOW);
                 ShowWindow(chx_favoris,SW_SHOW);
                 ShowWindow(chx_windata,SW_SHOW);
                 ShowWindow(groupbox3,SW_SHOW);
                 ShowWindow(dossajout,SW_SHOW);                 
                 ShowWindow(parcourir2,SW_SHOW);
                 ShowWindow(groupbox2,SW_HIDE);
                 ShowWindow(parcourir,SW_HIDE);
                 ShowWindow(dossdest,SW_HIDE);
                 ShowWindow(cmbbox,SW_HIDE);
                 ShowWindow(static1,SW_HIDE);
                 ShowWindow(static2,SW_HIDE);
              }
            if(pnmhdr->code == TCN_SELCHANGE && TabCtrl_GetCurSel(htabs) == 1)
              {
                 ShowWindow(groupbox1,SW_HIDE);
                 ShowWindow(chx_all,SW_HIDE);
                 ShowWindow(chx_thunder,SW_HIDE);
                 ShowWindow(chx_outlook,SW_HIDE);
                 ShowWindow(chx_outlookexp,SW_HIDE);
                 ShowWindow(chx_favoris,SW_HIDE);
                 ShowWindow(chx_windata,SW_HIDE);
                 ShowWindow(groupbox3,SW_HIDE);
                 ShowWindow(dossajout,SW_HIDE);                 
                 ShowWindow(parcourir2,SW_HIDE);
                 ShowWindow(groupbox2,SW_SHOW);
                 ShowWindow(parcourir,SW_SHOW);
                 ShowWindow(dossdest,SW_SHOW);
                 ShowWindow(cmbbox,SW_SHOW);
                 ShowWindow(static1,SW_SHOW);
                 ShowWindow(static2,SW_SHOW);
              }
            }
            break;
    
        /*case WM_QUERYENDSESSION:           // récupère le msg systeme de demande de fin de session
                     if (lParam ==0) {       // si on fait un arret ou un reboot (lparam = 0)
                                gestsauv(hwnd);  // gestion de la sauvegarde des fichiers (main.h)                             
                     }
                     exit(-1);     
             break;*/
        
        case WM_TRAY:              // evenements du systray
             if((lParam==WM_RBUTTONUP) || (lParam==WM_LBUTTONDBLCLK)){
             // On charge le menu Tray et le sous menu correspondant
          			HMENU menu = LoadMenu(hinst,MAKEINTRESOURCE(IDM_TRAY));
           		    if(!menu) return FALSE;
                   			 
          			HMENU submenu = GetSubMenu(menu,0);
          			if(!submenu) return FALSE;
                    			
          			if(lParam==WM_RBUTTONUP){
         				AffichageMenu = TRUE;
         				POINT mouse;
         				GetCursorPos(&mouse);
         				// TPM_RETURNCMD : un message WM_COMMAND est renvoyé à la boucle de messages
         				// TPM_RIGHTALIGN : Aligner le menu à droite
         				TrackPopupMenu(submenu, TPM_RETURNCMD || TPM_RIGHTALIGN, mouse.x, mouse.y, 0,hwnd, NULL);
         				AffichageMenu = FALSE;
         				DestroyMenu(submenu);
         				DestroyMenu(menu);
         				return TRUE;
          			}
             }
            break;   
             
        case WM_CLOSE:
                      ShowWindow(hwnd,0);   // si on click sur la croix, on cache la fenetre (systray)  
            break;
            
        case WM_DESTROY:
                     PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
            
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
