// ************************************************************************** //
//
//  STeCa2:    StressTexCalculator ver. 2
//
//! @file      out_table.h
//! @brief     A dynamic table for ouput data.
//!
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016
//! @authors   Scientific Computing Group at MLZ Garching
//! @authors   Original version: Christian Randau
//! @authors   Version 2: Antti Soininen, Jan Burle, Rebecca Brydon
//
// ************************************************************************** //

#ifndef OUT_TABLE_H
#define OUT_TABLE_H

#include "gui_helpers.h"
#include "panels/panel.h"
#include "types/core_type_variant.h"
#include <QFileDialog>

namespace gui { namespace io {
//------------------------------------------------------------------------------

class OutTableModel;

class OutTable : public TreeView {
  SUPER(OutTable, TreeView)
public:
  OutTable(TheHub&, uint numDataColumns);
 ~OutTable();

  void setHeaders(str_lst const&);
  void setCmpFuns(core::cmp_vec const&);

  void clear();
  void addRow(core::row_t const&);

  void sortData();

private:
  OutTableModel *model_;
};

//------------------------------------------------------------------------------

class OutTableWidget : public QWidget {
  SUPER(OutTableWidget, QWidget)
public:
  // Must have the right number of headers and comparators
  OutTableWidget(TheHub&, str_lst const &headers, core::cmp_vec const&);

  OutTable &table() const { return *outTable_; }

  void presetAll();
  void presetNone();

private:
  struct ShowColumn {
    str        name;
    QCheckBox *cb;
  };

  typedef QVector<ShowColumn> showcolumn_vec;

private:
  class ShowColumnsWidget : public QWidget {
    SUPER(ShowColumnsWidget, QWidget)
  public:
    ShowColumnsWidget(showcolumn_vec&);
    QGridLayout *grid_;
  };

private:
  OutTable          *outTable_;
  ShowColumnsWidget *showColumnsWidget_;
  showcolumn_vec     showColumns_;
};
//------------------------------------------------------------------------------

class SaveOutputWidget : public QWidget {
  SUPER(SaveOutputWidget, QWidget)
public:
  SaveOutputWidget();

  QAction *browsePath_;
  QLineEdit *dirPath_, *fileName_;
  QAction *saveOutput_;
  QBoxLayout *box_;
  QGridLayout *subGl_;

};

//------------------------------------------------------------------------------
/// a child window that deletes itself

class OutWindow : public QFrame, protected RefHub {
  SUPER(OutWindow, QFrame)
public:
  OutWindow(TheHub&, rcstr title, QWidget*);
  QAction *actClose_, *actCalculate_;

protected:
  virtual void calculate() = 0;  ///< here do the work

protected:
  void setWidgets(panel::BasicPanel*, QWidget*);

  QBoxLayout        *box_;
  panel::BasicPanel *panel_;
};

//------------------------------------------------------------------------------
}}
#endif  // OUT_TABLE_H
