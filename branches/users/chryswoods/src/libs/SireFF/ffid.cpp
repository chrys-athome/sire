/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "ffid.h"
#include "ffidx.h"
#include "ffname.h"

using namespace SireFF;

///////
/////// Implementation of FFID
///////

FFID::FFID() : SireID::ID()
{}

FFID::FFID(const FFID &other) : SireID::ID(other)
{}

FFID::~FFID()
{}

///////
/////// Implementation of FFIdx
///////

FFIdx::FFIdx() : SireID::Index_T_<FFIdx>(), FFID()
{}

FFIdx::FFIdx(qint32 idx) : SireID::Index_T_<FFIdx>(idx), FFID()
{}

FFIdx::FFIdx(const FFIdx &other) : SireID::Index_T_<FFIdx>(other), FFID(other)
{}

FFIdx::~FFIdx()
{}

///////
/////// Implementation of FFName
///////

FFName::FFName() : SireID::Name(), FFID()
{}

FFName::FFName(const QString &name) : SireID::Name(name), FFID()
{}

FFName::FFName(const FFName &other) : SireID::Name(other), FFID(other)
{}

FFName::~FFName()
{}
