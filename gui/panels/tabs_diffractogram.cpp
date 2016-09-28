// ************************************************************************** //
//
//  STeCa2:    StressTextureCalculator ver. 2
//
//! @file      tabs_diffractogram.cpp
//!
//! @homepage  http://apps.jcns.fz-juelich.de/steca2
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016
//! @authors   Scientific Computing Group at MLZ Garching
//! @authors   Rebecca Brydon, Jan Burle,  Antti Soininen
//! @authors   Based on the original STeCa by Christian Randau
//
// ************************************************************************** //

#include "tabs_diffractogram.h"
#include "panel_diffractogram.h"

namespace gui { namespace panel {
//------------------------------------------------------------------------------

TabsDiffractogram::TabsDiffractogram(TheHub& hub) : super(hub) {
  auto& box = addTab("Diffractogram").box();
  box.addWidget(new Diffractogram(hub));

  str_lst options = normStrLst();
  comboNormType_ = comboBox(options);
  box.addWidget(comboNormType_);

//  connect(comboNormType_, slot(QComboBox,currentIndexChanged,int), [this](int index) {
//    hub_.setNorm(eNorm(index));
//  });
}

//------------------------------------------------------------------------------
}}
// eof
