// @(#)root/treeviewer:$Name:  $:$Id: TSessionViewer.h
// Author: Marek Biskup, Jakub Madejczyk, Bertrand Bellenot 10/08/2005

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionViewer                                                       //
//                                                                      //
// Widget used to manage PROOF or local sessions, PROOF connections,    //
// queries construction and results handling.                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TSessionViewer
#define ROOT_TSessionViewer

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ROOT_TGTextEntry
#include "TGTextEntry.h"
#endif

#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif

#ifndef ROOT_TGTab
#include "TGTab.h"
#endif

#ifndef ROOT_TGListView
#include "TGListView.h"
#endif

#ifndef ROOT_TTime
#include "TTime.h"
#endif

#include <stdio.h>
#include <time.h>

class TList;
class TChain;
class TDSet;
class TGNumberEntry;
class TGTextEntry;
class TGTextButton;
class TGCheckButton;
class TGTextBuffer;
class TGTableLayout;
class TGIcon;
class TGLabel;
class TGHProgressBar;
class TGPopupMenu;
class TGLVContainer;
class TGListView;
class TGLVEntry;
class TGCanvas;
class TGListTree;
class TGListTreeItem;
class TGStatusBar;
class TGPicture;
class TGMenuBar;
class TGPopupMenu;
class TGToolBar;
class TGTextView;
class TGTab;
class TRootEmbeddedCanvas;
class TGListBox;
class TCanvas;
struct UserGroup_t;

class TVirtualProof;
class TSessionViewer;
class TSessionLogView;
class TQueryResult;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionViewer - A GUI for ROOT / PROOF Sessions                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// TQueryDescription class : Descrition of queries
//////////////////////////////////////////////////////////////////////////

class TQueryDescription : public TObject {

public:
   enum ESessionQueryStatus {
      kSessionQueryAborted = 0,
      kSessionQuerySubmitted,
      kSessionQueryRunning,
      kSessionQueryStopped,
      kSessionQueryCompleted,
      kSessionQueryFinalized,
      kSessionQueryCreated,
      kSessionQueryFromProof
   };

   ESessionQueryStatus fStatus;     // query status
   TString        fReference;       // query reference string (unique identifier)
   TString        fQueryName;       // query name
   TString        fSelectorString;  // selector name
   TString        fTDSetString;     // dataset name
   TString        fOptions;         // query processing options
   TString        fEventList;       // event list
   TString        fParFile;         // parameter file name
   Int_t          fNbFiles;         // number of files to process
   Int_t          fNoEntries;       // number of events/entries to process
   Int_t          fFirstEntry;      // first event/entry to process
   TObject       *fChain;           // dataset on which to process selector
   TQueryResult  *fResult;          // query result received back

public:
   ClassDef(TQueryDescription,1)
};


enum EMenuIdentification {
   kMenuAddToFeedback,
   kMenuShow,
   kMenuRemoveFromFeedback
};

//////////////////////////////////////////////////////////////////////////
// TSessionDescription class : Descrition of Session
//////////////////////////////////////////////////////////////////////////

class TSessionDescription : public TObject {

public:
   TString                    fName;         // session name
   TString                    fAddress;      // server address
   Int_t                      fPort;         // communication port
   TString                    fConfigFile;   // configuration file name
   Int_t                      fLogLevel;     // log (debug) level
   TString                    fUserName;     // user name (on server)
   Bool_t                     fConnected;    // kTRUE if connected
   Bool_t                     fLocal;        // kTRUE if session is local
   Bool_t                     fSync;         // kTRUE if in sync mode
   TList                     *fQueries;      // list of queries in this session
   TQueryDescription         *fActQuery;     // current (actual) query
   TVirtualProof             *fProof;        // pointer on TVirtualProof used by this session
   Int_t                      fNbHistos;     // number of feedback histos

   ClassDef(TSessionDescription,1)
};

//////////////////////////////////////////////////////////////////////////
// TSessionDescription class : Descrition of Session
//////////////////////////////////////////////////////////////////////////

class TPackageDescription : public TObject {

public:
   TString        fName;         // package name
   Int_t          fId;           // package id
   Bool_t         fUploaded;     // package has been uploaded
   Bool_t         fEnabled;      // package has been enabled

