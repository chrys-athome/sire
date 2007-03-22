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

#ifndef SIREFF_FORCEFIELDS_H
#define SIREFF_FORCEFIELDS_H

#include <QHash>
#include <QVector>

#include "forcefield.h"
#include "ffexpression.h"

#include "ffcomponent.h"

#include "SireCAS/values.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceFieldsBase;
class ForceFields;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceFieldsBase&);
QDataStream& operator>>(QDataStream&, SireFF::ForceFieldsBase&);

QDataStream& operator<<(QDataStream&, const SireFF::ForceFields&);
QDataStream& operator>>(QDataStream&, SireFF::ForceFields&);

namespace SireFF
{

using SireCAS::SymbolID;
using SireCAS::Function;
using SireCAS::Values;

/** This is the virtual base class of all of the ForceFields classes.
    ForceFields classes hold collections of forcefields, together with
    equations that evaluate combinations of those forcefields.

    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFieldsBase
{

friend QDataStream& ::operator<<(QDataStream&, const ForceFieldsBase&);
friend QDataStream& ::operator>>(QDataStream&, ForceFieldsBase&);

public:
    ForceFieldsBase();

    ForceFieldsBase(const ForceFieldsBase &other);

    virtual ~ForceFieldsBase();

    ForceFieldsBase& operator=(const ForceFieldsBase &other);

    virtual QHash<ForceFieldID,ForceField> forceFields() const=0;

    virtual void setEqualTo(const ForceFields &forcefields)=0;

    virtual void minorUpdate(const ForceFields &forcefields)=0;
    virtual void majorUpdate(const ForceFields &forcefields)=0;

    const Values& parameters() const;

    virtual bool contains(const Function &function) const;

    const FFExpression& expression(const Function &function) const;

    QVector<FFExpression> expressions(const QSet<Function> &functions) const;
    QVector<FFExpression> expressions() const;

    void setParameter(const Symbol &param, double value);

    void setTotal(const FFExpression &expression);

    bool contains(MoleculeID molid) const;
    bool contains(const Molecule &molecule) const;

    QSet<MoleculeID> moleculeIDs() const;
    virtual QSet<ForceFieldID> forceFieldIDs() const=0;

    ForceField forceField(ForceFieldID) const=0;
    void setForceField(const ForceField &forcefield)=0;

    int nMolecules() const;
    int nForceFields() const;

    const FFExpression& total() const;

    // Interface to ForceField functions (generalised over all ForceFields)
    double energy(const Expression &expression);
    double energy(const FFExpression &expression);
    double energy(const Function &component);

    double energy(const FFComponent &component);

    double energy();

    Values energies();
    Values energies(const QSet<FFComponent> &components);
    Values energies(const QSet<Function> &components);

    virtual void setProperty(const QString &name, const Property &property)=0;
    virtual void setProperty(ForceFieldID ffid,
                             const QString &name, const Property &property)=0;

    virtual void setProperty(const QSet<ForceFieldID> &ffids,
                             const QString &name, const Property &property)=0;

    virtual QSet<ForceFieldID,Property> getProperty(const QString &name) const=0;

    virtual Property getProperty(ForceFieldID ffid, const QString &name) const=0;
    virtual QSet<ForceFieldID,Property> getProperty(const QSet<ForceFieldID> &ffids,
                                                    const QString &name) const=0;

    virtual bool containsProperty(const QString &name) const=0;
    virtual bool containsProperty(ForceFieldID ffid, const QString &name) const=0;
    virtual QSet<ForceFieldID> forceFieldsWithProperty(const QString &name) const=0;

    virtual void change(const PartialMolecule &molecule)=0;
    virtual void change(const QHash<MoleculeID,PartialMolecule> &molecules)=0;

    virtual void add(ForceFieldID ffid, const PartialMolecule &molecule)=0;
    virtual void add(ForceFieldID ffid,
                     const QList<PartialMolecule> &molecules)=0;

    virtual void addTo(ForceFieldID ffid, const FFBase::Group &group,
                       const PartialMolecule &molecule)=0;
    virtual void addTo(ForceFieldID ffid, const FFBase::Group &group,
                       const QList<PartialMolecule> &molecules)=0;

    virtual void remove(const PartialMolecule &molecule)=0;
    virtual void remove(const QList<PartialMolecule> &molecules)=0;
    virtual void remove(ForceFieldID ffid, const PartialMolecule &molecule)=0;
    virtual void remove(ForceFieldID ffid,
                        const QList<PartialMolecule> &molecules)=0;

    virtual void removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                            const PartialMolecule &molecule)=0;
    virtual void removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                            const QList<PartialMolecule> &molecules)=0;

    virtual bool contains(const PartialMolecule &molecule, ForceFieldID ffid) const=0;
    virtual bool contains(const PartialMolecule &molecule,
                          ForceFieldID ffid, const FFBase::Group &group) const=0;

    virtual QSet<ForceFieldID> forceFieldsContaining(
                                    const PartialMolecule &molecule) const=0;

    virtual bool refersTo(MoleculeID molid) const=0;
    virtual bool refersTo(MoleculeID molid, ForceFieldID ffid) const=0;
    virtual bool refersTo(MoleculeID molid, ForceFieldID ffid,
                          const FFBase::Group &group) const=0;

    virtual QSet<ForceFieldID> forceFieldsReferringTo(MoleculeID molid) const=0;

    virtual QSet<MoleculeID> moleculeIDs() const=0;
    virtual QSet<MoleculeID> moleculeIDs(ForceFieldID ffid) const=0;
    virtual QSet<MoleculeID> moleculeIDs(ForceFieldID ffid,
                                         const FFBase::Group &group) const=0;

    PartialMolecule molecule(MoleculeID molid, ForceFieldID ffid) const=0;

    QHash<MoleculeID,PartialMolecule> contents() const=0;
    QHash<MoleculeID,PartialMolecule> contents(ForceFieldID ffid) const=0;
    QHash<MoleculeID,PartialMolecule> contents(ForceFieldID ffid,
                                               const FFBase::Group &group) const=0;

protected:
    void add(const FFExpression &ff_equation);
    void add(const QVector<FFExpression> &ff_equations);

    void remove(const Function &function);
    void remove(const QSet<Function> &functions);

    void removeAll();

    FFExpression take(const Function &function);
    QVector<FFExpression> take(const QSet<Function> &functions);

    QVector<FFExpression> takeAll();

    virtual QSet<ForceFieldID> getFFIDs(const QString &ffname) const=0;

    virtual Values getEnergies(ForceFieldID ffid)=0;
    virtual Values getEnergies(const QSet<ForceFieldID> &ffids)=0;
    virtual Values getEnergies(const QSet<FFComponent> &components)=0;

    virtual double getTotalEnergy()=0;

    virtual double getEnergy(const FFComponent &component)=0;

    virtual void assertValidComponents(const FFExpression &expression) const=0;

    virtual void add(MoleculeID molid, ForceFieldID ffid) throw();
    virtual void add(MoleculeID molid, const QSet<ForceFieldID> &ffids) throw();
    virtual void add(const QSet<MoleculeID> &molids, ForceFieldID ffid) throw();

    virtual void remove(MoleculeID molid, ForceFieldID ffid) throw();

    virtual void remove(ForceFieldID ffid) throw();

    virtual void changed(ForceFieldID ffid) throw();
    virtual void changed(const QSet<ForceFieldID> &ffids) throw();

    virtual const QHash< MoleculeID,QSet<ForceFieldID> >& getIndex() const;

private:

    /** Small internal class used by ForceFieldsBase to hold
        metainformation about the forcefield expressions used
        to calculate energies */
    class ExpressionInfo
    {

