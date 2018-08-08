//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      gui/view/plot_polefig.h
//! @brief     Defines class PlotPolefig
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#ifndef PLOT_POLEFIG_H
#define PLOT_POLEFIG_H

#include "QCustomPlot/qcustomplot.h"
#include "core/calc/peak_info.h"
#include "qcr/widgets/controls.h"

//! Tab in PoleFiguresFrame, to display the pole figure.

class PlotPolefig : public QWidget {
public:
    PlotPolefig();
    void refresh();

    QcrCell<bool> flat {false};
private:
    const PeakInfos* peakInfos_;
    void paintEvent(QPaintEvent*);

    QPointF p(deg alpha, deg beta) const;
    deg alpha(const QPointF&) const;
    deg beta(const QPointF&) const;

    void circle(QPointF c, double r);

    void paintGrid();
    void paintPoints();

    // valid during paintEvent
    QPainter* p_;
    QPointF c_;
    double r_;

    double alphaMax_, avgAlphaMax_;
};

#endif // PLOT_POLEFIG_H
