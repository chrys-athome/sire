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

#include <QTextStream>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "pdb.h"

#include "SireMol/element.h"
#include "SireMol/mover.hpp"

#include "SireError/errors.h"
#include "SireIO/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

#include <QDebug>

using namespace SireMol;
using namespace SireBase;
using namespace SireIO;
using namespace SireStream;

/** This internal class is used to store all of the data
    that is used in a PDB ATOM line 

    // PBB ATOM line
    //
    // Format as described in PDB 2.3 format guide
    // (see link above)
    //
    // COLUMNS             DATA TYPE        FIELD      DEFINITION 
    // ------------------------------------------------------------- 
    //  1 -   6            Record name      "ATOM    " 
    //
    //  7 - 11             Integer          serial     Atom serial number. 
    //
    // 13 - 16             Atom             name       Atom name. 
    //
    // 17                  Character        altLoc     Alternate location 
    //                                                 indicator. 
    //
    // 18 - 20             Residue name     resName    Residue name. 
    //
    // 22                  Character        chainID    Chain identifier. 
    //
    // 23 - 26             Integer          resSeq     Residue sequence number. 
    //
    // 27                  AChar            iCode      Code for insertion of 
    //                                                 residues. 
    // 
    // 31 - 38             Real(8.3)        x          Orthogonal coordinates for 
    //                                                 X in Angstroms 
    //
    // 39 - 46             Real(8.3)        y          Orthogonal coordinates for 
    //                                                 Y in Angstroms 
    //
    // 47 - 54             Real(8.3)        z          Orthogonal coordinates for 
    //                                                 Z in Angstroms 
    //
    // 55 - 60             Real(6.2)        occupancy  Occupancy. 
    // 
    // 61 - 66             Real(6.2)        tempFactor Temperature factor. 
    //
    // 73 - 76             LString(4)       segid      Segment ID, left-justified,
    //                                                 (not part of 2.1 spec, but used
    //                                                  widely)
    //
    // 77 - 78             LString(2)       element    Element symbol, 
    //                                                 right-justified. 
    //
    // 79 - 80             LString(2)       charge     Charge on the atom. 

    // 000000000011111111112222222222333333333344444444445555555555666666666677777777778
    // 012345678901234567890123456789012345678901234567890123456789012345678901234567890 
    // ATOM      1  N   MET     2     -15.160  18.227  60.039  1.00 13.87      MA  N
    // ATOM    145  N   VAL A  25      32.433  16.336  57.540  1.00 11.92          N 
    // ATOM    146  CA  VAL A  25      31.132  16.439  58.160  1.00 11.85          C 
    // ATOM    147  C   VAL A  25      30.447  15.105  58.363  1.00 12.34          C  
    // ATOM    148  O   VAL A  25      29.520  15.059  59.174  1.00 15.65          O 
    // ATOM    149  CB AVAL A  25      30.385  17.437  57.230  0.28 13.88          C 
    // ATOM    150  CB BVAL A  25      30.166  17.399  57.373  0.72 15.41          C 
    // ATOM    151  CG1AVAL A  25      28.870  17.401  57.336  0.28 12.64          C 
    // ATOM    152  CG1BVAL A  25      30.805  18.788  57.449  0.72 15.11          C 
    // ATOM    153  CG2AVAL A  25      30.835  18.826  57.661  0.28 13.58          C 
    // ATOM    154  CG2BVAL A  25      29.909  16.996  55.922  0.72 13.25          C 
    // ATOM    155 CA   CAL A  26      -5.000   3.234 -10.034  1.00  1.00          CA2+

    @author Christopher Woods
*/
class PDBAtom
{
public:
    PDBAtom();

    ~PDBAtom();

    static PDBAtom readFromLine(const QString &line, int linenum);
    
    QString toString() const;
    
    QString writeToLine() const;
    
    bool isATOM() const;
    bool isHETATM() const;
    
    Element guessElement() const;
    Vector coordinates() const;
    
    QString   record_name;
    int       serial;
    QString   name;
    QString   altloc;
    QString   resname;
    QString   chainid;
    int       resseq;
    QString   icode;
    double    x;
    double    y;
    double    z;
    double    occupancy;
    double    tempfactor;
    QString   segid;
    QString   element;
    int       charge;
};

/** The collection of all information read about 
    a molecule from a PDB file */
class PDBMolecule
{
public:
    PDBMolecule();
    ~PDBMolecule();
    
    void addAtom(const PDBAtom &atom);
    
    QList<int> availableFrames() const;
    
    const QList<PDBAtom>& atoms(int frame=1) const;
    