    friend QDataStream& ::operator<<(QDataStream&, const ForceFieldsBase&);
    friend QDataStream& ::operator>>(QDataStream&, ForceFieldsBase&);

    public:
        ExpressionInfo();

        ExpressionInfo(const FFExpression &ex,
                       const QHash<SymbolID,ExpressionInfo> &ff_equations);

        ExpressionInfo(const ExpressionInfo &other);

        ~ExpressionInfo();

        ExpressionInfo& operator=(const ExpressionInfo &other);

        /** Return the expression that is described by
            this object */
        const FFExpression& expression() const
        {
            return ex;
        }

        /** Return the ID of this function */
        SymbolID ID() const
        {
            return ex.function().ID();
        }

        /** Return the function */
        const Function& function() const
        {
            return ex.function();
        }

        /** Return all of the dependencies of this expression
            (all expressions on which this one depends, and before
            those, all that they depend on etc.) - this will
            return them in an order such that the first
            expressions don't depend on anything, and then
            each subsequent expression depends only on
            those that have come before. */
        const QVector<FFExpression>& dependencies() const
        {
            return deps;
        }

        /** Return the complete set of ID numbers of forcefields
            that contain components that are required by this
            expression, or required by any expression on
            which this depends. */
        const QSet<ForceFieldID>& forceFieldIDs() const
        {
            return ffids;
        }