   ClassDef(TPackageDescription,1)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionServerFrame                                                  //
// A composite Frame used in the right part of the Session Viewer GUI   //
// for any information relative to server side : address, port, user... //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TSessionServerFrame : public TGCompositeFrame {

private:
   TGCompositeFrame  *fFrmNewServer;   // main group frame
   TGTextEntry       *fTxtName;        // connection name text entry
   TGTextEntry       *fTxtAddress;     // server address text entry
   TGNumberEntry     *fNumPort;        // port number selector
   TGNumberEntry     *fLogLevel;       // log (debug) level selector
   TGTextEntry       *fTxtConfig;      // configuration file text entry
   TGTextEntry       *fTxtUsrName;     // user name text entry
   TGCheckButton     *fSync;           // sync / async flag selector
   TSessionViewer    *fViewer;         // pointer on the main viewer

public:
   TSessionServerFrame(TGWindow *parent, Int_t w, Int_t h);
   virtual ~TSessionServerFrame();

   void        Build(TSessionViewer *gui);

   const char *GetName() const { return fTxtName->GetText(); }
   const char *GetAddress() const { return fTxtAddress->GetText(); }
   Int_t       GetPortNumber() const { return fNumPort->GetIntNumber(); }
   Int_t       GetLogLevel() const { return fLogLevel->GetIntNumber(); }
   const char *GetConfigText() const { return fTxtConfig->GetText(); }
   const char *GetUserName() const { return fTxtUsrName->GetText(); }
   Bool_t      IsSync() const { return (Bool_t)(fSync->GetState() == kButtonDown); }

   void        SetName(const char *str) { fTxtName->SetText(str); }
   void        SetAddress(const char *str) { fTxtAddress->SetText(str); }
   void        SetPortNumber(Int_t port) { fNumPort->SetIntNumber(port); }
   void        SetLogLevel(Int_t log) { fLogLevel->SetIntNumber(log); }
   void        SetConfigText(const char *str) { fTxtConfig->SetText(str); }
   void        SetUserName(const char *str) { fTxtUsrName->SetText(str); }
   void        SetSync(Bool_t sync) { fSync->SetState(sync ? kButtonDown : kButtonUp); }

   void        OnBtnConnectClicked();
   void        OnBtnNewServerClicked();
   void        OnBtnDeleteClicked();
   void        OnBtnAddClicked();
   void        OnConfigFileClicked();
   void        Update(TSessionDescription* desc);
   virtual Bool_t HandleExpose(Event_t *event);
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

   Bool_t      WriteConfigFile(const TString &filePath, TList *vec);
   TList      *ReadConfigFile(const TString &filePath);

   ClassDef(TSessionServerFrame,0)
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionFrame                                                        //
// A composite Frame used in the right part of the Session Viewer GUI   //
// for any information, settings or controls relative to the current    //
// session.                                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TSessionFrame : public TGCompositeFrame {

private:

   TGTab             *fTab;                  // main tab frame
   TGCompositeFrame  *fFA, *fFB, *fFC, *fFD; // four tabs element
   TGTextEntry       *fCommandTxt;           // Command line text entry
   TGTextBuffer      *fCommandBuf;           // Command line text buffer
   TGTextView        *fInfoTextView;         // summary on current query
   TGCheckButton     *fClearCheck;           // clear text view after each command
   TGTextButton      *fBtnDisconnect;        // disconnect button
   TGTextButton      *fBtnShowLog;           // show log button
   TGTextButton      *fBtnNewQuery;          // new query button
   TGTextButton      *fBtnGetQueries;        // get entries button
   // Packages tab related items
   TList             *fPackages;             // list of packages
   TGListBox         *fLBPackages;           // packages listbox
   TGTextButton      *fBtnAdd;               // add package button
   TGTextButton      *fBtnRemove;            // remove package button
   TGTextButton      *fBtnUp;                // move package up button
   TGTextButton      *fBtnDown;              // move package down button
   TGTextButton      *fBtnShow;              // show packages button
   TGTextButton      *fBtnShowEnabled;       // show enabled packages button
   TGCheckButton     *fChkMulti;             // multiple selection check
   TGCheckButton     *fChkEnable;            // enable at session startup check
   TGTextButton      *fBtnUpload;            // upload packages button
   TGTextButton      *fBtnEnable;            // enable packages button
   TGTextButton      *fBtnClear;             // clear all packages button
   TGTextButton      *fBtnDisable;           // disable packages button
   // Options tab related items
   TGTextEntry       *fTxtParallel;          // parallel nodes text entry
   TGNumberEntry     *fLogLevel;             // log level number entry
   TGTextButton      *fApplyLogLevel;        // apply log level button
   TGTextButton      *fApplyParallel;        // apply parallel nodes button

