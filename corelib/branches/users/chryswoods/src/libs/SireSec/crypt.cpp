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
                int status = cryptEnd();
        
                if (status != CRYPT_OK)
                {
                    qDebug() << "cryptlib shutdown error!";
                }
            }
        }
        
        /** Return the status string corresponding to the cryptlib
            status 'status' */
        QString getStatusString(int status)
        {
            switch (status)
            {
                case CRYPT_OK:
                    return QObject::tr("No error.");
                case CRYPT_ERROR_PARAM1:
                    return QObject::tr("There is a problem with the parameter 1.");
                case CRYPT_ERROR_PARAM2:
                    return QObject::tr("There is a problem with the parameter 2.");
                case CRYPT_ERROR_PARAM3:
                    return QObject::tr("There is a problem with the parameter 3.");
                case CRYPT_ERROR_PARAM4:
                    return QObject::tr("There is a problem with the parameter 4.");
                case CRYPT_ERROR_PARAM5:
                    return QObject::tr("There is a problem with the parameter 5.");
                case CRYPT_ERROR_PARAM6:
                    return QObject::tr("There is a problem with the parameter 6.");
                case CRYPT_ERROR_PARAM7:
                    return QObject::tr("There is a problem with the parameter 7.");
                case CRYPT_ERROR_FAILED:
                    return QObject::tr("The cryptlib operation failed.");
                case CRYPT_ERROR_INITED:
                    return QObject::tr("The object or attribute that you have tried to "
                                       "initialise has already been initialised "
                                       "previously.");
                case CRYPT_ERROR_MEMORY:
                    return QObject::tr("There is not enough memory available to perform "
                                       "this operation.");
                case CRYPT_ERROR_NOSECURE:
                    return QObject::tr("cryptlib cannot perform an operation at the "
                                       "requested security level.");
                case CRYPT_ERROR_NOTINITED:
                    return QObject::tr("The object or attribute that you have tried to "
                                       "use hasn’t been initialised yet, or a resource "
                                       "which is required isn’t available.");
                case CRYPT_ERROR_RANDOM:
                    return QObject::tr("Not enough random data is available for "
                                       "cryptlib to perform the requested operation.");
                case CRYPT_ERROR_COMPLETE:
                    return QObject::tr("An operation that consists of multiple steps "
                                       "(such as a message hash) is complete and cannot "
                                       "be continued.");
                case CRYPT_ERROR_INCOMPLETE:
                    return QObject::tr("An operation that consists of multiple steps "
                                       "(such as a message hash) is still in progress "
                                       "and requires further steps before it can be "
                                       "regarded as having completed.");
                case CRYPT_ERROR_INVALID:
                    return QObject::tr("The public/private key context or certificate "
                                       "object or attribute is invalid for this type "
                                       "of operation.");
                case CRYPT_ERROR_NOTAVAIL:
                    return QObject::tr("The requested operation is not available for "
                                       "this object.");
                case CRYPT_ERROR_PERMISSION:
                    return QObject::tr("You don’t have permission to perform this "
                                       "type of operation.");
                case CRYPT_ERROR_SIGNALLED:
                    return QObject::tr("An external event such as a signal from a "
                                       "hardware device caused a change in the state "
                                       "of the object.");
                case CRYPT_ERROR_TIMEOUT:
                    return QObject::tr("The operation timed out, either because of a "
                                       "general timeout while accessing an object such "
                                       "as a network connection or data file, or "
                                       "because the object was in use for another "
                                       "operation such as a key database lookup.");
                case CRYPT_ERROR_WRONGKEY:
                    return QObject::tr("The key being used to decrypt or verify the "
                                       "signature on a piece of data is incorrect.");
                case CRYPT_ERROR_BADDATA:
                    return QObject::tr("The data item (typically encrypted or signed "
                                       "data, or a key certificate) was corrupt, or "
                                       "not all of the data was present, and it can’t "
                                       "be processed.");
                case CRYPT_ERROR_OVERFLOW:
                    return QObject::tr("There is too much data for this function "
                                       "to work with.");
                case CRYPT_ERROR_SIGNATURE:
                    return QObject::tr("The signature or integrity check value didn’t "
                                       "match the data.");
                case CRYPT_ERROR_UNDERFLOW:
                    return QObject::tr("There is too little data in the envelope or "
                                       "session for cryptlib to process.");
                case CRYPT_ERROR_DUPLICATE:
                    return QObject::tr("The given item is already present in the "
                                       "container object.");
                case CRYPT_ERROR_NOTFOUND:
                    return QObject::tr("The requested item isn’t present in the "
                                       "container object.");
                case CRYPT_ERROR_OPEN:
                    return QObject::tr("The container object couldn’t be opened, "
                                       "either because it wasn’t found or because "
                                       "the open operation failed.");
                case CRYPT_ERROR_READ:
                    return QObject::tr("The requested item couldn’t be read from the "
                                       "container object.");
                case CRYPT_ERROR_WRITE:
                    return QObject::tr("The item couldn’t be written to the container "
                                       "object or the data object couldn’t be updated.");
                case CRYPT_ENVELOPE_RESOURCE:
                    return QObject::tr("A resource such as an encryption key or "
                                       "password needs to be added to the envelope "
                                       "before cryptlib can continue processing "
                                       "the data in it.");
                default:
                    return QObject::tr("An unknown error occurred.");
            }
        }
        
        /** Assert that the cryptlib status is ok */
        void assertValidStatus(int status, QUICK_CODELOC_ARGS)
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
        int processThroughEnvelope(CRYPT_ENVELOPE crypt_envelope,
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

        /** Process the data from 'in_stream' through the passed envelopes,
            (first through envelope0, then envelop1)
            writing the output to 'out_stream', and using the passed function
            'key_function0' and 'key_function1' to get and set the key if 
            one is required.
            
             key_function is a function that is passed the envelope that
             needs the key, together with an integer to indicate how many
             times this function has been called previously in this session.
             
            This function returns the cryptlib content type of the 
            de-enveloped data. This is useful if you are decrypting and
            there are multiple nested envelopes
        */
        int processThroughEnvelopes(CRYPT_ENVELOPE crypt_envelope0,
                                    CRYPT_ENVELOPE crypt_envelope1,
                                    QDataStream &input_stream,
                                    QDataStream &output_stream,
                        boost::function<void (CRYPT_ENVELOPE, int)> key_function0,
                        boost::function<void (CRYPT_ENVELOPE, int)> key_function1)
        {
            const int BUFFER_SIZE = 4096;

            char *in_buffer = 0;
            char *mid_buffer = 0;
            char *out_buffer = 0;
            
            int n_key_calls0 = 0;
            int n_key_calls1 = 0;
            
            int n_total_bytes_read = 0;
            int n_total_bytes_written = 0;
            
            try
            {
                in_buffer = new char[BUFFER_SIZE];
                mid_buffer = new char[BUFFER_SIZE];
                out_buffer = new char[BUFFER_SIZE];
                
                while (not input_stream.atEnd())
                {
                    int bytes_read0 = input_stream.readRawData(in_buffer, BUFFER_SIZE);
                    
                    if (bytes_read0 == -1)
                        throw Siren::io_error( QObject::tr(
                                "Error occurred while reading in data!"), CODELOC );

                    int offset0 = 0;
                                
                    while (bytes_read0 > 0)
                    {
                        int bytes_copied_in0 = 0;
                    
                        int status = cryptPushData( crypt_envelope0, 
                                                    in_buffer+offset0, bytes_read0,
                                                    &bytes_copied_in0 );
                                       
                        if (status == CRYPT_ENVELOPE_RESOURCE)
                        {
                            //the envelope requires more information to continue
                            ++n_key_calls0;
                            key_function0(crypt_envelope0, n_key_calls0);
                            continue;
                        }
                        
                        assertValidStatus(status, QUICK_CODELOC);
                        
                        offset0 += bytes_copied_in0;
                        bytes_read0 -= bytes_copied_in0;
                        
                        n_total_bytes_read += bytes_copied_in0;
                        
                        if (bytes_read0 > 0)
                        {
                            int bytes_copied_out0 = 0;
                        
                            //the envelope was full - we couldn't push more data into it!
                            status = cryptPopData( crypt_envelope0, 
                                                   mid_buffer, BUFFER_SIZE, 
                                                   &bytes_copied_out0 );
                                                   
                            assertValidStatus(status, QUICK_CODELOC);
                                
                            n_total_bytes_written += bytes_copied_out0;

                            //immediately write these bytes into the second envelope
                            int bytes_read1 = bytes_copied_out0;
                            int offset1 = 0;
                            
                            while (bytes_read1 > 0)
                            {
                                int bytes_copied_in1 = 0;
                            
                                status = cryptPushData( crypt_envelope1,
                                                        mid_buffer+offset1,
                                                        bytes_read1,
                                                        &bytes_copied_in1 );

                                if (status == CRYPT_ENVELOPE_RESOURCE)
                                {
                                    //the second envelope requires 
                                    //more information to continue
                                    ++n_key_calls1;
                                    key_function1(crypt_envelope1, n_key_calls1);
                                    continue;
                                }
                        
                                assertValidStatus(status, QUICK_CODELOC);
                        
                                offset1 += bytes_copied_in1;
                                bytes_read1 -= bytes_copied_in1;

                                if (bytes_read1 > 0)
                                {
                                    int bytes_copied_out1 = 0;
                        
                                    //the second envelope was full 
                                    //- we couldn't push more data into it!
                                    status = cryptPopData( crypt_envelope1, 
                                                           out_buffer, BUFFER_SIZE, 
                                                           &bytes_copied_out1 );
                                                   
                                    assertValidStatus(status, QUICK_CODELOC);
                                    
                                    int bytes_written = output_stream.writeRawData(
                                                         out_buffer, bytes_copied_out1 );

                                    if (bytes_written != bytes_copied_out1)
                                        throw Siren::io_error( QObject::tr(
                                                "Error occurred while writing out data!"),
                                                    CODELOC );
                                }
                            }
                        }
                    }
                    
                    //flush out the remaining data from envelop0
                    int bytes_copied_out0 = 0;

                    do
                    {
                        int status = cryptFlushData( crypt_envelope0 );
                        
                        assertValidStatus(status, QUICK_CODELOC);
                        
                        status = cryptPopData( crypt_envelope0, mid_buffer, BUFFER_SIZE,
                                               &bytes_copied_out0 );
                                                   
                        assertValidStatus(status, QUICK_CODELOC);
                                      
                        n_total_bytes_written += bytes_copied_out0;

                        //write these bytes into the second envelope
                        int bytes_read1 = bytes_copied_out0;
                        int offset1 = 0;
                            
                        while (bytes_read1 > 0)
                        {
                            int bytes_copied_in1 = 0;
                            
                            status = cryptPushData( crypt_envelope1,
                                                    mid_buffer+offset1,
                                                    bytes_read1,
                                                    &bytes_copied_in1 );

                            if (status == CRYPT_ENVELOPE_RESOURCE)
                            {
                                //the second envelope requires 
                                //more information to continue
                                ++n_key_calls1;
                                key_function1(crypt_envelope1, n_key_calls1);
                                continue;
                            }
                        
                            assertValidStatus(status, QUICK_CODELOC);
                        
                            offset1 += bytes_copied_in1;
                            bytes_read1 -= bytes_copied_in1;

                            if (bytes_read1 > 0)
                            {
                                int bytes_copied_out1 = 0;
                        
                                //the second envelope was full 
                                //- we couldn't push more data into it!
                                status = cryptPopData( crypt_envelope1, 
                                                       out_buffer, BUFFER_SIZE, 
                                                       &bytes_copied_out1 );
                                                   
                                assertValidStatus(status, QUICK_CODELOC);
                                    
                                int bytes_written = output_stream.writeRawData(
                                                     out_buffer, bytes_copied_out1 );

                                if (bytes_written != bytes_copied_out1)
                                    throw Siren::io_error( QObject::tr(
                                            "Error occurred while writing out data!"),
                                                CODELOC );
                            }
                        }
                    } while (bytes_copied_out0 > 0);
                                        
                    //get the type decrypted data
                    int data_type;
                    
                    cryptGetAttribute( crypt_envelope1, CRYPT_ENVINFO_CONTENTTYPE, 
                                       &data_type );

                    //finally(!) flush out any remaining data in the output buffer
                    int bytes_copied_out1 = 0;

                    do
                    {
                        int status = cryptFlushData( crypt_envelope1 );
                        
                        assertValidStatus(status, QUICK_CODELOC);
                        
                        status = cryptPopData( crypt_envelope1, out_buffer, BUFFER_SIZE,
                                               &bytes_copied_out1 );
                                                   
                        assertValidStatus(status, QUICK_CODELOC);

                        if (bytes_copied_out1 > 0)
                        {
                            int bytes_written = output_stream.writeRawData(
                                                    out_buffer, bytes_copied_out1 );

                            if (bytes_written != bytes_copied_out1)
                                throw Siren::io_error( QObject::tr(
                                        "Error occurred while writing out data!"),
                                            CODELOC );
                        }
                    
                    } while (bytes_copied_out1 > 0);
                    
                    delete[] in_buffer;
                    delete[] out_buffer;
                    
                    return data_type;
                }
            }
            catch(...)
            {
                delete[] in_buffer;
                delete[] mid_buffer;
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

        int processThroughEnvelope(CRYPT_ENVELOPE envelope,
                                   QDataStream &in_stream,
                                   QDataStream &out_stream)
        {
            return processThroughEnvelope(envelope, in_stream, out_stream,
                                          &no_resource_available );
        }

        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream)
        {
            return processThroughEnvelopes(envelope0, envelope1,
                                           in_stream, out_stream,
                                           &no_resource_available,
                                           &no_resource_available);
        }

        int processThroughEnvelopes(CRYPT_ENVELOPE envelope0,
                                    CRYPT_ENVELOPE envelope1,
                                    QDataStream &in_stream,
                                    QDataStream &out_stream,
                                 boost::function<void (CRYPT_ENVELOPE,int)> key_function)
        {
            return processThroughEnvelopes(envelope0, envelope1,
                                           in_stream, out_stream,
                                           key_function,
                                           key_function);
        }

        CRYPT_ENVELOPE createEnvelope(CRYPT_FORMAT_TYPE format)
        {
            SireSec_init();
            
            CRYPT_ENVELOPE crypt_envelope;
            
            int status = cryptCreateEnvelope( &crypt_envelope, CRYPT_UNUSED, format );
            
            assertValidStatus(status, QUICK_CODELOC);
            
            return crypt_envelope;
        }

        CRYPT_ENVELOPE createDefaultEnvelope()
        {
            return createEnvelope( CRYPT_FORMAT_CRYPTLIB );
        }

        CRYPT_ENVELOPE createAutoFormatEnvelope()
        {
            return createEnvelope( CRYPT_FORMAT_AUTO );
        }

        QString getAlgorithm(int crypt_algo)
        {
            switch (crypt_algo)
            {
                case CRYPT_ALGO_AES:
                    return QObject::tr("AES (cipher)");
                case CRYPT_ALGO_BLOWFISH:
                    return QObject::tr("Blowfish (cipher)");
                case CRYPT_ALGO_CAST:
                    return QObject::tr("CAST-128 (cipher)");
                case CRYPT_ALGO_DES:
                    return QObject::tr("DES (cipher, insecure!)");
                case CRYPT_ALGO_3DES:
                    return QObject::tr("Triple DES (cipher)");
                case CRYPT_ALGO_IDEA:
                    return QObject::tr("IDEA (cipher - patented, so unavailable)");
                case CRYPT_ALGO_RC2:
                    return QObject::tr("Algorithm compatible with RC2 "
                                       "(cipher, insecure!)");
                case CRYPT_ALGO_RC4:
                    return QObject::tr("Algorithm compatible with RC4 "
                                       "(cipher, insecure!)");
                case CRYPT_ALGO_RC5:
                    return QObject::tr("RC5 (cipher - patented, so unavailable)");
                case CRYPT_ALGO_SKIPJACK:
                    return QObject::tr("Skipjack (cipher, obsolete!)");
                case CRYPT_ALGO_DH:
                    return QObject::tr("Diffie-Hellman (key-exchange algorithm)");
                case CRYPT_ALGO_DSA:
                    return QObject::tr("DSA (digital signature, royalty free patent)");
                case CRYPT_ALGO_ELGAMAL:
                    return QObject::tr("Elgamal (public key / digital signature)");
                case CRYPT_ALGO_RSA:
                    return QObject::tr("RSA (public key / digital signature)");
                case CRYPT_ALGO_MD2:
                    return QObject::tr("MD2 (message authentication, obsolete!)");
                case CRYPT_ALGO_MD4:
                    return QObject::tr("MD4 (message authentication, insecure!)");
                case CRYPT_ALGO_MD5:
                    return QObject::tr("MD5 (message authentication, insecure!)");
                case CRYPT_ALGO_RIPEMD160:
                    return QObject::tr("RIPE-MD 160 (message authentication)");
                case CRYPT_ALGO_SHA:
                    return QObject::tr("SHA/SHA-1 (message authentication)");
                case CRYPT_ALGO_SHA2:
                    return QObject::tr("SHA2/SHA-256 (message authentication)");
                case CRYPT_ALGO_HMAC_MD5:
                    return QObject::tr("HMAC-MD5 (message authentication)");
                case CRYPT_ALGO_HMAC_RIPEMD160:
                    return QObject::tr("HMAC-RIPEMD-160 (message authentication)");
                case CRYPT_ALGO_HMAC_SHA:
                    return QObject::tr("HMAC-SHA (message authentication)");
                default:
                    return QObject::tr("unknown");
            }
        }

        QString getMode(int crypt_mode)
        {
            switch (crypt_mode)
            {
                case CRYPT_MODE_ECB:
                    return QObject::tr("ECB");
                case CRYPT_MODE_CBC:
                    return QObject::tr("CBC");
                case CRYPT_MODE_CFB:
                    return QObject::tr("CFB");
                case CRYPT_MODE_OFB:
                    return QObject::tr("OFB");
                default:
                    return QObject::tr("unknown");
            }
        }

        QString getEnvelopeDetails(CRYPT_ENVELOPE crypt_envelope)
        {
            int crypt_algo;
            int crypt_mode;
            int key_size;
            
            cryptGetAttribute( crypt_envelope, CRYPT_CTXINFO_ALGO, &crypt_algo );
            cryptGetAttribute( crypt_envelope, CRYPT_CTXINFO_MODE, &crypt_mode );
            cryptGetAttribute( crypt_envelope, CRYPT_CTXINFO_KEYSIZE, &key_size );
            
            return QObject::tr("Envelope( algorithm : %1, mode : %2, key_size : %3 )")
                        .arg( getAlgorithm(crypt_algo) )
                        .arg( getMode(crypt_mode) )
                        .arg(key_size);
        }

    } // end of namespace Crypt
    
} // end of namespace SireSec
