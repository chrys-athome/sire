/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRESTREAM_STREAMHELPER_HPP
#define SIRESTREAM_STREAMHELPER_HPP

#include <QByteArray>
#include <QDataStream>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireStream
{

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

template<class T>
SIRE_OUTOFLINE_TEMPLATE
T load(const QByteArray &data)
{
    QDataStream ds(data);

    T new_obj;
    ds >> new_obj;

    return new_obj;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QByteArray save(const T &old_obj)
{
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);

    ds << old_obj;

    return data;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_END_HEADER

#endif
