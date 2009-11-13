/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREN_XMLSTREAM_H
#define SIREN_XMLSTREAM_H

#include <QDomNode>

#include "detail/sharedpolypointer.hpp"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This class provides the means to stream objects
    to XML */
class SIREN_EXPORT XMLStream
{
public:
    XMLStream();
    
    ~XMLStream();
    
    QDomNode createChild();
    
    void closeChild();
};

template<class T>
XMLStream& operator<<(XMLStream&, const detail::SharedPolyPointer<T>&);

template<class T>
XMLStream& operator>>(XMLStream&, detail::SharedPolyPointer<T>&);

template<class T>
XMLStream& operator<<(XMLStream&, const detail::GlobalSharedPointer<T>&);

template<class T>
XMLStream& operator>>(XMLStream&, detail::GlobalSharedPointer<T>&);

}

SIREN_EXPOSE_CLASS( Siren::XMLStream )

SIREN_END_HEADER

#endif
