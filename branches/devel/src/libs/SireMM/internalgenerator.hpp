#ifndef SIREMM_INTERNALGENERATOR_HPP
#define SIREMM_INTERNALGENERATOR_HPP

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include <QSet>

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Internals>
class InternalGenerator;

template<class Internals>
class UsePassedInternals;
}

template<class Internals>
QDataStream& operator<<(QDataStream&, const SireMM::InternalGenerator<Internals>&);
template<class Internals>
QDataStream& operator>>(QDataStream&, SireMM::InternalGenerator<Internals>&);

template<class Internals>
QDataStream& operator<<(QDataStream&, const SireMM::UsePassedInternals<Internals>&);
template<class Internals>
QDataStream& operator>>(QDataStream&, SireMM::UsePassedInternals<Internals>&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::MoleculeInfo;

/** This is the pure-virtual base class of all internal generators.
    Internal generators are used to generate the internals for a molecule
    and to add them to the Internals internal-info object.
    
    \author Christopher Woods
*/
template<class Internals>
class SIREMM_EXPORT InternalGenerator
{
public:
    typedef typename Internals::internal_type internal_type;
    typedef internal_type Internal;

    InternalGenerator()
    {}
    
    InternalGenerator(const InternalGenerator<Internals>&)
    {}
    
    virtual ~InternalGenerator()
    {}
    
    virtual void generate(const Molecule &molecule, 
                          Internals &internalinfo) const=0;

    Internals generate(const Molecule &molecule) const
    {
        Internals internals;
        this->generate(molecule, internals);
        return internals;
    }

    virtual InternalGenerator<Internals>* clone() const=0;
    virtual const char* what() const=0;

protected:
    /** Ensure that the info object is compatible with the molecule - this throws
        an exception if the molecule.info() != internalinfo.info()
        
        \throw SireError::incompatible_error
    */
    void checkMolecule(const Molecule &molecule, Internals &internalinfo) const
    {
        if (molecule.info() != internalinfo.info())
            throw SireError::incompatible_error(QObject::tr(
                    "Cannot place the internals for the molecule \"%1\" in "
                    "the info object for molecule \"%2\" as the molecule info "
                    "objects are not the same.")
                        .arg(molecule.name(), internalinfo.info().toString()),
                            CODELOC);
    }
};

/** This is a small generator that adds only the internals supplied in the set

    @author Christopher Woods
*/
template<class Internals>
class SIREMM_EXPORT UsePassedInternals : public InternalGenerator<Internals>
{

friend QDataStream& ::operator<<<>(QDataStream&, const UsePassedInternals<Internals>&);
friend QDataStream& ::operator>><>(QDataStream&, UsePassedInternals<Internals>&);

public:
    typedef typename Internals::internal_type internal_type;
    typedef internal_type Internal;
    
    UsePassedInternals() : InternalGenerator<Internals>()
    {}
    
    UsePassedInternals(const QSet<Internal> &internals)
                  : InternalGenerator<Internals>(), add_internals(internals)
    {}
    
    UsePassedInternals(const UsePassedInternals<Internals> &other)
                  : InternalGenerator<Internals>(other),
                    add_internals(other.add_internals)
    {}
    
    ~UsePassedInternals()
    {}
    
    /** This just adds all of the internals contained in this object to 
        the info object 'internalinfo'. Exceptions are thrown if the molecule
        and info object are incompatible, or if any of the atoms referenced
        in the internals are not contained in the molecule.
        
        \throw SireError::incompatible_error
        \throw SireMol::missing_atom
    */
    void generate(const Molecule &molecule, Internals &internalinfo) const
    {
        //check the this is the right molecule for this internal
        checkMolecule(molecule, internalinfo);
    
        //now check that all of the listed internals are actually in the
        //molecule
        for (typename QSet<Internal>::const_iterator it = add_internals.begin();
             it != add_internals.end();
             ++it)
        {
            const Internal &internal = *it;
            
            QStringList missing_atms;
            
            for (int i=0; i<internal.count(); ++i)
            {
                if (not molecule.info().contains(internal[i]))
                    missing_atms.append( internal[i].toString() );
            }
            
            if (not missing_atms.isEmpty())
                throw SireMol::missing_atom( QObject::tr(
                    "Cannot add %1 as the molecule \"%2\" does not contain %3")
                        .arg(internal.toString(), molecule.name(),
                             missing_atms.join(", ")), CODELOC );
        }
        
        //ok - all of the internals exist in the molecule, so we
        //can add them to the internalinfo object without worrying
        //about an exception being thrown half-way through
        for (typename QSet<Internal>::const_iterator it = add_internals.begin();
             it != add_internals.end();
             ++it)
        {
            internalinfo.addInternal(*it);
        }
    }

private:
    /** The internals to be added to the table */
    QSet<Internal> add_internals;
};

const SireStream::MagicID internalgenerator_magic = SireStream::getMagic(
                                                          "SireMM::InternalGenerator");

const SireStream::MagicID usepassedinternals_magic = SireStream::getMagic(
                                                          "SireMM::UsePassedInternals");

}

/** Serialise to a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, 
                        const SireMM::InternalGenerator<Internals>&)
{
    SireStream::writeHeader(ds, SireMM::internalgenerator_magic, 0);
             
    return ds;
}

/** Deserialise from a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, 
                        SireMM::InternalGenerator<Internals>&)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::internalgenerator_magic,
                                                     "SireMM::InternalGenerator");
                                                     
    if (v != 0)
        throw SireStream::version_error(v, "0", "SireMM::InternalGenerator", CODELOC);
        
    return ds;
}

/** Serialise to a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, 
                        const SireMM::UsePassedInternals<Internals> &assigner)
{
    SireStream::writeHeader(ds, SireMM::usepassedinternals_magic, 1)
                  << assigner.add_internals
                  << static_cast<const SireMM::InternalGenerator<Internals>&>(assigner);
             
    return ds;
}

/** Deserialise from a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, 
                        SireMM::UsePassedInternals<Internals> &assigner)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::usepassedinternals_magic,
                                                     "SireMM::UsePassedInternals");
                                                     
    if (v == 1)
    {
        ds >> assigner.add_internals
           >> static_cast<SireMM::InternalGenerator<Internals>&>(assigner);
    }
    else
        throw SireStream::version_error(v, "1", "SireMM::UsePassedInternals", CODELOC);
        
    return ds;
}

SIRE_END_HEADER

#endif