    bool hasMultipleOccupancy(int frame=1) const;
    bool hasMultipleFrames() const;
    
    void openFrame(int framenum);
    void closeFrame();
    
    bool isEmpty() const;

private:    
    /** Extra sets of coordinates (in case of multiple models,
        e.g. from NMR or from a trajectory) */
    QHash< int, QList<PDBAtom> > frames;
    
    int current_frame;
};

/** The collection of all molecules read from a PDB file */
class PDBMolecules
{
public:
    PDBMolecules();
    ~PDBMolecules();
    
    PDBMolecule& nextMolecule();
    void closeMolecule();
    
    void openFrame(int frame_number);
    void closeFrame();
    
    bool frameOpened() const;
    bool moleculeOpened() const;

    const QList<PDBMolecule>& molecules() const;

private:
    QList<PDBMolecule> mols;
    
    int last_frame;
    int current_molecule;
    
    bool frame_opened;
    bool molecule_opened;
};

////////
//////// Implementation of PDBAtom
////////

PDBAtom::PDBAtom()
{}

PDBAtom::~PDBAtom()
{}

QString PDBAtom::toString() const
{
    return QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16")
                .arg(record_name).arg(serial).arg(name)
                .arg(altloc).arg(resname).arg(chainid).arg(resseq)
                .arg(icode).arg(x).arg(y).arg(z).arg(occupancy)
                .arg(tempfactor).arg(segid).arg(element).arg(charge);
}

PDBAtom PDBAtom::readFromLine(const QString &line, int linenum)
{
    if (line.length() < 54)
        //the line is too short to be a PDB atom line!
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not look like a valid PDB line. It only contains "
            "%2 characters, when it should contain at least 54.\n%3.")
                .arg(linenum).arg(line.length()).arg(line), CODELOC );

    PDBAtom atom;

    bool ok = true;

    atom.record_name = line.mid(0,6).trimmed();
    
    atom.serial = line.mid(6,5).toInt(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB atom number (%2).\n%3")
                .arg(linenum).arg(line.mid(6,5).trimmed(), line), CODELOC );

    atom.name = line.mid(12,4);
    atom.altloc = line.mid(16,1).trimmed();
    atom.resname = line.mid(17,3);
    atom.chainid = line.mid(21,1).trimmed();
    atom.resseq = line.mid(22,4).toInt(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB residue number (%2).\n%3")
                .arg(linenum).arg(line.mid(22,4).trimmed(), line), CODELOC );
                
    atom.icode = line.mid(26,1).trimmed();
    
    atom.x = line.mid(30,8).toDouble(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB x coordinate (%2)\n%3")
                .arg(linenum).arg(line.mid(30,8).trimmed(), line), CODELOC );
    
    atom.y = line.mid(38,8).toDouble(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB y coordinate (%2)\n%3")
                .arg(linenum).arg(line.mid(38,8).trimmed(), line), CODELOC );
    
    atom.z = line.mid(46,8).toDouble(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB z coordinate (%2)\n%3")
                .arg(linenum).arg(line.mid(46,8).trimmed(), line), CODELOC );

    if (line.length() >= 55)
    {
        //we can read the occupancy
        atom.occupancy = line.mid(54,6).toDouble(&ok);
        
        if (not ok)
            //something went wrong reading the occupancy - ignore the error
            atom.occupancy = 1;
    }
    else
        atom.occupancy = 1;
    
    if (line.length() >= 61)
    {
        //try to read the temperature factor
        atom.tempfactor = line.mid(60,6).toDouble(&ok);
        
        if (not ok)
            //something went wrong
            atom.tempfactor = 0;
    }
    else
        atom.tempfactor = 0;
    
    if (line.length() >= 73)
    {
        atom.segid = line.mid(72,4).trimmed();
    }
    else
        atom.segid = QString::null;
    
    if (line.length() >= 77)
    {
        atom.element = line.mid(76,2).trimmed();
    }
    else
        atom.element = QString::null;
    
    if (line.length() >= 79)
    {
        QString chgstring = line.mid(78,2).trimmed();
        
        //format should be 2+ or 1- or something like that
        int factor = 1;
        
        if (chgstring.contains("-"))
            factor = -1;
            
        chgstring.remove("-").remove("+");
        
        atom.charge = factor * chgstring.toInt(&ok);
        
        if (not ok)
            //something went wrong - ignore the charge
            atom.charge = 0;
    }
    else
        atom.charge = 0;
        
    return atom;
}

