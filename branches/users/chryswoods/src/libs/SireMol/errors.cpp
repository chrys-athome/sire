
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
