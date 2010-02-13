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

#include "privatekey.h"
#include "publickey.h"
#include "pubprilock.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

using namespace SireSec;
using namespace Siren;

static const RegisterObject<PrivateKey> r_private_key;

/** Null Constructor */
PrivateKey::PrivateKey() : Implements<PrivateKey,Key>( Key::NonStreamable )
{}

/** Internal constructor */
PrivateKey::PrivateKey(const boost::shared_ptr<Crypt::KeyContext> &context)
           : Implements<PrivateKey,Key>( Key::NonStreamable ),
             d(context)
{}

/** Copy constructor */
PrivateKey::PrivateKey(const PrivateKey &other) 
           : Implements<PrivateKey,Key>(other), d(other.d)
{}

/** Destructor */
PrivateKey::~PrivateKey()
{}

/** Copy assignment operator */
PrivateKey& PrivateKey::operator=(const PrivateKey &other)
{
    d = other.d;
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool PrivateKey::operator==(const PrivateKey &other) const
{
    return d.get() == other.d.get() and super::operator==(other);
}

/** Comparison operator */
bool PrivateKey::operator!=(const PrivateKey &other) const
{
    return not PrivateKey::operator==(other);
}

bool PrivateKey::isValid() const
{
    return d.get() != 0;
}

/** Eventually I'll tie this down to a single thread... */
bool PrivateKey::availableToThisThread() const
{
    return true;
}

QString PrivateKey::toString() const
{
    if (d.get() == 0)
        return QObject::tr("PrivateKey::null");

    else if (availableToThisThread())
    {
        if (isTemporal())
            return QObject::tr("PrivateKey( available - expires %1 )")
                            .arg(this->bestBefore().toString());
        else
            return QObject::tr("PrivateKey( available )");
    }
    else
        return QObject::tr("PrivateKey( unavailable to this thread )");
}

uint PrivateKey::hashCode() const
{
    return qHash( PrivateKey::typeName() );
}

void PrivateKey::stream(Siren::Stream &s)
{
    this->assertIsStreamable();
}

/** Generate a public/private key pair, optionally supplying
    the key type (algorithm used by the pair) */
boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(QString label, KeyTypes::KeyType keytype, int keylength)
{
    CRYPT_CONTEXT crypt_context;
    
    try
    {
        Crypt::SireSec_init();
    
        int status;
    
        //generate the encryption context (this sets the key type)
        switch (keytype)
        {
            case KeyTypes::DEFAULT:
            case KeyTypes::RSA:
                status = cryptCreateContext( &crypt_context, CRYPT_UNUSED, 
                                             CRYPT_ALGO_RSA );
                break;
            
            case KeyTypes::Elgamal:
                status = cryptCreateContext( &crypt_context, CRYPT_UNUSED,
                                             CRYPT_ALGO_ELGAMAL );
                                             
            default:
                throw Siren::unsupported( QObject::tr(
                        "An unsupported public/private key type was requested."),
                            CODELOC );
        }
        
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        //now add a label to the key - this is necessary so that this
        //key can be retrieved from a keyset
        if (label.isEmpty())
            label = "unnamed";
        
        QByteArray utf8_label = label.toUtf8();
        
        status = cryptSetAttributeString( crypt_context, CRYPT_CTXINFO_LABEL,
                                          utf8_label.constData(), utf8_label.length() );
                                          
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        //if specified, set the key length
        if (keylength > 0)
        {
            //don't allow keys less than 128 bytes
            if (keylength < 128)
                keylength = 128;
                
            status = cryptSetAttribute( crypt_context, CRYPT_CTXINFO_KEYSIZE, keylength );
            
            Crypt::assertValidStatus(status, QUICK_CODELOC);
        }
        
        //finally(!) generate the keys
        status = cryptGenerateKey( crypt_context );
        
        Crypt::assertValidStatus(status, QUICK_CODELOC);
       
        //create the handle for the key context, and pass it to the keys
        boost::shared_ptr<Crypt::KeyContext> d( new Crypt::KeyContext(crypt_context) );
        
        return boost::tuple<PublicKey,PrivateKey>( PublicKey(d), PrivateKey(d) );
    }
    catch(...)
    {
        cryptDestroyContext(crypt_context);
        throw;
    }
}

/** Generate a public/private key pair, optionally supplying
    the key type (algorithm used by the pair) */
boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(KeyTypes::KeyType keytype, int keylength)
{
    return generate(QString::null, keytype, keylength);
}
