#ifndef SIREIO_MCT_H
#define SIREIO_MCT_H

#include "iobase.h"

SIRE_BEGIN_HEADER

namespace SireIO
{

/**
These functions are used to read and write Molecular Coordinate and Topology (MCT) files, a new format designed for this program. These files can contain multiple molecules and use a compact format to provide the coordinates of all of the atoms, and the bonds between atoms. The format requires uniqueness of identifiers so that it is possible to locate an individual atom within the file.
 
@author Christopher Woods
*/
class SIREIO_EXPORT MCT : public IOBase
{
public:
    MCT();
    virtual ~MCT();

protected:
    /** Read and return some EditMols from the data */
    virtual EditMolList readGroups(const QByteArray &data) const;
    
    /** Write the list of molecules to a bytearray and return it */
    virtual QByteArray writeGroups(const EditMolList &groups) const;
    
};

}

SIRE_END_HEADER

#endif
