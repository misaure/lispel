/*
 * LSMainWindow.hh ---
 *
 *  Main widget of a simple shell for running Lispel script in a KDE2 GUI.
 *
 * RCSID: $Id: LSMainWindow.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _neurokit_NKMAINWINDOW_HH_
#define _neurokit_NKMAINWINDOW_HH_ 1

#include <qpopupmenu.h>
#include <ktmainwindow.h>

class LSMainWindow : public KTMainWindow {
  Q_OBJECT

public:
  LSMainWindow();
  virtual ~LSMainWindow();

protected:
  virtual bool queryClose();

private:
  bool unsavedChanges;
  QPopupMenu *filePopup;

private slots:
  void fileQuit();
  void fileOpen();
}; /*NKMainWindow*/

#endif /*_neurokit_NKMAINWNDOW_HH_*/

