/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SPIER_MOLDISPLAYDATA_H
#define SPIER_MOLDISPLAYDATA_H

#include "moldisplaydataptr.h"

#include "SireMol/atomset.h"
#include "SireMol/editmol.h"
#include "SireMol/atomindexset.h"

#include <QSet>
#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMol::AtomIndex;
using SireMol::AtomIndexSet;
using SireMol::EditMol;

class MoleculeRep;
typedef boost::shared_ptr<MoleculeRep> MoleculeRepPtr;

class GLCanvas;

/**
This class contains all of the information necessary to display the contained molecule. This includes all of the representations of the molecule, the set of selected atoms etc. This class is also used to switch on and off different representations of the molecule, and is used by GLCanvas to organise all of the molecules that it is displaying.
 
@author Christopher Woods
*/
class SPIER_EXPORT MolDisplayData
{
public:
    ~MolDisplayData();

    static MolDisplayDataPtr create(GLCanvas *canvas, const EditMol &molecule);

    GLCanvas* canvas();
    const GLCanvas* canvas() const;

    void select(const AtomIndex &atom);
    void deselect(const AtomIndex &atom);
    void toggle(const AtomIndex &atom);
    
    bool selected(const AtomIndex &atom);

    void clearSelection();

    const EditMol& molecule() const;

    bool isNull() const;
    
    MolDisplayDataPtr self() const;

private:
    MolDisplayData(GLCanvas *canvas);

    /** The parent GLCanvas of this object */
    GLCanvas *cnvs;

    /** The molecule to be displayed */
    EditMol mol;
    
    /** The collection of representations of this molecule */
    QHash<QString, MoleculeRepPtr> reps;
    
    /** The set of selected atoms */
    AtomIndexSet selectatms;
    
    /** The self pointer */
    MolDisplayDataWeakPtr selfptr;
};

/** Return the parent canvas on which the representations of the 
    molecule will be drawn. */
inline GLCanvas* MolDisplayData::canvas()
{
    return cnvs;
}

/** Return the parent canvas on which the representations of the 
    molecule will be drawn. */
inline const GLCanvas* MolDisplayData::canvas() const
{
    return cnvs;
}

/** Return the molecule that is to be displayed */
inline const EditMol& MolDisplayData::molecule() const
{
    return mol;
}

}

SIRE_END_HEADER

#endif
