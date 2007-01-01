#ifndef SireFF_FFBASE_H
#define SireFF_FFBASE_H

#include <boost/scoped_ptr.hpp>

#include <QSet>

#include "SireBase/sharedpolypointer.hpp"
#include "SireBase/idmajminversion.h"

#include "SireCAS/function.h"
#include "SireCAS/values.h"

#include "SireMol/moleculeid.h"

#include "parametermap.h"
#include "ffcomponent.h"

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

class MoleculeID;
class ResNum;
class ResID;
}

namespace SireFF
{

class MovedMols;

using SireBase::Version;
using SireBase::IDMajMinVersion;

using SireCAS::Function;
using SireCAS::Values;

using SireMol::MoleculeID;
using SireMol::Molecule;
using SireMol::Residue;
using SireMol::ResNum;
using SireMol::ResID;

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
        Components(const FFBase &ffield);
        Components(const Components &other);

        virtual ~Components();

        virtual Components* clone() const=0;

        Components& operator=(const Components &other);

        const FFComponent& total() const
        {
            return e_total;
        }

        static QString describe_total();

        bool contains(const FFComponent &component) const
        {
            return symbolids.contains(component.ID());
        }

        void assertContains(const FFComponent &component) const;

    protected:
        virtual void setForceField(const FFBase &ffbase);

        void registerComponent(const FFComponent &component);

    private:
        /** The FFComponent representing the total energy
            of the forcefield */
        FFComponent e_total;

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

    virtual bool change(const Molecule &mol);
    virtual bool change(const Residue &res);

    virtual bool add(const Molecule &molecule, 
                     const ParameterMap &map = ParameterMap());
    virtual bool add(const Residue &residue,
                     const ParameterMap &map = ParameterMap());

    virtual bool remove(const Molecule &molecule);
    virtual bool remove(const Residue &residue);
    
    virtual bool replace(const Molecule &oldmol,
                         const Molecule &newmol,
                         const ParameterMap &map = ParameterMap());

    virtual bool contains(const Molecule &molecule) const;
    virtual bool contains(const Residue &residue) const;

    virtual Molecule molecule(MoleculeID molid) const;
    
    virtual Residue residue(MoleculeID molid, ResNum resnum) const;
    virtual Residue residue(MoleculeID molid, ResID resid) const;
    virtual Residue residue(MoleculeID molid, const QString &resname) const;
    
    virtual Molecule molecule(const Molecule &mol) const;
    virtual Residue residue(const Residue &res) const;

    bool isDirty() const;
    bool isClean() const;

    quint32 ID() const;
    const Version& version() const;

protected:
    void registerComponents(FFBase::Components *components);

    void incrementMajorVersion();
    void incrementMinorVersion();

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

    /** The ID number and version numbers of this forcefield. The
        major version number changes whenever the number of molecules
        in the forcefield changes - the minor number changes whenever
        the molecules themselves change (move or change parameters) */
    IDMajMinVersion id_and_version;

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

/** Internal function used by derived classes to increment the major
    number of the forcefield */
inline void FFBase::incrementMajorVersion()
{
    id_and_version.incrementMajor();
}

/** Internal function used by derived classes to increment the minor
    number of the forcefield */
inline void FFBase::incrementMinorVersion()
{
    id_and_version.incrementMinor();
}

/** Return the ID number of the forcefield */
inline quint32 FFBase::ID() const
{
    return id_and_version.ID();
}

/** Return the version number of the forcefield */
inline const Version& FFBase::version() const
{
    return id_and_version.version();
}

}

SIRE_END_HEADER

#endif
