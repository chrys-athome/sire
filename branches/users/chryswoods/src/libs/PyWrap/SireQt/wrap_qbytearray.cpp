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

#include <Python.h>
#include <boost/python.hpp>

#include <QByteArray>

#include "qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

void  
SIREQT_EXPORT
export_QByteArray()
{
    class_<QByteArray>("QByteArray", init<>())
        .def( init<const char*>() )
        .def( init<const char*, int>() )
        .def( init<int, char>() )
        .def( init<const QByteArray&>() )
    
        .def( "at", &QByteArray::at )
        .def( "size", &QByteArray::size )
    
        .def( "__rrshift__", &__rrshift__QDataStream<QByteArray>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<QByteArray>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
#warning need to complete wrapping of QByteArray
    ;
}