    private:
        void validateDependencies(const FFExpression &ffexpression,
                                  const QHash<SymbolID,ExpressionInfo> &ff_equations);

        /** This is the actual expression */
        FFExpression ex;

        /** The list of expressions on which this expression depends */
        QVector<FFExpression> deps;

        /** The complete list of ForceFieldIDs of all of the forcefields
            on which this expression depends (includes dependencies from
            expressions that this depends on) */
        QSet<ForceFieldID> ffids;
    };

    double energy(const ExpressionInfo &expr, bool cache_result=true);

    /** An index that maps the ID of each molecule in the forcefields
        to the IDs of the force fields that contain that molecule */
    QHash< MoleculeID, QSet<ForceFieldID> > index;

    /** Hash mapping all of the different forcefield functions
        to their corresponding equation infos */
    QHash<SymbolID, ExpressionInfo> ff_equations;

    /** The symbols that depend on each forcefield */
    QHash< ForceFieldID, QSet<SymbolID> > ff_dependents;

    /** A collection of parameters used when evaluating the combined
        energy of these forcefields */
    Values ff_params;

    /** A cache of component energies (this is cleared whenever the
        system is changed) */
    QHash<SymbolID, double> cached_energies;

    /** The ID of the function representing the total energy of this
        system. */
    SymbolID total_id;
};

