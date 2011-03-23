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

#include <QFile>
#include <QTextStream>

#include "perturbationslibrary.h"
#include "tostring.h"


#include "SireMM/ljparameter.h"
//#include "SireUnits/convert.h"
#include "SireUnits/units.h"
#include "SireUnits/dimensions.h"

#include "SireError/errors.h"
#include "SireIO/errors.h"
#include "SireMol/errors.h"

using namespace SireIO;
using namespace SireMM;
//using namespace SireMol;
//using namespace SireMove;
using namespace SireStream;
using namespace SireUnits::Dimension;
using namespace SireUnits;
//
// Implementation of PerturbationsTemplate
//

static RegisterMetaType<PerturbationsTemplate> r_pertstemplate;

QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds,
                                      const PerturbationsTemplate &pertstemplate)
{
    writeHeader(ds, r_pertstemplate, 1);
    
    SharedDataStream sds(ds);
    
    sds << pertstemplate.name 
        << pertstemplate.initcharges << pertstemplate.finalcharges;
        
    return ds;
}

QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, PerturbationsTemplate &pertstemplate)
{
    VersionID v = readHeader(ds, r_pertstemplate);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> pertstemplate.name 
            >> pertstemplate.initcharges >> pertstemplate.finalcharges;
    }
    else
        throw version_error(v, "1", r_pertstemplate, CODELOC);
        
    return ds;
}

/** Constructor */
PerturbationsTemplate::PerturbationsTemplate()
{}

PerturbationsTemplate::PerturbationsTemplate(const QString &name)
                    : name(name)
{}

/** Copy constructor */
PerturbationsTemplate::PerturbationsTemplate(const PerturbationsTemplate &other)
                    : name(other.name), initcharges(other.initcharges),
                      finalcharges(other.finalcharges)
{}

/** Destructor */
PerturbationsTemplate::~PerturbationsTemplate()
{}

const char* PerturbationsTemplate::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PerturbationsTemplate>() );
}

/** Copy assignment operator */
PerturbationsTemplate& PerturbationsTemplate::operator=(const PerturbationsTemplate &other)
{
    if (this != &other)
    {
        name = other.name;
        initcharges = other.initcharges;
        finalcharges = other.finalcharges;
    }
    
    return *this;
}

/** Comparison operator */
bool PerturbationsTemplate::operator==(const PerturbationsTemplate &other) const
{
    return this == &other or
           (name == other.name and initcharges == other.initcharges and
            finalcharges == other.finalcharges);
}

/** Comparison operator */
bool PerturbationsTemplate::operator!=(const PerturbationsTemplate &other) const
{
    return not PerturbationsTemplate::operator==(other);
}

const QString PerturbationsTemplate::getName()
{
    return this->name;
}

void PerturbationsTemplate::setInitCharge(const QString &atomname, const Charge &atomcharge)
{
    this->initcharges[atomname] = atomcharge;
}

void PerturbationsTemplate::setFinalCharge(const QString &atomname, const Charge &atomcharge)
{
    this->finalcharges[atomname] = atomcharge;
}

Charge PerturbationsTemplate::getInitCharge(const QString &atomname) const
{
    return this->initcharges[atomname];
}

Charge PerturbationsTemplate::getFinalCharge(const QString &atomname) const
{
    return this->finalcharges[atomname];
}

void PerturbationsTemplate::setInitLJ(const QString &atomname, const LJParameter &atomlj)
{
    this->initLJs[atomname] = atomlj;
}

void PerturbationsTemplate::setFinalLJ(const QString &atomname, const LJParameter &atomlj)
{
    this->finalLJs[atomname] = atomlj;
}

LJParameter PerturbationsTemplate::getInitLJ(const QString &atomname) const
{
    return this->initLJs[atomname];
}

LJParameter PerturbationsTemplate::getFinalLJ(const QString &atomname) const
{
    return this->finalLJs[atomname];
}
//
// Helper functions to parse a templates input file
//

static int processVersionLine( QString& line)
{
    QStringList words = line.split(" ", QString::SkipEmptyParts);
    bool ok;
    int version = words[1].toInt(&ok);
    
    if (not ok)
        throw SireError::program_bug( QObject::tr(
                    "Unexpected error while trying to read the version line "
                    "of the perturbations template file"), CODELOC);

    return version;
}

//
// Implementation of PerturbationsLibrary
//
static const RegisterMetaType<PerturbationsLibrary> r_pertslibrary;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, 
                                      const PerturbationsLibrary &pertslibrary)
{
    writeHeader(ds, r_pertslibrary, 1);
    
    SharedDataStream sds(ds);

    sds << pertslibrary.templates;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, 
                                      PerturbationsLibrary &pertslibrary)
{
    VersionID v = readHeader(ds, r_pertslibrary);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> pertslibrary.templates;
    }
    else
        throw version_error( v, "1", r_pertslibrary, CODELOC );
        
    return ds;
}

/** Default constructor */
PerturbationsLibrary::PerturbationsLibrary()
{}

/** Construct, loading the library from the passed file */
PerturbationsLibrary::PerturbationsLibrary(const QString &file)
{
    this->loadTemplates(file);
}

