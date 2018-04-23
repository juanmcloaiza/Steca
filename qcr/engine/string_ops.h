//  ***********************************************************************************************
//
//  libqcr: capture and replay Qt widget actions
//
//! @file      qcr/engine/string_ops.h
//! @brief     Defines namespace strOp with various string operations
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018-
//! @author    Joachim Wuttke
//
//  ***********************************************************************************************

#ifndef CONVERT_H
#define CONVERT_H

#include <QtGlobal>

//! String operations

namespace strOp {

bool to_b(const QString&);
int to_i(const QString&);
double to_d(const QString&);

QString to_s(bool);
QString to_s(int);
QString to_s(double);
QString to_s(QString);

void splitOnce(const QString&, QString&, QString&);

} // namespace strOp

#endif // CONVERT_H