   TSessionViewer    *fViewer;               // pointer on main viewer
   TGLabel           *fInfoLine[13];         // infos on session

public:
   TSessionFrame(TGWindow* parent, Int_t w, Int_t h);
   virtual ~TSessionFrame();

   void     Build(TSessionViewer *gui);
   TGTab   *GetTab() const { return fTab; }

   //Function that handle input from user:
   void     OnApplyLogLevel();
   void     OnApplyParallel();
   void     OnBtnAddClicked();
   void     OnBtnRemoveClicked();
   void     OnBtnUpClicked();
   void     OnBtnDownClicked();
   void     OnBtnShowLogClicked();
   void     OnBtnNewQueryClicked();
   void     OnBtnGetQueriesClicked();
   void     OnBtnDisconnectClicked();
   void     OnCommandLine();
   void     OnUploadPackages();
   void     OnEnablePackages();
   void     OnDisablePackages();
   void     OnClearPackages();
   void     OnMultipleSelection(Bool_t on);
   void     ProofInfos();

   ClassDef(TSessionFrame,0)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionQueryFrame                                                   //
// A composite Frame used in the right part of the Session Viewer GUI   //
// for any information, settings or controls relative to queries.       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TSessionQueryFrame : public TGCompositeFrame {

private:

   enum EQueryStatus { kRunning = 0, kDone, kStopped, kAborted };

   TGTextButton         *fBtnSubmit;         // submit query button
   TGTextButton         *fBtnFinalize;       // finalize query button
   TGTextButton         *fBtnStop;           // stop process button
   TGTextButton         *fBtnAbort;          // abort process button
   TGTextButton         *fBtnShowLog;        // show log button
   TGTextButton         *fBtnRetrieve;       // retrieve query button
   TGTextView           *fInfoTextView;      // summary on current query

   Int_t                fFiles;              // number of files processed
   TTime                fStartTime;          // start time of the process
   TTime                fEndTime;            // end time of the process
   Long64_t             fFirst;              // first event/entry to process
   Long64_t             fEntries;            // number of events/entries to process
   Long64_t             fPrevTotal;          // used for progress bar
   Long64_t             fPrevProcessed;      // used for progress bar
   TGLabel              *fLabInfos;          // infos on current process
   TGLabel              *fLabStatus;         // actual process status
   TGLabel              *fTotal;             // total progress info
   TGLabel              *fRate;              // rate of process in events/sec
   EQueryStatus         fStatus;             // status of actual query
   TGTab                *fTab;               // main tab frame
   TGCompositeFrame     *fFA, *fFB, *fFC;    // three tabs element
   TGHProgressBar       *frmProg;            // current process progress bar
   TRootEmbeddedCanvas  *fECanvas;           // node statistics embeded canvas
   TCanvas              *fStatsCanvas;       // node statistics canvas

   TSessionViewer       *fViewer;            // pointer on main viewer

   TQueryDescription    *fDesc;

public:
   TSessionQueryFrame(TGWindow* parent, Int_t w, Int_t h);
   virtual ~TSessionQueryFrame();

   void     Build(TSessionViewer *gui);

   TCanvas *GetStatsCanvas() const { return fStatsCanvas;}
   TTime    GetStartTime() const { return fStartTime; }
   TTime    GetEndTime() const   { return fEndTime; }

   void     SetStartTime(TTime time) { fStartTime = time; }
   void     SetEndTime(TTime time) { fEndTime = time; }

   void     Feedback(TList *objs);
   void     Progress(Long64_t total, Long64_t processed);
   void     IndicateStop(Bool_t aborted);
   void     ResetProgressDialog(const char *selec, Int_t files, Long64_t first, Long64_t entries);

