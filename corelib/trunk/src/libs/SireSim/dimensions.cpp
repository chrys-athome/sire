/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "dimensions.h"
#include "dimensions.hpp"

#include <cmath>

/** Here are the physical constants, in internal units of this program

    We use the AKMA units (same as charmm)
        Angstroms, Kilocalories per Mole, Atomic mass units

    energy = kcal mol-1 (thermal)     (really MolarEnergy)
    length = angstrom
    mass = g mol-1                    (really MolarMass)
    time = AKMA time == 48.88821 fs == 0.04888821 ps
    charge = unit electrons

    Where necessary, physical constants were downloaded from
    the NIST website (web pages referenced where appropriate).
    
    Physical constants were last checked on 28/10/2008
*/

/** Avogadro's number */
//http://physics.nist.gov/cgi-bin/cuu/Value?na|search_for=physchem_in!
const double mole = 6.02214179e23;
const double dozen = 12;
const double pi = 3.14159265358979323846;

/////////////////////////////////////////////////
// Units of angle. Internal unit = radians     //
/////////////////////////////////////////////////

const double radians = 1;
const double degrees = radians * pi / 180.0;
const double angle_minute = degrees / 60;
const double angle_second = angle_minute / 60;
const double octant = 45 * degrees;
const double sextant = 60 * degrees;
const double quadrant = 90 * degrees;
const double gradian = quadrant / 100;
const double revolution = 360 * degrees;

/////////////////////////////////////////////////
// Units of length. Internal unit = Angstroms  //
/////////////////////////////////////////////////

const double angstrom = 1;
const double picometer = 0.01*angstrom;
const double nanometer = 1000 * picometer;
const double micrometer = 1000 * nanometer;
const double millimeter = 1000 * micrometer;
const double centimeter = 10 * millimeter;
const double meter = 100 * centimeter;
const double kilometer = 1000 * meter;
const double megameter = 1000 * kilometer;

//http://physics.nist.gov/cgi-bin/cuu/Value?bohrrada0|search_for=bohr
const double bohr_radii = 0.52917720859 * angstrom;

const double inch = 2.54 * centimeter;
const double foot = 12 * inch;
const double yard = 3 * foot;
const double mile = 1760 * yard;
const double light_year = 9460730472580.8 * kilometer;
const double parsec = 3.261564 * light_year;

/////////////////////////////////////////////////
// Units of area. Internal unit = Angstroms^2  //
/////////////////////////////////////////////////

double angstrom2 = angstrom*angstrom;
double picometer2 = picometer*picometer;
double nanometer2 = nanometer*nanometer;
double micrometer2 = micrometer*micrometer;
double millimeter2 = millimeter*millimeter;
double centimeter2 = centimeter*centimeter;
double meter2 = meter*meter;
double kilometer2 = kilometer*kilometer;
double megameter2 = megameter*megameter;
double bohr_radii2 = bohr_radii*bohr_radii;
double inch2 = inch*inch;
double foot2 = foot*foot;
double yard2 = yard*yard;
double mile2 = mile*mile;
double light_year2 = light_year*light_year;
double parsec2 = parsec*parsec;

/////////////////////////////////////////////////
// Units of volume. Internal unit = Angstroms^3  //
/////////////////////////////////////////////////

const double angstrom3 = angstrom * angstrom * angstrom;
const double picometer3 = picometer * picometer * picometer;
const double nanometer3 = nanometer * nanometer * nanometer;

const double meter3 = meter * meter * meter;

///////////////////////////////////////////////////////
// Units of mass. Internal unit = g mol-1            //
///////////////////////////////////////////////////////

const double g_per_mol = 1;

const double gram = mole * g_per_mol;
const double kilogram = 1000 * gram;
const double tonne = 1000 * kilogram;

const double milligram = 0.001 * gram;
const double microgram = 0.001 * milligram;
const double nanogram = 0.001 * microgram;
const double picogram = 0.001 * nanogram;
const double femtogram = 0.001 * picogram;

const double kg_per_mol = 1000 * g_per_mol;
const double tonne_per_mol = 1000 * kg_per_mol;
const double mg_per_mol = 0.001 * g_per_mol;
const double ug_per_mol = 0.001 * mg_per_mol;
const double ng_per_mol = 0.001 * ug_per_mol;
const double pg_per_mol = 0.001 * ng_per_mol;
const double fg_per_mol = 0.001 * pg_per_mol;

