#ifndef SIREIO_IOBASE_H
#define SIREIO_IOBASE_H

class QIODevice;
class QByteArray;
#include <QString>

#include "SireMol/editmolset.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireIO
{

using SireMol::EditMolList;
using SireMol::EditMol;

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
    virtual EditMolList read(QString filename) const;
    /** Read in some molecules from the QIODevice 'dev' */
    virtual EditMolList read(QIODevice &dev) const;
    
    /** Write some molecules to the file 'filename'. Note that
        this will overwrite the contents of the file if it already exists. */
    virtual void write(const EditMolList &mols, QString filename) const;
    
    /** Write some molecules to the QIODevice 'dev'. This is the 
        method to use if you want to append some molecules to a file, or
        if you want to write some molecules over a network on into another
        type of device other than a file */
    virtual void write(const EditMolList &mols, QIODevice &dev) const;
    
    virtual void write(const EditMol &mol, QString filename) const;
    virtual void write(const EditMol &mol, QIODevice &dev) const;
    
protected:

    /** 
      * You need to override the below two functions for your custom 
      * IO class.
      *
      */

    /** Read and return some EditMols from the data */
    virtual EditMolList readGroups(const QByteArray &data) const=0;

    /** Write the list of EditMols to a QByteArray and return it */
    virtual QByteArray writeGroups(const EditMolList &groups) const=0;

};

}

SIRE_END_HEADER

#endif
