#ifndef SireFF_FFBASE_H
#define SireFF_FFBASE_H

#include <boost/scoped_ptr.hpp>

#include <QSet>

#include "SireBase/sharedpolypointer.hpp"

#include "SireCAS/function.h"
#include "SireCAS/values.h"

#include "SireMol/moleculeid.h"

#include "parametermap.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFBase;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFBase&);
QDataStream& operator>>(QDataStream&, SireFF::FFBase&);

namespace SireMol
{
class Molecule;
class Residue;
}

namespace SireFF
{

class MovedMols;

using SireCAS::Function;
using SireCAS::Values;

using SireMol::MoleculeID;
using SireMol::Molecule;
using SireMol::Residue;

/**
This class is the base class of all of the forcefield classes. The forcefields all form
a polymorphic class hierarchy derived from this class. They are then held via
the SharedPolyPointer in ForceField, which provides the common user-interface to the
forcefields.

@author Christopher Woods
*/
class SIREFF_EXPORT FFBase : public QSharedData
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

    /** This encapsulated class must be derived by
        each inheriting forcefield to provide information
        about all of the components of the forcefield */
    class SIREFF_EXPORT Components
    {
    friend class FFBase;

    public:
        Components();
        Components(const QString &basename);
        Components(const Components &other);

        virtual ~Components();

        virtual Components* clone() const=0;

        const Function& total() const
        {
            return e_total;
        }

        static QString describe_total();

        bool contains(const Function &function) const
        {
            return symbolids.contains(function.ID());
        }

        void assertContains(const Function &function) const;

    protected:
        virtual void setBaseName(const QString &basename);

        void unregisterFunction(const Function &function);
        void registerFunction(const Function &function);

        static Function getFunction(const QString &root,
                                    const QString &component);

    private:
        /** The function representing the total energy
            of the forcefield */
        Function e_total;

        /** The SymbolIDs of all of the functions that
            represent the components */
        QSet<SireCAS::SymbolID> symbolids;
    };

    /** Return the object describing the components of this
        forcefield */
    const FFBase::Components& components() const
    {
        return *components_ptr;
    }

    /** This encapsulated class must be derived by all
        inheriting classes to provide the object
        that contains information about the parameters
        used by the forcefield, and the default source
        properties from which to obtain those parameters */
    class SIREFF_EXPORT Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);
        virtual ~Parameters();
    };

    virtual const FFBase::Parameters& parameters() const=0;

    const QString& name() const;
    void setName(const QString &name);

    double energy();
    double energy(const Function &component);

    Values energies();

    virtual bool move(const Molecule &mol)=0;
    virtual bool move(const Residue &res)=0;

    bool isDirty() const;
    bool isClean() const;

protected:
    void registerComponents(FFBase::Components *components);

    void setComponent(const Function &comp, double nrg);
    void changeComponent(const Function &comp, double delta);

    void setDirty();
    void setClean();

    Values currentEnergies() const;

    static QHash<MoleculeID,int> index(const QVector<Molecule> &molecules);

    /** Virtual function used to trigger a recalculation of the total energy
        and of all of the component energies */
    virtual void recalculateEnergy()=0;

private:
    /** The name of this forcefield - this may be used to give a unique
        name to all of the component-symbols in this forcefield. */
    QString ffname;

    /** All of the cached energy components in this forcefield, indexed
        by their symbol ID number (includes the total energy) */
    Values nrg_components;

    /** Active pointer to the object containing the information about all
        of the components of this forcefield */
    boost::scoped_ptr<FFBase::Components> components_ptr;

    /** Whether or not this forcefield is dirty (requires an update) */
    bool isdirty;
};

/** Set the energy value of the component 'comp' */
inline void FFBase::setComponent(const Function &comp, double nrg)
{
    nrg_components.set(comp,nrg);
}

/** Change the existing value of the component 'comp' by delta */
inline void FFBase::changeComponent(const Function &comp, double delta)
{
    nrg_components.set( comp, delta + nrg_components.value(comp) );
}

/** Return whether or not the forcefield is dirty (the energy
    needs to be recalculated) */
inline bool FFBase::isDirty() const
{
    return isdirty;
}

/** Return whether or not the forcefield is clean (the energy
    does not need to be recalculated) */
inline bool FFBase::isClean() const
{
    return not isDirty();
}

}

SIRE_END_HEADER

#endif