///////////////////////////////////////////////////////
// Units of Charge. Internal unit = |e|              //
///////////////////////////////////////////////////////

const double mod_electron =1;
const double faraday = 1;

//http://physics.nist.gov/cgi-bin/cuu/Value?e|search_for=elecmag_in!
const double coulomb = mod_electron / 1.602176487e-19;
const double coulomb_per_mol = coulomb / mole;

const double e_charge = -mod_electron;

/////////////////////////////////////////////////
// Units of Energy. Internal unit = kcal mol-1 //
/////////////////////////////////////////////////

const double kcal_per_mol = 1;
const double kcal = mole * kcal_per_mol;

const double cal_per_mol = 0.001 * kcal_per_mol;
const double cal = 0.001 * kcal;

const double kJ_per_mol = kcal_per_mol / 4.184;
const double kilojoule = mole * kJ_per_mol;

const double MJ_per_mol = 1000 * kJ_per_mol;
const double megajoule = 1000 * kilojoule;

const double J_per_mol = 0.001 * kJ_per_mol;
const double joule = 0.001 * kilojoule;

/** Conversion factor from international kcal mol-1 to internal units  */
const double int_kcal_per_mol = 4.1868 * kJ_per_mol;
const double int_cal_per_mol = 0.001 * int_kcal_per_mol;

const double int_kcal = mole * int_kcal_per_mol;
const double int_cal = 0.001 * int_kcal;

//http://physics.nist.gov/cgi-bin/cuu/Value?hr|search_for=hartree
const double hartree = 4.35974394e-18 * joule;

////////////////////////////////////////////////////////////
// Units of time. Internal unit = akma_time == 48.8882 fs //
////////////////////////////////////////////////////////////

const double akma_time = 1;

const double second = std::sqrt( (kg_per_mol * meter * meter) / J_per_mol );
const double millisecond = 0.001 * second;
const double microsecond = 0.001 * millisecond;
const double nanosecond = 0.001 * microsecond;
const double picosecond = 0.001 * nanosecond;
const double femtosecond = 0.001 * picosecond;

const double minute = 60 * second;
const double hour = 60 * minute;
const double day = 24 * hour;
const double week = 7 * day;
const double fortnight = 2 * week;

/////////////////////////////////////////////////////////////
// Units of velocity. Internal unit = Angstrom / AKMA time //
/////////////////////////////////////////////////////////////

const double akma_velocity = 1;

const double angstroms_per_fs = angstrom / femtosecond;
const double meters_per_second = meter / second;
const double kilometers_per_hour = kilometer / hour;
const double miles_per_hour = mile / hour;
const double mph = miles_per_hour;
const double kph = kilometers_per_hour;

//////////////////////////////////////////////////////////
// Units of force. Internal units = kcal mol-1 A-1      //
//////////////////////////////////////////////////////////

/** Convert a force in Newtons to internal units */
const double newton = joule / meter;

/** Weights */
const double ounce = 0.27801385095 * newton;
const double pound = 16 * ounce;
const double stone = 14 * pound;
const double hundredweight = 8 * stone;

//////////////////////////////////////////////////////////
// Units of pressure. Internal units = kcal mol-1 A-2   //
//////////////////////////////////////////////////////////

const double pascal = newton / (meter*meter);

const double bar = 100000 * pascal;
const double atm = 101325 * pascal;

const double psi = pound / (inch*inch);
const double mmHg = 133.322 * pascal;

//////////////////////////////////////////////////////////
// Now some miscellaneous units                         //
//////////////////////////////////////////////////////////

/** Convert the units of current (amps) */
const double amp = coulomb / second;

/** Volts */
const double volt = joule / coulomb;

/** Convert the units of capacitance (farads) */
const double farad = coulomb / volt;

/** Convert power in Watts */
const double watt = joule / second;
const double watt_per_mol = J_per_mol / second;

