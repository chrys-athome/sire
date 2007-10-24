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

#ifndef SIREMOL_VIEWSOFMOL_H
#define SIREMOL_VIEWSOFMOL_H

#include "moleculeview.h"
#include "atomselection.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ViewsOfMol;
}

QDataStream& operator<<(QDataStream&, const SireMol::ViewsOfMol&);
QDataStream& operator>>(QDataStream&, SireMol::ViewsOfMol&);

namespace SireMol
{

template<class T>
class Mover;
template<class T>
class Editor;
template<class T>
class Selector;

class Evaluator;

class Atom;
class CutGroup;
class Residue;
class Chain;
class Segment;
class Molecule;
class PartialMolecule;

/** This class holds multiple arbitrary views of the  
    same molecule.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT ViewsOfMol : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const ViewsOfMol&);
friend QDataStream& ::operator>>(QDataStream&, ViewsOfMol&);

public:
    ViewsOfMol();
    
    ViewsOfMol(const MoleculeData &moldata);

    ViewsOfMol(const MoleculeData &moldata,
               const AtomSelection &selected_atoms);
    
    ViewsOfMol(const MoleculeData &moldata,
               const QList<AtomSelection> &views);

    ViewsOfMol(const Selector<Segment> &segments);
    ViewsOfMol(const Selector<Chain> &chains);
    ViewsOfMol(const Selector<CutGroup> &cgroups);
    ViewsOfMol(const Selector<Residue> &residue);
    ViewsOfMol(const Selector<Atom> &atoms);
    
    ViewsOfMol(const MoleculeView &view);
    
    ViewsOfMol(const ViewsOfMol &other);
    
    ~ViewsOfMol();
   
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ViewsOfMol>() );
    }
    
    const char* what() const
    {
        return ViewsOfMol::typeName();
    }
    
    ViewsOfMol* clone() const
    {
        return new ViewsOfMol(*this);
    }
   
    ViewsOfMol& operator=(const ViewsOfMol &other);
    
    ViewsOfMol& operator=(const Selector<Atom> &atoms);
    ViewsOfMol& operator=(const Selector<CutGroup> &cgroups);
    ViewsOfMol& operator=(const Selector<Residue> &residues);
    ViewsOfMol& operator=(const Selector<Chain> &chains);
    ViewsOfMol& operator=(const Selector<Segment> &segments);
    
    ViewsOfMol& operator=(const MoleculeView &view);
    
    bool operator==(const ViewsOfMol &other) const;
    bool operator!=(const ViewsOfMol &other) const;

    PartialMolecule operator[](int i) const;
    
    PartialMolecule at(int i) const;
    
    MolNum number() const;
    const MolName& name() const;
    
    quint64 version() const;
    quint64 version(const PropertyKey &key) const;
    
    int nViews() const;
    int count() const;

    ViewsOfMol add(const AtomSelection &view) const;
    ViewsOfMol add(const QList<AtomSelection> &views) const;
    
    ViewsOfMol unite(const AtomSelection &view) const;
    ViewsOfMol unite(const QList<AtomSelection> &views) const;
    
    ViewsOfMol remove(const AtomSelection &view) const;
    ViewsOfMol remove(const QList<AtomSelection> &views) const;
    
    ViewsOfMol removeAt(int i) const;
    
    ViewsOfMol removeAll(const AtomSelection &view) const;
   
    ViewsOfMol removeAll() const;
   
    PartialMolecule join() const;
    PartialMolecule all() const;
    
    ViewsOfMol removeDuplicates() const;
    
    Molecule molecule() const;

    Mover<ViewsOfMol> move() const;
    Mover<ViewsOfMol> move(int i) const;
    
    Evaluator evaluate() const;
    Evaluator evaluate(int i) const;

    AtomSelection selection() const;
    AtomSelection selection(int i) const;
    
    QList<AtomSelection> selections() const;
    
    bool contains(AtomIdx atomidx) const;
    bool contains(const AtomID &atomid) const;
    bool intersects(const AtomID &atomid) const;
    
    bool contains(const AtomSelection &selection) const;
    bool intersects(const AtomSelection &selection) const;
    
    bool contains(const QList<AtomSelection> &selections) const;
    bool intersects(const QList<AtomSelection> &selections) const;
    
    Atom select(const AtomID &atomid) const;

    Selector<Atom> selectAll(const AtomID &atomid) const;
    Selector<Atom> selectAll() const;

    Atom atom(const AtomID &atomid) const;
    
    Selector<Atom> atoms(const AtomID &atomid) const;
    Selector<Atom> atoms() const;
    
    QList<AtomIdx> atomIdxs() const;
    
    bool hasProperty(const PropertyName&) const
    {
        return false;
    }
    
    bool hasMetadata(const PropertyName&) const
    {
        return false;
    }
    
    bool hasMetadata(const PropertyName&, const PropertyName&) const
    {
        return false;
    }
    
    QStringList propertyKeys() const
    {
        return QStringList();
    }
    
    QStringList metadataKeys() const
    {
        return QStringList();
    }
    
    QStringList metadataKeys(const PropertyName&) const
    {
        return QStringList();
    }
   
private:
    template<class T>
    void setEqualTo(const Selector<T> &selection);

    void _pvt_add(const AtomSelection &view);

    /** The union of all of the views of the molecule
        (and the first view, if there is only one view) */
    AtomSelection selected_atoms;
    
    /** The array of views of the molecule */
    QList<AtomSelection> views;
};

}

Q_DECLARE_METATYPE( SireMol::ViewsOfMol );
Q_DECLARE_METATYPE( SireMol::Mover<SireMol::ViewsOfMol> );

SIRE_END_HEADER

#endif
