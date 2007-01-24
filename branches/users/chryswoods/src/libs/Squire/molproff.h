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

#include "SireFF/ffbase.h"

#include "SireVol/space.h"
#include "SireMM/switchingfunction.h"

class QTextStream;

SIRE_BEGIN_HEADER

namespace Squire
{
class MolproFF;
}

QDataStream& operator<<(QDataStream&, const Squire::MolproFF&);
QDataStream& operator>>(QDataStream&, Squire::MolproFF&);

namespace Squire
{

class MolproCalculator;
class MolproSession;

using SireCAS::Symbols;
using SireCAS::Values;

using SireMol::Molecule;
using SireMol::MoleculeID;
using SireMol::Residue;
using SireMol::NewAtom;

using SireFF::FFComponent;
using SireFF::ParameterMap;
using SireFF::ParameterName;

using SireVol::Space;

using SireMM::SwitchingFunction;

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

friend class MolproSession;    //friend so can all protected functions
friend class MolproCalculator; //used to recalculate the energy

public:
    MolproFF();

    MolproFF(const Space &space,
             const SwitchingFunction &switchfunc);

    MolproFF(const MolproFF &other);

    ~MolproFF();

    MolproFF& operator=(const MolproFF &other);

    class SQUIRE_EXPORT Components : public SireFF::FFBase::Components
    {
    public:
        Components();
        Components(const FFBase &ffbase, const Symbols &symbols = Symbols());

        Components(const Components &other);

        ~Components();

        Components& operator=(const Components &other);

        Components* clone() const
        {
            return new Components(*this);
        }

        const FFComponent& qm() const
        {
            return e_qm;
        }

        static QString describe_qm();

    protected:
        void setForceField(const FFBase &ffbase);

    private:
        /** The QM component */
        FFComponent e_qm;
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

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);

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

    /** The space for this forcefield - this is used to map
        to infinite cartesian space for the QM calculation
        (only maps the position of the MM parts) */
    Space spce;

    /** The switching function used to scale down the MM atom
        charges */
    SwitchingFunction switchfunc;

    /** Array containing all of the coordinates of the QM atoms */
    QVector<double> qm_coords;

    /** Array containing all of the coordinates and charges
        of the MM atoms */
    QVector<double> mm_coords_and_charges;

    /** Pointer to the object containing the components of
        this forcefield */
    const MolproFF::Components *components_ptr;
};

}

Q_DECLARE_METATYPE(Squire::MolproFF);

SIRE_END_HEADER

#endif
