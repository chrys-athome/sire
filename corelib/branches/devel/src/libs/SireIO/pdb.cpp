/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "SireMol/qhash_siremol.h"

#include <QTextStream>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "pdb.h"

#include "SireMol/elementdb.h"
#include "SireMol/editmol.h"
#include "SireMol/cutgroup.h"
#include "SireMol/editres.h"
#include "SireMol/bond.h"
#include "SireMol/errors.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/atom.h"
#include "SireMol/residatomid.h"
#include "SireMol/resnumatomid.h"

#include "SireBase/increment.h"
#include "SireError/errors.h"
#include "SireUnits/units.h"
#include "SireUnits/convert.h"

using namespace SireMol;
using namespace SireIO;

using SireUnits::angstrom;
using SireUnits::convertTo;

PDB::PDB() : IOBase()
{}

PDB::~PDB()
{}

QList<Molecule> PDB::readMols(const QByteArray &data,
                              const CuttingFunction &cutfunc) const
{
    //read through the file and check the elements
    QTextStream ts(data,QIODevice::ReadOnly);

    //now create an EditMol to hold the first read molecule, and
    //pass it the cutting function used to assign atoms to cutgroups
    EditMol mol(cutfunc);

    //use an index to map atom numbers to loaded atoms
    QHash<int,Atom> atomindex;
    QHash<int,int> molindex;

    //create space to hold the loaded molecules
    QList<EditMol> loadedmols;

    //create the first molecule...
    EditMol currentmol( QString("PDB_1") );

    for (QString line = ts.readLine(); not line.isNull(); line = ts.readLine())
    {
        QString lowline = line.toLower();

        if (lowline.startsWith("atom") or lowline.startsWith("hetatm"))
        {
            //This is a typical PDB line. It is column based!
            //000000000011111111112222222222333333333344444444445555555555666666666677777777778
            //012345678901234567890123456789012345678901234567890123456789012345678901234567890
            //ATOM    120 2HB  TRP A   7       3.314  -3.019  -5.136  1.00  0.00           H
            //ATOM   1899 1HE2 GLN A 123       9.012 -12.715   9.848  1.00  0.00           H
            //ATOM     16 Cl2  <1>     1      -4.385   2.853  -0.636  1.00  0.00          CL
            //
            //columns 6-10 contain the atom number. This is only used if the PDB file contains
            //'CONECT' lines (that describe which atoms are bonded together). Note that
            //the CONECT lines will break if the PDB file contains > 99999 atoms!
            //columns 12-15 contain atom name, 17-20 contain residue name, 22-25 the
            //residue number, 30-37, 38-45 and 46-53 the coordinates, and 76-77 contain
            //the element (the element is rarely present)

            QString atmnam = line.mid(12,4).simplified();
            QString resnam = line.mid(17,4).simplified();
            AtomNum atmnum( line.mid(6,5).toInt() );
            ResNum resnum( line.mid(22,4).toInt() );

            //the x,y,z coordinates are in A
            double x = line.mid(30,8).toDouble() * angstrom;
            double y = line.mid(38,8).toDouble() * angstrom;
            double z = line.mid(46,8).toDouble() * angstrom;

            QString el = line.mid(76,2).simplified();

            if (el.isEmpty())
            {
                //there is no element column, so try to work from the atom name...
                el = atmnam;
            }

            //PDB files are generally used with biological molecules, so try to
            //get a biological element from the name (bio elements are in rows 1 and 2,
            //i.e. their proton number is < 18 and not noble gas
            Element elmnt = Element::biologicalElement(el);

            //turn this data into an atom
            Atom addatm( atmnum, AtomIndex(atmnam,resnum), elmnt, Vector(x,y,z) );

            //get the residue that will contain the atom...
            if (not currentmol.contains(resnum))
                currentmol.add(resnum, resnam);
            else if (currentmol.residueName(resnum) != resnam)
                throw SireMol::duplicate_residue(QObject::tr(
                    "Cannot have two residues with number %1 but with different names! "
                    "(\"%1\" vs. \"%2\")").arg(resnum).arg(currentmol.residueName(resnum))
                        .arg(resnam), CODELOC);

            //try to add this atom to the molecule - this may throw
            //a SireMol::duplicate_atom exception if this atom name is
            //already in use!
            try
            {
                currentmol.add(addatm);
            }
            catch(SireMol::duplicate_atom &error)
            {
                //come up with a new name, and try that...
                QString newname = SireBase::increment(addatm.name());

                while( currentmol.contains( AtomIndex(newname,resnum) ) )
                    newname = SireBase::increment(newname);

                addatm = Atom(newname,addatm);

                currentmol.add( addatm );
            }

            //index the added atom by the pdb atom number
            molindex.insert(atmnum, loadedmols.count()-1);
            atomindex.insert(atmnum, addatm);
        }
        else if (lowline.startsWith("conect"))
        {
            //this is a PDB 'conect' line, that says which atoms are bonded together...
            //The first number is the first atom in the bond, the remaining numbers refer
            //to all of the atoms that this atom is bonded to
            int lgth = line.length();
            if (lgth > 11)
            {
                //get the first atom in the bond.
                bool ok;
                int atm0 = line.mid(6,5).toInt(&ok);

                if (ok)
                {
                    //get the atom and EditMol associated with this PDB atom number
                    if (not molindex.contains(atm0))
                    {
                        QString err = QObject::tr("No atom %1 in the PDB").arg(atm0);
                        throw SireMol::missing_atom(err,CODELOC);
                    }

                    EditMol &mol = loadedmols[ molindex.value(atm0) ];
                    Atom atom0 = atomindex.value(atm0);

                    if (not mol.contains(atom0))
                    {
                        QString err = QObject::tr("Missing atom number %1 (%2) in molecule %3. "
                                "(Bonds can only be added between atoms in the same molecule)")
                                .arg(atm0).arg(atom0.toString()).arg(mol.name());
                        throw SireMol::missing_atom(err,CODELOC);
                    }

                    int strtpos = 6;
                    while(ok)
                    {
                        strtpos += 6;
                        //read in the next atom that we're bonded to...
                        int atm1 = line.mid(strtpos,5).toInt(&ok);
                        if (ok)
                        {
                            if (not atomindex.contains(atm1))
                            {
                                QString err = QObject::tr("No atom %1 in the PDB").arg(atm1);
                                throw SireMol::missing_atom(err,CODELOC);
                            }

                            Atom atom1 = atomindex.value(atm1);

                            if (not mol.contains(atom1))
                            {
                                QString err =
                                    QObject::tr("Missing atom number %1 (%2) in molecule %3. "
                                    "(Bonds can only be added between atoms in the same molecule)")
                                    .arg(atm1).arg(atom1.toString()).arg(mol.name());
                                throw SireMol::missing_atom(err,CODELOC);
                            }
                            //add the bond...
                            mol.add( Bond(atom0,atom1) );
                        }
                    }
                }
            }
        }
        else if (lowline.startsWith("ter"))
        {
            loadedmols.append(currentmol);
            currentmol = EditMol( QString("PDB_%1").arg(loadedmols.count()) );
        }
    }

    //the last loaded molecule needs to be added to the list
    loadedmols.append(currentmol);

    //convert all of the loaded EditMols to Molecules, and return the
    //results
    QList<Molecule> molecules;

    for (QList<EditMol>::const_iterator it = loadedmols.constBegin();
         it != loadedmols.constEnd();
         ++it)
    {
        Molecule molecule;
        molecule.setNewID();

        molecule = it->commit();

        molecules.append(molecule);
    }

    return molecules;
}

