#ifndef SIREFF_FORCEFIELDS_H
#define SIREFF_FORCEFIELDS_H

#include "forcefield.h"
#include "ffexpression.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

class ForceFields;

/** This is the virtual base class of all of the ForceFields classes.
    ForceFields classes hold collections of forcefields, together with
    equations that evaluate combinations of those forcefields.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFieldsBase
{
public:
    ForceFieldsBase();
    
    ForceFieldsBase(const ForceFieldsBase &other);
    
    virtual ~ForceFieldsBase();

    void add(const FFExpression &ff_equation);
    void add(const QHash<Function,FFExpression> &ff_equations);

    void remove(const FFExpression &ff_equation);
    void remove(const QList<FFExpression> &ff_equations);
    void remove(const Function &function);
    void remove(const QSet<Function> &functions);

    bool contains(const Function &function) const;

    void replace(const FFExpression &newexpr);
    void replace(const FFExpression &oldexpr, const FFExpression &newexpr);
    void replace(const Function &oldfunc, const FFExpression &newexpr);
    
    double energy(const FFExpression &expression);
    double energy(const Function &component);
    double energy(const FFComponent &component);

    void setParameter(const Symbol &param, double value);

    QHash<ForceFieldID,ForceField> forceFields() const;

protected:
    virtual Values getEnergyComponents(ForceFieldID ffid)=0;
    virtual Values getEnergyComponents(const QSet<ForceFieldID> &ffids)=0;
    virtual Values getEnergyComponents(const QSet<FFComponent> &components)=0;

    virtual double getEnergyComponent(const FFComponent &component)=0;

    virtual void assertValidComponents(const QList<FFComponent> &components)=0;

    void add(MoleculeID molid, ForceFieldID ffid);
    void remove(MoleculeID molid, ForceFieldID ffid);

    void changed(ForceFieldID ffid);

private:
    
    /** Small internal class used by ForceFieldsBase to hold
        metainformation about the forcefield expressions used
        to calculate energies */
    class ExpressionInfo
    {

    friend QDataStream& ::operator<<(QDataStream&, const SystemData&);
    friend QDataStream& ::operator>>(QDataStream&, SystemData&);

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
        const QSet<ForceFieldID>& forcefieldIDs() const
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
};

/** This is the default ForceFields class - this is the default
    class used to hold a collection of ForceFields, all of which 
    will be evaluated using the local processor and local thread.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFields : class ForceFieldsBase
{

friend class ForceFieldsBase; //so can call protected constructor

public:
    ForceFields();
    
    ForceFields(const ForceFieldsBase &other);
    ForceFields(const ForceFields &other);
    
    ~ForceFields();

    void add(const ForceField &ffield);
    
    void remove(ForceFieldID ffid);
    void remove(const QString &ffname);

    void add(const Molecule &molecule, ForceFieldID ffid);
    void add(const Residue &residue, ForceFieldID ffid);
    void add(const NewAtom &atom, ForceFieldID ffid);
    
    void add(const Molecule &molecule, const QString &ffname);
    void add(const Residue &residue, const QString &ffname);
    void add(const NewAtom &atom, const QString &ffname);

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);
    
    bool remove(const Molecule &molecule);
    
    bool remove(const Molecule &molecule, ForceFieldID ffid);
    bool remove(const Residue &residue, ForceFieldID ffid);
    bool remove(const NewAtom &atom, ForceFieldID ffid);
    
    bool remove(const Molecule &molecule, const QString &ffname);
    bool remove(const Residue &residue, const QString &ffname);
    bool remove(const NewAtom &atom, const QString &ffname);

protected:
    ForceFields(const ForceFieldsBase &other,
                const QHash<ForceFieldID,ForceField> &ffields);
    
    Values getEnergyComponents(ForceFieldID ffid);
    Values getEnergyComponents(const QSet<ForceFieldID> &ffids);
    Values getEnergyComponents(const QSet<FFComponent> &components);

    double getEnergyComponent(const FFComponent &component);

    void reindex();
    void synchronise();

private:
    /** All of the forcefields in this group, indexed by their ID */
    QHash<ForceFieldID, ForceField> ffields;
};

}

SIRE_END_HEADER

#endif
