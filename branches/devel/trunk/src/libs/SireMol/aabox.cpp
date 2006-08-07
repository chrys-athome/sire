
#include "aabox.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AABox> r_aabox("SireMol::AABox");

/** Serialise an AABox to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AABox &aabox)
{
    writeHeader(ds, r_aabox, 1) << aabox.cent << aabox.halfextents << aabox.rad;
    
    return ds;
}

/** Deserialise an AABox from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AABox &aabox)
{
    VersionID v = readHeader(ds, r_aabox);
    
    if (v == 1)
    {
        ds >> aabox.cent >> aabox.halfextents >> aabox.rad;
    }
    else
        throw version_error(v, "1", r_aabox, CODELOC);
    
    return ds;
}

/** Construct an empty AABox */
AABox::AABox() : cent(), halfextents(), rad(0.0)
{}

/** Construct an AABox with center at 'cent', and half-extents 'extents' */
AABox::AABox(const Vector &c, const Vector &extents) : cent(c), halfextents(extents)
{
    rad = halfextents.length();
}

/** Construct an AABox that completely encases the atoms 'atoms' */
AABox::AABox(const AtomSet &atoms)
{
    recalculate(atoms);
}

/** Construct an AABox that completely encases the atoms 'atoms' */
AABox::AABox(const AtomVector &atoms)
{
    recalculate(atoms);
}

/** Construct an AABox that completely encases the atoms 'atoms' */
AABox::AABox(const AtomList &atoms)
{
    recalculate(atoms);
}

/** Destructor */
AABox::~AABox()
{}

/** Comparison operator */
bool AABox::operator==(const AABox &other) const
{
    return this == &other or
          (rad == other.rad and cent == other.cent and halfextents == other.halfextents);
}

/** Comparison operator */
bool AABox::operator!=(const AABox &other) const
{
    return this != &other and 
          (rad != other.rad or cent != other.cent or halfextents != other.halfextents);
}

/** Operator '=' also recalculates the AABox to encase 'atoms' */
const AABox& AABox::operator=(const AtomVector &atoms)
{
    recalculate(atoms);
    return *this;
}

/** Operator '=' also recalculates the AABox to encase 'atoms' */
const AABox& AABox::operator=(const AtomSet &atoms)
{
    recalculate(atoms);
    return *this;
}

/** This is a template function that calculates an AABox from the supplied atom container */
template<class T>
AABox getAABox(const T &atoms)
{
    if (atoms.count() > 0)
    {
        //set the initial max and min coords from the first atom in the CutGroup
        Vector maxcoords(atoms.at(0));
        Vector mincoords(maxcoords);
    
        //loop through all of the atoms of the CutGroup
        for (int i=1; i < atoms.count(); i++)
        {
            //calculate the maximum and minimum coordinates
            const Atom &atom = atoms.at(i);
            maxcoords.setMax( atom );
            mincoords.setMin( atom );
        }
        
        //now calculate the center as half the maximum and minimum coordinates
        Vector cent = 0.5 * (maxcoords + mincoords);
        
        //the positive half-extent is the difference between the maximum
        //coordinates and the center
        Vector halfextents = maxcoords - cent;
    
        return AABox(cent,halfextents);
    }
    else
    {
        return AABox();
    }   
}

/** Recalculate the box to completely encase 'atoms' */
void AABox::recalculate(const AtomVector &atoms)
{
    *this = getAABox<AtomVector>(atoms);
}        

/** Recalculate the box to completely encase 'atoms' */
void AABox::recalculate(const AtomSet &atoms)
{
    *this = getAABox<AtomSet>(atoms);
}

/** Recalculate the box to completely encase 'atoms' */
void AABox::recalculate(const AtomList &atoms)
{
    *this = getAABox<AtomList>(atoms);
}

/** Return whether or not this box is within 'dist' of box 'box'.
    (using infinite cartesian axes) */
bool AABox::withinDistance(double dist, const AABox &box) const
{
   //look at the components of the distance along the x, y and z axes
    double dx = std::abs(cent.x() - box.cent.x()) - halfextents.x() - box.halfextents.x();
    double dy = std::abs(cent.y() - box.cent.y()) - halfextents.y() - box.halfextents.y();
    double dz = std::abs(cent.z() - box.cent.z()) - halfextents.z() - box.halfextents.z();
        
    dx = SIRE_MIN(dx,0.0);
    dy = SIRE_MIN(dy,0.0);
    dz = SIRE_MIN(dz,0.0);

    return dx*dx + dy*dy + dz*dz <= dist*dist;
}

/** Return whether this box intersects with 'box' */
bool AABox::intersects(const AABox &box) const
{
    //look at the components of the distance along the x, y and z axes
    double dx = std::abs(cent.x() - box.cent.x()) - halfextents.x() - box.halfextents.x();
    double dy = std::abs(cent.y() - box.cent.y()) - halfextents.y() - box.halfextents.y();
    double dz = std::abs(cent.z() - box.cent.z()) - halfextents.z() - box.halfextents.z();
        
    return dx <= 0.0 and dy <= 0.0 and dz <= 0.0;
}

/** Translate this AABox by 'delta' */
void AABox::translate(const Vector &delta)
{
    cent += delta;
}
