#ifndef SIREIO_IOBASE_H
#define SIREIO_IOBASE_H

#include <QString>
#include <QIODevice>
#include <QList>

#include "SireMol/residuecutting.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireIO
{

using SireMol::Molecule;
using SireMol::CuttingFunction;
using SireMol::ResidueCutting;

/**
This is the virtual base class of all molecule readers and writers.

@author Christopher Woods
*/
class SIREIO_EXPORT IOBase
{
public:
    IOBase();
    virtual ~IOBase();

    /**
      * The below functions are all high level interfaces that all eventually
      * call the three protected functions below
      *
      */

    /** Read in some molecules from the file 'filename' */
    virtual QList<Molecule> read(QString filename,
                                 const CuttingFunction &cutfunc = ResidueCutting()) const;

    /** Read in some molecules from the QIODevice 'dev' */
    virtual QList<Molecule> read(QIODevice &dev,
                                 const CuttingFunction &cutfunc = ResidueCutting()) const;

    /** Write some molecules to the file 'filename'. Note that
        this will overwrite the contents of the file if it already exists. */
    virtual void write(const QList<Molecule> &molecules, QString filename) const;

    /** Write some molecules to the QIODevice 'dev'. This is the
        method to use if you want to append some molecules to a file, or
        if you want to write some molecules over a network on into another
        type of device other than a file */
    virtual void write(const QList<Molecule> &molecules, QIODevice &dev) const;

    virtual void write(const Molecule &molecule, QString filename) const;
    virtual void write(const Molecule &molecule, QIODevice &dev) const;

protected:

    /**
      * You need to override the below two functions for your custom
      * IO class.
      *
      */

    /** Read and return some Molecules from the data */
    virtual QList<Molecule> readMols(const QByteArray &data,
                                     const CuttingFunction &cutfunc) const=0;

    /** Write the list of Molecules to a QByteArray and return it */
    virtual QByteArray writeMols(const QList<Molecule> &molecules) const=0;

};

}

SIRE_END_HEADER

#endif
