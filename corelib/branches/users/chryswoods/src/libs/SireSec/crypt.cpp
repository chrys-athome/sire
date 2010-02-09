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

#include "cryptlib.h" // CONDITIONAL_INCLUDE

#include <QAtomicInt>

#include "crypt.h"

#include "Siren/errors.h"

#include <QDebug>

namespace SireSec
{

static QAtomicInt init_int(-1);

void SireSec_init()
{
    if (not init_int.ref())
    {
        const int status = cryptInit();
    
        if (status != CRYPT_OK)
        {
            throw Siren::unsupported( QObject::tr(
                    "cryptlib could not be initialised - encryption not supported!"),
                        CODELOC );
        }
    }
}

void SireSec_end()
{
    cryptEnd();
}

void check_status(int status)
{
    if (status != CRYPT_OK)
        qDebug() << "ERROR";
}

/** Encrypt, compress and return the passed data, using the  
    password 'password' */
QByteArray SIRESEC_EXPORT encrypt(const QByteArray &data, const QString &password)
{
    if (data.isEmpty())
        return QByteArray();

    SireSec_init();

    qDebug() << "ENCRYPING '" << data << "'...";
        
    CRYPT_ENVELOPE crypt_envelope;
    int bytes_copied;
    int status;
    
    cryptCreateEnvelope( &crypt_envelope, CRYPT_UNUSED, CRYPT_FORMAT_CRYPTLIB );
    
    cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_COMPRESSION, CRYPT_UNUSED );

    QByteArray utf8_pword = password.toUtf8();

    cryptSetAttributeString( crypt_envelope, CRYPT_ENVINFO_PASSWORD,
                             utf8_pword.constData(), utf8_pword.length() );

    cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_DATASIZE, data.length() );
    
    status = cryptPushData( crypt_envelope, data.constData(), 
                            data.length(), &bytes_copied );
    
    check_status(status);
    
    status = cryptFlushData( crypt_envelope );

    check_status(status);
    
    QByteArray encrypted_data(data);
    
    status = cryptPopData( crypt_envelope, encrypted_data.data(), 
                           encrypted_data.length(), &bytes_copied );

    check_status(status);

    qDebug() << "...TO '" << encrypted_data << "'";
    
    return encrypted_data;
}

/** Decrypt and uncompress that passed encrypted data, which 
    was encrypted using the passed password */
QByteArray SIRESEC_EXPORT decrypt(const QByteArray &data, const QString &password)
{
    if (data.isEmpty())
        return data;
        
    qDebug() << "Decrypting '" << data << "'...";
        
    SireSec_init();
    
    CRYPT_ENVELOPE crypt_envelope; 
    int bytes_copied, status;

    cryptCreateEnvelope( &crypt_envelope, CRYPT_UNUSED, CRYPT_FORMAT_AUTO );

    cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_COMPRESSION, CRYPT_UNUSED );

    status = cryptPushData( crypt_envelope, data, data.length(), &bytes_copied );

    check_status(status);

    QByteArray utf8_pword = password.toUtf8();

    cryptSetAttributeString( crypt_envelope, CRYPT_ENVINFO_PASSWORD, 
                             utf8_pword, utf8_pword.length() );

    status = cryptFlushData( crypt_envelope ); 

    check_status(status);
    
    QByteArray decrypted_data( 5 * data.length(), ' ' );
    status = cryptPopData( crypt_envelope, decrypted_data.data(), decrypted_data.length(),
                           &bytes_copied ); 

    check_status(status);
                  
    cryptDestroyEnvelope( crypt_envelope );

    qDebug() << "...to '" << decrypted_data << "'";
    
    return decrypted_data;
}

} // end of namespace SireSec
