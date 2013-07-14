/*
 * NNelmos.cpp ---
 *
 * RCSID: $Id: LispelShell.cpp,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 * (c) 2001 by Michael Saure
 */

#include <kapp.h>
//#include <kmenubar.h>
#include <qmultilinedit.h>
#include <qkeycode.h>
#include "LSMainWindow.hh"

int main( int argc, char *argv[])
{
  KApplication app( argc, argv);
  LSMainWindow *top = new LSMainWindow();

  QMultiLineEdit *dlg = new QMultiLineEdit( top);
  top->setView( dlg);

  const char *aboutText = 
    i18n( "LispelShell\nA development environment for Lispel\n"
	  "Version 0.0.1\n"
	  "(c) 2001 by Michael Saure\n<MichaelSaure@web.de>");

  //FIXME: this should happen in LSMainWindow
  QPopupMenu *helpPopup = app.getHelpMenu( TRUE, aboutText);
  top->menuBar()->insertSeparator();
  top->menuBar()->insertItem( i18n( "&Help"), helpPopup);

  top->show();

  int result = app.exec();
  if (!KTMainWindow::memberList->isEmpty())
    delete top;

  return result;
}
