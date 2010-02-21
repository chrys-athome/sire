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

#ifndef SIRESEC_LOCK_H
#define SIRESEC_LOCK_H

#include "Siren/object.h"

#include "key.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

class Lock;

typedef Siren::ObjPtr<Lock> LockPtr;

/** This is the virtual base class of all Lock objects.
    A Lock is an object that can be used, together with 
    a Key, to encrypt or decrypt some data, e.g.
    
    encrypted_data = lock.encrypt(data, key);
    
    or
    
    data = lock.decrypt(encrypted_data, key);
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT Lock : public Siren::Extends<Lock,Siren::Object>
{
public:
    Lock();
    Lock(const Lock &other);
    
    virtual ~Lock();
    
    static QString typeName();
    
    void stream(Siren::Stream &s);

    bool hasKey() const;

    LockPtr activate(const Key &key) const;
    LockPtr deactivate() const;

    void encrypt(QDataStream &in_stream, QDataStream &out_stream) const;
    void decrypt(QDataStream &in_stream, QDataStream &out_stream) const;
    
    QByteArray encryptString(const QString &string) const;
    QString decryptString(const QByteArray &string) const;
    
    void encryptFile(const QString &in_file, const QString &out_file) const;
    void decryptFile(const QString &in_file, const QString &out_file) const;
    
    QByteArray encrypt(const QByteArray &data) const;
    QByteArray decrypt(const QByteArray &data) const;
    
protected:
    Lock(const Key &key);

    Lock& operator=(const Lock &other);
    bool operator==(const Lock &other) const;
    bool operator!=(const Lock &other) const;
    
    void assertReadingStream(QDataStream &ds) const;
    void assertWritingStream(QDataStream &ds) const;

    virtual void setKey(const Key &key);
    
    const Key& getKey() const;

    /** Encrypt the data read from 'in_stream' and write the 
        output to 'out_stream'. If 'nbytes' is > 0, then
        this is the number of bytes to read from the
        input stream
        
        \throw Siren::io_error
        \throw SireSec::key_error
    */
    virtual void encryptStream(QDataStream &in_stream,
                               QDataStream &out_stream,
                               int nbytes) const=0;

    /** Decrypt the data read from 'in_stream' and write the 
        output to 'out_stream'. If 'nbytes' is > 0, then
        this is the number of bytes to read from the
        input stream
    
        \throw Siren::io_error
        \throw SireSec::key_error
    */
    virtual void decryptStream(QDataStream &in_stream,
                               QDataStream &out_stream,
                               int nbytes) const=0;

private:
    /** The key that is currently placed in this lock */
    KeyPtr current_key;
};

}

SIRE_EXPOSE_CLASS( SireSec::Lock )

SIRE_EXPOSE_OBJECT_PTR( SireSec::LockPtr, SireSec::Lock )

SIRE_END_HEADER

#endif
