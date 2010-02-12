/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIRESEC_ERRORS_H
#define SIRESEC_ERRORS_H

#include "Siren/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

/** This exception is thrown an attempt is made to copy or stream
    a key that is private */
class SIRESEC_EXPORT key_is_private
        : public Siren::ImplementsException<key_is_private,Siren::exception>
{
public:
    key_is_private();

    key_is_private(QString err, QString place = QString::null);

    key_is_private(const key_is_private &other);

    ~key_is_private() throw();
};

/** This exception is thrown when a key is missing */
class SIRESEC_EXPORT missing_key
        : public Siren::ImplementsException<missing_key,Siren::exception>
{
public:
    missing_key();

    missing_key(QString err, QString place = QString::null);

    missing_key(const missing_key &other);

    ~missing_key() throw();
};

/** This exception is thrown when an invalid (or just plain wrong) key is used */
class SIRESEC_EXPORT invalid_key
        : public Siren::ImplementsException<invalid_key,Siren::exception>
{
public:
    invalid_key();

    invalid_key(QString err, QString place = QString::null);

    invalid_key(const invalid_key &other);

    ~invalid_key() throw();
};

/** This exception is thrown when something went wrong with cryptlib */
class SIRESEC_EXPORT crypt_error
        : public Siren::ImplementsException<crypt_error,Siren::exception>
{
public:
    crypt_error();

    crypt_error(QString err, QString place = QString::null);

    crypt_error(const crypt_error &other);

    ~crypt_error() throw();
};

}

Q_DECLARE_METATYPE(SireSec::key_is_private)
Q_DECLARE_METATYPE(SireSec::missing_key)
Q_DECLARE_METATYPE(SireSec::invalid_key)
Q_DECLARE_METATYPE(SireSec::crypt_error)

SIRE_END_HEADER

#endif