QString PDBAtom::writeToLine() const
{
    QString chg;
    
    if (charge > 0)
        chg = QString("%1+").arg(charge);
    else if (charge < 0)
        chg = QString("%1-").arg(charge);

    char line[82];
    line[81] = '\0';
    
    qsnprintf(line, 81,
     "%6-s%5d %4s%1s%3s %1s%4d%1s   %8.3_f%8.3_f%8.3_f%6.2_f%6.2_f      %4s%2s%2s",
            qPrintable(record_name), serial, qPrintable(name),
            qPrintable(altloc), qPrintable(resname), qPrintable(chainid),
            resseq, qPrintable(icode), x, y, z, occupancy, tempfactor,
            qPrintable(segid), qPrintable(element), qPrintable(chg));

    return QString::fromLocal8Bit(line).trimmed();
}

bool PDBAtom::isATOM() const
{
    return QString::compare(record_name, "ATOM", Qt::CaseInsensitive) == 0;
}

bool PDBAtom::isHETATM() const
{
    return QString::compare(record_name, "HETATM", Qt::CaseInsensitive) == 0;
}

Element PDBAtom::guessElement() const
{
    if (element.isEmpty())
    {
        //we need to guess the element from the atom name - the
        //element is the first two characters of the name, right
        //justified
        return Element( element.mid(0,2).trimmed() );
    }
    else
        return Element(element);
}

Vector PDBAtom::coordinates() const
{
    return Vector(x,y,z);
}

////////
//////// Implementation of PDBMolecule
////////

void PDBMolecule::openFrame(int framenum)
{
    frames.insert(framenum, QList<PDBAtom>());
    current_frame = framenum;
}

void PDBMolecule::closeFrame()
{
    if (current_frame != -1)
    {
        if (frames.constFind(current_frame)->isEmpty())
        {
            frames.remove(current_frame);
            current_frame = -1;
        }
    }
}

PDBMolecule::PDBMolecule() : current_frame(-1)
{}

PDBMolecule::~PDBMolecule()
{}

void PDBMolecule::addAtom(const PDBAtom &atom)
{
    if (current_frame == -1)
        //open the default frame (1)
        this->openFrame(1);

    frames[current_frame].append(atom);
}

QList<int> PDBMolecule::availableFrames() const
{
    return frames.keys();
}

bool PDBMolecule::isEmpty() const
{
    return frames.isEmpty();
}

const QList<PDBAtom>& PDBMolecule::atoms(int frame) const
{
    QHash< int,QList<PDBAtom> >::const_iterator it = frames.constFind(frame);
    
    if (it == frames.constEnd())
        throw SireError::invalid_index( QObject::tr(    
            "There is no frame at index %1. Available frames are %2.")
                .arg(frame).arg( Sire::toString(frames.keys()) ), CODELOC );

    return *it;
}

bool PDBMolecule::hasMultipleOccupancy(int frame) const
{
    foreach (const PDBAtom &atom, frames.value(frame))
    {
        if (atom.occupancy < 1)
            //as the sum of occupancies for a site must equal 1,
            //this implies that we do have multiple occupancy in this frame
            return true;
    }
    
    return false;
}

bool PDBMolecule::hasMultipleFrames() const
{
    return frames.count() > 1;
}

////////
//////// Implementation of PDBMolecules
////////

PDBMolecules::PDBMolecules() : last_frame(0), current_molecule(-1), 
                               frame_opened(false), molecule_opened(false)
{}

PDBMolecules::~PDBMolecules()
{}

void PDBMolecules::closeMolecule()
{
    if (molecule_opened)
    {
        PDBMolecule &mol = mols[current_molecule];
        mol.closeFrame();
    
        if (mol.isEmpty())
        {
            //remove the empty molecule
            mols.removeAt(current_molecule);
            
            --current_molecule;
        }
    }
    
    molecule_opened = false;
}

PDBMolecule& PDBMolecules::nextMolecule()
{
    if (molecule_opened)
        this->closeMolecule();

    ++current_molecule;

    if (current_molecule >= mols.count())
    {
        mols.append(PDBMolecule());
        
        if (last_frame > 0)
            mols[current_molecule].openFrame(last_frame);
    }
        
    molecule_opened = true;
        
    return mols[current_molecule];
}

void PDBMolecules::openFrame(int frame_number)
{
    if (frame_number <= 0)
        frame_number = last_frame + 1;

    current_molecule = -1;
    
    for (QList<PDBMolecule>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        it->openFrame(frame_number);
    }
    
    frame_opened = true;
    last_frame = frame_number;
}

void PDBMolecules::closeFrame()
{
    for (QList<PDBMolecule>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        it->closeFrame();
    }
    
    frame_opened = false;
}

bool PDBMolecules::frameOpened() const
{
    return frame_opened;
}

bool PDBMolecules::moleculeOpened() const
{
    return molecule_opened;
}

