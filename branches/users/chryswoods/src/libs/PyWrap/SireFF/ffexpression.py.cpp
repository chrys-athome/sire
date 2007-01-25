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

#include <boost/python.hpp>

#include "SireCAS/function.h"
#include "SireCAS/expression.h"

#include "SireFF/forcefieldid.h"

#include "SireSystem/ffexpression.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireCAS;
using namespace SireFF;
using namespace SireQt;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_FFExpression()
{
    class_<FFExpression>( "FFExpression", init<>() )
          .def( init<const QString&, const Expression&>() )
          .def( init<const Function&>() )
          .def( init<FFExpression&>() )
          
          .def( self == self )
          .def( self != self )
          
          .def( "__rrshift__", &__rrshift__QDataStream<FFExpression>,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
          .def( "__rlshift__", &__rlshift__QDataStream<FFExpression>,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
          
          .def( "forceFieldIDs", (const QSet<ForceFieldID>& (FFExpression::*)() const)
                    &FFExpression::forceFieldIDs, 
                    return_value_policy<copy_const_reference>() )
    
          .def( "dependencies", (const QSet<Function>& (FFExpression::*)() const)
                    &FFExpression::dependencies,
                    return_value_policy<copy_const_reference>() )

          .def( "function", (const Function& (FFExpression::*)() const)
                    &FFExpression::function,
                    return_value_policy<copy_const_reference>() )

          .def( "expression", (const Expression& (FFExpression::*)() const)
                    &FFExpression::expression,
                    return_value_policy<copy_const_reference>() )

          .def( "evaluate", (double (FFExpression::*)(const Values&) const)
                    &FFExpression::evaluate )
    ;
}

}
