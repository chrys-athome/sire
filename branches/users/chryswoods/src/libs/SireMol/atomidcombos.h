/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOL_ATOMIDCOMBOS_H
#define SIREMOL_ATOMIDCOMBOS_H

#include "atomidcombo.hpp"

#include "atomidx.h"
#include "atomnum.h"
#include "atomname.h"

#include "cgidx.h"
#include "cgnum.h"
#include "cgname.h"

#include "residx.h"
#include "resnum.h"
#include "resname.h"

#include "segidx.h"
#include "segnum.h"
#include "segname.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** Define all of the CutGroup/Atom ID pairs
    (CGAtomIdx is already defined explicitly in cgatomidx.h)
*/
typedef AtomIDCombo<CGIdx,AtomNum> CGIdxAtomNum;
typedef AtomIDCombo<CGIdx,AtomName> CGIdxAtomName;

typedef AtomIDCombo<CGNum,AtomIdx> CGNumAtomIdx;
typedef AtomIDCombo<CGNum,AtomNum> CGAtomNum;
typedef AtomIDCombo<CGNum,AtomName> CGNumAtomName;

typedef AtomIDCombo<CGName,AtomIdx> CGNameAtomIdx;
typedef AtomIDCombo<CGName,AtomNum> CGNameAtomNum;
typedef AtomIDCombo<CGName,AtomName> CGAtomName;

/** Define all of the Residue/Atom ID pairs */
typedef AtomIDCombo<ResIdx,AtomIdx> ResAtomIdx;
typedef AtomIDCombo<ResIdx,AtomNum> ResIdxAtomNum;
typedef AtomIDCombo<ResIdx,AtomName> ResIdxAtomName;

typedef AtomIDCombo<ResNum,AtomIdx> ResNumAtomIdx;
typedef AtomIDCombo<ResNum,AtomNum> ResAtomNum;
typedef AtomIDCombo<ResNum,AtomName> ResNumAtomName;

typedef AtomIDCombo<ResName,AtomIdx> ResNameAtomIdx;
typedef AtomIDCombo<ResName,AtomNum> ResNameAtomNum;
typedef AtomIDCombo<ResName,AtomName> ResAtomName;

/** Define all of the Segment/Atom ID pairs */
typedef AtomIDCombo<SegIdx,AtomIdx> SegAtomIdx;
typedef AtomIDCombo<SegIdx,AtomNum> SegIdxAtomNum;
typedef AtomIDCombo<SegIdx,AtomName> SegIdxAtomName;

typedef AtomIDCombo<SegNum,AtomIdx> SegNumAtomIdx;
typedef AtomIDCombo<SegNum,AtomNum> SegAtomNum;
typedef AtomIDCombo<SegNum,AtomName> SegNumAtomName;

typedef AtomIDCombo<SegName,AtomIdx> SegNameAtomIdx;
typedef AtomIDCombo<SegName,AtomNum> SegNameAtomNum;
typedef AtomIDCombo<SegName,AtomName> SegAtomName;

}

Q_DECLARE_METATYPE(SireMol::CGIdxAtomNum);
Q_DECLARE_METATYPE(SireMol::CGIdxAtomName);

Q_DECLARE_METATYPE(SireMol::CGNumAtomIdx);
Q_DECLARE_METATYPE(SireMol::CGAtomNum);
Q_DECLARE_METATYPE(SireMol::CGNumAtomName);

Q_DECLARE_METATYPE(SireMol::CGNameAtomIdx);
Q_DECLARE_METATYPE(SireMol::CGNameAtomNum);
Q_DECLARE_METATYPE(SireMol::CGAtomName);

Q_DECLARE_TYPEINFO(SireMol::CGIdxAtomNum, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::CGNumAtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::CGAtomNum, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(SireMol::ResAtomIdx);
Q_DECLARE_METATYPE(SireMol::ResIdxAtomNum);
Q_DECLARE_METATYPE(SireMol::ResIdxAtomName);

Q_DECLARE_METATYPE(SireMol::ResNumAtomIdx);
Q_DECLARE_METATYPE(SireMol::ResAtomNum);
Q_DECLARE_METATYPE(SireMol::ResNumAtomName);

Q_DECLARE_METATYPE(SireMol::ResNameAtomIdx);
Q_DECLARE_METATYPE(SireMol::ResNameAtomNum);
Q_DECLARE_METATYPE(SireMol::ResAtomName);

Q_DECLARE_TYPEINFO(SireMol::ResAtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::ResIdxAtomNum, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::ResNumAtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::ResAtomNum, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(SireMol::SegAtomIdx);
Q_DECLARE_METATYPE(SireMol::SegIdxAtomNum);
Q_DECLARE_METATYPE(SireMol::SegIdxAtomName);

Q_DECLARE_METATYPE(SireMol::SegNumAtomIdx);
Q_DECLARE_METATYPE(SireMol::SegAtomNum);
Q_DECLARE_METATYPE(SireMol::SegNumAtomName);

Q_DECLARE_METATYPE(SireMol::SegNameAtomIdx);
Q_DECLARE_METATYPE(SireMol::SegNameAtomNum);
Q_DECLARE_METATYPE(SireMol::SegAtomName);

Q_DECLARE_TYPEINFO(SireMol::SegAtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::SegIdxAtomNum, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::SegNumAtomIdx, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(SireMol::SegAtomNum, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif

