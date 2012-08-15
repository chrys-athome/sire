#ifndef SIREMOL_MOVER_METAID_H
#define SIREMOL_MOVER_METAID_H

#include "atom.h"
#include "chain.h"
#include "cutgroup.h"
#include "residue.h"
#include "segment.h"

#include "mover.hpp"
#include "selector.hpp"

Q_DECLARE_METATYPE( SireMol::Mover< SireMol::Selector<SireMol::Atom> > )
Q_DECLARE_METATYPE( SireMol::Mover< SireMol::Selector<SireMol::Chain> > )
Q_DECLARE_METATYPE( SireMol::Mover< SireMol::Selector<SireMol::CutGroup> > )
Q_DECLARE_METATYPE( SireMol::Mover< SireMol::Selector<SireMol::Residue> > )
Q_DECLARE_METATYPE( SireMol::Mover< SireMol::Selector<SireMol::Segment> > )


#ifdef SIRE_INSTANTIATE_TEMPLATES

#include "mover.hpp"

template class SireMol::Selector<SireMol::Segment>;
template class SireMol::Mover< SireMol::Selector<SireMol::Segment> >;
template class SireMol::Selector<SireMol::Residue>;
template class SireMol::Mover< SireMol::Selector<SireMol::Residue> >;
template class SireMol::Selector<SireMol::CutGroup>;
template class SireMol::Mover< SireMol::Selector<SireMol::CutGroup> >;
template class SireMol::Selector<SireMol::Chain>;
template class SireMol::Mover< SireMol::Selector<SireMol::Chain> >;
template class SireMol::Selector<SireMol::Atom>;
template class SireMol::Mover< SireMol::Selector<SireMol::Atom> >;

#endif

#endif

