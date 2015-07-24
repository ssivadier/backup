/*
  Name: ressource.h
  Copyright: -
  Author: SIVADIER Sebastien
  Date: 17/04/08 14:09
  Description: Déclare les composants de la fenetre
*/
#ifndef DEF_RESS
#define DEF_RESS


#define IDM_MENU    990
#define IDM_TRAY    991

#define CM_SHOW     9080
#define CM_INIT     9081
#define CM_OUVRIR	9082
#define CM_SAUV     9083
#define CM_QUITTER	9085 
#define CM_APROPOS	9086
#define CM_INACTIVE 9087


#define CHX_FAV     9088
#define CHX_OUT     9089
#define CHX_OUTXP   9090
#define CHX_THUN	9091
#define CHX_WIND	9092
#define CHX_ALL     9093

#define TXT_DD          9100
#define BTN_PARCOUR	    9101
#define TXT_DA          9102
#define BTN_PARCOUR2	9103

#define IDI_APPICON 10200
#define IDR_ACCEL   10201


/*
  Déclaration des controles
*/
static HWND groupbox1;
static HWND groupbox2;
static HWND groupbox3;
static HWND chx_all;
static HWND chx_thunder;
static HWND chx_outlook;
static HWND chx_outlookexp;
static HWND chx_favoris;
static HWND chx_windata;
static HWND parcourir;
static HWND dossdest;
static HWND parcourir2;
static HWND dossajout;
static HWND sauvconf;
static HWND reinit;
static HWND static1;
static HWND static2;
static HWND cmbbox;

static HWND htabs;

#endif 
