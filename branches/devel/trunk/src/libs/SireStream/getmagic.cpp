
#include "datastream.h"

#include "ThirdParty/md5.h"

using namespace SireStream;

/** This function is used to obtain a reasonably unique 
    MagicID number for each class type (with typename 'name') */
MagicID SIRESTREAM_EXPORT SireStream::getMagic(const char *name)
{
    //use L. Peter Deutsch's free implementation of
    //the md5 algorithm
    md5_state_t state;
    //initialise the md5 engine
    md5_init(&state);
    //make it encode the name...
    md5_append(&state, (unsigned char*)name, std::strlen(name));
    //this variable will hold the returned digest
    md5_byte_t dgst[16];
    //actually calculate the digest
    md5_finish(&state,dgst);
    
    //only use the first 4 bytes of the digest to calculate the
    //MagicID
    MagicID magic = dgst[0] + 256*dgst[1] + 65536*dgst[2] + 16777216*dgst[3];
    
    return magic;
}

