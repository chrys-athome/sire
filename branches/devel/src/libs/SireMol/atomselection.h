#ifndef SIREMOL_ATOMSELECTION_H
#define SIREMOL_ATOMSELECTION_H

#include <QSet>
#include <QHash>

#include "moleculeinfo.h"
#include "cutgroupid.h"
#include "atomid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomSelection;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomSelection&);
QDataStream& operator>>(QDataStream&, SireMol::AtomSelection&);

namespace SireMol
{

class Molecule;

/** This class holds information about a selection of atoms in a Molecule.
    The selection is held in the most memory-efficient manner possible,
    and takes advantage of the CutGroup-based layout of Molecule objects.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomSelection
{

friend QDataStream& ::operator<<(QDataStream&, const AtomSelection&);
friend QDataStream& ::operator>>(QDataStream&, AtomSelection&);

public:
    AtomSelection();

    AtomSelection(const Molecule &molecule);

    AtomSelection(const AtomSelection &other);

    ~AtomSelection();

    bool isEmpty() const;

    int nSelected() const;

    int nSelected(CutGroupID cgid) const;
    int nSelected(ResNum resnum) const;

    bool selected(const CGAtomID &cgatomid) const;

    bool selectedAll() const;

    bool selectedAll(CutGroupID cgid) const;
    bool selectedAll(ResNum resnum) const;

    void selectAll();
    void deselectAll();

    void selectAll(CutGroupID cgid);
    void deselectAll(CutGroupID cgid);

    void selectAll(ResNum resnum);
    void deselectAll(ResNum resnum);

    void select(const CGAtomID &cgatomid);
    void deselect(const CGAtomID &cgatomid);

private:
    bool _pvt_selected(const CGAtomID &cgatomid) const;

    void _pvt_select(const CGAtomID &cgatomid);
    void _pvt_deselect(const CGAtomID &cgatomid);

    /** The AtomIDs of selected atoms, arranged by CutGroupID */
    QHash< CutGroupID, QSet<AtomID> > selected_atoms;

    /** The MoleculeInfo describing the molecule whose parts
        are being selected by this object */
    MoleculeInfo molinfo;

    /** The total number of selected atoms */
    int nselected;
};

}

Q_DECLARE_METATYPE(SireMol::AtomSelection);

SIRE_END_HEADER

#endif
