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

#ifndef SIREDB_TERM14DB_H
#define SIREDB_TERM14DB_H

#include "funcdb.h"
#include "matchdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
The Term14DB is a database of 1-4 interatomic interaction functions (e.g. dihedrals)
 
@author Christopher Woods
*/
class SIREDB_EXPORT Term14DB : public FuncDB
{
public:
    Term14DB();
    Term14DB(const Term14DB &other);
    
    ~Term14DB();

    static const char* typeName()
    {
        return "SireDB::Term14DB";
    }

protected:
    void prepareToDump();
    void postLoad();

    void relateFunc(const AssertMatch<4> &matchtriple, const QString &userid,
                    bool mirrored=true);
    void relateFunc(const AssertMatch<4> &matchtriple, const Expression &func,
                    bool mirrored=true);

};

}

SIRE_END_HEADER

#endif
