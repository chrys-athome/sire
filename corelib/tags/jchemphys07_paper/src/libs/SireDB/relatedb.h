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

#ifndef SIREDB_RELATEDB_H
#define SIREDB_RELATEDB_H

#include "dbbase.h"
#include "matchdata.h"
#include "relateidmap.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This is the database that is used to relate parameters to user-supplied identifiers, and also to relate RelateID numbers to parameters. This class must form the base class of any parameter database that wants to include complex relationships to parameters.

@author Christopher Woods
*/
class SIREDB_EXPORT RelateDB : public DBBase
{
public:
    RelateDB();
    RelateDB(const RelateDB &other);
    
    ~RelateDB();
    
    static const char* typeName()
    {
        return "SireDB::RelateDB";
    }
    
protected:
    void initialise();

    void prepareToDump();
    void postLoad();

    void relateParameter(const QString &userid, ParamID param);
    ParamID getParameter(const QString &userid);
    
    void relateParameter(RelateID relateid, ParamID param);
    void relateParameter(RelateID relateid, const QString &param);
    ParamID getParameter(RelateID relateid);
    
    ParamID getParameter(const RelateIDMap &relateids);

private:

    QString userIDToParamID() const;
    QString relateIDToParameter() const;

};

}

SIRE_END_HEADER

#endif