QByteArray PDB::writeMols(const QList<Molecule> &molecules) const
{
    QByteArray data;

    QTextStream ts(&data, QIODevice::WriteOnly | QIODevice::Text);

    //the maximum line length for a PDB line is less than 128 characters
    std::auto_ptr<char> line(new char[128]);

    int atcount = 0;

    int sz = molecules.count();
    for (int i=0; i<sz; i++)
    {
        const Molecule &mol = molecules[i];

        //loop over each residue in the editmol...
        int nres = mol.nResidues();
        for (ResID ires(0); ires<nres; ++ires)
        {
            Residue res = mol[ires];

            //now loop over each atom in the residue...
            int nats = res.nAtoms();
            for (AtomID iat(0); iat<nats; ++iat)
            {
                ++atcount;
                const Atom &atm = res[iat];

                //print out the atom info (write out coordinates in angstroms)
                std::snprintf(line.get(),128,
                               "ATOM  %5d %4s %4s %4d    %8.3f%8.3f%8.3f\n",
                               atcount,
                               qPrintable(atm.name().left(4)),
                               qPrintable(res.name().left(4)),
                               res.number().toInt(),
                               convertTo(atm.x(), angstrom),
                               convertTo(atm.y(), angstrom),
                               convertTo(atm.z(), angstrom));

                //write the line to the textstream
                ts << line.get();
            }
        }

        //if this is not the last molecule, then write a 'TER'
        if (i != sz - 1)
          ts << "TER\n";
    }

    return data;
}

QByteArray PDB::writeMols(const QList<EditMol> &molecules) const
{
    QByteArray data;

    QTextStream ts(&data, QIODevice::WriteOnly | QIODevice::Text);

    //the maximum line length for a PDB line is less than 128 characters
    std::auto_ptr<char> line(new char[128]);

    int atcount = 0;

    int sz = molecules.count();
    for (int i=0; i<sz; i++)
    {
        const EditMol &mol = molecules[i];

        //loop over each residue in the editmol...
        int nres = mol.nResidues();
        for (ResID ires(0); ires<nres; ++ires)
        {
            EditRes res = mol[ires];

            //now loop over each atom in the residue...
            int nats = res.nAtoms();
            for (AtomID iat(0); iat<nats; ++iat)
            {
                ++atcount;
                const Atom &atm = res[iat];

                //print out the atom info (write out coordinates in angstroms)
                std::snprintf(line.get(),128,
                               "ATOM  %5d %4s %4s %4d    %8.3f%8.3f%8.3f\n",
                               atcount,
                               qPrintable(atm.name().left(4)),
                               qPrintable(res.name().left(4)),
                               res.number().toInt(),
                               convertTo(atm.x(), angstrom),
                               convertTo(atm.y(), angstrom),
                               convertTo(atm.z(), angstrom));

                //write the line to the textstream
                ts << line.get();
            }
        }

        //if this is not the last molecule, then write a 'TER'
        if (i != sz - 1)
          ts << "TER\n";
    }

    return data;
}
