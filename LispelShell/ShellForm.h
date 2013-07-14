//---------------------------------------------------------------------------
#ifndef ShellFormH
#define ShellFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "SynEdit.h"
#include "SynMemo.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmLispelShell : public TForm
{
__published:	// Komponenten, die von der IDE verwaltet werden
    TMainMenu *MainMenu;
    TMenuItem *mmFile;
    TMenuItem *miQuit;
    TStatusBar *StatusBar;
    TSynMemo *SourceEdit;
    TSplitter *Splitter1;
    TMemo *OutputWin;
    TMenuItem *mmEdit;
    TMenuItem *miCut;
    TMenuItem *miCopy;
    TMenuItem *miPaste;
    TMenuItem *N1;
    TMenuItem *miProtocol;
    TMenuItem *mmHelp;
    TMenuItem *miInfo;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    void __fastcall miQuitClick(TObject *Sender);
private:	// Benutzerdeklarationen
public:		// Benutzerdeklarationen
    __fastcall TfrmLispelShell(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLispelShell *frmLispelShell;
//---------------------------------------------------------------------------
#endif
