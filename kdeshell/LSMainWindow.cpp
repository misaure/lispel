#include <qmessagebox.h>
#include <qkeycode.h>
#include <kapp.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <kfiledialog.h>
#include "LSMainWindow.hh"
//#include "nnlib/defs.h"

LSMainWindow::LSMainWindow()
  : KTMainWindow(), unsavedChanges( false), filePopup( 0)
{
  // "File" menu
  filePopup = new QPopupMenu();
  filePopup->insertItem( i18n( "&New"));
  filePopup->insertItem( i18n( "&Open..."),
			 this, SLOT( fileOpen()), CTRL+Key_O);
  filePopup->insertSeparator();
  filePopup->insertItem( i18n( "&Save"));
  filePopup->insertItem( i18n( "Save &As..."));
  filePopup->insertSeparator();
  filePopup->insertItem( i18n( "E&xit"),
			 this, SLOT( fileQuit()), CTRL+Key_Q);

  // insert menus into menu bar
  menuBar()->insertItem( i18n( "&File"), filePopup);

  // Toolbar
  toolBar()->insertButton( Icon( "exit.xpm"), 0);
  toolBar()->insertSeparator();
  toolBar()->insertButton( Icon( "filenew2.xpm"), 1);
  toolBar()->insertButton( Icon( "fileopen.xpm"), 2);
  toolBar()->insertButton( Icon( "filefloppy.xpm"), 3);
  toolBar()->insertSeparator();
}

LSMainWindow::~LSMainWindow()
{
   if (0 != filePopup) {
      delete filePopup;
      filePopup = 0;
   }
}

bool LSMainWindow::queryClose()
{
   if (unsavedChanges)
      return QMessageBox::information( this,
	       i18n( "NeuroKit: Unsaved Changes"),
	       i18n( "There are unsaved changes!\nQuit anyway?"),
	       i18n( "Yes"), i18n( "No"), 0, 1) == 0;
   else
      return true;
}

void LSMainWindow::fileQuit()
{
   close();
}

void
LSMainWindow::fileOpen()
{
  QString fileName = KFileDialog::getOpenFileName();
}
