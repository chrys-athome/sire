/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREIO_PROTOMSWS_H
#define SIREIO_PROTOMSWS_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "SireDB/parameterdb.h"
#include "SireDB/atomtypedb.h"
#include "SireDB/matchdata.h"
#include "SireDB/matchres.h"
#include "SireDB/relatemrdb.h"
#include "SireDB/relatemradb.h"
#include "SireDB/relateatomtypedb.h"

#include "SireMM/chargedb.h"
#include "SireMM/ljdb.h"
#include "SireMM/bonddb.h"
#include "SireMM/angledb.h"
#include "SireMM/dihedraldb.h"
#include "SireMM/ureybradleydb.h"

SIRE_BEGIN_HEADER

namespace SireIO
{

/**
This is a very simple (and private) class that is used by SireIO::ProtoMS as a workspace that is used when reading in a ProtoMS2 parameter file. This is used to store items such as the databases available, the current line information, current template etc.

@author Christopher Woods
*/
class ProtoMSWS
{
public:

    /** Enum describing the 'MODE' that the workspace is currently in. */
    enum ProtoMS_Mode{ INVALID_MODE = 0,      ///< Invalid MODE
                       CLJ_MODE = 1,          ///< MODE used to read charge and LJ parameters
                       BOND_MODE = 2,         ///< MODE used to read bond parameters
                       ANGLE_MODE = 3,        ///< MODE used to read angle parameters
                       DIHEDRAL_MODE = 4,     ///< MODE used to read dihedral parameters
                       UREYBRADLEY_MODE = 5,  ///< MODE used to read Urey-Bradley parameters
                       TEMPLATE_MODE = 6,     ///< MODE used to read in templates
                       OVERRIDE_MODE = 7,     ///< MODE used to read in override parameters
                       INFO_MODE = 8          ///< MODE used to supply additional information
                     };

    /** Enum describing the type of template that is being read in */
    enum ProtoMS_Tmpl { INVALID_TMPL,  ///< Invalid type of template
                        RESIDUE_TMPL,  ///< Residue template
                        SOLUTE_TMPL,   ///< Solute template
                        SOLVENT_TMPL,  ///< Solvent template
                        CHAIN_TMPL     ///< Chain template
                      };

    ProtoMSWS(const QString &filename, SireDB::ParameterDB &paramdb);

    ~ProtoMSWS();

    QString filename() const;
    QString lineInfo() const;
    const QStringList& words() const;
    const QString& firstWord() const;
    const QString& line() const;
    int lineNumber() const;

    ProtoMSWS::ProtoMS_Tmpl templateType() const;
    const QString& templateName() const;

    SireDB::RelateMRDB& relateMRDB();
    SireDB::RelateMRADB& relateMRADB();
    SireDB::RelateAtomTypeDB& relateAtomTypeDB();
    SireDB::AtomTypeDB& atomTypeDB();
    SireMM::ChargeDB& chargeDB();
    SireMM::LJDB& ljDB();
    SireMM::BondDB& bondDB();
    SireMM::AngleDB& angleDB();
    SireMM::UreyBradleyDB& ureyBradleyDB();
    SireMM::DihedralDB& dihedralDB();

    ProtoMS_Mode mode() const;

    bool readNextLine();

    void relateAtom( const SireDB::AssertMatch<1> &matchdata,
                     const QString &parameter );

    void addLog(const QString &message);

private:
    /** The QFile object being used to access the file */
    QFile _file;

    /** The QTextStream object being used to read the file */
    QTextStream _ts;

    /** The current line being read */
    QString _line;
    /** The current line number */
    int _linenum;
    /** The words in the line */
    QStringList _words;

    /** Lowercase form of the first word in the line */
    QString _firstword;

    /** The log of errors. The errors are collected together so that
        they are added to the database all in one go when this workspace
        is deleted. */
    QStringList _log;

    /** The current ProtoMS mode */
    ProtoMS_Mode _mode;

    /** Hash mapping strings to modes */
    QHash<QString,ProtoMS_Mode> _modes;

    /** The name of the current template */
    QString _tmplname;

    /** The type of the current template */
    ProtoMS_Tmpl _tmpltype;

    /** Hash mapping strings to template types */
    QHash<QString,ProtoMS_Tmpl> _templates;

    /** Pointer to the database into which the parameters will be read */
    SireDB::ParameterDB &db;

