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

#include "propertyextractor.h"
#include "moleculeview_inlines.h"

#include "cutgroup.h"
#include "atominfogroup.h"
#include "cgatomid.h"
#include "moleculeproperty.h"

#include "partialmolecule.h"

#include "SireBase/property.h"

#include "SireVol/coordgroup.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireVol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<PropertyExtractor> r_propex;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const PropertyExtractor &propex)
{
    writeHeader(ds, r_propex, 1)
          << static_cast<const MolDataView&>(propex);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, PropertyExtractor &propex)
{
    VersionID v = readHeader(ds, r_propex);

    if (v == 1)
    {
        ds >> static_cast<MolDataView&>(propex);
    }
    else
        throw version_error(v, "1", r_propex, CODELOC);

    return ds;
}

/** Null constructor */
PropertyExtractor::PropertyExtractor() : MolDataView()
{}

/** Construct the extractor to extract properties from 'molecule' */
PropertyExtractor::PropertyExtractor(const MoleculeView &molecule)
                  : MolDataView(molecule)
{}

/** Construct the extractor to extract properties from the 'selection'
    within the molecule 'molecule'

    \throw SireError::incompatible_error
*/
PropertyExtractor::PropertyExtractor(const MoleculeView &molecule,
                                     const SelectionFromMol &selection)
                  : MolDataView(molecule)
{
    _pvt_selection() = selectedAtoms().intersect(selection);
}

/** Copy constructor */
PropertyExtractor::PropertyExtractor(const PropertyExtractor &other)
                  : MolDataView(other)
{}

/** Copy assignment operator */
PropertyExtractor& PropertyExtractor::operator=(const PropertyExtractor &other)
{
    MolDataView::operator=(other);
    return *this;
}

/** Destructor */
PropertyExtractor::~PropertyExtractor()
{}

/** Return the index of CutGroupID into the CoordGroup, AtomicProperty
    or CutGroup arrays. This is necessary as while CutGroupID corresponds
    exactly to the array index for a whole molecule, it does not
    correspond exactly for a partial molecule (as CutGroups that don't contain
    any selected atoms are left out, and the order of the CutGroups in the
    array may change). This function returns the index that maps CutGroupID
    to the location of the CutGroup in the arrays returned by coordGroups(),
    getProperty() and all of the extract() functions. */
QHash<CutGroupID,quint32> PropertyExtractor::cutGroupIndex() const
{
    const AtomSelection &selected_atoms = selectedAtoms();

    if (selected_atoms.selectedAllCutGroups())
    {
        //all CutGroups are selected, so CutGroupID => index into array
        // - this is a pretty pointless index!
        QHash<CutGroupID,quint32> index;
        uint ncg = data().info().nCutGroups();
        index.reserve(ncg);

        for (CutGroupID i(0); i<ncg; ++i)
        {
            index.insert( i, i );
        }

        return index;
    }
    else
    {
        //there are some missing CutGroups - the arrays are ordered
        //using the iterator order of selectedCutGroups()
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();

        QHash<CutGroupID,quint32> index;

        if (not cgids.isEmpty())
        {
            index.reserve(cgids.count());

            quint32 i = 0;

            foreach (CutGroupID cgid, cgids)
            {
                index.insert( cgid, i );
                ++i;
            }
        }

        return index;
    }
}

/** Return the property with the name 'name'. If this is a molecular
    property then this property will be masked by the atom selection
    (e.g. AtomicProperties will only be returned for selected atoms,
     with the order the same as that returned by coordGroups() )

    \throw SireBase::missing_property
*/
Property PropertyExtractor::property(const QString &name) const
{
    const AtomSelection &selected_atoms = selectedAtoms();

    Property property = data().getProperty(name);

    if (selected_atoms.selectedAll() or not property.isA<MoleculeProperty>())
    {
        return property;
    }

    //mask this property by the current selection
    return property.asA<MoleculeProperty>().mask(selected_atoms);
}

/** Return the CoordGroups that contain the coordinates of atoms that
    have been selected. Only CoordGroups that contain selected atoms
    will be returned. */
QVector<CoordGroup> PropertyExtractor::coordGroups() const
{
    const AtomSelection &selected_atoms = selectedAtoms();

    QVector<CoordGroup> coords = data().coordGroups();

    if (selected_atoms.selectedAllCutGroups())
        return coords;
    else
    {
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();
        QVector<CoordGroup> selected_coords( cgids.count() );

        const CoordGroup *coords_array = coords.constData();
        CoordGroup *selected_coords_array = selected_coords.data();

        int i=0;

        foreach (CutGroupID cgid, cgids)
        {
            selected_coords_array[i] = coords_array[cgid];
            ++i;
        }

        return selected_coords;
    }
}

/** Return the CutGroups that contain the atoms that
    have been selected. Only CutGroups that contain selected atoms
    will be returned. */
QVector<CutGroup> PropertyExtractor::cutGroups() const
{
    const AtomSelection &selected_atoms = selectedAtoms();

    if (selected_atoms.selectedAllCutGroups())
        return data().cutGroups();
    else
    {
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();
        QVector<CutGroup> selected_cgroups( cgids.count() );

        CutGroup *selected_cgroups_array = selected_cgroups.data();

        int i=0;

        foreach (CutGroupID cgid, cgids)
        {
            selected_cgroups_array[i] = data().cutGroup(cgid);
            ++i;
        }

        return selected_cgroups;
    }
}

