
#include "iobase.h"

#include "SireMol/molecule.h"
#include "SireMol/editmol.h"

#include "SireError/errors.h"

#include <QFile>

using namespace SireIO;
using namespace SireMol;
using namespace SireStream;

IOBase::IOBase()
{}

IOBase::~IOBase()
{}

QList<Molecule> IOBase::read(QString filename, const CuttingFunction &cutfunc) const
{
    //open the file for reading
    QFile fle(filename);
    if (not fle.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        throw SireError::file_error(fle, CODELOC);
    }

    //set the name of the object to 'filename' - this helps with debugging
    fle.setObjectName(filename);

    return this->read(fle, cutfunc);
}

QList<Molecule> IOBase::read(QIODevice &dev, const CuttingFunction &cutfunc) const
{
    if (not dev.isReadable())
    {
        throw SireError::io_error(QObject::tr("Cannot read molecules from an unreadble "
                                              "device!"), CODELOC );
    }

    //see if there is any data that is immediately ready for reading
    //(e.g. if the device is a file)
    QByteArray data;
    if (dev.bytesAvailable() > 0)
    {
        data += dev.readAll();
    }

    //keep reading data until none has arrived for 2 seconds. A QFile has
    //no more data for reading, so this will immediately exit
    while (dev.waitForReadyRead(2000))
    {
        data += dev.readAll();
    }

    if (data.isEmpty())
    {
        return QList<Molecule>();
    }

    //now use a virtual function to obtain a list of EditMols from this data...
    return this->readMols(data, cutfunc);
}

void IOBase::write(const QList<Molecule> &molecules, QString filename) const
{
    //open a file into which to write the molecules
    QFile fle(filename);
    if (not fle.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        throw SireError::file_error(fle);
    }

    fle.setObjectName(filename);

    this->write(molecules,fle);
}

void IOBase::write(const QList<EditMol> &molecules, QString filename) const
{
    QFile fle(filename);
    if (not fle.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        throw SireError::file_error(fle);
    }

    fle.setObjectName(filename);

    this->write(molecules,fle);
}

void IOBase::write(const QList<Molecule> &molecules, QIODevice &dev) const
{
    if (not dev.isWritable())
    {
        throw SireError::io_error(QObject::tr("Cannot write molecules to a read-only "
                                              "device!"), CODELOC);
    }

    //use a virtual function to get a bytearray containing the molecules
    QByteArray data = this->writeMols(molecules);

    //now write this data to the device
    dev.write(data);
}

void IOBase::write(const QList<EditMol> &molecules, QIODevice &dev) const
{
    if (not dev.isWritable())
    {
        throw SireError::io_error(QObject::tr("Cannot write molecules to a read-only "
                                              "device!"), CODELOC);
    }

    QByteArray data = this->writeMols(molecules);

    dev.write(data);
}

void IOBase::write(const Molecule &mol, QString filename) const
{
    QList<Molecule> l;
    l.append(mol);
    this->write(l,filename);
}

void IOBase::write(const EditMol &mol, QString filename) const
{
    QList<EditMol> l;
    l.append(mol);
    this->write(l, filename);
}

void IOBase::write(const Molecule &mol, QIODevice &dev) const
{
    QList<Molecule> l;
    l.append(mol);
    this->write(l,dev);
}

void IOBase::write(const EditMol &mol, QIODevice &dev) const
{
    QList<EditMol> l;
    l.append(mol);
    this->write(l,dev);
}
