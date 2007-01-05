/**
  * @file
  *
  * C++ Interface: MolproFF
  *
  * Description: Interface for MolproFF
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SQUIRE_MOLPROFF_H
#define SQUIRE_MOLPROFF_H

#include <QVector>

#include "SireFF/parametermap.h"

#include "SireFF/ffbase.h"
#include "SireMM/chargeparameter.h"

class QTextStream;

SIRE_BEGIN_HEADER

namespace Squire
{
class MolproFF;
}

QDataStream& operator<<(QDataStream&, const Squire::MolproFF&);
QDataStream& operator>>(QDataStream&, Squire::MolproFF&);

namespace SireMM
{
class ChargeTable;
}

namespace Squire
{

class MolproCalculator;
class MolproSession;

using SireMol::Molecule;
using SireMol::MoleculeID;
using SireMol::Residue;

using SireFF::ParameterMap;
using SireFF::ParameterName;

using SireMM::ChargeTable;
using SireMM::ChargeParameter;

using SireCAS::Values;
using SireCAS::Function;

using SireStream::VersionID;

/** This class implements a forcefield that calculates the QM energy
    of atoms in the QM region as they are polarised by the charges
    of atoms in the MM region. This forcefield uses Molpro to perform
    the energy calculation, and therefore must be placed on a
    MolproProcessor to work. Note that the current version only
    supports entire molecules in the QM or MM regions. Later
    versions of this forcefield will expand this support to
    include parts of molecules, and defined link atoms.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproFF : public SireFF::FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolproFF&);
friend QDataStream& ::operator>>(QDataStream&, MolproFF&);

friend class MolproCalculator; //friend so can all protected functions
                               //used to recalculate the energy

public:
    MolproFF();
    MolproFF(const QString &name);

    MolproFF(const ForceField &forcefield);

    MolproFF(const MolproFF &other);

    ~MolproFF();

    MolproFF& operator=(const MolproFF &other);
    MolproFF& operator=(const ForceField &forcefield);

    class SQUIRE_EXPORT Components : public SireFF::FFBase::Components
    {
    public:
        Components();
        Components(const QString &basename);

        Components(const Components &other);

        ~Components();

        Components* clone() const
        {
            return new Components(*this);
        }

        const Function& qm() const
        {
            return e_qm;
        }

        static QString describe_qm();

    protected:
        void setBaseName(const QString &basename);

    private:
        /** The QM component */
        Function e_qm;
    };

    class SQUIRE_EXPORT Parameters : public SireFF::FFBase::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();

        /** Return the default source of the coulomb parameters (charges) */
        const ParameterName& coulomb() const
        {
            return coulomb_params;
        }

        static Parameters default_sources;

     private:
        /** The name and default source property of the coulomb parameters
            of the MM region */
        ParameterName coulomb_params;
    };

    const Parameters& parameters() const
    {
        return MolproFF::Parameters::default_sources;
    }

    const MolproFF::Components& components() const
    {
        return *components_ptr;
    }

    static const char* typeName()
    {
        return "Squire::MolproFF";
    }

    const char* what() const
    {
        return MolproFF::typeName();
    }

    MolproFF* clone() const
    {
        return new MolproFF(*this);
    }

    void addToQM(const Molecule &molecule);
    void addToMM(const Molecule &molecule,
                 const ParameterMap &map = ParameterMap());

    void addToQM(const QList<Molecule> &molecules);
    void addToMM(const QList<Molecule> &molecules,
                 const ParameterMap &map = ParameterMap());

    bool move(const Molecule &mol);
    bool move(const Residue &res);

protected:
    void recalculateEnergy();  //throw an exception

    bool updateArrays();

    //protected functions designed to be overloaded by child classes, and
    //only called by MolproCalculator
    virtual void initialise(QTextStream &ts) const;
    virtual Values recalculateEnergy(MolproSession &session);

private:
    void registerComponents();

    void _pvt_addToQM(const Molecule &molecule);
    void _pvt_addToMM(const Molecule &molecule, const ParameterMap &map);

    /** Pointer to the object holding the descriptions of the
        components in this forcefield */
    const MolproFF::Components *components_ptr;
};

}

Q_DECLARE_METATYPE(Squire::MolproFF)

SIRE_END_HEADER

#endif
