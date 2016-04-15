// ************************************************************************** //
//
//  STeCa2:    StressTexCalculator ver. 2
//
//! @file      mainwin.h
//! @brief     The main window
//!
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016
//! @authors   Scientific Computing Group at MLZ Garching
//! @authors   Original version: Christian Randau
//! @authors   Version 2: Antti Soininen, Jan Burle, Rebecca Brydon
//
// ************************************************************************** //

#ifndef MAINWIN_H
#define MAINWIN_H

#include "core_defs.h"
#include "thehub.h"
#include <QMainWindow>

namespace gui {
//------------------------------------------------------------------------------

class MainWin : public QMainWindow {
  SUPER(MainWin, QMainWindow)
  Q_OBJECT
public:
  MainWin();
 ~MainWin();

private:
  void initMenus();
  void initLayout();
  void initStatus();
  void connectActions();

public:
  void about();

  void show();
  void close();

  void addFiles();
  void loadCorrFile();

  void loadSession();
  void saveSession();

  void outputPoleFigures();

private:
  // the hub
  gui::TheHub theHub;
  Actions &actions;

private:
  void closeEvent(QCloseEvent*);

  void onShow();
  void onClose();

private:
  QMenu *menuFile_, *menuEdit_, *menuView_, *menuDatasets_, *menuReflect_,
        *menuOutput_, *menuHelp_;

  QDockWidget *dockFiles_, *dockDatasets_, *dockDatasetInfo_;

private:
  QByteArray initialState_;

  void readSettings();
  void saveSettings();

  void checkActions();

  void viewStatusbar(bool);
  void viewFullScreen(bool);
  void viewDockFiles(bool);
  void viewDockDatasets(bool);
  void viewDockDatasetInfo(bool);
  void viewReset();
};

//------------------------------------------------------------------------------
}
#endif // MAINWIN_H
