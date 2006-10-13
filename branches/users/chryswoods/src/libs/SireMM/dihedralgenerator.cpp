
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/atom.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"

#include "dihedralgenerator.h"
#include "moldihedralinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireMM;

static const RegisterMetaType<DihedralGenerator>
                        r_dihedralgenerator("SireMM::DihedralGenerator");
static const RegisterMetaType<UsePassedDihedrals>
                        r_usepasseddihedrals("SireMM::UsePassedDihedrals");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const DihedralGenerator &generator)
{
    writeHeader(ds, r_dihedralgenerator, 0)
          << static_cast<const DihedralGeneratorBase&>(generator);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, DihedralGenerator &generator)
{
    VersionID v = readHeader(ds, r_dihedralgenerator);

    if (v == 0)
    {
        ds >> static_cast<DihedralGeneratorBase&>(generator);
    }
    else
        throw version_error(v, "0", r_dihedralgenerator, CODELOC);

    return ds;
}

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const UsePassedDihedrals &generator)
{
    writeHeader(ds, r_usepasseddihedrals, 0)
          << static_cast<const UsePassedDihedralsBase&>(generator);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, UsePassedDihedrals &generator)
{
    VersionID v = readHeader(ds, r_usepasseddihedrals);

    if (v == 0)
    {
        ds >> static_cast<UsePassedDihedrals&>(generator);
    }
    else
        throw version_error(v, "0", r_usepasseddihedrals, CODELOC);

    return ds;
}

/** Constructor */
DihedralGenerator::DihedralGenerator() : DihedralGeneratorBase()
{}

/** Copy constructor */
DihedralGenerator::DihedralGenerator(const DihedralGenerator &other)
              : DihedralGeneratorBase(other)
{}

/** Destructor */
DihedralGenerator::~DihedralGenerator()
{}

/** Internal function used to generate all of the dihedrals that involve
    the residue described by 'resinfo' */
void generateDihedrals(const MoleculeInfo &molinfo, const MoleculeBonds &molbonds,
                       const ResidueInfo &resinfo, MolDihedralInfo &dihedralinfo)
{
    //get all of the asymmetric bonds that involve this residue
    //(use asymmetric bonds to avoid counting some bonds twice!)

    QList<Bond> bonds = molbonds.residue(resinfo.resNum()).asymmetricBonds();

    //loop over each bond. A potential dihedral goes from atom0-atom1-atom2-atom3
    //If this bond is the bond between atom1-atom2 of a dihedral, then
    //atom1 will be bonded to atom0, atom2 will be bonded to atom3, and
    //atom0 will not be bonded to atom2 or atom3 and atom3 will not
    //be bonded to atom1 or atom0.

    //loop over each bond and test to see if it is a bond between
    //atom1-atom2 of a dihedral - if so then add the dihedral
    int nbnds = bonds.count();
    for (int i=0; i<nbnds; ++i)
    {
        const Bond &bond = bonds.at(i);

        //get the atoms involved in this bond
        const AtomInfo &atom1 = molinfo.atom(bond.atom0());
        const AtomInfo &atom2 = molinfo.atom(bond.atom1());

        //skip this dihedral if either of these atoms are dummies
        if (atom1.nProtons() + atom2.nProtons() == 0)
            continue;

        //get all of the bonds that involve atom1, then all that involve atom2
        QList<Bond> atom1_bonds = molbonds.bonds(atom1);
        QList<Bond> atom2_bonds = molbonds.bonds(atom2);

        //this is not part of a dihedral if atom1_bonds or atom2_bonds are empty
        if (atom1_bonds.isEmpty() or atom1_bonds.isEmpty())
            continue;

        //generate the set of possible 'atom0' atoms from atom1_bonds
        QSet<AtomIndex> atom0s;
        for (int j=0; j<atom1_bonds.count(); ++j)
            atom0s.insert( atom1_bonds.at(j).other(atom1) );

        //get the set of possible 'atom3' atoms from atom2_bonds
        QSet<AtomIndex> atom3s;
        for (int j=0; j<atom2_bonds.count(); ++j)
            atom3s.insert( atom2_bonds.at(j).other(atom2) );

        //now loop over all of the possible atom0's
        foreach( AtomIndex atom0, atom0s )
        {
            //is atom0 bonded to atom2? It will be if atom0
            //is contained in atom3s. if this is the case then
            //atom0-atom1-atom2 form a triangle, and thus
            //atom0 cannot be form a dihedral with atom1-atom2

            //equally, skip the atom if it is a dummy
            if ( atom3s.contains(atom0) or molinfo.atom(atom0).nProtons() == 0 )
                continue;

            //now loop over all of the possible atom3s
            foreach( AtomIndex atom3, atom3s )
            {
                //is atom3 bonded to atom0? If it is then
                //atom0-atom1-atom2-atom3 form a square, and thus
                //do not form a valid dihedral

                //equal, skip this atom if it is a dummy
                if ( not ( molbonds.bonded(atom0,atom3) or
                           molinfo.atom(atom3).nProtons() == 0 ) )
                {
                    //add the dihedral :-)
                    dihedralinfo.addDihedral( Dihedral(atom0,atom1,atom2,atom3) );
                }
            }
        }
    }
}

/** Find all of the dihedrals in the molecule 'mol' that don't involve dummy atoms
    and add them to 'dihedralinfo'. This uses the connectivity of the molecule
    to find the dihedrals. */
void DihedralGenerator::generate(const Molecule &mol, MolDihedralInfo &dihedralinfo) const
{
    //check that we are working with the same molecule!
    checkMolecule(mol, dihedralinfo);

    //get the connectivity of the molecule
    const MoleculeBonds &molbonds = mol.connectivity();

    //there are no dihedrals if there are no bonds!
    if (molbonds.isEmpty())
        return;

    //loop over each residue that has bonds...
    QSet<ResNum> resnums = molbonds.resNums();

    foreach( ResNum resnum, resnums )
    {
        //generate all of the dihedrals that involve this residue
        generateDihedrals( mol.info(), molbonds,
                           mol.info().residue(resnum), dihedralinfo );
    }
}

//////////////////////
////////////////////// Implementation of UsePassedDihedrals
//////////////////////

/** Constructor */
UsePassedDihedrals::UsePassedDihedrals() : UsePassedDihedralsBase()
{}

/** Construct to use the passed dihedrals */
UsePassedDihedrals::UsePassedDihedrals(const QSet<Dihedral> &dihedrals)
               : UsePassedDihedralsBase(dihedrals)
{}

/** Copy constructor */
UsePassedDihedrals::UsePassedDihedrals(const UsePassedDihedrals &other)
               : UsePassedDihedralsBase(other)
{}

/** Destructor */
UsePassedDihedrals::~UsePassedDihedrals()
{}

/** Generate the dihedrals - instantiated here to reduce over-template instantiation */
void UsePassedDihedrals::generate(const Molecule &mol, MolDihedralInfo &dihedralinfo) const
{
    UsePassedDihedralsBase::generate(mol, dihedralinfo);
}