    /** Pointers to the different parts of the database - these pointers
        will be 0 if the database doesn't provide the required functionality */
    SireDB::AtomTypeDB &atomtypedb;
    SireMM::ChargeDB &chargedb;
    SireMM::LJDB &ljdb;
    SireMM::BondDB &bonddb;
    SireMM::AngleDB &angdb;
    SireMM::UreyBradleyDB &uybydb;
    SireMM::DihedralDB &dihdb;
    SireDB::RelateMRDB &relatemrdb;
    SireDB::RelateMRADB &relatemradb;
    SireDB::RelateAtomTypeDB &relateatomtypedb;
};

/** Return the name of the file that is being processed */
inline QString ProtoMSWS::filename() const
{
    return _file.fileName();
}

/** Return the list of words on the current line - this will be empty if
    the current line is empty */
inline const QStringList& ProtoMSWS::words() const
{
    return _words;
}

/** Return the first word in the line (lowercased) - this returns a null string
    if are no words on the current line */
inline const QString& ProtoMSWS::firstWord() const
{
    return _firstword;
}

/** Return the current line (in a raw, unprocessed state). This will return
    a null string if we have not yet started reading the file or if we
    have reached the end of the file */
inline const QString& ProtoMSWS::line() const
{
    return _line;
}

/** Return the line number of the current line - this will return 0 if no
    lines have been read */
inline int ProtoMSWS::lineNumber() const
{
    return _linenum;
}

/** Return the type of the currently processing template (or INVALID_TMPL if there
    is no template currently being processed) */
inline ProtoMSWS::ProtoMS_Tmpl ProtoMSWS::templateType() const
{
    return _tmpltype;
}

/** Return the name of the currently processing template (or a null string if there
    is no template currently being processed) */
inline const QString& ProtoMSWS::templateName() const
{
    return _tmplname;
}

/** Return the part of the database that store atom types - this will have
    undefined results unless hasAtomTypeDB() returns true. */
inline SireDB::AtomTypeDB& ProtoMSWS::atomTypeDB()
{
    return atomtypedb;
}

/** Return the part of the database that store charge parameters - this will have
    undefined results unless hasChargeDB() returns true. */
inline SireMM::ChargeDB& ProtoMSWS::chargeDB()
{
    return chargedb;
}

/** Return the part of the database that store LJ parameters - this will have
    undefined results unless hasLJDB() returns true. */
inline SireMM::LJDB& ProtoMSWS::ljDB()
{
    return ljdb;
}

/** Return the part of the database that store bond parameters - this will have
    undefined results unless hasBondDB() returns true. */
inline SireMM::BondDB& ProtoMSWS::bondDB()
{
    return bonddb;
}

/** Return the part of the database that store angle parameters - this will have
    undefined results unless hasAngleDB() returns true. */
inline SireMM::AngleDB& ProtoMSWS::angleDB()
{
    return angdb;
}

/** Return the part of the database that store dihedral parameters - this will have
    undefined results unless hasDihedralDB() returns true. */
inline SireMM::DihedralDB& ProtoMSWS::dihedralDB()
{
    return dihdb;
}

/** Return the part of the database that store Urey-Bradley parameters - this will have
    undefined results unless hasUreyBradleyDB() returns true. */
inline SireMM::UreyBradleyDB& ProtoMSWS::ureyBradleyDB()
{
    return uybydb;
}

/** Return the part of the database that stores relationships involving only molecules
    and residues. This will have undefined results unless hasRelateMRDB() returns true. */
inline SireDB::RelateMRDB& ProtoMSWS::relateMRDB()
{
    return relatemrdb;
}

/** Return the part of the database that stores relationships involving molecules,
    residues and atoms. This will have undefined results unless hasRelateMRADB()
    returns true. */
inline SireDB::RelateMRADB& ProtoMSWS::relateMRADB()
{
    return relatemradb;
}

/** Return the part of the database that stores relationships involving molecules,
    residues and atom types. This will have undefined results unless
    hasRelateAtomTypeDB() returns true. */
inline SireDB::RelateAtomTypeDB& ProtoMSWS::relateAtomTypeDB()
{
    return relateatomtypedb;
}

/** Return the current reading mode of the workspace - this returns
    INVALID_MODE if the mode is not set of invalid. */
inline ProtoMSWS::ProtoMS_Mode ProtoMSWS::mode() const
{
    return _mode;
}

}

SIRE_END_HEADER

#endif
