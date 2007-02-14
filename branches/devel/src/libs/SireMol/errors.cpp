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

#include "SireMol/errors.h"

using namespace SireMol;

static const RegisterMetaType<siremol_error> r_mol;
static const RegisterMetaType<missing_atom> r_matom;
static const RegisterMetaType<missing_group> r_mgrp;
static const RegisterMetaType<duplicate_atom> r_datom;
static const RegisterMetaType<missing_molecule> r_mmol;
static const RegisterMetaType<missing_residue> r_mres;
static const RegisterMetaType<missing_cutgroup> r_mcgrp;
static const RegisterMetaType<duplicate_residue> r_dres;
static const RegisterMetaType<duplicate_molecule> r_dmol;
static const RegisterMetaType<template_error> r_tmplerr;
static const RegisterMetaType<anchor_error> r_ancerr;
static const RegisterMetaType<ring_error> r_ringerr;
static const RegisterMetaType<incompatible_molecule> r_incompat;
static const RegisterMetaType<missing_property> r_mprop;
static const RegisterMetaType<duplicate_property> r_dprop;
