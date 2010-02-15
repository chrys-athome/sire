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

#include "crypt.h"  // CONDITIONAL_INCLUDE

#include <QUuid>
#include <QThreadStorage>

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>

#include "password.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"
#include "Siren/mutex.h"

#include <QDebug>

using namespace SireSec;
using namespace Siren;

////
//// Implementation of PasswordData
////

namespace SireSec 
{ 
    namespace detail
    {
        Q_GLOBAL_STATIC( Mutex, passwordMutex );
        
        static QHash<QUuid,QString> *global_passwords(0);
        static QThreadStorage< QHash<QUuid,QString>* > local_passwords;

        QString get_password(const QUuid &uid, bool is_local)
        {
            if (is_local)
            {
                if (local_passwords.hasLocalData())
                {
                    return local_passwords.localData()->value(uid);
                }
            }
            else
            {
                MutexLocker lkr( passwordMutex() );
                
                if (global_passwords)
                {
                    return global_passwords->value(uid);
                }
            }
            
            return QString::null;
        }
    
        class PasswordData : public boost::noncopyable
        {
        public:
            QUuid uid;
            bool is_local;
        
            PasswordData(const QString &password, bool thread_local)
                   : boost::noncopyable()
            {
                uid = QUuid::createUuid();
                is_local = thread_local;
            
                if (is_local)
                {
                    if (not local_passwords.hasLocalData())
                    {
                        local_passwords.setLocalData( new QHash<QUuid,QString>() );
                    }
                    
                    local_passwords.localData()->insert(uid, password);
                }
                else
                {
                    MutexLocker lkr( passwordMutex() );
                    
                    if (global_passwords == 0)
                    {
                        global_passwords = new QHash<QUuid,QString>();
                    }
                    
                    global_passwords->insert(uid, password);
                }
            }
            
            ~PasswordData()
            {
                if (is_local)
                {
                    if (local_passwords.hasLocalData())
                    {
                        if (local_passwords.localData()->contains(uid))
                        {
                            local_passwords.localData()->remove(uid);
                            
                            if (local_passwords.localData()->isEmpty())
                            {
                                local_passwords.setLocalData(0);
                            }
                        }
                    }
                }
                else 
                {
                    MutexLocker lkr( passwordMutex() );
                    
                    if (global_passwords)
                    {
                        global_passwords->remove(uid);
                        
                        if (global_passwords->isEmpty())
                        {
                            delete global_passwords;
                            global_passwords = 0;
                        }
                    }
                }
            }
            
            bool canCopy() const
            {
                if (is_local)
                {
                    if (local_passwords.hasLocalData())
                    {
                        if (local_passwords.localData()->contains(uid))
                            return true;
                    }
                    
                    return false;
                }
                
                return true;
            }
            
            bool availableToThisThread() const
            {
                return canCopy();
            }
        };
    }
}

using namespace SireSec::detail;

//////
////// Implementation of Password
//////

static const RegisterObject<Password> r_password;

/** Null constructor */
Password::Password() : Implements<Password,Key>()
{}

/** Construct using the passed password - note that this password
    will be streamable and copyable between threads, so is not 
    the most secure */
Password::Password(const QString &password)
         : Implements<Password,Key>()
{
    if (not password.isEmpty())
    {
        d.reset( new PasswordData(password,false) );
    }
}

/** Construct using the passed password and key options. */
Password::Password(const QString &password, const Key::Options &key_options)
         : Implements<Password,Key>(key_options)
{
    if (not password.isEmpty())
    {
        d.reset( new PasswordData(password, key_options.testFlag(Key::LockedToThread) ) ); 
    }
    else
        this->operator=(Password());
}

/** Construct using the passed password, providing an expiry date for 
    the password */
Password::Password(const QString &password, const QDateTime &expiry_date)
         : Implements<Password,Key>(expiry_date, Key::Unrestricted)
{
    if (not password.isEmpty())
    {
        if (expiry_date < QDateTime::currentDateTime())
        {
            d.reset( new PasswordData(password,false) );
        }
    }
    else
        this->operator=(Password());
}

