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


#endif

