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

#ifndef SIREN_DETAIL_MD5SUM_H
#define SIREN_DETAIL_MD5SUM_H

#include "object.h"

SIREN_BEGIN_HEADER

class QFile;
class QString;
class QByteArray;

typedef unsigned char md5_byte_t;

namespace SireStream
{

/**
 * These functions are used to simplify the generation of md5sums of 
 * buffers and files
 *
 * @author Christopher Woods
 */

class SIREN_EXPORT MD5Sum : public ConcreteObject<MD5Sum,Object>
{

public:
    MD5Sum();

    MD5Sum(const QByteArray &buffer);

    MD5Sum(const char *buffer, unsigned int sz);

    MD5Sum(const QString &file);

    MD5Sum(const MD5Sum &sum);
  
    ~MD5Sum();
  
    const MD5Sum& operator=(const MD5Sum &other);

    bool operator==(const MD5Sum &other) const;
    bool operator!=(const MD5Sum &other) const;

    const md5_byte_t* digest() const;

    QString toString() const;

private:
    void generate(const char *buffer, unsigned int sz);
  
    /** Pointer to the storage of the md5 digest */
    md5_byte_t dgst[16];
};

}

Q_DECLARE_METATYPE( Siren::MD5Sum )

SIREN_EXPOSE_CLASS( Siren::MD5Sum )

SIREN_END_HEADER

#endif