/** This is the default ForceFields class - this is the default
    class used to hold a collection of ForceFields, all of which
    will be evaluated using the local processor and local thread.

    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFields : public ForceFieldsBase
{

friend QDataStream& ::operator<<(QDataStream&, const ForceFields&);
friend QDataStream& ::operator>>(QDataStream&, ForceFields&);

public:
    ForceFields();

    ForceFields(const ForceFieldsBase &other);
    ForceFields(const ForceFields &other);

    ~ForceFields();

    ForceFields& operator=(const ForceFieldsBase &other);
    ForceFields& operator=(const ForceFields &other);

    QHash<ForceFieldID,ForceField> forceFields() const;

    QSet<ForceFieldID> forceFieldIDs() const;

    void setEqualTo(const ForceFields &forcefields);

    void majorUpdate(const ForceFields &forcefields);
    void minorUpdate(const ForceFields &forcefields);

    void add(const ForceField &ffield);

    void remove(ForceFieldID ffid);
    void remove(const QString &ffname);

    // Interface to ForceField functions (generalised over all ForceFields)
    void setProperty(const QString &name, const Property &property)=0;
    void setProperty(ForceFieldID ffid, const QString &name, const Property &property);

    void setProperty(const QSet<ForceFieldID> &ffids,
                     const QString &name, const Property &property);

    QSet<ForceFieldID,Property> getProperty(const QString &name) const;

    Property getProperty(ForceFieldID ffid, const QString &name) const;
    QSet<ForceFieldID,Property> getProperty(const QSet<ForceFieldID> &ffids,
                                            const QString &name) const;

    bool containsProperty(const QString &name) const;
    bool containsProperty(ForceFieldID ffid, const QString &name) const;
    QSet<ForceFieldID> forceFieldsWithProperty(const QString &name) const;

    void change(const PartialMolecule &molecule);
    void change(const QHash<MoleculeID,PartialMolecule> &molecules);

    void add(ForceFieldID ffid, const PartialMolecule &molecule);
    void add(ForceFieldID ffid, const QList<PartialMolecule> &molecules);

    void addTo(ForceFieldID ffid, const FFBase::Group &group,
               const PartialMolecule &molecule);
    void addTo(ForceFieldID ffid, const FFBase::Group &group,
               const QList<PartialMolecule> &molecules);

    void remove(const PartialMolecule &molecule);
    void remove(const QList<PartialMolecule> &molecules);
    void remove(ForceFieldID ffid, const PartialMolecule &molecule);
    void remove(ForceFieldID ffid, const QList<PartialMolecule> &molecules);

    void removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                    const PartialMolecule &molecule);
    void removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                    const QList<PartialMolecule> &molecules);

    bool contains(const PartialMolecule &molecule, ForceFieldID ffid) const;
    bool contains(const PartialMolecule &molecule,
                  ForceFieldID ffid, const FFBase::Group &group) const;

    QSet<ForceFieldID> forceFieldsContaining(const PartialMolecule &molecule) const;

    bool refersTo(MoleculeID molid) const;
    bool refersTo(MoleculeID molid, ForceFieldID ffid) const;
    bool refersTo(MoleculeID molid, ForceFieldID ffid,
                  const FFBase::Group &group) const;

    QSet<ForceFieldID> forceFieldsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs() const;
    QSet<MoleculeID> moleculeIDs(ForceFieldID ffid) const;
    QSet<MoleculeID> moleculeIDs(ForceFieldID ffid, const FFBase::Group &group) const;

    PartialMolecule molecule(MoleculeID molid, ForceFieldID ffid) const;

    QHash<MoleculeID,PartialMolecule> contents() const;
    QHash<MoleculeID,PartialMolecule> contents(ForceFieldID ffid) const;
    QHash<MoleculeID,PartialMolecule> contents(ForceFieldID ffid,
                                               const FFBase::Group &group) const;

protected:
    Values getEnergies(ForceFieldID ffid);
    Values getEnergies(const QSet<ForceFieldID> &ffids);
    Values getEnergies(const QSet<FFComponent> &components);

    double getEnergy(const FFComponent &component);

    double getTotalEnergy();

    QSet<ForceFieldID> getFFIDs(const QString &ffname) const;

    void assertValidComponents(const FFExpression &expression) const;

    void assertSameContents(const ForceFields &forcefields) const;

    void synchronise(ForceFieldID ffid);
    void synchronise(MoleculeID molid);
    void synchronise();

private:
    template<class T>
    bool _pvt_change(const T &obj);

    template<class T>
    bool _pvt_add(const T &obj, const QSet<ForceFieldID> &ffids,
                  const ParameterMap &map);

    template<class T>
    bool _pvt_remove(const T &obj, QSet<ForceFieldID> ffids);

    ForceField& getForceField(ForceFieldID ffid);
    const ForceField& getForceField(ForceFieldID ffid) const;

    const ForceField& getConstForceField(ForceFieldID ffid) const;

    /** All of the forcefields in this group, indexed by their ID */
    QHash<ForceFieldID, ForceField> ffields;
};

}

Q_DECLARE_METATYPE(SireFF::ForceFields);

SIRE_END_HEADER

#endif