namespace SireSim
{
    namespace detail
    {
        QPair<QString,qint64> SIRESIM_EXPORT getPostFactor(QString text)
        {
            text = text.trimmed();

            //process the text to see if any multiples are added
            //to the end of the string (e.g. million, thousand etc.)
            QStringList words = text.split(" ", QString::SkipEmptyParts);
            
            qint64 postfac = 1;
            
            QMutableListIterator<QString> it(words);
            
            bool start_words = false;
            
            // NEED TO ADD CHECKS FOR OVERFLOW!!!
            while (it.hasNext())
            {
                QString text = it.next().toLower();
                
                if (text == "hundred")
                {
                    it.remove();
                    postfac *= 100;
                    start_words = true;
                }
                else if (text == "thousand")
                {
                    it.remove();
                    postfac *= 1000;
                    start_words = true;
                }
                else if (text == "million")
                {
                    it.remove();
                    postfac *= 1000000;
                    start_words = true;
                }
                else if (text == "billion")
                {
                    it.remove();
                    postfac *= 1000000000;
                    start_words = true;
                }
                else if (start_words)
                {
                    throw SireError::invalid_arg( QObject::tr(
                            "Cannot read a number from the text \"%1\".")
                                .arg(text), CODELOC );
                }
            }
            
            //the rest of the string should now just be the number
            text = words.join("");
            
            return QPair<QString,qint64>(text,postfac);
        }
        
        qint64 SIRESIM_EXPORT readInt(QString text)
        {
            text = text.trimmed();

            QPair<QString,qint64> value = getPostFactor(text);
            
            bool ok;
            qint64 val = value.first.toLongLong(&ok);
            
            if (not ok)
                throw SireError::invalid_arg( QObject::tr(
                        "Cannot read an integer from the text \"%1\".")
                            .arg(text), CODELOC );
                            
            return val * value.second;
        }
        
        double SIRESIM_EXPORT readFloat(QString text)
        {
            text = text.trimmed();
        
            QPair<QString,qint64> value = getPostFactor(text);
            
            bool ok;
            double val = value.first.toDouble(&ok);
            
            if (not ok)
                throw SireError::invalid_arg( QObject::tr(
                        "Cannot read a floating point number from the text \"%1\".")
                            .arg(text), CODELOC );

            return val * value.second;
        }
        
        QPair<double,QString> SIRESIM_EXPORT readDimension(QString value,
                                                           QStringList units)
        {
            value = value.trimmed();
            
            if (value.isEmpty())
                return QPair<double,QString>(0,QString::null);
        
            int index = -1;
        
            //we need to find the longest matching unit string
            foreach (const QString &unit, units)
            {
                int idx = value.indexOf(unit);
                
                if (idx + unit.length() == value.length())
                {
                    //this matches the whole of the string
                    if (idx != -1)
                    {
                        if (index == -1)
                            index = idx;
                        
                        else if (idx < index)
                            index = idx;
                    }
                }
            }
            
            if (index == -1)
            {
                double val = readFloat(value);
                return QPair<double,QString>(val,QString::null);
            }
            else
            {
                double val = readFloat(value.mid(0,index));
                return QPair<double,QString>(val,value.mid(index));
            }
        }

        bool readBool(QString text)
        {
            text = text.trimmed().toLower();
            
            if (text == "true" or text == "yes" or text == "on" or text == "1")
            {
                return true;
            }
            else if (text == "false" or text == "no" or text == "off" or text == "0")
            {
                return false;
            }
            else
                throw SireError::invalid_arg( QObject::tr(
                        "Could not get a true/false value from the string \"%1\".")
                            .arg(text), CODELOC );
                            
            return false;
        }

    }
}

using namespace SireSim;
using namespace SireSim::detail;

//////////
////////// Implementation of LengthData
//////////

const char* LengthData::typeName()
{
    return "SireSim::LengthValue";
}

QString LengthData::tagName()
{
    static QString tagname = "length";
    return tagname;
}

QHash<QString,double> *length_units(0);

