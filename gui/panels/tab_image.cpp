// ************************************************************************** //
//
//  Steca: stress and texture calculator
//
//! @file      gui/panels/tab_image.cpp
//! @brief     Implements classes DataImageTab, CorrImageTab, and their dependences
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
// ************************************************************************** //

#include "tab_image.h"
#include "core/session.h"
#include "gui/cfg/colors.h"
#include "gui/actions/toggles.h"
#include "gui/actions/triggers.h"
#include <qmath.h>
#include <iostream>

// ************************************************************************** //
//  class ImageWidget
// ************************************************************************** //

ImageWidget::ImageWidget()
    : scale_(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(&gGui->toggles->showOverlay, &QAction::toggled, [this](bool /*unused*/) { update(); });
}

void ImageWidget::setPixmap(const QPixmap& pixmap)
{
    original_ = pixmap;
    setScale();
}

void ImageWidget::setScale()
{
    if (original_.isNull()) {
        scale_ = 0;
    } else {
        const QSize& sz = size();
        const QSize& os = original_.size();
        scale_ = qMin(qreal(sz.width() - 2) / os.width(), qreal(sz.height() - 2) / os.height());
    }

    if (scale_ <= 0)
        scaled_ = QPixmap();
    else
        scaled_ = original_.scaled(original_.size() * scale_);

    update();
}

void ImageWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    setScale();
}

void ImageWidget::paintEvent(QPaintEvent*)
{
    // paint centered
    const QSize margin = (size() - scaled_.size()) / 2;
    const QRect rect(QPoint(margin.width(), margin.height()), scaled_.size());

    QPainter p(this);

    // image
    p.drawPixmap(rect.left(), rect.top(), scaled_);

    // overlay
    if (gGui->toggles->showOverlay.isChecked()) {
        p.setPen(Qt::lightGray);

        // cut
        const ImageCut& cut = gSession->imageCut();
        const QRect r = rect.adjusted(-1, -1, 0, 0)
                      .adjusted(
                          qRound(scale_ * cut.left()), qRound(scale_ * cut.top()),
                          -qRound(scale_ * cut.right()), -qRound(scale_ * cut.bottom()));
        p.drawRect(r);

        const QPoint rc = r.center();
        const int rcx = rc.x(), rcy = rc.y();

        int rl, rt, rr, rb;
        r.getCoords(&rl, &rt, &rr, &rb);
        const int rw = rr - rl;

        // cross
        const IJ& off = gSession->geometry().midPixOffset();
        const int x = qRound(rcx + scale_ * off.i);
        const int y = qRound(rcy + scale_ * off.j);
        p.drawLine(x, rt, x, rb);
        p.drawLine(rl, y, rr, y);

        // text
        QPoint pos(rr - rw / 5, rcy);
        p.setPen(Qt::cyan);
        p.drawText(pos, "γ=0");
    }

    // frame
    p.setPen(Qt::black);
    p.drawRect(rect.adjusted(-1, -1, 0, 0));
}

// ************************************************************************** //
//  class IdxMeas
// ************************************************************************** //

IdxMeas::IdxMeas()
    : CSpinBox {"idxMeas", 4, false, 1, INT_MAX,
        "Number of measurement within the current group of measurements"}
{
    connect(gSession, &Session::sigDataHighlight, this, &IdxMeas::fromCore);
    connect(this, _SLOT_(CSpinBox, valueReleased, int), [](int val) {
            gSession->dataset().highlight().setMeasurement(val-1); });
    fromCore();
}

void IdxMeas::fromCore()
{
    auto& hl = gSession->dataset().highlight();
    if (!hl.cluster()) {
        setEnabled(false);
        setValue(1);
        return;
    }
    setEnabled( gSession->dataset().binning() > 1);
    int max = hl.cluster()->count();
    setMaximum(max);
    if ( hl.measurementIndex()+1>max )
        hl.setMeasurement(max-1);
    setValue(hl.measurementIndex()+1);
}

// ************************************************************************** //
//  base class ImageTab
// ************************************************************************** //

ImageTab::ImageTab()
    : btnScale_ {&gGui->toggles->fixedIntenImage}
    , btnOverlay_ {&gGui->toggles->showOverlay}
{
    // internal connections
    connect(&gGui->toggles->enableCorr, &QAction::toggled, [this](bool /*unused*/) { render(); });
    connect(&gGui->toggles->showBins, &QAction::toggled, [this](bool /*unused*/) { render(); });

    // layout
    box1_.addWidget(&btnScale_, Qt::AlignLeft);
    box1_.addWidget(&btnOverlay_, Qt::AlignLeft);
    controls_.addLayout(&box1_);

    box_.addLayout(&controls_);
    box_.addWidget(&imageView_);
    setLayout(&box_);
}

void ImageTab::render()
{
    gSession->corrset().clearIntens(); // TODO move this to more appriate place
    imageView_.setPixmap(pixmap());
}

QPixmap ImageTab::makePixmap(shp_Image image)
{
    QImage im = makeImage(image);
    return QPixmap::fromImage(im);
}

