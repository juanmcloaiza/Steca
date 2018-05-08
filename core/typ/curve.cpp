//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/typ/curve.cpp
//! @brief     Implements class Curve
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "curve.h"
#include "qcr/engine/debug.h"
#include "core/def/idiomatic_for.h"

void Curve::clear()
{
    xs_.clear();
    ys_.clear();
    rgeX_.invalidate();
    rgeY_.invalidate();
}

bool Curve::isEmpty() const
{
    return xs_.empty();
}

int Curve::count() const
{
    ASSERT(xs_.size() == ys_.size());
    return xs_.size();
}

bool Curve::isOrdered() const
{
    return std::is_sorted(xs_.cbegin(), xs_.cend());
}

void Curve::append(double x, double y)
{
    xs_.push_back(x);
    ys_.push_back(y);
    rgeX_.extendBy(x);
    rgeY_.extendBy(y);
}

Curve Curve::intersect(const Range& range) const
{
    if (range.isEmpty())
        return {};
    ASSERT(isOrdered());
    Curve ret;
    int xi = 0;
    const int cnt = count();
    while (xi < cnt && xs_.at(xi) < range.min)
        ++xi;
    while (xi < cnt && xs_.at(xi) <= range.max) {
        ret.append(xs_.at(xi), ys_.at(xi));
        ++xi;
    }
    return ret;
}

//! collect points that are in ranges

//! it works because both curve points and ranges are ordered and ranges are non-overlapping

Curve Curve::intersect(const Ranges& ranges) const
{
    Curve ret;
    ASSERT(isOrdered());
    int xi = 0, cnt = count();
    for_i (ranges.count()) {
        const Range& range = ranges.at(i);
        while (xi < cnt && xs_.at(xi) < range.min)
            ++xi;
        while (xi < cnt && xs_.at(xi) <= range.max) {
            ret.append(xs_.at(xi), ys_.at(xi));
            ++xi;
        }
    }
    return ret;
}

//! Subtracts a background that is given as a funtion y(x).
void Curve::subtract(const std::function<double(double)>& func)
{
    for_i (count())
        ys_[i] -= func(xs_.at(i));
}

int Curve::maqpairindex() const
{
    if (isEmpty())
        return 0;
    double yMax = ys_.front();
    int ret = 0;
    for_i (count()) {
        const double y = ys_.at(i);
        if (y > yMax) {
            yMax = y;
            ret = i;
        }
    }
    return ret;
}

double Curve::sumY() const
{
    double ret = 0;
    for_i (count())
        ret += ys_.at(i);
    return ret;
}