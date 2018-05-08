//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      gui/view/plot_polefig.cpp
//! @brief     Implements class PlotPolefig
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "plot_polefig.h"
#include "core/session.h"
#include "gui/mainwin.h"
#include "gui/state.h"

namespace {

//! Color map for polefigure: shades of blue.
QColor intenGraph(float inten, float maxInten) {
    if (!qIsFinite(inten) || qIsNaN(maxInten) || maxInten <= 0)
        return { qRgb(0x00, 0x00, 0x00) };

    inten /= maxInten;

    return { qRgb(0, 0, int(0xff * (1 - inten / 3))) };
}

} //namespace


PlotPolefig::PlotPolefig()
    : flat_(false)
    , alphaMax_(90)
    , avgAlphaMax_(0)

{
    update();
}

void PlotPolefig::refresh()
{
    rs_ = gSession->peakInfos();
    flat_ = gGui->state->polefigShowGridPts->checkState();
    update();
}

void PlotPolefig::update()
{
    avgAlphaMax_ = gSession->interpol().avgAlphaMax();
    QWidget::update();
}

void PlotPolefig::paintEvent(QPaintEvent*)
{
    int w = size().width(), h = size().height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(w / 2, h / 2);

    p_ = &painter;
    c_ = QPointF(0, 0);
    r_ = qMin(w, h) / 2;

    paintGrid();
    paintPoints();
}

//! Point in floating-point precision
QPointF PlotPolefig::p(deg alpha, deg beta) const
{
    double r = r_ * alpha / alphaMax_;
    rad betaRad = beta.toRad();
    return QPointF(r * cos(betaRad), -r * sin(betaRad));
}

deg PlotPolefig::alpha(const QPointF& p) const
{
    return sqrt(p.x() * p.x() + p.y() * p.y()) / r_ * alphaMax_;
}

deg PlotPolefig::beta(const QPointF& p) const
{
    deg b = rad(atan2(p.y(), p.x())).toDeg();
    return b <= 0 ? -b : 360 - b;
}

void PlotPolefig::circle(QPointF c, double r)
{
    p_->drawEllipse(c, r, r);
}

void PlotPolefig::paintGrid()
{
    QPen penMajor(Qt::gray), penMinor(Qt::lightGray);

    for (int alpha = 10; alpha <= 90; alpha += 10) {
        double r = r_ / alphaMax_ * alpha;
        p_->setPen(!(alpha % 30) ? penMajor : penMinor);
        circle(c_, r);
    }

    for (int beta = 0; beta < 360; beta += 10) {
        p_->setPen(!(beta % 30) ? penMajor : penMinor);
        p_->drawLine(p(10, beta), p(90, beta));
    }

    QPen penMark(Qt::darkGreen);
    p_->setPen(penMark);
    circle(c_, r_ * avgAlphaMax_ / alphaMax_);
}

void PlotPolefig::paintPoints()
{
    double rgeMax = rs_.rgeInten().max;

    /*
[Jan:] As I read the code: the body of the 'for' loop (for all points) is guarded by
'if (qIsFinite(inten))'. NaNs are not finite, so they do not get painted.

Inside the outer 'if' (for finite inten) is 'if (flat_) ... else' where the 'then'
branch paints all points blue and the same size (.5), and the 'else' branch paints
them in various colours and size according to intensity.

The 'flat_' flag is controlled by the check box that is in the corner of the pole graph.

NaNs (intensities) do not occur in computed points, only in interpolated points,
when interpolation fails.
    */
    for (const PeakInfo& r : rs_.peaks()) {
        double inten = r.inten();
        if (!qIsFinite(inten)) // nan comes from interpolation
            continue;
        const QPointF& pp = p(r.alpha(), r.beta());
        if (flat_) {
            QColor color(Qt::blue);
            p_->setPen(color);
            p_->setBrush(color);
            circle(pp, .5);
        } else {
            inten /= rgeMax;
            QColor color = intenGraph(inten, 1);
            p_->setPen(color);
            p_->setBrush(color);
            circle(pp, inten * r_ / 60); // TODO scale to max inten
        }
    }
}