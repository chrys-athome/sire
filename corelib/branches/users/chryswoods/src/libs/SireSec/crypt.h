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

#ifndef SIRESEC_CRYPT_H
#define SIRESEC_CRYPT_H

#include "cryptlib.h"  // CONDITIONAL_INCLUDE

#include <boost/function.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

/** This file contains functions used to interface SireSec with cryptlib.
    These functions are private to SireSec and are not exported.
    
    This header is not in the public API!
*/

namespace SireSec
{
    namespace Crypt
    {
        class KeyContext
        {
        public:
            KeyContext(CRYPT_CONTEXT context) : crypt_context(context)
            {}
            
            ~KeyContext()
            {
                cryptDestroyContext(crypt_context);
            }
            
            CRYPT_CONTEXT crypt_context;
        };
    
        class CertContext
        {
        public:
            CertContext(CRYPT_CERTIFICATE cert) : crypt_certificate(cert)
            {}
            
            ~CertContext()
            {
                cryptDestroyCert(crypt_certificate);
            }
            
            CRYPT_CERTIFICATE crypt_certificate;
        };
    
        CRYPT_ENVELOPE createEnvelope(CRYPT_FORMAT_TYPE format);
        CRYPT_ENVELOPE createDefaultEnvelope();
        CRYPT_ENVELOPE createAutoFormatEnvelope();
        
        QString getEnvelopeDetails(CRYPT_ENVELOPE crypt_envelope);
        
        int processThroughEnvelope(CRYPT_ENVELOPE envelope,
                                   QDataStream &in_stream,
                                   QDataStream &out_stream,
                               boost::function<void (CRYPT_ENVELOPE,int)> key_function
                                   );
        
        int processThroughEnvelope(CRYPT_ENVELOPE envelope,
                                   QDataStream &in_stream,
                                   QDataStream &out_stream);
        
        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function0,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function1
                                   );
        
        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function
                                   );
        
        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream
                                   );
        
        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    CRYPT_ENVELOPE envelope2,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function0,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function1,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function2
                                    );
        
        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    CRYPT_ENVELOPE envelope2,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream,
                                boost::function<void (CRYPT_ENVELOPE,int)> key_function0
                                    );
        
        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    CRYPT_ENVELOPE envelope2,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream);
        
        void assertValidStatus(int status, QUICK_CODELOC_ARGS);
        
        QString getStatusString(int status);
    }
}

SIRE_END_HEADER

#endif
