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

#include "protomsws.h"

#include "SireError/errors.h"

using namespace SireDB;
using namespace SireMM;
using namespace SireIO;

/** Construct a workspace that will be used to read the ProtoMS2 parameter file 
    'filename' into the parameter database 'paramdb' */
ProtoMSWS::ProtoMSWS(const QString &filename, ParameterDB &paramdb) 
          : _linenum(0), db(paramdb),
            atomtypedb( db.addComponent<AtomTypeDB>() ),
            chargedb(db.addComponent<ChargeDB>() ),
            ljdb(db.addComponent<LJDB>() ),
            bonddb(db.addComponent<BondDB>() ),
            angdb(db.addComponent<AngleDB>() ),
            uybydb(db.addComponent<UreyBradleyDB>() ),
            dihdb(db.addComponent<DihedralDB>() ),
            relatemrdb(db.addComponent<RelateMRDB>() ),
            relatemradb(db.addComponent<RelateMRADB>() ),
            relateatomtypedb(db.addComponent<RelateAtomTypeDB>())
{
    //try to open the file
    _file.setFileName(filename);
    
    if (not _file.open(QIODevice::ReadOnly))
    {
        throw SireError::file_error(QObject::tr(
            "Cannot open ProtoMS parameter file %1: %2")
                .arg(filename).arg(SireError::fileError(_file)), CODELOC );
        
    }
    
    _ts.setDevice(&_file);
    
    //set the modes
    _modes.insert("clj", CLJ_MODE);
    _modes.insert("bond", BOND_MODE);
    _modes.insert("angle", ANGLE_MODE);
    _modes.insert("ureybradley", UREYBRADLEY_MODE);
    _modes.insert("dihedral", DIHEDRAL_MODE);
        
    _modes.insert("template", TEMPLATE_MODE);
    _modes.insert("override", OVERRIDE_MODE);
    
    //also save the types of template that we can work with
    _templates.insert("residue", RESIDUE_TMPL);
    _templates.insert("solute", SOLUTE_TMPL);
    _templates.insert("solvent", SOLVENT_TMPL);
    _templates.insert("chain", CHAIN_TMPL);
    
    _modes.insert("info", INFO_MODE);
}

/** Destroy the workspace - this will add any log messages to the main 
    database log */
ProtoMSWS::~ProtoMSWS()
{
    if (not _log.isEmpty())
        db.addLog( _log.join("\n") );
}

/** Return a string allowing the identification of the current line */
QString ProtoMSWS::lineInfo() const
{
    return QString("(%2:%1) | \"%3\"").arg(_linenum).arg(_file.fileName(),_line);
}

/** Read the next line of the file - this returns true if a new line was successfully
    read, or false if there is no next line (e.g. we have reached the end of the file) */
bool ProtoMSWS::readNextLine()
{
    while (not _ts.atEnd())
    {
        _line = _ts.readLine();
        _linenum++;
        
        //remove complicated spacing patters...
        QString simpleline = _line.simplified();
        
        if ( not (simpleline.isEmpty() or simpleline.startsWith("#")) )
        {
            //split the line by '#' - this removes any comments that are on the end of the line
            QStringList tmp = simpleline.split("#",QString::SkipEmptyParts);
        
            //split the line into words
            _words = tmp[0].split(" ",QString::SkipEmptyParts);
            
            if (_words.count() > 0)
            {
                _firstword = _words[0].toLower();
            
                //does this line signify a change of mode?
                if ( _firstword == "mode" )
                {
                    QString newmode = _words[1].toLower();
                    if ( _modes.contains(newmode) )
                        _mode = _modes[newmode];
                    else
                        _mode = INVALID_MODE;
                        
                    //clear the current template information
                    _tmplname = QString::null;
                    _tmpltype = INVALID_TMPL;
                }
                else if ( _mode == TEMPLATE_MODE and _templates.contains(_firstword) )
                {
                    if (_words.count() > 1)
                    {
                        //we have just started a new template
                        _tmpltype = _templates[_firstword];
                        
                        if (_tmpltype != INVALID_TMPL)
                            //the template's name is the second word...
                            _tmplname = _words[1];
                        else
                            _tmplname = QString::null;
                    }
                    else
                    {
                        _tmplname = QString::null;
                        _tmpltype = INVALID_TMPL;
                        
                        addLog( QObject::tr("Error while parsing 'template' line %1")
                                              .arg(lineInfo()) );
                    }
                }
                else
                    return true;
            }
        }
    }
    
    //no more file left to read
    _words.clear();
    _firstword = QString::null;
    _line = QString::null;
    return false;
}
    
/** Append 'message' to the log. This log will be added to the database log
    when this workspace is deleted. This allows all errors to be grouped together
    into a single entry in the log (rather than the database time-stamping
    each individual error) */    
void ProtoMSWS::addLog(const QString &message)
{
    _log.append(message);
}

/** Relate the atom match specified in 'matchdata' to the atomtype, charge and LJ
    parameter identified via the ID string 'parameter' */
void ProtoMSWS::relateAtom( const SireDB::AssertMatch<1> &matchdata, 
                            const QString &parameter )
{
    //get a relationship ID number for this match...
    RelateID relateid = matchdata.addTo(db);
    
    if (relateid != 0)
    {
        //associate the atomtype, charge and LJ parameter with this relationship ID
        atomTypeDB().relateAtomType( relateid, parameter );
        chargeDB().relateCharge( relateid, parameter );
        ljDB().relateLJ( relateid, parameter );
    }
}
