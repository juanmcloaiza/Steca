/** \file
 */

#ifndef DIFFRACTOGRAM_H
#define DIFFRACTOGRAM_H

#include "panel.h"
#include "core_curve.h"
#include "core_dataset.h"
#include "core_reflection.h"
#include "core_types.h"
#include "core_fit_methods.h"
#include "QCP/qcustomplot.h"

namespace panel {
//------------------------------------------------------------------------------

class DiffractogramPlot;

class DiffractogramPlotOverlay: public QWidget {
  SUPER(DiffractogramPlotOverlay,QWidget)
public:
  DiffractogramPlotOverlay(DiffractogramPlot&);

  void setMargins(int left,int right);

private:
  DiffractogramPlot& plot;
  QColor addColor, remColor, color;
  int marginLeft, marginRight;

protected:
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);

  void paintEvent(QPaintEvent*);

  bool hasCursor, mouseDown;
  int  cursorPos, mouseDownPos;

  void updateCursorRegion();
};

class Diffractogram;

class DiffractogramPlot: public QCustomPlot {
  SUPER(DiffractogramPlot,QCustomPlot)
public:
  enum Tool {
    TOOL_NONE,
    TOOL_BACKGROUND,
    TOOL_PEAK_REGION,
  };

  DiffractogramPlot(TheHub&,Diffractogram&);

public:
  void setTool(Tool);
  Tool getTool() const { return tool; }

  void plot(core::Curve const&,core::Curve const&, core::Curve const&, core::Curves const&, uint);

  core::Range fromPixels(int,int);

  void clearBg();
  void addBg(core::Range const&);
  void remBg(core::Range const&);
  void setNewReflRange(core::Range const&);
  void updateBg();

  void clearReflLayer();

protected:
  void addBgItem(core::Range const&);
  void resizeEvent(QResizeEvent*);

private:
  TheHub        &theHub;
  Diffractogram &diffractogram;
  Tool tool;
  QCPGraph *bgGraph, *dgramGraph, *dgramBgFittedGraph, *guesses, *fits;
  QVector<QCPGraph*> reflGraph;
  DiffractogramPlotOverlay *overlay;
};

class Diffractogram: public BoxPanel {
  SUPER(Diffractogram,BoxPanel)
  friend class DiffractogramPlot; // TODO remove
public:
  Diffractogram(TheHub&);

private:
  void setDataset(core::shp_Dataset);
  void renderDataset();

  core::shp_Dataset dataset;

  DiffractogramPlot *plot;

  core::Curve  dgram, dgramBgFitted, bg;
  core::Curves refls;
  core::fit::Polynomial bgPolynomial;
  bool showBgFit;

  uint currReflIndex;
  core::shp_Reflection currentReflection;

public:
  void calcDgram();
  void calcBackground();
  void calcReflections();

  void setCurrReflNewRange(core::Range const&);
  core::Range currReflRange() const;
};

//------------------------------------------------------------------------------
}
#endif
