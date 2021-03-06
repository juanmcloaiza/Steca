//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/loaders/loaders.h
//! @brief     Defines loader functions in namespace load.
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#ifndef LOADERS_H
#define LOADERS_H

#include "core/raw/rawfile.h"

//! Functions loadRawfile and loadComment, and their dependences.

namespace load {

//! load a file; file type will be sensed
Rawfile loadRawfile(const QString& filePath);

QString loadComment(const QFileInfo& info);

} // namespace load

#endif // LOADERS_H
