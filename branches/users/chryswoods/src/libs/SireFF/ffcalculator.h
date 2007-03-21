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

#ifndef SIREFF_FFCALCULATOR_H
#define SIREFF_FFCALCULATOR_H

#include "forcefield.h"

#include <boost/noncopyable.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class Residue;
class MoleculeID;
}

namespace SireCAS
{
class Values;
}

namespace SireFF
{

using SireCAS::Values;
using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

/** This is the pure virtual base class of all of the FFCalculatorBases. These are
    simple classes that provide an interface to a ForceField that allows their
    use by the FFProcessor classes.

    FFWorker classes are non-copyable!

    @author Christopher Woods
*/
class SIREFF_EXPORT FFCalculatorBase : public boost::noncopyable
{
public:
    FFCalculatorBase();

    virtual ~FFCalculatorBase();

    virtual double getEnergies(Values &values)=0;

    virtual void calculateEnergy()=0;

    virtual bool add(const Molecule &molecule,
                     const ParameterMap &map = ParameterMap())=0;
    virtual bool add(const Residue &residue,
                     const ParameterMap &map = ParameterMap())=0;
    virtual bool add(const NewAtom &atom,
                     const ParameterMap &map = ParameterMap())=0;
    virtual bool add(const PartialMolecule &molecule,
                     const ParameterMap &map = ParameterMap())=0;

    virtual bool add(const QList<Molecule> &molecules,
                     const ParameterMap &map = ParameterMap())=0;
    virtual bool add(const QList<Residue> &residues,
                     const ParameterMap &map = ParameterMap())=0;
    virtual bool add(const QList<NewAtom> &atoms,
                     const ParameterMap &map = ParameterMap())=0;
    virtual bool add(const QList<PartialMolecule> &molecules,
                     const ParameterMap &map = ParameterMap())=0;

    virtual bool addTo(const FFBase::Group &group,
                       const Molecule &molecule,
                       const ParameterMap &map = ParameterMap())=0;
    virtual bool addTo(const FFBase::Group &group,
                       const Residue &residue,
                       const ParameterMap &map = ParameterMap())=0;
    virtual bool addTo(const FFBase::Group &group,
                       const NewAtom &atom,
                       const ParameterMap &map = ParameterMap())=0;
    virtual bool addTo(const FFBase::Group &group,
                       const PartialMolecule &molecule,
                       const ParameterMap &map = ParameterMap())=0;

    virtual bool addTo(const FFBase::Group &group,
                       const QList<Molecule> &molecules,
                       const ParameterMap &map = ParameterMap())=0;
    virtual bool addTo(const FFBase::Group &group,
                       const QList<Residue> &residues,
                       const ParameterMap &map = ParameterMap())=0;
    virtual bool addTo(const FFBase::Group &group,
                       const QList<NewAtom> &atoms,
                       const ParameterMap &map = ParameterMap())=0;
    virtual bool addTo(const FFBase::Group &group,
                       const QList<PartialMolecule> &molecules,
                       const ParameterMap &map = ParameterMap())=0;

    virtual bool change(const Molecule &molecule)=0;
    virtual bool change(const Residue &residue)=0;
    virtual bool change(const NewAtom &atom)=0;

    virtual bool change(const QHash<MoleculeID,Molecule> &molecules)=0;
    virtual bool change(const QHash<MoleculeID,PartialMolecule> &molecules)=0;

    virtual bool remove(const Molecule &molecule)=0;
    virtual bool remove(const Residue &residue)=0;
    virtual bool remove(const NewAtom &atom)=0;
    virtual bool remove(const PartialMolecule &molecule)=0;

    virtual bool remove(const QList<Molecule> &molecules)=0;
    virtual bool remove(const QList<Residue> &residues)=0;
    virtual bool remove(const QList<NewAtom> &atoms)=0;
    virtual bool remove(const QList<PartialMolecule> &molecules)=0;