/** Construct using the passed password, key options and expiry date */
Password::Password(const QString &password, const Key::Options &key_options,
                   const QDateTime &expiry_date)
         : Implements<Password,Key>(expiry_date, key_options)
{
    if (not password.isEmpty())
    {
        if (expiry_date < QDateTime::currentDateTime())
        {
            d.reset( new PasswordData(password, 
                                      key_options.testFlag(Key::LockedToThread) ) );
        }
    }
    else
        this->operator=(Password());
}

/** Construct using the passed password, key options and expiry date */
Password::Password(const QString &password, 
                   const QDateTime &expiry_date,
                   const Key::Options &key_options)
         : Implements<Password,Key>(expiry_date, key_options)
{
    if (not password.isEmpty())
    {
        if (expiry_date < QDateTime::currentDateTime())
        {
            d.reset( new PasswordData(password, 
                                      key_options.testFlag(Key::LockedToThread) ) );
        }
    }
    else
        this->operator=(Password());
}

/** Copy constructor */
Password::Password(const Password &other) : Implements<Password,Key>(other)
{
    if (other.d.get() != 0)
    {
        if (other.d->canCopy())
            d = other.d;
        else
            this->operator=(Password());
    }
}

/** Destructor */
Password::~Password()
{}

/** Copy assignment operator */
Password& Password::operator=(const Password &other)
{
    if (this != &other)
    {
        if (d.get() != other.d.get())
        {
            if (other.d.get())
            {
                if (other.d->canCopy())
                    d = other.d;
                else
                    return this->operator=(Password());
            }
        }
        
        super::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Password::operator==(const Password &other) const
{
    return d.get() == other.d.get() and super::operator==(other);
}

/** Comparison operator */
bool Password::operator!=(const Password &other) const
{
    return not Password::operator==(other);
}

/** Return whether or not this password is available to this thread */
bool Password::availableToThisThread() const
{
    if (d.get() != 0)
    {
        return d->availableToThisThread();
    }
    else
        return true;
}

QString Password::toString() const
{
    if (d.get() == 0)
        return QObject::tr("Password::null");

    else if (availableToThisThread())
    {
        if (isTemporal())
            return QObject::tr("Password( available - expires %1 )")
                                    .arg(bestBefore().toString());
        else if (isStreamable())
            return QObject::tr("Password( available and streamable )");
        else
            return QObject::tr("Password( available, but not streamable )");
    }
    else
    {
        return QObject::tr("Password( unavailable to this thread )");
    }
}

uint Password::hashCode() const
{
    return qHash( Password::toString() );
}

void Password::stream(Siren::Stream &s)
{
    if (s.isSaving())
        this->assertIsStreamable();
    
    QString password;
    
    if (s.isSaving())
    {
        if (d.get() != 0)
            password = get_password( d->uid, d->is_local );
    }
    
    s.assertVersion<Password>(1);
    
    Schema schema = s.item<Password>();
    
    schema.data("password") & password;
    
    super::stream( schema.base() );
    
    if (s.isLoading())
    {
        d.reset( new PasswordData(password,false) );
    }
}

//////
////// Implementation of PasswordLock
//////

static const RegisterObject<PasswordLock> r_passwordlock;

/** Construc without a key */
PasswordLock::PasswordLock() : Implements<PasswordLock,Lock>()
{}

/** Construct with the passed key 

    \throw SireSec::missing_key
*/
PasswordLock::PasswordLock(const Key &key) 
             : Implements<PasswordLock,Lock>(key.extract<Password>())
{}

/** Construct with the passed password key */
PasswordLock::PasswordLock(const Password &password)
             : Implements<PasswordLock,Lock>(password)
{}

/** Copy constructor */
PasswordLock::PasswordLock(const PasswordLock &other)
             : Implements<PasswordLock,Lock>(other)
{}

/** Destructor */
PasswordLock::~PasswordLock()
{}

/** Copy assignment operator */
PasswordLock& PasswordLock::operator=(const PasswordLock &other)
{
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool PasswordLock::operator==(const PasswordLock &other) const
{
    return super::operator==(other);
}

/** Comparison operator */
bool PasswordLock::operator!=(const PasswordLock &other) const
{
    return super::operator!=(other);
}

QString PasswordLock::toString() const
{
    if (this->hasKey())
        return QObject::tr("PasswordLock( %1 )")
                    .arg( this->getKey().toString() );
    
    else
        return QObject::tr("PasswordLock( Password::null )");
}

uint PasswordLock::hashCode() const
{
    return qHash( PasswordLock::typeName() );
}

void PasswordLock::stream(Siren::Stream &s)
{
    s.assertVersion<PasswordLock>(1);
    
    Schema schema = s.item<PasswordLock>();
    
    super::stream( schema.base() );
}

QByteArray PasswordLock::extractUtf8(const Password &password) const
{
    if (password.d.get() == 0)
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to encrypt or decrypt password-protected data with "
                "a null or empty password!"), CODELOC );
        
    password.assertAvailableToThisThread();

    QByteArray utf8_passwd = get_password(password.d->uid, 
                                          password.d->is_local).toUtf8();
    
    if (utf8_passwd.isEmpty())
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to encrypt or decrypt password-protected data with "
                "a null or empty password!"), CODELOC );

    return utf8_passwd;
}

