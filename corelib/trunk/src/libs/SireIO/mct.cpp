#include <QTextStream>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "mct.h"

#include "SireMol/elementdb.h"
#include "SireMol/editmol.h"
#include "SireMol/editres.h"

#include "SireUnits/units.h"

using namespace SireIO;
using namespace SireMol;

using SireUnits::angstrom;

MCT::MCT() : IOBase()
{}

MCT::~MCT()
{}

EditMolList MCT::readGroups(const QByteArray &data) const
{
    //read through the file and check the elements
    QTextStream ts(data,QIODevice::ReadOnly | QIODevice::Text);

    //create an EditMolList to hold the molecules that are read in...
    EditMolList mollist;
    
    //now create an EditMol to hold the first read molecule
    EditMol mol;
    
    int resnum = 0;
    QString resnam = QString::null;
    
    for (QString line = ts.readLine(); not ts.atEnd(); line = ts.readLine())
    {
        QString lowline = line.toLower();
    
        if (lowline.startsWith("atom"))
        {
            QStringList words = line.split(' ',QString::SkipEmptyParts);
            
            //read in the coordinates in angstroms
            double x = words[3].toDouble() * angstrom;
            double y = words[4].toDouble() * angstrom;
            double z = words[5].toDouble() * angstrom;
            
            mol.residue(resnum).add(
                              Atom(words[1],Element(words[2]),Vector(x, y, z)) );
        
        }
        else if (lowline.startsWith("resi"))
        {
            QStringList words = line.split(' ',QString::SkipEmptyParts);
            
            resnum = words[1].toInt();
            
            if (words.count() > 2)
                resnam = words[2];
            else
                resnam = QString::null;
                
            mol.addResidue(resnum,resnam);
        }
        else if (line.toLower().startsWith("molecule"))
        {
            QStringList words = line.split(' ',QString::SkipEmptyParts);
            
            //if the previous molecule was not empty then save it
            if (not mol.isEmpty())
            {
                mollist.append(mol);
            }

            //now get the name of this Molecule (need to remove the 'molecule' word!)
            words.removeAt(0);
            
            mol = EditMol();
        }
    }
    
    //the last molecule loaded has not been added to the list - add it if it
    //contains any atoms
    if (not mol.isEmpty())
      mollist.append(mol);
        
    return mollist;
}

QByteArray MCT::writeGroups(const EditMolList &groups) const
{
    return QByteArray();
}
