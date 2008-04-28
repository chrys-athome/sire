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

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireIO;
using namespace SireStream;

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
    // ATOM      1  N   MET     2     -15.160  18.227  60.039  1.00 13.87      MA   N
    // ATOM    145  N   VAL A  25      32.433  16.336  57.540  1.00 11.92           N 
    // ATOM    146  CA  VAL A  25      31.132  16.439  58.160  1.00 11.85           C 
    // ATOM    147  C   VAL A  25      30.447  15.105  58.363  1.00 12.34           C  
    // ATOM    148  O   VAL A  25      29.520  15.059  59.174  1.00 15.65           O 
    // ATOM    149  CB AVAL A  25      30.385  17.437  57.230  0.28 13.88           C 
    // ATOM    150  CB BVAL A  25      30.166  17.399  57.373  0.72 15.41           C 
    // ATOM    151  CG1AVAL A  25      28.870  17.401  57.336  0.28 12.64           C 
    // ATOM    152  CG1BVAL A  25      30.805  18.788  57.449  0.72 15.11           C 
    // ATOM    153  CG2AVAL A  25      30.835  18.826  57.661  0.28 13.58           C 
    // ATOM    154  CG2BVAL A  25      29.909  16.996  55.922  0.72 13.25           C 
    // ATOM    155 CA   CAL A  26      -5.000   3.234 -10.034  1.00  1.00           CA2+

    @author Christopher Woods
*/
class PDBAtom
{
public:
    PDBAtom();

    ~PDBAtom();

    static PDBAtom readFromLine(const QString &line, int linenum);
    
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
    void addFrameAtom(const PDBAtom &atom);
    
    bool hasMultipleOccupancy() const;
    bool hasMultipleFrames() const;
    
    void openFrame(int framenum);
    bool closeFrame();
    
    /** All of the atoms in the molecule, as read from 
        the PDB file */
    QList<PDBAtom> atoms;
    
    /** Extra sets of coordinates (in case of multiple models,
        e.g. from NMR or from a trajectory) */
    QHash< int, QList<PDBAtom> > frames;
};

/** The collection of all molecules read from a PDB file */
class PDBMolecules
{
public:
    PDBMolecules();
    ~PDBMolecules();
    
    PDBMolecule& openMolecule();
    void closeMolecule();
    
    void openFrame(int frame_number);
    void closeFrame();
    
    bool moleculeOpened() const;
    bool frameOpened() const;
    
    PDBMolecule& getMolecule(int index);

    void closeCurrent();

private:
    QList<PDBMolecule> mols;
    
    bool first_model;
    bool open_frame;
};

////////
//////// Implementation of PDBAtom
////////

PDBAtom::PDBAtom()
{}

PDBAtom::~PDBAtom()
{}

PDBAtom PDBAtom::readFromLine(const QString &line, int linenum)
{
    if (line.length() < 54)
        //the line is too short to be a PDB atom line!
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not look like a valid PDB line. It only contains "
            "%2 characters, when it should contain at least 54.\n%3.")
                .arg(linenum).arg(line.length()).arg(line), CODELOC );

    bool ok = true;

    record_name = line.mid(0,6).trimmed();
    
    serial = line.mid(6,5).toInt(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB atom number (%2).\n%3")
                .arg(linenum).arg(line.mid(6,5).trimmed(), line), CODELOC );

    name = line.mid(12,4);
    altloc = line.mid(16,1).trimmed();
    resname = line.mid(17,3);
    chainid = line.mid(21,1).trimmed();
    resseq = line.mid(22,4).toInt(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB residue number (%2).\n%3")
                .arg(linenum).arg(resseq.mid(22,4).trimmed(), line), CODELOC );
                
    icode = line.mid(26,1).trimmed();
    
    x = line.mid(30,8).toDouble(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB x coordinate (%2)\n%3")
                .arg(linenum).arg(line.mid(30,8).trimmed(), line), CODELOC );
    
    y = line.mid(38,8).toDouble(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB y coordinate (%2)\n%3")
                .arg(linenum).arg(line.mid(38,8).trimmed(), line), CODELOC );
    
    z = line.mid(46,8).toDouble(&ok);
    
    if (not ok)
        throw SireIO::parse_error( QObject::tr(
            "Line %1 does not have a valid PDB z coordinate (%2)\n%3")
                .arg(linenum).arg(line.mid(46,8).trimmed(), line), CODELOC );

    if (line.length() >= 60)
    {
        //we can read the occupancy
        occupancy = line.mid(54,6).toDouble(&ok);
        
        if (not ok)
            //something went wrong reading the occupancy - ignore the error
            occupancy = 1;
    }
    
    if (line.length() >= 66)
    {
        //try to read the temperature factor
        tempfactor = line.mid(60,6).toDouble(&ok);
        
        if (not ok)
            //something went wrong
            tempfactor = -1;
    }
    
    if (line.length() >= 73)
    {
        segid = line.mid(73,4).trimmed();
    }
    
    if (line.length() >= 78)
    {
        element = line.mid(76,2).trimmed();
    }
    
    if (line.length() >= 79)
    {
        QString chgstring = line.mid(78,2).trimmed();
        
        //format should be 2+ or 1- or something like that
        int factor = 1;
        
        if (chgstring.contains("-"))
            factor = -1;
            
        chgstring.remove("-").remove("+");
        
        charge = factor * chgstring.toInt(&ok);
        
        if (not ok)
            //something went wrong - ignore the charge
            charge = 0;
    }
}

QString PDBAtom::writeToLine() const
{
    return QString::null;
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

////////
//////// Implementation of PDBMolecules
////////

////////
//////// Implementation of PDB
////////

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

    PDBMolecule &current_molecule = pdbmols.openMolecule();
    
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
                current_molecule = pdbmols.openMolecule();
        
            if (first_model)
                current_molecule.addAtom( PDBAtom::readFromLine(line, linenum) );
            else
                current_molecule.addFrameAtom( PDBAtom::readFromLine(line, linenum) );
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
                
            frame_number = getModelFrameNumber(line, pdbmols.lastFrame());
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

    return MoleculeGroup();
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