/** Use this lock to encrypt the data from 'in_stream' and write 
    the encrypted output to 'out_stream'
    
    \throw SireSec::missing_key
*/
void PasswordLock::encryptStream(QDataStream &in_stream, 
                                 QDataStream &out_stream,
                                 int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (not this->hasKey())
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to password protect data without "
                "providing a password!"), CODELOC );
    
    const Password &passwd = this->getKey().asA<Password>();
    
    //get the password - convert to UTF8
    QByteArray utf8_passwd = extractUtf8(passwd);

    //create an envelope to compress the data
    CRYPT_ENVELOPE compress_envelope = Crypt::createDefaultEnvelope();
    
    //create an envelope to encrypt the data
    CRYPT_ENVELOPE crypt_envelope = Crypt::createDefaultEnvelope();

    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_COMPRESSION, CRYPT_UNUSED );

        //add integrity checking so that the data cannot be tampered with
        int status = cryptSetAttribute( crypt_envelope,
                                        CRYPT_ENVINFO_INTEGRITY,
                                        CRYPT_INTEGRITY_FULL );

        Crypt::assertValidStatus(status, QUICK_CODELOC);

        //set the password
        status = cryptSetAttributeString( crypt_envelope, CRYPT_ENVINFO_PASSWORD,
                                          utf8_passwd.constData(), 
                                          utf8_passwd.length() );
                                              
        Crypt::assertValidStatus(status, QUICK_CODELOC);
                             
        //process the data - compress, then encrypt
        Crypt::processThroughEnvelopes(compress_envelope, crypt_envelope, 
                                       in_stream, out_stream);
        
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(crypt_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(crypt_envelope);
        throw;
    }
}

static void supply_key(QByteArray utf8_passwd,
                       CRYPT_ENVELOPE crypt_envelope, int ntries)
{
    int status = cryptSetAttributeString( crypt_envelope, CRYPT_ENVINFO_PASSWORD,
                                          utf8_passwd.constData(),
                                          utf8_passwd.length() );
        
    if (status == CRYPT_OK)
        return;

    else if (status == CRYPT_ERROR_WRONGKEY)
        throw SireSec::invalid_key( QObject::tr(
                "The supplied password is incorrect. Cannot decrypt the data!"),
                     CODELOC );
               
    else if (ntries > 5)
        Crypt::assertValidStatus(status, QUICK_CODELOC);
}

/** Decrypt the data read from 'in_stream' and write the output to 'out_stream'

    \throw SireSec::missing_key
    \throw SireSec::invalid_key
*/
void PasswordLock::decryptStream(QDataStream &in_stream, 
                                 QDataStream &out_stream,
                                 int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (not this->hasKey())
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to decrypt password-protected data without "
                "providing a password!"), CODELOC );
    
    const Password &passwd = this->getKey().asA<Password>();
    
    //get the password - convert to UTF8
    QByteArray utf8_passwd = extractUtf8(passwd);
    
    //create an envelope to encrypt the data
    CRYPT_ENVELOPE crypt_envelope = Crypt::createAutoFormatEnvelope();
    CRYPT_ENVELOPE compress_envelope = Crypt::createAutoFormatEnvelope();

    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        //process the data - decrypt, then uncompress, 
        //providing the password when required
        Crypt::processThroughEnvelopes(crypt_envelope, compress_envelope,
                                       in_stream, out_stream,
                                       boost::bind(supply_key, utf8_passwd, _1, _2));
        
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(compress_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(compress_envelope);
        throw;
    }
}
