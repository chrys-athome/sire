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

#include "sysid.h"
#include "sysidx.h"
#include "sysname.h"

using namespace SireSystem;

///////
/////// Implementation of SysID
///////

SysID::SysID() : SireID::ID()
{}

SysID::SysID(const SysID &other) : SireID::ID(other)
{}

SysID::~SysID()
{}

///////
/////// Implementation of SysIdx
///////

SysIdx::SysIdx() : SireID::Index_T_<SysIdx>(), SysID()
{}

SysIdx::SysIdx(qint32 idx) : SireID::Index_T_<SysIdx>(idx), SysID()
{}

SysIdx::SysIdx(const SysIdx &other) : SireID::Index_T_<SysIdx>(other), SysID(other)
{}

SysIdx::~SysIdx()
{}

///////
/////// Implementation of SysName
///////

SysName::SysName() : SireID::Name(), SysID()
{}

SysName::SysName(const QString &name) : SireID::Name(name), SysID()
{}

SysName::SysName(const SysName &other) : SireID::Name(other), SysID(other)
{}

SysName::~SysName()
{}
