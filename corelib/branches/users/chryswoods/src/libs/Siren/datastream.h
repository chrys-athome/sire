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

#ifndef SIREN_DATASTREAM_H
#define SIREN_DATASTREAM_H

#include "stream.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class SharedDataRegistry;

/** This is a streaming class that is used to help stream implicitly shared data. 
    This class ensures that only a single copy of the implicitly shared data is actually 
    written to the stream, thus reducing data bloat, and also preserving the shared data
    of the objects when they are read back into the program.

    @author Christopher Woods
*/
class SIREN_EXPORT DataStream : public Stream
{
public:
    DataStream();
   
    DataStream(QIODevice *d);
    DataStream(QByteArray *a, QIODevice::OpenMode mode);
    DataStream(const QByteArray &a);
    
    DataStream(QDataStream &ds);

    ~DataStream();

private:
    void readVersion();
    void writeVersion();

    static quint64 magic();

    bool peekMagic();

    quint32 readObjectID();

    /** Shared pointer to the registry of shared objects that 
        have already been streamed */
    std::auto_ptr<SharedDataRegistry> registry;
    
    /** Shared pointer to the datastream */
    std::auto_ptr<QDataStream> local_ds;

    /** Reference to the actual QDataStream that is used to
        stream the data */
    QDataStream &ds;
};

} // end of namespace Siren

SIREN_END_HEADER

#endif
