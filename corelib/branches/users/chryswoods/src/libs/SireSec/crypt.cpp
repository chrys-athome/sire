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

#include "crypt.h" // CONDITIONAL_INCLUDE

#include "SireSec/errors.h"

#include "Siren/forages.h"
#include "Siren/errors.h"

#include <QDebug>

using namespace Siren;

namespace SireSec
{
    namespace Crypt
    {
        static QAtomicInt init_int(0);

        /** Initialise SireSec */
        void SireSec_init()
        {
            if (init_int.testAndSetOrdered(0, 1))
            {
                qDebug() << "cryptInit";
                const int status = cryptInit();
    
                if (status != CRYPT_OK)
                {
                    init_int = 0;
        
                    throw Siren::unsupported( QObject::tr(
                            "cryptlib could not be initialised - "
                            "encryption not supported!"),
                                CODELOC );
                }

                //initiate randomness poll to collect entropy for random number generation
                //(this is a slow process that will run asynchronously)
                cryptAddRandom( 0, CRYPT_RANDOM_SLOWPOLL );
            }
        }

        /** Finalise SireSec */
        void SireSec_end()
        {
            if (init_int.testAndSetOrdered(1,0))
            {
                qDebug() << "cryptEnd";
                int status = cryptEnd();
        
                if (status != CRYPT_OK)
                {
                    qDebug() << "cryptlib shutdown error!";
                }
            }
        }
        
        /** Return the status string corresponding to the cryptlib
            status 'status' */
        QString SIRESEC_EXPORT getStatusString(int status)
        {
            return "unknown";
        }
        
        /** Assert that the cryptlib status is ok */
        void SIRESEC_EXPORT assertValidStatus(int status, QUICK_CODELOC_ARGS)
        {
            if (status != CRYPT_OK)
            {
                throw SireSec::crypt_error( QObject::tr(
                        "There was a problem when running a cryptlib command. "
                        "The error was; %1.\n"
                        "It occurred in %2, line %3, function %4.")
                            .arg(getStatusString(status))
                            .arg(file).arg(line).arg(current_function), CODELOC );
            }
        }

        /** Process the data from 'in_stream' through the passed envelope,
            writing the output to 'out_stream', and using the passed function
            'key_function' to get and set the key if one is required.
            
             key_function is a function that is passed the envelope that
             needs the key, together with an integer to indicate how many
             times this function has been called previously in this session.
             
            This function returns the cryptlib content type of the 
            de-enveloped data. This is useful if you are decrypting and
            there are multiple nested envelopes
        */
        int SIRESEC_EXPORT processThroughEnvelope(CRYPT_ENVELOPE crypt_envelope,
                                                  QDataStream &input_stream,
                                                  QDataStream &output_stream,
                        boost::function<void (CRYPT_ENVELOPE, int)> key_function)
        {
            const int BUFFER_SIZE = 4096;

            char *in_buffer = 0;
            char *out_buffer = 0;
            
            int n_key_calls = 0;
            
            int n_total_bytes_read = 0;
            int n_total_bytes_written = 0;
            
            try
            {
                in_buffer = new char[BUFFER_SIZE];
                out_buffer = new char[BUFFER_SIZE];
                
                while (not input_stream.atEnd())
                {
                    int bytes_read = input_stream.readRawData(in_buffer, BUFFER_SIZE);
                    
                    if (bytes_read == -1)
                        throw Siren::io_error( QObject::tr(
                                "Error occurred while reading in data!"), CODELOC );

                    int offset = 0;
                                
                    while (bytes_read > 0)
                    {
                        int bytes_copied_in = 0;
                    
                        int status = cryptPushData( crypt_envelope, 
                                                    in_buffer+offset, bytes_read,
                                                    &bytes_copied_in );
                                       
                        if (status == CRYPT_ENVELOPE_RESOURCE)
                        {
                            //the envelope requires more information to continue
                            ++n_key_calls;
                            qDebug() << "NEED RESOURCE" << n_key_calls;
                            key_function(crypt_envelope, n_key_calls);
                            continue;
                        }
                        
                        assertValidStatus(status, QUICK_CODELOC);
                        
                        offset += bytes_copied_in;
                        bytes_read -= bytes_copied_in;
                        
                        n_total_bytes_read += bytes_copied_in;
                        
                        if (bytes_read > 0)
                        {
                            int bytes_copied_out = 0;
                        
                            //the envelope was full - we couldn't push more data into it!
                            status = cryptPopData( crypt_envelope, 
                                                   out_buffer, BUFFER_SIZE, 
                                                   &bytes_copied_out );
                                                   
                            assertValidStatus(status, QUICK_CODELOC);
                                
                            n_total_bytes_written += bytes_copied_out;
                                                              
                            int bytes_written = output_stream.writeRawData(
                                                    out_buffer, bytes_copied_out );

                            if (bytes_written != bytes_copied_out)
                                throw Siren::io_error( QObject::tr(
                                        "Error occurred while writing out data!"),
                                            CODELOC );
                        }
                    }
                    
                    //get the type decrypted data
                    int data_type;
                    
                    cryptGetAttribute( crypt_envelope, CRYPT_ENVINFO_CONTENTTYPE, 
                                       &data_type );
                    
                    //flush out the remaining data
                    int bytes_copied_out = 0;

                    do
                    {
                        int status = cryptFlushData( crypt_envelope );
                        
                        assertValidStatus(status, QUICK_CODELOC);
                        
                        status = cryptPopData( crypt_envelope, out_buffer, BUFFER_SIZE,
                                               &bytes_copied_out );
                                                   
                        assertValidStatus(status, QUICK_CODELOC);
                                      
                        n_total_bytes_written += bytes_copied_out;

                        if (bytes_copied_out > 0)
                        {
                            int bytes_written = output_stream.writeRawData(
                                                    out_buffer, bytes_copied_out );

                            if (bytes_written != bytes_copied_out)
                                throw Siren::io_error( QObject::tr(
                                        "Error occurred while writing out data!"),
                                            CODELOC );
                        }
                    
                    } while (bytes_copied_out > 0);
                    
                    delete[] in_buffer;
                    delete[] out_buffer;
                    
                    qDebug() << "READ" << n_total_bytes_read << "WRITTEN"
                             << n_total_bytes_written;
                    
                    return data_type;
                }
            }
            catch(...)
            {
                delete[] in_buffer;
                delete[] out_buffer;
                throw;
            }
            
            return -1;
        }

        void no_resource_available(CRYPT_ENVELOPE, int)
        {
            throw SireSec::missing_key( QObject::tr(
                    "No key is available, even though one is required "
                    "to de-envelope the data!"), CODELOC );
        }

        int SIRESEC_EXPORT processThroughEnvelope(CRYPT_ENVELOPE envelope,
                                                  QDataStream &in_stream,
                                                  QDataStream &out_stream)
        {
            return processThroughEnvelope(envelope, in_stream, out_stream,
                                          &no_resource_available );
        }

        CRYPT_ENVELOPE SIRESEC_EXPORT createDefaultEnvelope()
        {
            SireSec_init();
        
            CRYPT_ENVELOPE crypt_envelope;
            
            int status = cryptCreateEnvelope( &crypt_envelope,
                                              CRYPT_UNUSED,
                                              CRYPT_FORMAT_CRYPTLIB );
                                              
            assertValidStatus(status, QUICK_CODELOC);
            
            return crypt_envelope;
        }

    } // end of namespace Crypt
    
} // end of namespace SireSec