QHash<QString,double> LengthData::units()
{
    while (length_units == 0)
    {
        QHash<QString,double> units;

        units.insert("angstrom", angstrom);
        units.insert("angstroms", angstrom);
        units.insert("A", angstrom);
        units.insert("Å", angstrom);
        
        units.insert("picometer", picometer);
        units.insert("picometers", picometer);
        units.insert("pm", picometer);
        
        units.insert("nanometer", nanometer);
        units.insert("nanometers", nanometer);
        units.insert("nm", nanometer);

        units.insert("micrometer", micrometer);
        units.insert("micrometers", micrometer);
        units.insert("μm", micrometer);

        units.insert("millimeter", millimeter);
        units.insert("millimeters", millimeter);
        units.insert("mm", millimeter);

        units.insert("centimeter", centimeter);
        units.insert("centimeters", centimeter);
        units.insert("cm", centimeter);

        units.insert("meter", meter);
        units.insert("meters", meter);
        units.insert("m", meter);

        units.insert("kilometer", kilometer);
        units.insert("kilometers", kilometer);
        units.insert("km", kilometer);

        units.insert("megameter", megameter);
        units.insert("megameters", megameter);
        units.insert("Mm", megameter);

        units.insert("bohr_radii", bohr_radii);
        units.insert("bohr_radius", bohr_radii);

        units.insert("inch", inch);
        units.insert("inches", inch);
        
        units.insert("foot", foot);
        units.insert("feet", foot);

        units.insert("yard", yard);
        units.insert("yards", yard);

        units.insert("mile", mile);
        units.insert("miles", mile);

        units.insert("light_year", light_year);
        units.insert("light_years", light_year);
        units.insert("ly", light_year);
        
        units.insert("parsec", parsec);
        units.insert("parsecs", parsec);
        units.insert("pc", parsec);
        
        if (length_units == 0)
            length_units = new QHash<QString,double>(units);
    }
    
    return *length_units;
}

namespace SireSim{ template class DimensionValue<detail::LengthData>; }


//////////
////////// Implementation of AreaData
//////////

const char* AreaData::typeName()
{
    return "SireSim::AreaValue";
}

QString AreaData::tagName()
{
    static QString tagname = "area";
    return tagname;
}

QHash<QString,double> *area_units(0);

QHash<QString,double> AreaData::units()
{
    while (area_units == 0)
    {
        QHash<QString,double> units;

        units.insert("angstrom^2", angstrom2);
        units.insert("A^2", angstrom2);
        units.insert("Å^2", angstrom2);
        
        units.insert("picometer^2", picometer2);
        units.insert("pm^2", picometer2);
        
        units.insert("nanometer^2", nanometer2);
        units.insert("nm^2", nanometer2);

        units.insert("micrometer^2", micrometer2);
        units.insert("μm^2", micrometer2);

        units.insert("millimeter^2", millimeter2);
        units.insert("mm^2", millimeter2);

        units.insert("centimeter^2", centimeter2);
        units.insert("cm^2", centimeter2);

        units.insert("meter^2", meter2);
        units.insert("m^2", meter2);

        units.insert("kilometer^2", kilometer2);
        units.insert("km^2", kilometer2);

        units.insert("megameter^2", megameter2);
        units.insert("Mm^2", megameter2);

        units.insert("bohr_radii^2", bohr_radii2);
        units.insert("bohr_radius^2", bohr_radii2);

        units.insert("inch^2", inch2);
        units.insert("inches^2", inch2);
        
        units.insert("foot^2", foot2);
        units.insert("feet^2", foot2);

        units.insert("yard^2", yard2);
        units.insert("yards^2", yard2);

        units.insert("mile^2", mile2);
        units.insert("miles^2", mile2);

        units.insert("light_year^2", light_year2);
        units.insert("light_years^2", light_year2);
        units.insert("ly^2", light_year2);
        
        units.insert("parsec^2", parsec2);
        units.insert("parsecs^2", parsec2);
        units.insert("pc^2", parsec2);
        
        if (area_units == 0)
            area_units = new QHash<QString,double>(units);
    }
    
    return *area_units;
}

namespace SireSim{ template class DimensionValue<detail::AreaData>; }

//////////
////////// Implementation of PressureData
//////////

const char* PressureData::typeName()
{
    return "SireSim::PressureValue";
}

QString PressureData::tagName()
{
    static QString tagname = "pressure";
    return tagname;
}

QHash<QString,double> *pressure_units(0);

QHash<QString,double> PressureData::units()
{
    while (pressure_units == 0)
    {
        QHash<QString,double> units;

        units.insert("pascal", pascal);
        units.insert("pascals", pascal);
        
        units.insert("bar", bar);

        units.insert("atmospheres", atm);
        units.insert("atmosphere", atm);
        units.insert("atm", atm);

        units.insert("pounds inch^-2", psi);
        units.insert("pound inch^-2", psi);
        units.insert("psi", psi);
        
        units.insert("mmHg", mmHg);
        
        if (pressure_units == 0)
            pressure_units = new QHash<QString,double>(units);
    }
    
    return *pressure_units;
}

namespace SireSim{ template class DimensionValue<detail::PressureData>; }