/** Copy constructor */
PerturbationsLibrary::PerturbationsLibrary(const PerturbationsLibrary &other)
                 : templates(other.templates)
{}

//** Destructor */
PerturbationsLibrary::~PerturbationsLibrary()
{}

/** Copy assignment operator */
PerturbationsLibrary& PerturbationsLibrary::operator=(const PerturbationsLibrary &other)
{
    templates = other.templates;
    return *this;
}

/** Comparison operator */
bool PerturbationsLibrary::operator==(const PerturbationsLibrary &other) const
{
    return templates == other.templates;
}

/** Comparison operator */
bool PerturbationsLibrary::operator!=(const PerturbationsLibrary &other) const
{
    return not PerturbationsLibrary::operator==(other);
}

const char* PerturbationsLibrary::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PerturbationsLibrary>() );
}

/** Add the templates in 'other' into this library */
PerturbationsLibrary& PerturbationsLibrary::operator+=(const PerturbationsLibrary &other)
{
    if (templates.isEmpty())
    {
        templates = other.templates;
    }
    else
    {
        for (QHash<QString,PerturbationsTemplate>::const_iterator 
                                            it = other.templates.constBegin();
             it != other.templates.constEnd();
             ++it)
        {
            templates.insert( it.key(), it.value() );
        }
    }
    
    return *this;
}

/** Return the combination of this library with 'other' */
PerturbationsLibrary PerturbationsLibrary::operator+(const PerturbationsLibrary &other) const
{
    PerturbationsLibrary ret(*this);
    ret += other;
    return ret;
}

/** Add the templates in 'other' into this library */
void PerturbationsLibrary::add(const PerturbationsLibrary &other)
{
    this->operator+=(other);
}

/** Return the template for the specified 'key'

    \throw SireError::invalid_key
*/
const PerturbationsTemplate& PerturbationsLibrary::getTemplate(const QString &key)
{
    QHash<QString,PerturbationsTemplate>::const_iterator it = templates.constFind(key);
    
    if (it == templates.constEnd())
        throw SireError::invalid_key( QObject::tr(
                "Cannot find the template with key \"%1\". Available templates "
                "are %2.")
                    .arg(key, Sire::toString(templates.keys())), CODELOC );

    return it.value();
}

/** Set the template associated with the passed key */
void PerturbationsLibrary::setTemplate(const QString &key, 
                                     const PerturbationsTemplate &tmplate)
{
    templates.insert(key, tmplate);
}

void PerturbationsLibrary::loadTemplates(const QString &templatefile)
{
    QFile template_f(templatefile);
  
    if ( not (template_f.exists() and template_f.open(QIODevice::ReadOnly) ) )
    {
        throw SireError::file_error(template_f, CODELOC);
    }
  
    QTextStream ts(&template_f);
  
    QString line = ts.readLine();

    // The first line contains the version
    int version = ::processVersionLine(line);

    if (version != 1)
        throw SireError::process_error( QObject::tr(
                    "Invalid version of the template, got '%1' but only support '1'")
                        .arg(version), CODELOC);
  
    QString current = " "; // the template currently being read

    QHash <QString,PerturbationsTemplate> new_templates; 

    /** Define temporary place holders */
    QString atname = " ";
    Charge atchargeinit = 0 * mod_electron;
    Charge atchargefinal = 0 * mod_electron;
    Length atsigmainit = 0 * angstrom;
    MolarEnergy atepsiloninit = 0 * kcal_per_mol;
    Length atsigmafinal = 0 * angstrom;
    MolarEnergy atepsilonfinal = 0 * kcal_per_mol;

    bool inatom = false;
    /** Now read rest of the file */
    while ( not line.isNull() )
    {
        line = ts.readLine();
	line = line.simplified();
        QStringList words = line.split(" ", QString::SkipEmptyParts);
	//qDebug() << line;
	qDebug() << words;
	if ( line.startsWith("molecule") )
	  {
	    // create a new perturbations template
	    PerturbationsTemplate pertstemplate = PerturbationsTemplate( words[1] );
	    current = pertstemplate.getName();
	    new_templates[current] = pertstemplate;
	  }
	if ( line.startsWith("atom") )
	  {
	    atname = "";
	    atchargeinit = 0 * mod_electron;
	    atchargefinal = 0 * mod_electron;
	    atsigmainit = 0 * angstrom;
	    atsigmafinal = 0 * angstrom;
	    atepsiloninit = 0 * kcal_per_mol;
	    atepsilonfinal = 0 * kcal_per_mol;
	    inatom = true;
	  }
	if (line.startsWith("name") and inatom)
	  {
	    atname = words[1];
	  }
	if (line.startsWith("initial_charge") and inatom)
	  {
	    atchargeinit = words[1].toDouble() * mod_electron;
	  }
	if (line.startsWith("final_charge") and inatom)
	  {
	    atchargefinal = words[1].toDouble() * mod_electron;
	  }
	if (line.startsWith("initial_LJ") and inatom)
	  {
	    atsigmainit = words[1].toDouble() * angstrom;
	    atepsiloninit = words[2].toDouble() * kcal_per_mol;
	  }
	if (line.startsWith("final_LJ") and inatom)
	  {
	    atsigmafinal = words[1].toDouble() * angstrom;
	    atepsilonfinal = words[2].toDouble() * kcal_per_mol;
	  }	

	if (line.startsWith("endatom") )
	  {
	    inatom = false;
	    qDebug() << atname << atchargeinit.toString() << atchargefinal.toString() << atsigmainit.toString();
	    new_templates[current].setInitCharge(atname, atchargeinit);
	    new_templates[current].setFinalCharge(atname, atchargefinal);
	    new_templates[current].setInitLJ(atname, LJParameter(atsigmainit, atepsiloninit) );
	    new_templates[current].setFinalLJ(atname, LJParameter(atsigmafinal, atepsilonfinal) );
	  }
    }
    //
    //    if ( line.startsWith("molecule") )
    //    {
    //        // create a new flexibilitytemplate
    //        FlexibilityTemplate flextemplate = FlexibilityTemplate( words[1] );
    //        current = flextemplate.getName();
    //        new_templates[current] = flextemplate;
    //    }
    //    else if ( line.startsWith("rigidbody") )
    //    {
    //        new_templates[current].setRotation( words[2].toDouble() * degrees );
    //        new_templates[current].setTranslation( words[4].toDouble() * angstroms );
    //    }
    //    else if ( line.startsWith("maximumvariables") )
    //    {
    //        new_templates[current].setMaximumVar( words[1].toInt() );
    //    }
    //    else if ( line.startsWith("bond") )
    //    {
    //        new_templates[current].setBondDelta( BondID(AtomName(words[1]),
    //                                                    AtomName(words[2])),
    //                                             words[4].toDouble() * angstroms );
    //    }
    //    else if ( line.startsWith("angle") )
    //    {
    //        new_templates[current].setAngleDelta( AngleID(AtomName(words[1]),
    //                                                      AtomName(words[2]),
    //                                                      AtomName(words[3])),
    //                                              words[5].toDouble() * degrees );
    //    }
    //    else if ( line.startsWith("dihedral") )
    //    {
    //        new_templates[current].setDihedralDelta( DihedralID(AtomName(words[1]),
    //                                                            AtomName(words[2]),
    //                                                            AtomName(words[3]),
    //                                                            AtomName(words[4])),
    //                                                 words[6].toDouble() * degrees );
    //    }
    //}

    foreach (PerturbationsTemplate templ, new_templates)
    {
        QString templname = templ.getName();

        if ( not templates.contains(templname) )
            templates.insert(templname, templ);
    }
}

