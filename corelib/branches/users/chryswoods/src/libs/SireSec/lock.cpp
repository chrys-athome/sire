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

#include <QFile>

#include "lock.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace SireSec;
using namespace Siren;

static const RegisterObject<Lock> r_lock( VIRTUAL_CLASS );

/** Constructor */
Lock::Lock() : Extends<Lock,Object>()
{}

/** Internal constructor used to pass a key to the lock */
Lock::Lock(const Key &key) : Extends<Lock,Object>(), current_key(key)
{}

/** Copy constructor */
Lock::Lock(const Lock &other) 
     : Extends<Lock,Object>(other), current_key(other.current_key)
{}

/** Destructor */
Lock::~Lock()
{}

QString Lock::typeName()
{
    return "SireSec::Lock";
}

/** Copy assignment operator */
Lock& Lock::operator=(const Lock &other)
{
    current_key = other.current_key;
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool Lock::operator==(const Lock &other) const
{
    return current_key == other.current_key and super::operator==(other);
}

/** Comparison operator */
bool Lock::operator!=(const Lock &other) const
{
    return current_key != other.current_key or super::operator!=(other);
}
    
void Lock::assertReadingStream(QDataStream &ds) const
{}

void Lock::assertWritingStream(QDataStream &ds) const
{}

/** Internal function used to set the key used by this lock */
void Lock::setKey(const Key &key)
{
    current_key = key;
}

/** Return whether or not this lock has a key in it */
bool Lock::hasKey() const
{
    return current_key.constData() != 0;
}

/** Return the key in this lock - this raises an exception if there
    is no key
    
    \throw SireSec::missing_key
*/
const Key& Lock::getKey() const
{
    if (current_key.constData() == 0)
        throw SireSec::missing_key( QObject::tr(
                "There is no key available in the lock %1.")
                    .arg(this->what()), CODELOC );
                    
    return current_key.read();
}

/** Return an activated form of this lock, activated using the passed key 'key' 
    - this will tell the lock to use the passed key for all encryption or decryption.
    Note that this returns an activated lock (it doesn't change this
    lock) */
LockPtr Lock::activate(const Key &key) const
{
    if (current_key == key)
        return *this;
        
    LockPtr new_lock( this->clone().asA<Lock>() );
    new_lock.edit().setKey(key);
    
    return new_lock;
}

/** Return a deactivated form of this lock - this removes the key,
    thus making it safe to stream this lock to an untrusted source */
LockPtr Lock::deactivate() const
{
    if (current_key.constData() == 0)
        return *this;
        
    LockPtr new_lock( this->clone().asA<Lock>() );
    new_lock.edit().current_key = 0;
    
    return new_lock;
}

/** Stream this lock - note that any key that is currently
    in this lock will be streamed as well - this is potentially
    a security concern, as it could allow the key to be retrieved
    if you are not careful! If you want to ensure that you send
    the lock without the key, then you should deactivate the
    lock before streaming it, e.g 
    
    s << lock.deactivate();
*/
void Lock::stream(Siren::Stream &s)
{
    s.assertVersion<Lock>(1);
    
    Schema schema = s.item<Lock>();
    
    schema.data("key") & current_key;
    
    super::stream(schema.base());
}

/** Encrypt the data read from the stream 'in_stream' and write
    the output to the stream 'out_stream'
    
    \throw SireSec::missing_key
*/
void Lock::encrypt(QDataStream &in_stream, QDataStream &out_stream) const
{
    this->encryptStream(in_stream, out_stream, -1);
}

/** Encrypt the data read from the stream 'in_stream' and write
    the output to the stream 'out_stream'
    
    \throw SireSec::missing_key
    \throw SireSec::invalid_key
*/
void Lock::decrypt(QDataStream &in_stream, QDataStream &out_stream) const
{
    this->decryptStream(in_stream, out_stream, -1);
}
    
/** Encrypt the passed string, returning the result

    \throw SireSec::missing_key
*/
QByteArray Lock::encryptString(const QString &string) const
{
    if (string.isEmpty())
        return QByteArray();

    QByteArray utf8_string = string.toUtf8();
    QByteArray encrypted_string;
    
    QDataStream in_stream(utf8_string);
    QDataStream out_stream(&encrypted_string, QIODevice::WriteOnly);
    
    this->encryptStream(in_stream, out_stream, utf8_string.count());
    
    return encrypted_string;
}

/** Encrypt the passed string, returning the result

    \throw SireSec::missing_key
*/
QString Lock::decryptString(const QByteArray &data) const
{
    if (data.isEmpty())
        return QString();
        
    QByteArray decrypted_string;
    
    QDataStream in_stream(data);
    QDataStream out_stream(&decrypted_string, QIODevice::WriteOnly);
    
    this->decryptStream(in_stream, out_stream, data.count());
    
    return QString::fromUtf8(decrypted_string);
}

/** Encrypt the contents of the file 'in_file', writing the output
    to 'out_file'
    
    \throw SireSec::missing_key
*/
void Lock::encryptFile(const QString &in_file, const QString &out_file) const
{
    QFile in(in_file);
    
    if (not in.open(QIODevice::ReadOnly))
        throw Siren::file_error(in, CODELOC);
        
    QFile out(out_file);
    
    if (not out.open(QIODevice::WriteOnly))
        throw Siren::file_error(out, CODELOC);
        
    QDataStream in_stream(&in);
    QDataStream out_stream(&out);
    
    this->encryptStream(in_stream, out_stream, -1);
}

/** Decrypt the contents of the file 'in_file', writing the output
    to 'out_file'
    
    \throw SireSec::missing_key
*/
void Lock::decryptFile(const QString &in_file, const QString &out_file) const
{
    QFile in(in_file);
    
    if (not in.open(QIODevice::ReadOnly))
        throw Siren::file_error(in, CODELOC);
        
    QFile out(out_file);
    
    if (not out.open(QIODevice::WriteOnly))
        throw Siren::file_error(out, CODELOC);
        
    QDataStream in_stream(&in);
    QDataStream out_stream(&out);
    
    this->decryptStream(in_stream, out_stream, -1);
}

/** Encrypt the data in 'data', returning the output.
    
    \throw SireSec::missing_key
*/
QByteArray Lock::encrypt(const QByteArray &data) const
{
    if (data.isEmpty())
        return data;

    QByteArray encrypted_data;
    
    QDataStream in_stream(data);
    QDataStream out_stream(&encrypted_data, QIODevice::WriteOnly);
    
    this->encryptStream(in_stream, out_stream, data.count());
    
    return encrypted_data;
}

/** Decrypt the data in 'data', returning the output.
    
    \throw SireSec::missing_key
*/
QByteArray Lock::decrypt(const QByteArray &data) const
{
    if (data.isEmpty())
        return data;

    QByteArray decrypted_data;
    
    QDataStream in_stream(data);
    QDataStream out_stream(&decrypted_data, QIODevice::WriteOnly);
    
    this->decryptStream(in_stream, out_stream, data.count());
    
    return decrypted_data;
}
