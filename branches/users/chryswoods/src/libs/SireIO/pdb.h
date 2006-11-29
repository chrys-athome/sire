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
    ~PDB();

protected:
    /** Read and return some molecules from the data */
    QList<Molecule> readMols(const QByteArray &data,
                             const CuttingFunction &cutfunc) const;

    /** Write the list of molecules to a bytearray and return it */
    QByteArray writeMols(const QList<Molecule> &molecules) const;

    QByteArray writeMols(const QList<EditMol> &molecules) const;

};

}

SIRE_END_HEADER

#endif