/** Generate the Flexibility property for the atoms in the passed molecule view */
//Flexibility FlexibilityLibrary::getFlexibility(const MoleculeView &molecule) const
//{
//    AtomSelection selected_atoms = molecule.selection();
//
//    Flexibility flexibility = Flexibility(molecule);
//  
//    MolName moleculename = molecule.data().name();
//
//    if ( not this->templates.contains(moleculename) )
//        throw SireError::invalid_key(QObject::tr("There is no flexibility template for the "
//               "molecule with name \"%1\" - available templates are %2.")
//                    .arg(moleculename, Sire::toString(templates.keys())), CODELOC);
//
//    FlexibilityTemplate templ = this->templates[moleculename];
//
//    flexibility.setRotation(templ.getRotation());
//    flexibility.setTranslation(templ.getTranslation());
//    flexibility.setMaximumVar(templ.getMaximumVar());
//
//    for (QHash<BondID,Length>::const_iterator it = templ.getBondDeltas().constBegin();
//         it != templ.getBondDeltas().constEnd();
//         ++it)
//    {
//        const BondID &bond = it.key();
//    
//        if ( selected_atoms.selectedAll() or
//             (selected_atoms.selected(bond.atom0()) and
//              selected_atoms.selected(bond.atom1())) )
//        {
//            flexibility.add(bond, it.value());
//        }
//    }
//
//    for (QHash<AngleID,Angle>::const_iterator it = templ.getAngleDeltas().constBegin();
//         it != templ.getAngleDeltas().constEnd();
//         ++it)
//    {
//        const AngleID &angle = it.key();
//        
//        if ( selected_atoms.selectedAll() or
//             (selected_atoms.selected(angle.atom0()) and
//              selected_atoms.selected(angle.atom1()) and
//              selected_atoms.selected(angle.atom2())) )
//        {
//            flexibility.add(angle, it.value());
//        }
//    }
//
//    for (QHash<DihedralID,Angle>::const_iterator 
//                                    it = templ.getDihedralDeltas().constBegin();
//         it != templ.getDihedralDeltas().constEnd();
//         ++it)
//    {
//        const DihedralID &dihedral = it.key();
//        
//        if ( selected_atoms.selectedAll() or
//             (selected_atoms.selected(dihedral.atom0()) and
//              selected_atoms.selected(dihedral.atom1()) and
//              selected_atoms.selected(dihedral.atom2()) and
//              selected_atoms.selected(dihedral.atom3())) )
//        {
//            flexibility.add(dihedral, it.value());
//        }
//    }
//
//    return flexibility;
//}
