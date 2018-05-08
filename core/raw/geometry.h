//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/raw/geometry.h
//! @brief     Defines classes Geometry, ImageCut, ScatterDirection, ImageKey
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "core/typ/angles.h"
#include "core/typ/size2d.h"
#include "core/typ/ij.h"
#include "core/typ/range.h"

//! Detector geometry.
class Geometry {
public:
    static double const DEF_DETECTOR_DISTANCE;
    static double const DEF_DETECTOR_PIXEL_SIZE;

    Geometry();
    COMPARABLE(const Geometry&);

    void fromSettings();
    void fromJson(const JsonObj& obj);

    void setDetectorDistance(double);
    void setPixSize(double);
    void setOffset(const IJ& midPixOffset);

    double detectorDistance() const { return detectorDistance_; }
    double pixSize() const { return pixSize_; }
    IJ& midPixOffset() { return midPixOffset_; }
    const IJ& midPixOffset() const { return midPixOffset_; }
    void toSettings() const;
    QJsonObject toJson() const;

private:
    double detectorDistance_; // the distance from the sample to the detector
    double pixSize_; // size of the detector pixel
    IJ midPixOffset_;
};


//! Image cut (margins)
class ImageCut {
public:
    ImageCut() = default;
    ImageCut(int left, int top, int right, int bottom);
    COMPARABLE(const ImageCut&);

    void clear();
    void fromJson(const JsonObj& obj);
    void setLeft(int);
    void setRight(int);
    void setTop(int);
    void setBottom(int);
    void setLinked(bool);

    int left() const { return left_; }
    int right() const { return right_; }
    int top() const { return top_; }
    int bottom() const { return bottom_; }
    int linked() const { return linked_; }

    size2d marginSize() const;
    QJsonObject toJson() const;

private:
    void confine(int& m1, int& m2, int maxTogether);
    void setAll(int);

    int left_ {0}, top_ {0}, right_ {0}, bottom_ {0};
    bool linked_ { false };
};


//! A pair of angles (gamma, 2theta) that designate a scattering direction.
class ScatterDirection {
public:
    ScatterDirection();
    ScatterDirection(deg, deg);

    deg tth;
    deg gma;
};


//! Holds geometry parameters that define a mapping of image coordinates onto (gamma,2theta).

//! Needed for caching such coordinate maps.
class ImageKey {
public:
    ImageKey(deg midTth);

    COMPARABLE(const ImageKey&);
//    bool operator<(const ImageKey& that) const { return compare(that) < 0; }

    const Geometry geometry;
    const size2d size;
    const ImageCut cut;
    const IJ midPix;
    const deg midTth;
};

#endif // GEOMETRY_H