#ifndef SireFF_FFBASE_H
#define SireFF_FFBASE_H

#include "SireCAS/function.h"
#include "SireCAS/values.h"

#include "SireMol/moleculeid.h"
//#include "SireMol/molresnumid.h"

#include "changedmols.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFBase;

namespace detail
{
class ComponentInfo;
}

}

QDataStream& operator<<(QDataStream&, const SireFF::FFBase&);
QDataStream& operator>>(QDataStream&, SireFF::FFBase&);

QDataStream& operator<<(QDataStream&, const SireFF::detail::ComponentInfo&);
QDataStream& operator>>(QDataStream&, SireFF::detail::ComponentInfo&);

namespace SireMol
{
class Molecule;
class Residue;
}

namespace SireDB
{
class ParameterTable;
}

namespace SireFF
{

class MovedMols;

using SireCAS::Function;
using SireCAS::Values;

using SireMol::MoleculeID;
//using SireMol::MolResNumID;
using SireMol::Molecule;
using SireMol::Residue;

using SireDB::ParameterTable;

namespace detail
{
    /** Small internal class used to hold information about a
        component of the forcefield 
        
        @author Christopher Woods
    */
    class ComponentInfo
    {
    
    friend QDataStream& ::operator<<(QDataStream&, const ComponentInfo&);
    friend QDataStream& ::operator>>(QDataStream&, ComponentInfo&);
    
    public:
        ComponentInfo();
        ComponentInfo(const QString &root, const QString &name,
                      const QString &description);
        
        ComponentInfo(const ComponentInfo &other);
        
        ~ComponentInfo();
        
        void setRoot(const QString &root);
        
        /** Return the component's name */
        const QString& name() const
        {
            return nme;
        }
        
        /** Return the component's description */
        const QString& description() const
        {
            return desc;
        }
        
        /** Return the function used to represent the component */
        const Function& function() const
        {
            return func;
        }
        
    private:
        /** The name of the component */
        QString nme;
        /** A description of the component */
        QString desc;
        /** The function used to represent the component */
        Function func;
    };
}

/**
This class is the base class of all of the forcefield classes. The forcefields all form
a polymorphic class hierarchy derived from this class. They are then held via
the DynamicSharedPtr in ForceField, which provides the common user-interface to the 
forcefields.

@author Christopher Woods
*/
class SIREFF_EXPORT FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const FFBase&);
friend QDataStream& ::operator>>(QDataStream&, FFBase&);

public:
    FFBase();
    FFBase(const QString &name);

    FFBase(const FFBase &other);

    virtual ~FFBase();

    /** Return the class name of the forcefield */
    virtual const char* what() const=0;

    /** Return a clone of this forcefield */
    virtual FFBase* clone() const=0;

    const QString& name() const;

    double energy();
    double energy(const Function &component);

    Values energies();

    static int TOTAL()
    {
        return 0;
    }

    const Function& total() const;

    const Function& component(int componentid) const;
    QList<Function> components() const;

    virtual const Molecule& molecule(MoleculeID molid) const=0;
    //virtual const Residue& residue(const MolResNumID &molresid) const=0;

    virtual void move(const MovedMols &movemols)=0;

    virtual void move(const Molecule &mol)=0;
    virtual void move(const Residue &res)=0;

    virtual void change(const ChangedMols &changemols)=0;

    virtual void change(const Molecule &mol, const ParameterTable &params)=0;
    virtual void change(const Residue &res, const ParameterTable &params)=0;

    virtual void add(const Molecule &mol, const ParameterTable &params,
                     int groupid)=0;

    virtual void add(const Residue &res, const ParameterTable &params,
                     int groupid)=0;

    virtual void remove(const Molecule &mol)=0;

    virtual void remove(const Residue &res)=0;

    void assertContains(const Function &component) const;

    void assertContains(const Molecule &molecule) const;
    void assertContains(const Residue &residue) const;
    
    void assertSameMajorVersion(const Molecule &molecule) const;
    void assertSameMajorVersion(const Residue &residue) const;
    
    void assertSameVersion(const Molecule &molecule) const;
    void assertSameVersion(const Residue &residue) const;

protected:
    void registerComponent(int id, const QString &name,
                           const QString &description);

    void setComponent(const Function &comp, double nrg);

    void addToRegister(const Molecule &molecule);
    void addToRegister(const Residue &residue);

    bool isDirty() const;

    void setDirty();
    void setClean();

    /** Virtual function used to trigger a recalculation of the total energy
        and of all of the component energies */
    virtual void recalculateEnergy()=0;

private:
    void registerComponents();
    
    /** The name of this forcefield - this may be used to give a unique
        name to all of the component-symbols in this forcefield. */
    QString ffname;

    /** Hash mapping component ID numbers to their function */
    QHash< int, detail::ComponentInfo > id_to_component;

    /** All of the cached energy components in this forcefield, indexed
        by their symbol ID number (includes the total energy) */
    Values nrg_components;

    /** Set of MoleculeID numbers of the molecules that are in this forcefield */
    QSet<MoleculeID> mols_in_ff;

    /** Set of MolResNumID numbers of residues that are in this forcefield that
        are here without their parent molecules */
    //QSet<MolResNumID> res_in_ff;
    
    /** Whether or not this forcefield is dirty (requires an update) */
    bool isdirty;
};

/** Set the energy value of the component 'comp' */
inline void FFBase::setComponent(const Function &comp, double nrg)
{
    nrg_components.set(comp,nrg);
}

}

SIRE_END_HEADER

#endif
