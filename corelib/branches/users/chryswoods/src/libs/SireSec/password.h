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

#ifndef SIRESEC_PASSWORD_H
#define SIRESEC_PASSWORD_H

#include <boost/shared_ptr.hpp>

#include "lock.h"
#include "key.h"

SIRE_BEGIN_HEADER

class QUuid;

namespace SireSec
{

class Password;

/** This is a lock that allows encryption of decryption
    of data using a Password
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT PasswordLock : public Siren::Implements<PasswordLock,Lock>
{
public:
    PasswordLock();
    PasswordLock(const Key &key);
    PasswordLock(const Password &password);
    
    PasswordLock(const PasswordLock &other);
    
    ~PasswordLock();
    
    PasswordLock& operator=(const PasswordLock &other);
    
    bool operator==(const PasswordLock &other) const;
    bool operator!=(const PasswordLock &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

protected:
    void encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;
                       
    void decryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;

private:
    QByteArray extractUtf8(const Password &password) const;
};

namespace detail{ class PasswordData; }

/** This is a Password, which is the Key used for a PasswordLock */
class SIRESEC_EXPORT Password : public Siren::Implements<Password,Key>
{
public:
    Password();
    Password(const QString &password);

    Password(const QString &password, const Key::Options &key_options);

    Password(const QString &password, const QDateTime &expiry_date);
    
    Password(const QString &password, 
             const Key::Options &key_options,
             const QDateTime &expiry_date);
    
    Password(const QString &password, 
             const QDateTime &expiry_date,
             const Key::Options &key_options);
    
    Password(const Password &other);
    
    ~Password();
    
    Password& operator=(const Password &other);
    
    bool operator==(const Password &other) const;
    bool operator!=(const Password &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    bool availableToThisThread() const;

private:
    friend class PasswordLock;

    /** Data containing the UID of the password in the password hash
        (passwords are not stored directly in this object!) */
    boost::shared_ptr<detail::PasswordData> d;
};

}

Q_DECLARE_METATYPE( SireSec::PasswordLock )
Q_DECLARE_METATYPE( SireSec::Password )

SIRE_EXPOSE_CLASS( SireSec::PasswordLock )
SIRE_EXPOSE_CLASS( SireSec::Password )

SIRE_END_HEADER

#endif
