
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<siremol_error> r_mol("SireMol::siremol_error");
static const RegisterMetaType<missing_atom> r_matom("SireMol::missing_atom");
static const RegisterMetaType<missing_group> r_mgrp("SireMol::missing_group");
static const RegisterMetaType<duplicate_atom> r_datom("SireMol::duplicate_atom");
static const RegisterMetaType<missing_residue> r_mres("SireMol::missing_residue");
static const RegisterMetaType<missing_cutgroup> r_mcgrp("SireMol::missing_cutgroup");
static const RegisterMetaType<duplicate_residue> r_dres("SireMol::duplicate_residue");
static const RegisterMetaType<template_error> r_tmplerr("SireMol::template_error");
static const RegisterMetaType<anchor_error> r_ancerr("SireMol::anchor_error");
static const RegisterMetaType<ring_error> r_ringerr("SireMol::ring_error");
static const RegisterMetaType<incompatible_molecule> r_incompat("SireMol::incompatible_molecule");
