#ifndef SIREIO_PDB_H
#define SIREIO_PDB_H

#include "iobase.h"

SIRE_BEGIN_HEADER

namespace SireIO
{

/**
This is a IOBase object that has been specialised to read and write PDB format files.
 
@author Christopher Woods
*/
class SIREIO_EXPORT PDB : public IOBase
{
public:
    PDB();
    virtual ~PDB();

protected:
    /** Read and return some EditMols from the data */
    virtual EditMolList readGroups(const QByteArray &data) const;
    
    /** Write the list of molecules to a bytearray and return it */
    virtual QByteArray writeGroups(const EditMolList &groups) const;

};

}

SIRE_END_HEADER

#endif
