
#include "aabox.h"
#include "coordgroup.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireVol;

static const RegisterMetaType<AABox> r_aabox("SireVol::AABox");

/** Serialise an AABox to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const AABox &aabox)
{
    writeHeader(ds, r_aabox, 1) << aabox.cent << aabox.halfextents << aabox.rad;
    
    return ds;
}

/** Deserialise an AABox from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, AABox &aabox)
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

/** Construct an AABox that completely encases the CoordGroup 'coordgroup' */
AABox::AABox(const CoordGroup &coordgroup)
{
    recalculate(coordgroup);
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

/** Internal function used to recalculate the AABox from the coordinates in the 
    array 'coords' (which has size 'sz') */
void AABox::recalculate(const Vector *coords, int sz)
{
    if (sz > 0)
    {
        //set the initial max and min coords from the first coordinate in the group
        Vector maxcoords( coords[0] );
        Vector mincoords( maxcoords );
    
        //loop through all of the remaining coordinates in the group
        for (int i=1; i < sz; ++i)
        {
            //calculate the maximum and minimum coordinates
            const Vector &coord = coords[i];
            maxcoords.setMax( coord );
            mincoords.setMin( coord );
        }
        
        //now calculate the center as half the maximum and minimum coordinates
        cent = 0.5 * (maxcoords + mincoords);
        
        //the positive half-extent is the difference between the maximum
        //coordinates and the center
        halfextents = maxcoords - cent;
        
        //the radius is the length of 'halfextents'
        rad = halfextents.length();
    }
    else
    {
        cent = Vector(0);
        halfextents = Vector(0);
        sz = 0;
    }
}

/** Recalculate the AABox so that it completely encloses the CoordGroup 'coordgroup' */
void AABox::recalculate(const CoordGroup &coordgroup)
{
    this->recalculate( coordgroup.constData(), coordgroup.size() );
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
