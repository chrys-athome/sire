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

#include "atomidcombos.h"

using namespace SireMol;

/* Register all of the ID combos with the Qt metatype system.
   This allows these IDs to be streamed and loaded via QDataStream */
static const RegisterMetaType<CGAtomIdx> r_cgatomidx;
static const RegisterMetaType<CGIdxAtomNum> r_cgidxatomnum;
static const RegisterMetaType<CGIdxAtomName> r_cgidxatomname;

static const RegisterMetaType<CGNumAtomIdx> r_cgnumatomidx;
static const RegisterMetaType<CGAtomNum> r_cgatomnum;
static const RegisterMetaType<CGNumAtomName> r_cgnumatomname;

static const RegisterMetaType<CGNameAtomIdx> r_cgnameatomidx;
static const RegisterMetaType<CGNameAtomNum> r_cgnameatomnum;
static const RegisterMetaType<CGAtomName> r_cgatomname;

static const RegisterMetaType<ResAtomIdx> r_resatomidx;
static const RegisterMetaType<ResIdxAtomNum> r_residxatomnum;
static const RegisterMetaType<ResIdxAtomName> r_residxatomname;

static const RegisterMetaType<ResNumAtomIdx> r_resnumatomidx;
static const RegisterMetaType<ResAtomNum> r_resatomnum;
static const RegisterMetaType<ResNumAtomName> r_resnumatomname;

static const RegisterMetaType<ResNameAtomIdx> r_resnameatomidx;
static const RegisterMetaType<ResNameAtomNum> r_resatomnum;
static const RegisterMetaType<ResAtomName> r_resatomname;

static const RegisterMetaType<SegAtomIdx> r_segatomidx;
static const RegisterMetaType<SegIdxAtomNum> r_segidxatomnum;
static const RegisterMetaType<SegIdxAtomName> r_segidxatomname;

static const RegisterMetaType<SegNumAtomIdx> r_segnumatomidx;
static const RegisterMetaType<SegAtomNum> r_segatomnum;
static const RegisterMetaType<SegNumAtomName> r_segnumatomname;

static const RegisterMetaType<SegNameAtomIdx> r_segnameatomidx;
static const RegisterMetaType<SegNameAtomNum> r_segnameatomnum;
static const RegisterMetaType<SegAtomName> r_segatomname;
