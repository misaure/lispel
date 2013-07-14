//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ShellForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SynEdit"
#pragma link "SynMemo"
#pragma resource "*.dfm"
TfrmLispelShell *frmLispelShell;
//---------------------------------------------------------------------------
__fastcall TfrmLispelShell::TfrmLispelShell(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLispelShell::miQuitClick(TObject *Sender)
{
   Close();    
}
//---------------------------------------------------------------------------
