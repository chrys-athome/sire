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

    QSet<ForceFieldID> forceFieldsContaining(MoleculeID molid) const;
    QSet<ForceFieldID> forceFieldsContaining(const Molecule &molecule) const;

    const FFExpression& total() const;

    virtual bool add(const Molecule &molecule, const QSet<ForceFieldID> &ffids,
                     const ParameterMap &map)=0;

    virtual bool add(const Residue &residue, const QSet<ForceFieldID> &ffids,
                     const ParameterMap &map)=0;

    virtual bool add(const NewAtom &atom, const QSet<ForceFieldID> &ffids,
                     const ParameterMap &map)=0;

    bool add(const Molecule &molecule, ForceFieldID ffid,
             const ParameterMap &map = ParameterMap());

    bool add(const Residue &residue, ForceFieldID ffid,
             const ParameterMap &map = ParameterMap());

    bool add(const NewAtom &atom, ForceFieldID ffid,
             const ParameterMap &map = ParameterMap());

    bool add(const Molecule &molecule, const QString &ffname,
             const ParameterMap &map = ParameterMap());

    bool add(const Residue &residue, const QString &ffname,
             const ParameterMap &map = ParameterMap());

    bool add(const NewAtom &atom, const QString &ffname,
             const ParameterMap &map = ParameterMap());

    virtual bool change(const Molecule &molecule)=0;
    virtual bool change(const Residue &residue)=0;
    virtual bool change(const NewAtom &atom)=0;

    virtual bool remove(const Molecule &molecule)=0;

    virtual bool remove(const Molecule &molecule, const QSet<ForceFieldID> &ffids)=0;
    virtual bool remove(const Residue &residue, const QSet<ForceFieldID> &ffids)=0;
    virtual bool remove(const NewAtom &atom, const QSet<ForceFieldID> &ffids)=0;

    bool remove(const Molecule &molecule, ForceFieldID ffid);
    bool remove(const Residue &residue, ForceFieldID ffid);
    bool remove(const NewAtom &atom, ForceFieldID ffid);

    bool remove(const Molecule &molecule, const QString &ffname);
    bool remove(const Residue &residue, const QString &ffname);
    bool remove(const NewAtom &atom, const QString &ffname);

    double energy(const FFExpression &expression);
    double energy(const Function &component);
    double energy(const FFComponent &component);

    double energy();

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

    ///indexing functions below /////////////

    void add(MoleculeID molid, ForceFieldID ffid) throw();
    void add(MoleculeID molid, const QSet<ForceFieldID> &ffids) throw();
    void add(const QSet<MoleculeID> &molids, ForceFieldID ffid) throw();

    void remove(MoleculeID molid, ForceFieldID ffid) throw();

    void remove(ForceFieldID ffid) throw();

    void changed(ForceFieldID ffid) throw();
    void changed(const QSet<ForceFieldID> &ffids) throw();

    /////////////////////////////////////////

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

    double energy(const ExpressionInfo &expr);

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

    void setEqualTo(const ForceFields &forcefields);
    
    void majorUpdate(const ForceFields &forcefields);
    void minorUpdate(const ForceFields &forcefields);

    void add(const ForceField &ffield);

    void remove(ForceFieldID ffid);
    void remove(const QString &ffname);

    bool add(const Molecule &molecule, const QSet<ForceFieldID> &ffids,
             const ParameterMap &map = ParameterMap());

    bool add(const Residue &residue, const QSet<ForceFieldID> &ffids,
             const ParameterMap &map = ParameterMap());

    bool add(const NewAtom &atom, const QSet<ForceFieldID> &ffids,
             const ParameterMap &map = ParameterMap());

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);

    bool remove(const Molecule &molecule, const QSet<ForceFieldID> &ffids);
    bool remove(const Residue &residue, const QSet<ForceFieldID> &ffids);
    bool remove(const NewAtom &atom, const QSet<ForceFieldID> &ffids);

    bool remove(const Molecule &molecule);

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
