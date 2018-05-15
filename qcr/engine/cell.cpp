//  ***********************************************************************************************
//
//  libqcr: capture and replay Qt widget actions
//
//! @file      qcr/engine/cell.cpp
//! @brief     Implements class Cell
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018-
//! @author    Joachim Wuttke
//
//  ***********************************************************************************************

#include "cell.h"
#include "qcr/engine/mixin.h"
#include <QtDebug>

void remakeAll()
{
    ASSERT(gRoot);
    gRoot->fullRemake();
}


Cell::stamp_t ValueCell::latestTimestamp__ = 0;

void Cell::addSource(Cell* src) {
    if (sources_.find(src)!=sources_.end())
        qFatal("duplicate cell source entry");
    sources_.insert(src);
}

void Cell::rmSource(Cell* src) {
    auto it = sources_.find(src);
    if (it==sources_.end())
        qFatal("failed removing cell source entry");
    sources_.erase(it);
}

void Cell::clearSources() {
    sources_.clear();
}

Cell::stamp_t Cell::update()
{
    //stamp_t t_new = timestamp_;
    qDebug() << "Updating " << name() << " t=" << timestamp_;
    recompute();
    /*
    for (Cell* src: sources_)
        t_new = std::max(t_new, src->update());
    if (t_new>timestamp_) {
        recompute();
        timestamp_ = t_new;
    }
    */
    qDebug() << " Updated " << name() << " t=" << timestamp_;
    return timestamp_;
}

void Cell::connectAction(std::function<void()>&& f)
{
    actionsOnChange_.push_back(std::move(f));
}

void Cell::actOnChange()
{
    for (const std::function<void()>& f: actionsOnChange_)
        f();
}