const QList<PDBMolecule>& PDBMolecules::molecules() const
{
    return mols;
}

////////
//////// Implementation of PDB
////////

static const RegisterMetaType<PDB> r_pdb;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, const PDB &pdb)
{
    writeHeader(ds, r_pdb, 1);
    
    ds << static_cast<const IOBase&>(pdb);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, PDB &pdb)
{
    VersionID v = readHeader(ds, r_pdb);
    
    if (v == 1)
    {
        ds >> static_cast<IOBase&>(pdb);
    }
    else
        throw version_error(v, "1", r_pdb, CODELOC);
        
    return ds;
}

/** Constructor */
PDB::PDB() : ConcreteProperty<PDB,IOBase>()
{}

/** Copy constructor */
PDB::PDB(const PDB &other) : ConcreteProperty<PDB,IOBase>(other)
{}

/** Destructor */
PDB::~PDB()
{}

/** Copy assignment operator */
PDB& PDB::operator=(const PDB &other)
{
    IOBase::operator=(other);
    return *this;
}

/** Comparison operator */
bool PDB::operator==(const PDB&) const
{
    return true;
}

/** Comparison operator */
bool PDB::operator!=(const PDB&) const
{
    return false;
}

/** Convert a PDBMolecule to a Molecule */
static Molecule convert(const PDBMolecule &pdbmol)
{
    
}

/** Read a group of molecules from the data */
MoleculeGroup PDB::readMols(const QByteArray &data,
                            const PropertyMap &map) const
{
    // See the PDB format description for details on the format
    //
    // Version 3.1 <http://www.wwpdb.org/documentation/format3.1-20080211.pdf>
    // Version 2.3 <http://www.wwpdb.org/documentation/format2.3-0108-a4.pdf>
    //
    
    //connect a text stream to this bytearray so that we can read it as text
    QTextStream ts(data, QIODevice::ReadOnly | QIODevice::Text);

    PDBMolecules pdbmols;
    int linenum = -1;

    PDBMolecule *current_molecule = &(pdbmols.nextMolecule());
    
    while (not ts.atEnd())
    {
        ++linenum;
    
        //read a line from the file
        QString line = ts.readLine();
        
        if (line.startsWith("END"))
        {
            QStringList words = line.split(" ", QString::SkipEmptyParts);

            if (words[0].length() == 3)
                //this is the end of the PDB file
                break;
        }
        
        if (line.startsWith("ATOM", Qt::CaseInsensitive) or
            line.startsWith("HETATM", Qt::CaseInsensitive))
        {
            if (not pdbmols.moleculeOpened())
                current_molecule = &(pdbmols.nextMolecule());
        
            current_molecule->addAtom( PDBAtom::readFromLine(line, linenum) );
        }
        else if (line.startsWith("TER", Qt::CaseInsensitive))
        {
            pdbmols.closeMolecule();
        }
        else if (line.startsWith("MODEL", Qt::CaseInsensitive))
        {
            //we've started a new model
            if (pdbmols.frameOpened())
            {
                throw SireIO::parse_error( QObject::tr(
                    "Encountered a 'MODEL' entry at line %1 "
                    "before the previous model has been terminated "
                    "(via a valid ENDMDL line).")
                        .arg(linenum), CODELOC );
            }
                
            bool ok;
            int frame_number = line.mid(10,4).toInt(&ok);
            if (not ok)
                frame_number = -1;
            
            pdbmols.openFrame(frame_number);
        }
        else if (line.startsWith("ENDMDL", Qt::CaseInsensitive))
        {
            pdbmols.closeFrame();
        }
    }

    if (pdbmols.moleculeOpened())
    {
        pdbmols.closeMolecule();
    }
    
    if (pdbmols.frameOpened())
    {
        pdbmols.closeFrame();
    }

    //we now need to convert each PDBMolecule into a real molecule
    MolGroup molgroup;
    
    foreach (const PDBMolecule &pdbmol, pdbmols.molecules())
    {
        molgroup.add( convert(pdbmol) );
    }

    return molgroup;
}

/** Write a group of molecules to a bytearray */
QByteArray PDB::writeMols(const MolGroup &molgroup,
                          const PropertyMap &map) const
{
    QByteArray data;
    QTextStream ts(&data, QIODevice::WriteOnly | QIODevice::Text);

    return data;
}

/** Write a group of molecules to a bytearray */
QByteArray PDB::writeMols(const Molecules &molecules,
                          const PropertyMap &map) const
{
    QByteArray data;
    QTextStream ts(&data, QIODevice::WriteOnly | QIODevice::Text);

    return data;
}
