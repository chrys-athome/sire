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

#ifndef SIRE_PYTHON_PYTHONPACKET_H
#define SIRE_PYTHON_PYTHONPACKET_H

#include "SireCluster/workpacket.h"

SIRE_BEGIN_HEADER

class PythonPacket;

QDataStream& operator<<(QDataStream&, const PythonPacket&);
QDataStream& operator>>(QDataStream&, PythonPacket&);

/** This class provides a WorkPacket that runs a supplied Python script */
class SIRE_EXPORT PythonPacket : public SireCluster::WorkPacketBase
{

friend QDataStream& ::operator<<(QDataStream&, const PythonPacket&);
friend QDataStream& ::operator>>(QDataStream&, PythonPacket&);

public:
    PythonPacket();

    PythonPacket(const QString &filename);

    PythonPacket(const PythonPacket &other);

    ~PythonPacket();

    PythonPacket& operator=(const PythonPacket &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<PythonPacket>() );
    }

    const char* what() const
    {
        return PythonPacket::typeName();
    }

    PythonPacket* clone() const
    {
        return new PythonPacket(*this);
    }

    bool hasFinished() const;

protected:
    float chunk();

private:
    /** The contents of the script to execute */
    QString script_contents;
};

Q_DECLARE_METATYPE( PythonPacket )

SIRE_END_HEADER

#endif