    virtual bool contains(const Molecule &molecule)=0;
    virtual bool contains(const Residue &residue)=0;
    virtual bool contains(const NewAtom &atom)=0;
    virtual bool contains(const PartialMolecule &molecule)=0;

    virtual bool contains(const Molecule &molecule,
                          const FFBase::Group &group)=0;
    virtual bool contains(const Residue &residue,
                          const FFBase::Group &group)=0;
    virtual bool contains(const NewAtom &atom,
                          const FFBase::Group &group)=0;
    virtual bool contains(const PartialMolecule &molecule,
                          const FFBase::Group &group)=0;

    virtual bool refersTo(MoleculeID molid)=0;
    virtual bool refersTo(MoleculeID molid, const FFBase::Group &group)=0;

    virtual QSet<MoleculeID> moleculeIDs()=0;
    virtual QSet<MoleculeID> moleculeIDs(const FFBase::Group &group)=0;

    virtual PartialMolecule contents(MoleculeID molid)=0;
    virtual QHash<MoleculeID,PartialMolecule> contents(const FFGroup::Group group)=0;
    virtual QHash<MoleculeID,PartialMolecule> contents()=0;

    virtual bool isDirty()=0;
    virtual bool isClean()=0;

    virtual ForceFieldID ID()=0;
    virtual Version version()=0;

    virtual void assertContains(const FFComponent &component)=0;

    virtual bool setForceField(const ForceField &forcefield)=0;

    virtual ForceField forcefield() const=0;
};

/** This is the default, and most simple, FFCalculator. This will
    work with any forcefield that can be used from the command line
    (pretty much all of them).

    @author Christopher Woods
*/
class SIREFF_EXPORT FFCalculator : public FFCalculatorBase
{
public:
    FFCalculator();
    FFCalculator(const ForceField &forcefield);

    ~FFCalculator();

    double getEnergies(Values &values);

    void calculateEnergy();

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

    bool addTo(const FFBase::FFGroup &group,
               const Molecule &molecule,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const Residue &residue,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const NewAtom &atom,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const PartialMolecule &molecule,
               const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::FFGroup &group,
               const QList<Molecule> &molecules,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const QList<Residue> &residues,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const QList<NewAtom> &atoms,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const QList<PartialMolecule> &molecules,
               const ParameterMap &map = ParameterMap());

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);

    bool change(const QHash<MoleculeID,Molecule> &molecules);
    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);
    bool remove(const NewAtom &atom);
    bool remove(const PartialMolecule &molecule);

    bool contains(const Molecule &molecule);
    bool contains(const Residue &residue);
    bool contains(const NewAtom &atom);
    bool contains(const PartialMolecule &molecule);

    bool contains(const Molecule &molecule,
                  const FFBase::Group &group);
    bool contains(const Residue &residue,
                  const FFBase::Group &group);
    bool contains(const NewAtom &atom,
                  const FFBase::Group &group);
    bool contains(const PartialMolecule &molecule,
                  const FFBase::Group &group);

    bool refersTo(MoleculeID molid)=0;
    bool refersTo(MoleculeID molid, const FFBase::Group &group);

    QSet<MoleculeID> moleculeIDs();
    QSet<MoleculeID> moleculeIDs(const FFBase::Group &group);

    PartialMolecule contents(MoleculeID molid);
    QHash<MoleculeID,PartialMolecule> contents(const FFGroup::Group group);
    QHash<MoleculeID,PartialMolecule> contents();

    bool isDirty();
    bool isClean();

    ForceFieldID ID();
    Version version();

    void assertContains(const FFComponent &component);

    bool setForceField(const ForceField &forcefield);

    ForceField forcefield() const;

private:
    /** The forcefield being evaluated */
    ForceField ffield;

    /** The total energy of the forcefield at the last calculation */
    double total_nrg;

    /** The values of the energy components at the last evaluation */
    Values nrg_components;
};

}

SIRE_END_HEADER

#endif