/** Return groups containing all of the element types of all of the
    atoms in the molecule. This returns the elements according
    to CutGroup, with only groups that contain at least one
    selected atom being returned. Atoms in the group that have
    not been selected will be set to dummy atoms. */
QVector< QVector<Element> > PropertyExtractor::elements() const
{
    const AtomSelection &selected_atoms = selectedAtoms();

    QHash<CutGroupID,AtomInfoGroup> atominfos = data().info().atomGroups();

    if (selected_atoms.selectedAllCutGroups())
    {
        uint ngroups = atominfos.count();

        if (ngroups == 0)
            return QVector< QVector<Element> >();

        QVector< QVector<Element> > grouped_elements(ngroups);

        for (CutGroupID i(0); i<ngroups; ++i)
        {
            const AtomInfoGroup &info_group = *(atominfos.constFind(i));

            uint nats = info_group.nAtoms();
            const AtomInfo *atominfo_array = info_group.constData();

            QVector<Element> &element_group = grouped_elements[i];

            element_group.resize(nats);
            Element *element_group_array = element_group.data();

            if (selected_atoms.selectedAll(i))
            {
                for (AtomID j(0); j<nats; ++j)
                {
                    element_group_array[j] = atominfo_array[j];
                }
            }
            else
            {
                for (AtomID j(0); j<nats; ++j)
                {
                    if ( selected_atoms.selected(CGAtomID(i,j)) )
                    {
                        element_group_array[j] = atominfo_array[j];
                    }
                    else
                    {
                        element_group_array[j] = Element(0);
                    }
                }
            }
        }

        return grouped_elements;
    }
    else
    {
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();

        int ngroups = cgids.count();

        if (ngroups == 0)
            return QVector< QVector<Element> >();

        int i=0;
        QVector< QVector<Element> > grouped_elements(ngroups);

        foreach (CutGroupID cgid, cgids)
        {
            QVector<Element> &element_group = grouped_elements[i];
            ++i;

            const AtomInfoGroup &info_group = *(atominfos.constFind(cgid));
            const AtomInfo *atominfo_array = info_group.constData();
            uint nats = info_group.nAtoms();

            element_group.resize(nats);
            Element *element_group_array = element_group.data();

            if (selected_atoms.selectedAll(cgid))
            {
                for (AtomID j(0); j<nats; ++j)
                {
                    element_group_array[j] = atominfo_array[j];
                }
            }
            else
            {
                for (AtomID j(0); j<nats; ++j)
                {
                    if ( selected_atoms.selected(CGAtomID(cgid,j)) )
                    {
                        element_group_array[j] = atominfo_array[j];
                    }
                    else
                    {
                        element_group_array[j] = Element(0);
                    }
                }
            }
        }

        return grouped_elements;
    }
}

static AABox getAABox(CutGroupID cgid, const CoordGroup &cgroup,
                      const AtomSelection &selected_atoms)
{
    if (selected_atoms.selectedAll(cgid))
        return cgroup.aaBox();
    else
    {
        //we need to create the AABox from scratch...
        // Get the coordinates of the selected atoms
        QSet<CGAtomID> cgatomids = selected_atoms.selectedAtoms(cgid);

        if (cgatomids.isEmpty())
            throw SireMol::missing_atom( QObject::tr(
                "There are no atoms selected in the CutGroup with ID == %1, "
                "so it is not possible to find their center!")
                    .arg(cgid), CODELOC );

        QVector<Vector> coords;
        coords.reserve(cgatomids.count());

        foreach (const CGAtomID &cgatomid, cgatomids)
        {
            coords.append( cgroup.at(cgatomid.atomID()) );
        }

        return AABox(coords);
    }
}

/** This function returns the geometric center of the selected atoms of the
    molecule. There must be some atoms in this molecule for the center
    to be found!

    \throw SireMol::missing_atom
*/
Vector PropertyExtractor::geometricCenter() const
{
    QVector<CoordGroup> cgroups = data().coordGroups();
    const CoordGroup *cgroups_array = cgroups.constData();
    uint ncg = cgroups.count();

    const AtomSelection &selected_atoms = selectedAtoms();

    if (ncg == 0 or selected_atoms.selectedNone())
        throw SireMol::missing_atom( QObject::tr(
              "Cannot find the center of no atoms!"),
                  CODELOC );

    if (selected_atoms.selectedAll())
    {
        //combine together the boxes of all of the CoordGroups...
        AABox box = cgroups_array[0].aaBox();

        for (CutGroupID i(1); i<ncg; ++i)
        {
            box += cgroups_array[i].aaBox();
        }

        return box.center();
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        AABox box = getAABox(CutGroupID(0), cgroups_array[0], selected_atoms);

        for (CutGroupID i(1); i<ncg; ++i)
        {
            box += getAABox(i, cgroups_array[i], selected_atoms);
        }

        return box.center();
    }
    else
    {
        QSet<CutGroupID> cgids = selected_atoms.selectedCutGroups();

        QSet<CutGroupID>::const_iterator it = cgids.constBegin();

        AABox box = getAABox(*it, cgroups_array[*it], selected_atoms);

        for (++it; it != cgids.constEnd(); ++it)
        {
            box += getAABox(*it, cgroups_array[*it], selected_atoms);
        }

        return box.center();
    }
}
