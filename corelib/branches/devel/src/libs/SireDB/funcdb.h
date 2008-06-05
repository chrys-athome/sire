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

#ifndef SIREDB_FUNCDB_H
#define SIREDB_FUNCDB_H

#include "relatedb.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Expression;
class Function;
}

namespace SireDB
{

using SireCAS::Expression;
using SireCAS::Function;

using boost::tuple;

/**
This is a database that is used to store SireCAS expressions.

This is the base class of all Function database classes (e.g. Term12DB, Term13DB and Term14DB). This class should not be used on its own, but is meant to be used to allow code reuse between the other Term**DB classes. This class should not be virtually inherited (as all of the Term**DB classes should not be virtually inherited).
 
@author Christopher Woods
*/
class SIREDB_EXPORT FuncDB : public RelateDB
{
public:
    FuncDB();
    FuncDB(const FuncDB &other);
    
    ~FuncDB();

    static const char* typeName()
    {
        return "SireDB::FuncDB";
    }

    Expression getFunc(const QString &userid, bool *foundfunc = 0);
    Expression getFunc(const Function &func, bool *foundfunc = 0);

    Expression getFunc(RelateID relateid, bool *foundfunc = 0);
    Expression getFunc(const RelateIDMap &relateids, bool *foundfunc = 0);

    Expression resolveTerms(const Expression &function);

protected:
    void initialise();
    
    void prepareToDump();
    void postLoad();
    
    void addFunc(const QString &userid, const Expression &function);
    void addFunc(const Function &func, const Expression &equals);

    ParamID addFunc(const Expression &function);
    Expression retrieveFunc(ParamID id);
    tuple<Function,Expression> retrieveFunc(const QString &functionname);
    
private:
    QString functionTable() const;
    QString termTable() const;
    
    static QByteArray dump(const Expression &func);
    static Expression load(const QByteArray &data);
    
    static QByteArray dumpFunction(const Function &func);
    static Function loadFunction(const QByteArray &data);
};

}

SIRE_END_HEADER

#endif
