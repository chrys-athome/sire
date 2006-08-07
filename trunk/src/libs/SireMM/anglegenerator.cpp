
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/moleculecginfo.h"

#include "anglegenerator.h"
#include "molangleinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireMM;

static const RegisterMetaType<AngleGenerator> r_anglegenerator("SireMM::AngleGenerator");
static const RegisterMetaType<UsePassedAngles> r_usepassedangles("SireMM::UsePassedAngles");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AngleGenerator &generator)
{
    writeHeader(ds, r_anglegenerator, 0) 
          << static_cast<const AngleGeneratorBase&>(generator);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AngleGenerator &generator)
{
    VersionID v = readHeader(ds, r_anglegenerator);
    
    if (v == 0)
    {
        ds >> static_cast<AngleGeneratorBase&>(generator);
    }
    else
        throw version_error(v, "0", r_anglegenerator, CODELOC);
    
    return ds;
}

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const UsePassedAngles &generator)
{
    writeHeader(ds, r_usepassedangles, 0) 
          << static_cast<const UsePassedAnglesBase&>(generator);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, UsePassedAngles &generator)
{
    VersionID v = readHeader(ds, r_usepassedangles);
    
    if (v == 0)
    {
        ds >> static_cast<UsePassedAngles&>(generator);
    }
    else
        throw version_error(v, "0", r_usepassedangles, CODELOC);
    
    return ds;
}

/** Constructor */
AngleGenerator::AngleGenerator() : AngleGeneratorBase()
{}

/** Copy constructor */
AngleGenerator::AngleGenerator(const AngleGenerator &other)
              : AngleGeneratorBase(other)
{}

/** Destructor */
AngleGenerator::~AngleGenerator()
{}

/** Internal function used to generate all of the angles that involve
    the residue described by 'resinfo' */
void generateAngles(const Molecule &mol, const MoleculeBonds &molbonds,
                    const Residue &res, MolAngleInfo &angleinfo)
{
    //loop over each atom in this residue
    int nats = res.nAtoms();
    
    for (int i=0; i<nats; ++i)
    {
        const Atom &atom1 = res[i];
        
        //if this is a dummy atom then skip it
        if (atom1.nProtons() == 0)
            continue;
    
        //get all of the bonds that involve this atom
        QList<Bond> bonds = molbonds.bonds(atom1);
        
        //if there are no bonds involving this atom, or there
        //is only a single bond to this atom, then it cannot
        //be involved in any angles.
        if (bonds.count() <= 1)
            continue;
        
        //all of the 'other' atoms of these bonds must be
        //in angle relationships with one another, with this
        //atom as the central atom (unless the 'other' atoms
        //are bonded to one another, in which case they form a
        //triangle with this atom
        
        //get the list of 'other' atoms
        QList<AtomIndex> other_atoms;
        
        for (int j=0; j<bonds.count(); ++j)
            other_atoms.append( bonds.at(j).other(atom1) );
            
        //loop over each pair of 'other' atoms and add the angle
        //between them involving this atom, unless the pair of 
        //atoms are bonded
        int nothers = other_atoms.count();
        for (int j=0; j<nothers-1; ++j)
        {
            const Atom &atom0 = mol.atom(other_atoms.at(j));
        
            //only process this atom if it is not a dummy
            if (atom0.nProtons() == 0)
                continue;
        
            for (int k=j+1; k<nothers; ++k)
            {
                const Atom &atom2 = mol.atom(other_atoms.at(k));
                //only process this atom if it is not a dummy
                if ( atom2.nProtons() > 0 and 
                     not molbonds.bonded(atom0, atom2) )
                {
                    //atom0-atom1-atom2 must be an angle!
                    angleinfo.addAngle( Angle(atom0,atom1,atom2) );
                }
            }
        }
    }
}

/** Find all of the angles in the molecule 'mol' that don't involve dummy atoms
    and add them to 'angleinfo'. This uses the connectivity of the molecule
    to find the angles. */
void AngleGenerator::generate(const Molecule &mol, MolAngleInfo &angleinfo) const
{
    //check that we are working with the same molecule!
    checkMolecule(mol, angleinfo);

    //get the connectivity of the molecule
    MoleculeBonds molbonds = mol.connectivity();
    
    //there are no angles if there are no bonds!
    if (molbonds.isEmpty())
        return;
    
    //loop over each residue of the molecule
    int nres = mol.nResidues();
    for (int i=0; i<nres; ++i)
    {
        //get the ith residue
        Residue res = mol[i];
        
        //are there any bonds that involve atoms in this residue?
        if ( molbonds.contains(res.resNum()) )
        {
            //get all of the angles involving this residue
            generateAngles(mol, molbonds, res, angleinfo);
        }
    }
}

//////////////////////
////////////////////// Implementation of UsePassedAngles
//////////////////////

/** Constructor */
UsePassedAngles::UsePassedAngles() : UsePassedAnglesBase()
{}
  
/** Construct to use the passed angles */
UsePassedAngles::UsePassedAngles(const QSet<Angle> &angles)
               : UsePassedAnglesBase(angles)
{}

/** Copy constructor */
UsePassedAngles::UsePassedAngles(const UsePassedAngles &other)
               : UsePassedAnglesBase(other)
{}

/** Destructor */
UsePassedAngles::~UsePassedAngles()
{}

/** Generate the angles - instantiated here to reduce over-template instantiation */
void UsePassedAngles::generate(const Molecule &mol, MolAngleInfo &angleinfo) const
{
    UsePassedAnglesBase::generate(mol, angleinfo);
}