   //Function that handle input from user:
   void     OnBtnSubmit();
   void     OnBtnFinalize();
   void     OnBtnStop();
   void     OnBtnAbort();
   void     OnBtnShowLog();
   void     OnBtnRetrieve();
   void     UpdateInfos();
   void     UpdateButtons(TQueryDescription *desc);
   void     UpdateHistos(TList *objs);

   ClassDef(TSessionQueryFrame,0)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionOutputFrame                                                  //
// A composite Frame used in the right part of the Session Viewer GUI   //
// displaying output list objects coming from query result.             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TSessionOutputFrame : public TGCompositeFrame {

private:
   TGLVEntry              *fEntryTmp;      // used to transfer to feedback
   TGLVContainer          *fLVContainer;   // output list view
   TSessionViewer         *fViewer;        // pointer on the main viewer

public:
   TSessionOutputFrame(TGWindow* parent, Int_t w, Int_t h);
   virtual ~TSessionOutputFrame();

   void           AddObject(TObject *obj);
   void           Build(TSessionViewer *gui);
   TGLVContainer  *GetLVContainer() { return fLVContainer; }
   void           OnElementClicked(TGLVEntry* entry, Int_t btn, Int_t x, Int_t y);
   void           OnElementDblClicked(TGLVEntry *entry ,Int_t btn, Int_t x, Int_t y);
   void           RemoveAll() { fLVContainer->RemoveAll(); }

   ClassDef(TSessionOutputFrame,0)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionInputFrame                                                   //
// A composite Frame used in the right part of the Session Viewer GUI   //
// displaying input list objects coming from query result.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TSessionInputFrame : public TGCompositeFrame {

private:
   TSessionViewer   *fViewer;       // pointer on the main viewer
   TGLVContainer    *fLVContainer;  // container for the input list view

public:
   TSessionInputFrame(TGWindow* parent, Int_t w, Int_t h);
   virtual ~TSessionInputFrame();

   void           AddObject(TObject *obj);
   void           Build(TSessionViewer *gui);
   void           RemoveAll() { fLVContainer->RemoveAll(); }
   TGLVContainer  *GetLVContainer() { return fLVContainer; }

   ClassDef(TSessionInputFrame,0)
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSessionViewer                                                       //
// This is the main widget, mother of all the previous classes          //
// Used to manage sessions, servers, queries...                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TSessionViewer : public TGMainFrame {

private:
   time_t                  fStart, fElapsed;    // time of connection
   Bool_t                  fChangePic;          // KTRUE if animation active
   Bool_t                  fBusy;               // KTRUE if busy i.e : connecting
   TGHorizontalFrame      *fHf;                 //
   TGVerticalFrame        *fV1;                 //
   TGVerticalFrame        *fV2;                 //
   TSessionServerFrame    *fServerFrame;        // right side server frame
   TSessionFrame          *fSessionFrame;       // right side session frame
   TSessionQueryFrame     *fQueryFrame;         // right side query frame
   TSessionOutputFrame    *fOutputFrame;        // output frame
   TSessionInputFrame     *fInputFrame;         // input frame
   TSessionLogView        *fLogWindow;          // external log window
   TSessionDescription    *fActDesc;            // actual session description
   TList                  *fSessions;           // list of sessions
   const TGPicture        *fLocal;              // local session icon picture
   const TGPicture        *fProofCon;           // connected server icon picture
   const TGPicture        *fProofDiscon;        // disconnected server icon picture
   const TGPicture        *fQueryCon;           // connected(?) query icon picture
   const TGPicture        *fQueryDiscon;        // disconnected(?) query icon picture
   const TGPicture        *fBaseIcon;           // base list tree icon picture

   TGFrame                *fActFrame;           // actual (displayed) frame
   TGToolBar              *fToolBar;            // application tool bar
   TGMenuBar              *fMenuBar;            // application main menu bar
   TGPopupMenu            *fFileMenu;           // file menu entry
   TGPopupMenu            *fSessionMenu;        // session menu entry
   TGPopupMenu            *fQueryMenu;          // query menu entry
   TGPopupMenu            *fOptionsMenu;        // options menu entry
   TGPopupMenu            *fCascadeMenu;        // options menu entry
   TGPopupMenu            *fHelpMenu;           // help menu entry

