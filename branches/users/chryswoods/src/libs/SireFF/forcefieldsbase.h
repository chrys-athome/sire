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

#ifndef SIREFF_FORCEFIELDSBASE_H
#define SIREFF_FORCEFIELDSBASE_H

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
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceFieldsBase&);
QDataStream& operator>>(QDataStream&, SireFF::ForceFieldsBase&);

namespace SireFF
{

using SireCAS::SymbolID;
using SireCAS::Function;
using SireCAS::Values;

class FFGroupID;

/** This is the virtual base class of all of the ForceFields classes.
    ForceFields classes hold collections of forcefields, together with
    equations that evaluate combinations of those forcefields.

    A ForceFields class holds lots of forcefields, with the constraint
    that only a single version of a molecule exists in all of the forcefields
    (so if a molecule is changed, than it is changed simultaneously in
    all forcefields, and if a molecule is added, then all forcefields
    are synchronised to the same version as the newly added molecule)

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

    virtual ForceField forceField(ForceFieldID ffid) const;

    virtual QHash<ForceFieldID,ForceField>
                forceFields(const QSet<ForceFieldID> &ffids) const;

    const Values& parameters() const;

    FFExpression expression(const Function &function) const;
    FFExpression expression(const Symbol &symbol) const;

    QHash<Function,FFExpression> expressions(const QSet<Function> &functions) const;
    QHash<Function,FFExpression> expressions() const;

    bool setParameter(const Symbol &param, double value);
    
    bool setTotal(const FFExpression &expression);
    bool setTotal(const Symbol &symbol);

    const Symbol& total() const;

    virtual bool contains(const Function &function) const;
    virtual bool contains(const ForceFieldID ffid) const;

    virtual QSet<ForceFieldID> forceFieldIDs() const=0;

    int nMolecules() const;
    int nForceFields() const;

    double energy(const Expression &expression);
    double energy(const FFExpression &expression);
    double energy(const Function &component);
    double energy(const Symbol &symbol);
    double energy(SymbolID symbolid);

    double energy(const FFComponent &component);

    double energy();

    Values energies();
    Values energies(const QSet<FFComponent> &components);
    Values energies(const QSet<Function> &components);

    virtual bool setProperty(const QString &name, const Property &property);
    virtual bool setProperty(ForceFieldID ffid,
                             const QString &name, const Property &property)=0;

    virtual bool setProperty(const QSet<ForceFieldID> &ffids,
                             const QString &name, const Property &property);

    virtual QHash<ForceFieldID,Property> getProperty(const QString &name) const;

    virtual Property getProperty(ForceFieldID ffid, const QString &name) const;
    virtual QHash<ForceFieldID,Property> getProperty(const QSet<ForceFieldID> &ffids,
                                                     const QString &name) const;

    virtual QHash< QString, QHash<ForceFieldID,Property> > properties() const=0;

    virtual QHash< QString, QHash<ForceFieldID,Property> >
                    properties(const QSet<ForceFieldID> &ffids) const;

    virtual bool containsProperty(const QString &name) const;
    virtual bool containsProperty(ForceFieldID ffid, const QString &name) const;
    virtual QSet<ForceFieldID> forceFieldsWithProperty(const QString &name) const;

    virtual void mustNowRecalculateFromScratch()=0;

    virtual bool change(const PartialMolecule &molecule)=0;
    virtual bool change(const Molecules &molecules);

    virtual bool add( ForceFieldID ffid,
                      const PartialMolecule &molecule,
                      const ParameterMap &map = ParameterMap() );

    virtual bool add( ForceFieldID ffid,
                      const Molecules &molecules,
                      const ParameterMap &map = ParameterMap() );

    virtual bool add( const QSet<ForceFieldID> &ffids,
                      const PartialMolecule &molecule,
                      const ParameterMap &map = ParameterMap() );

    virtual bool add( const QSet<ForceFieldID> &ffids,
                      const Molecules &molecules,
                      const ParameterMap &map = ParameterMap() );

    virtual bool addTo( ForceFieldID ffid, const FFBase::Group &group,
                        const PartialMolecule &molecule,
                        const ParameterMap &map = ParameterMap() )=0;

    virtual bool addTo( ForceFieldID ffid, const FFBase::Group &group,
                        const Molecules &molecules,
                        const ParameterMap &map = ParameterMap() );

    virtual bool addTo( const FFGroupID &ffgroupid,
                        const PartialMolecule &molecule,
                        const ParameterMap &map = ParameterMap() );

    virtual bool addTo( const FFGroupID &ffgroupid,
                        const Molecules &molecules,
                        const ParameterMap &map = ParameterMap() );

    virtual bool addTo( const QSet<FFGroupID> &ffgroupids,
                        const PartialMolecule &molecule,
                        const ParameterMap &map = ParameterMap() );

    virtual bool addTo( const QSet<FFGroupID> &ffgroupids,
                        const Molecules &molecules,
                        const ParameterMap &map = ParameterMap() );

    virtual bool remove(const PartialMolecule &molecule);
    virtual bool remove(const Molecules &molecules);

    virtual bool remove(ForceFieldID ffid, const PartialMolecule &molecule);
    virtual bool remove(ForceFieldID ffid, const Molecules &molecules);

    virtual bool remove(const QSet<ForceFieldID> &ffids,
                        const PartialMolecule &molecule);
    virtual bool remove(const QSet<ForceFieldID> &ffids,
                        const Molecules &molecules);

    virtual bool removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                            const PartialMolecule &molecule)=0;
    virtual bool removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                            const Molecules &molecules);

    virtual bool removeFrom(const FFGroupID &ffgroupid,
                            const PartialMolecule &molecule);
    virtual bool removeFrom(const FFGroupID &ffgroupid,
                            const Molecules &molecules);

    virtual bool removeFrom(const QSet<FFGroupID> &ffgroupids,
                            const PartialMolecule &molecule);
    virtual bool removeFrom(const QSet<FFGroupID> &ffgroupids,
                            const Molecules &molecules);

    bool refersTo(MoleculeID molid) const;
    bool refersTo(MoleculeID molid, ForceFieldID ffid) const;
    bool refersTo(MoleculeID molid, const QSet<ForceFieldID> &ffids) const;

    virtual bool refersTo(MoleculeID molid, ForceFieldID ffid,
                          const FFBase::Group &group) const=0;

    bool refersTo(MoleculeID molid, const FFGroupID &ffgroupid) const;
    bool refersTo(MoleculeID molid, const QSet<FFGroupID> &ffgroupids) const;

    QSet<ForceFieldID> forceFieldsReferringTo(MoleculeID molid) const;
    virtual QSet<FFGroupID> forceFieldGroupsReferringTo(MoleculeID molid) const=0;

    virtual bool contains(const PartialMolecule &molecule) const;

    virtual bool contains(const PartialMolecule &molecule, ForceFieldID ffid) const;
    virtual bool contains(const PartialMolecule &molecule,
                          const QSet<ForceFieldID> &ffids) const;

    virtual bool contains(const PartialMolecule &molecule,
                          ForceFieldID ffid, const FFBase::Group &group) const;

    virtual bool contains(const PartialMolecule &molecule,
                          const FFGroupID &ffgroupid) const;

    virtual bool contains(const PartialMolecule &molecule,
                          const QSet<FFGroupID> &ffgroupids) const;

    virtual QSet<ForceFieldID> forceFieldsContaining(
                                    const PartialMolecule &molecule) const;

    virtual QSet<FFGroupID> forceFieldGroupsContaining(
                                    const PartialMolecule &molecule) const;

    QSet<MoleculeID> moleculeIDs() const;
    virtual QSet<MoleculeID> moleculeIDs(ForceFieldID ffid) const;
    virtual QSet<MoleculeID> moleculeIDs(ForceFieldID ffid,
                                         const FFBase::Group &group) const=0;

    QSet<MoleculeID> moleculeIDs(const FFGroupID &ffgroupid) const;

    virtual PartialMolecule molecule(MoleculeID molid) const;
    virtual PartialMolecule molecule(MoleculeID molid, ForceFieldID ffid) const;
    virtual PartialMolecule molecule(MoleculeID molid,
                                     const QSet<ForceFieldID> &ffids) const;

    virtual PartialMolecule molecule(MoleculeID molid, ForceFieldID ffid,
                                     const FFBase::Group &group) const=0;

    PartialMolecule molecule(MoleculeID molid, const FFGroupID &ffgroupid) const;

    virtual PartialMolecule molecule(MoleculeID molid,
                                     const QSet<FFGroupID> &ffgroupids) const;

    virtual Molecules contents() const;
    virtual Molecules contents(ForceFieldID ffid) const;
    virtual Molecules contents(const QSet<ForceFieldID> &ffids) const;
    virtual Molecules contents(ForceFieldID ffid, const FFBase::Group &group) const;

    Molecules contents(const FFGroupID &ffgroupid) const;
    Molecules contents(const QSet<FFGroupID> &ffgroupids) const;

    Molecules molecules() const;
    Molecules molecules(ForceFieldID ffid) const;
    Molecules molecules(const QSet<ForceFieldID> &ffids) const;

    Molecules molecules(ForceFieldID ffid, const FFBase::Group &group) const;
    Molecules molecules(const FFGroupID &ffgroupid) const;
    Molecules molecules(const QSet<FFGroupID> &ffgroupids) const;

    virtual void assertValidComponents(const FFExpression &expression) const=0;
    virtual void assertSameContents(const ForceFieldsBase &other) const=0;

protected:
    virtual void reindex()=0;

    void clearIndex();

    void add(const FFExpression &ff_equation);
    void add(const QVector<FFExpression> &ff_equations);

    void remove(const Function &function);
    void remove(const QSet<Function> &functions);

    void removeAll();

    virtual void _pvt_copy(const ForceFieldsBase &forcefields)=0;

    FFExpression take(const Function &function);
    QHash<Function,FFExpression> take(const QSet<Function> &functions);

    QHash<Function,FFExpression> takeAll();

    virtual QSet<ForceFieldID> getFFIDs(const QString &ffname) const=0;

    virtual Values getEnergies(ForceFieldID ffid)=0;
    virtual Values getEnergies(const QSet<ForceFieldID> &ffids)=0;
    virtual Values getEnergies(const QSet<FFComponent> &components)=0;

    virtual double getTotalEnergy()=0;

    virtual double getEnergy(const FFComponent &component)=0;

    virtual void addToIndex(MoleculeID molid, ForceFieldID ffid) throw();
    virtual void addToIndex(MoleculeID molid, const QSet<ForceFieldID> &ffids) throw();
    virtual void addToIndex(const QSet<MoleculeID> &molids, ForceFieldID ffid) throw();

    virtual void removeFromIndex(MoleculeID molid, ForceFieldID ffid) throw();

    virtual void removeFromIndex(ForceFieldID ffid) throw();

    virtual void changed(ForceFieldID ffid) throw();
    virtual void changed(const QSet<ForceFieldID> &ffids) throw();
    virtual void changedAll() throw();

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

    /** The symbol of the function representing the total energy of this
        system. */
    Symbol total_nrg;
};

}

SIRE_END_HEADER

#endif
