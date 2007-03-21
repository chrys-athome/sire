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

#ifndef SIREFF_FORCEFIELD_H
#define SIREFF_FORCEFIELD_H

#include "ffbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceField;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceField&);
QDataStream& operator>>(QDataStream&, SireFF::ForceField&);

namespace SireFF
{

class FFBase;

/**
This class is the "user" interface to all forcefield classes. This holds an implicitly
shared copy of the forcefield class.

@author Christopher Woods
*/
class SIREFF_EXPORT ForceField
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    ForceField(const ForceField &other);
    ForceField(const FFBase &ffbase);
    ForceField(const SireBase::SharedPolyPointer<FFBase> &ffptr);

    ~ForceField();

    ForceField& operator=(const ForceField &other);
    ForceField& operator=(const FFBase &ffbase);

    const FFBase& base() const;

    const char* what() const;

    const QString& name() const;
    void setName(const QString& name);

    const FFBase::Components& components() const;
    const FFBase::Parameters& parameters() const;

    double energy();
    double energy(const FFComponent &component);

    Values energies();
    Values energies(const QSet<FFComponent> &components);

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);
    bool change(const PartialMolecule &molecule);

    bool change(const QHash<MoleculeID,Molecule> &molecules);
    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    bool add(const Molecule &molecule,
             const ParameterMap &map = ParameterMap());
    bool add(const Residue &residue,
             const ParameterMap &map = ParameterMap());
    bool add(const NewAtom &atom,
             const ParameterMap &map = ParameterMap());
    bool add(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());

    bool add(const QList<Molecule> &molecules,
             const ParameterMap &map = ParameterMap());
    bool add(const QList<Residue> &residues,
             const ParameterMap &map = ParameterMap());
    bool add(const QList<NewAtom> &atoms,
             const ParameterMap &map = ParameterMap());
    bool add(const QList<PartialMolecule> &molecules,
             const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::Group &group, const Molecule &molecule,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group, const Residue &residue,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group, const NewAtom &atom,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group, const PartialMolecule &molecule,
               const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::Group &group,
               const QList<Molecule> &molecules,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group,
               const QList<Residue> &residues,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group,
               const QList<NewAtom> &atoms,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group,
               const QList<PartialMolecule> &molecules,
               const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);
    bool remove(const NewAtom &atom);
    bool remove(const PartialMolecule &molecule);

    bool remove(const QList<Molecule> &molecules);
    bool remove(const QList<Residue> &residues);
    bool remove(const QList<NewAtom> &atoms);
    bool remove(const QList<PartialMolecule> &molecules);

    bool contains(const Molecule &molecule) const;
    bool contains(const Residue &residue) const;
    bool contains(const NewAtom &atom) const;
    bool contains(const PartialMolecule &molecule) const;

    bool contains(const Molecule &molecule, const FFBase::Group &group) const;
    bool contains(const Residue &residue, const FFBase::Group &group) const;
    bool contains(const NewAtom &atom, const FFBase::Group &group) const;
    bool contains(const PartialMolecule &molecule, 
                  const FFBase::Group &group) const;

    bool refersTo(MoleculeID molid) const;
    bool refersTo(MoleculeID molid, const FFBase::Group &group) const;

    QSet<MoleculeID> moleculeIDs() const;
    QSet<MoleculeID> moleculeIDs(const FFBase::Group &group) const;

    Molecule molecule(MoleculeID molid) const;
    Residue residue(MoleculeID molid, ResNum resnum) const;
    Residue residue(MoleculeID molid, ResID resid) const;
    Residue residue(MoleculeID molid, const QString &resname) const;
    NewAtom atom(MoleculeID molid, const IDMolAtom &atomid) const;

    Molecule molecule(const Molecule &mol) const;
    Residue residue(const Residue &res) const;
    NewAtom atom(const NewAtom &atom) const;

    PartialMolecule contents(MoleculeID molid) const;
    
    QHash<MoleculeID,PartialMolecule> contents(const FFBase::Group &group) const;
    QHash<MoleculeID,PartialMolecule> contents() const;

    bool isDirty() const;
    bool isClean() const;

    ForceFieldID ID() const;
    const Version& version() const;

    void assertContains(const FFComponent &component) const;

private:
    /** Shared pointer to the actual forcefield */
    SharedPolyPointer<FFBase> d;
};

}

Q_DECLARE_METATYPE(SireFF::ForceField);

SIRE_END_HEADER

#endif