QPixmap ImageTab::makeOverlayPixmap(const Measurement& cluster)
{
    QImage im = makeImage(cluster.image());
    shp_AngleMap angleMap = gSession->angleMap(cluster);
    const Range& rgeGma = gSession->gammaSelection().range();
    const Range& rgeTth = gSession->thetaSelection().range();
    const QSize& size = im.size();
    for_ij (size.width(), size.height()) {
        const ScatterDirection& a = angleMap->at(i, j);
        QColor color = im.pixel(i, j);
        if (rgeGma.contains(a.gma)) {
            if (rgeTth.contains(a.tth))
                color = Qt::yellow;
            else
                color.setGreen(qFloor(color.green() * .3 + 255 * .7));
        } else if (rgeTth.contains(a.tth)) {
            color.setGreen(qFloor(color.green() * .3 + 255 * .7));
        }
        im.setPixel(i, j, color.rgb());
    }
    return QPixmap::fromImage(im);
}

QPixmap ImageTab::makeBlankPixmap()
{
    const size2d size = gSession->imageSize();
    QPixmap pixmap(size.w, size.h);
    pixmap.fill(QColor(0, 0, 0, 0));
    return pixmap;
}

QImage ImageTab::makeImage(shp_Image image)
{
    if (!image)
        return {};

    ImageLens imageLens(*image, true, false);
    const size2d size = imageLens.imgSize();
    if (size.isEmpty())
        return {};

    QImage ret(QSize(size.w, size.h), QImage::Format_RGB32);

    bool fixedScale = gGui->toggles->fixedIntenImage.isChecked();
    const Range rgeInten = imageLens.rgeInten(fixedScale);
    inten_t maxInten = inten_t(rgeInten.max);

    for_ij (size.w, size.h)
        ret.setPixel(i, j, colormap::intenImage(imageLens.imageInten(i, j), maxInten, !fixedScale));
    return ret;
}

// ************************************************************************** //
//  class DataImageTab
// ************************************************************************** //

DataImageTab::DataImageTab()
    : btnShowBins_ {&gGui->toggles->showBins}
{
    // inbound connection
    connect(gSession, &Session::sigGamma, [this]() {
            idxSlice_.setValue(gSession->gammaSelection().idxSlice()+1);
            const Measurement* measurement = gSession->dataset().highlight().measurement();
            gammaRangeTotal_.setText(measurement->rgeGmaFull().to_s()+" deg");
            gammaRangeSlice_.setText(gSession->gammaSelection().range().to_s()+" deg");
            thetaRangeTotal_.setText(measurement->rgeTth().to_s()+" deg");
            emit gSession->sigImage(); });
    connect(gSession, &Session::sigTheta, [this]() {
            idxTheta_.setValue(gSession->thetaSelection().iSlice()+1);
            emit gSession->sigImage(); });

    // outbound connections and control widget setup
    connect(&idxTheta_, _SLOT_(CSpinBox, valueReleased, int), [](int val) {
            gSession->thetaSelection().selectSlice(val-1); });
    connect(&idxSlice_, _SLOT_(CSpinBox, valueReleased, int), [](int val) {
            gSession->gammaSelection().selectSlice(val-1); });

    // layout
    box1_.addWidget(&btnShowBins_, Qt::AlignLeft);

    boxIdx_.addWidget(new QLabel("image #"), 0, 0, Qt::AlignLeft);
    boxIdx_.addWidget(&idxMeas_, 0, 1, Qt::AlignLeft);
    boxIdx_.addWidget(new QLabel("ϑ bin #"), 1, 0, Qt::AlignLeft);
    boxIdx_.addWidget(&idxTheta_, 1, 1, Qt::AlignLeft);
    boxIdx_.addWidget(new QLabel("γ slice #"), 2, 0, Qt::AlignLeft);
    boxIdx_.addWidget(&idxSlice_, 2, 1, Qt::AlignLeft);
    controls_.addStretch(100);
    controls_.addLayout(&boxIdx_);

    controls_.addStretch(1000);
    boxRanges_.addWidget(new QLabel("γ total:"), 0, 0, Qt::AlignLeft);
    boxRanges_.addWidget(new QLabel("γ slice:"), 1, 0, Qt::AlignLeft);
    boxRanges_.addWidget(new QLabel("ϑ total:" ), 2, 0, Qt::AlignLeft);
    boxRanges_.addWidget(new QLabel("ϑ bin:"   ), 3, 0, Qt::AlignLeft);
    boxRanges_.addWidget(&gammaRangeTotal_, 0, 1, Qt::AlignLeft);
    boxRanges_.addWidget(&gammaRangeSlice_, 1, 1, Qt::AlignLeft);
    boxRanges_.addWidget(&thetaRangeTotal_, 2, 1, Qt::AlignLeft);
    boxRanges_.addWidget(&thetaRangeBin_,   3, 1, Qt::AlignLeft);
    controls_.addLayout(&boxRanges_, Qt::AlignLeft|Qt::AlignBottom);
}

DataImageTab::~DataImageTab()
{
    box1_.removeWidget(&btnShowBins_);
    controls_.removeItem(&boxIdx_);
    controls_.removeItem(&boxRanges_);
}

QPixmap DataImageTab::pixmap()
{
    const Measurement* measurement = gSession->dataset().highlight().measurement();
    if (!measurement)
        return makeBlankPixmap();
    if (gGui->toggles->showBins.isChecked())
        return makeOverlayPixmap(*measurement);
    return makePixmap(measurement->image());
}

// ************************************************************************** //
//  class CorrImageTab
// ************************************************************************** //

CorrImageTab::CorrImageTab()
{
    controls_.addStretch(1);
}

QPixmap CorrImageTab::pixmap()
{
    return makePixmap(gSession->corrset().image());
}