   TGPopupMenu            *fPopupSrv;           // server related popup menu
   TGPopupMenu            *fPopupQry;           // query related popup menu
   TContextMenu           *fContextMenu;        // input/output objects context menu

   TGHProgressBar         *fConnectProg;        // connection progress bar
   TGCanvas               *fTreeView;           // main right sessions/queries tree view
   TGListTree             *fSessionHierarchy;   // main sessions/queries hierarchy list tree
   TGListTreeItem         *fSessionItem;        // base (main) session list tree item
   TGStatusBar            *fStatusBar;          // bottom status bar
   TGPicture              *fRightIconPicture;   // lower bottom left icon used to show connection status
   TGIcon                 *fRightIcon;          // associated picture
   TTimer                 *fTimer;              // timer used to change icon picture
   UserGroup_t            *fUserGroup;          // user connected to session

public:

   TSessionViewer(const char *title = "ROOT Session Viewer", UInt_t w = 550, UInt_t h = 320);
   TSessionViewer(const char *title, Int_t x, Int_t y, UInt_t w, UInt_t h);
   virtual ~TSessionViewer();
   virtual void Build();
   virtual void BuildSessionHierarchy(TList *vec);
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

   TSessionServerFrame    *GetServerFrame() const { return fServerFrame; }
   TSessionFrame          *GetSessionFrame() const { return fSessionFrame; }
   TSessionQueryFrame     *GetQueryFrame() const { return fQueryFrame; }
   TSessionOutputFrame    *GetOutputFrame() const { return fOutputFrame; }
   TSessionInputFrame     *GetInputFrame() const { return fInputFrame; }
   TSessionDescription    *GetActDesc() const { return fActDesc; }
   TList                  *GetSessions() const { return fSessions; }
   TGListTree             *GetSessionHierarchy() const { return fSessionHierarchy; }
   TGListTreeItem         *GetSessionItem() const { return fSessionItem; }
   const TGPicture        *GetLocalPict() const { return fLocal; }
   const TGPicture        *GetProofConPict() const { return fProofCon; }
   const TGPicture        *GetProofDisconPict() const { return fProofDiscon; }
   const TGPicture        *GetQueryConPict() const { return fQueryCon; }
   const TGPicture        *GetQueryDisconPict() const { return fQueryDiscon; }
   const TGPicture        *GetBasePict() const { return fBaseIcon; }
   TGPopupMenu            *GetPopupSrv() const { return fPopupSrv; }
   TGPopupMenu            *GetPopupQry() const { return fPopupQry; }
   TContextMenu           *GetContextMenu() const { return fContextMenu; }
   TGStatusBar            *GetStatusBar() const { return fStatusBar; }
   TGHProgressBar         *GetConnectProg() const { return fConnectProg; }
   TGPopupMenu            *GetCascadeMenu() const { return fCascadeMenu; }
   TGPopupMenu            *GetOptionsMenu() const { return fOptionsMenu; }

   void     ChangeRightLogo(const char *name);
   void     CleanupSession();
   void     CloseWindow();
   void     DisableTimer();
   void     EditQuery();
   void     EnableTimer();
   Bool_t   HandleTimer(TTimer *);
   Bool_t   IsBusy() const {return fBusy; }
   void     LogMessage(const char *msg, Bool_t all);
   void     MyHandleMenu(Int_t);
   void     OnCascadeMenu();
   void     OnListTreeClicked(TGListTreeItem *entry, Int_t btn, Int_t x, Int_t y);
   void     QueryResultReady(char *query);
   void     DeleteQuery();
   void     SetBusy(Bool_t busy = kTRUE) { fBusy = busy; }
   void     SetChangePic(Bool_t change) { fChangePic = change;}
   void     SetLogWindow(TSessionLogView *log) { fLogWindow = log; }
   void     ShowEnabledPackages();
   void     ShowPackages();
   void     ShowInfo(const char *txt);
   void     ShowLog(const char *queryref);
   void     ShowStatus();
   void     StartupMessage(char *msg, Bool_t stat, Int_t curr, Int_t total);
   void     StartViewer();

   ClassDef(TSessionViewer,0)
};

R__EXTERN TSessionViewer *gSessionViewer;

#endif
