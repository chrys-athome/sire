/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "cljgroup.h"

using namespace SireMM;
using namespace SireMol;

void CLJGroup::accept()
{
    foreach (MolNum molnum, changed_mols)
    {
        cljexts[molnum].commit(cljboxes,cljworkspace);
    }
    
    changed_mols.clear();
    
    molgroup.accept();
}

void CLJGroup::updatedConnectedGroup()
{
    cljworkspace.removeSameIDAtoms(cljboxes);
}

CLJAtoms CLJGroup::changedAtoms() const
{
    return cljworkspace.merge();
}

/** Tell the group that calculations will be made completely from scratch,
    so there is no need to maintain a delta */
void CLJGroup::mustRecalculateFromScratch()
{
    this->accept();
    cljworkspace.mustRecalculateFromScratch():
}

/** Tell the group that calculations will be made completely from scratch,
    and to also re-extract all of the molecules */
void CLJGroup::mustReallyRecalculateFromScratch()
{
    cljworkspace = CLJWorkspace();
    cljworkspace.mustRecalculateFromScratch();
    
    cljboxes = CLJBoxes();
    
    cljexts.clear();
    
    for (MoleculeGroup::const_iterator it = molgroup.constBegin();
         it != molgroup.constEnd();
         ++it)
    {
        cljexts.insert( it.key(), CLJExtractor(cljboxes, cljworkspace,
                                               it.value(),id_source,split_by_residue,
                                               props.value(it.key(),PropertyMap())) );
                                               
    }
}
