#ifndef SIREIO_MOL2_H
#define SIREIO_MOL2_H

#include "iobase.h"

SIRE_BEGIN_HEADER

namespace SireIO
{

/**
These functions are used to read and write Tripos MOL2 files, as described at 'website'.

@author Christopher Woods
*/
class SIREIO_EXPORT Mol2 : public IOBase
{
public:
    Mol2();
    virtual ~Mol2();

protected:
    /** Read and return some EditMols from the data */
    virtual EditMolList readGroups(const QByteArray &data) const;
    
    /** Write the list of molecules to a bytearray and return it */
    virtual QByteArray writeGroups(const EditMolList &groups) const;
};

}

SIRE_END_HEADER

#endif
