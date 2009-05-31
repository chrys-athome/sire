/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SPIER_CAMERA_H
#define SPIER_CAMERA_H

#include <QSize>

#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/plane.h"
#include "SireMaths/sphere.h"

#include "SireUnits/dimensions.h"

#include <QObject>

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Plane;
using SireMaths::Sphere;

/** The Camera is the view onto the world!
 
    @author Christopher Woods
*/
class SPIER_EXPORT Camera : public QObject
{

    Q_OBJECT

public:
    Camera(QObject *parent=0);
    ~Camera();

    const Vector& position() const;

    const Vector& viewVec() const;
    const Vector& sideVec() const;
    const Vector& upVec() const;
    
    const SireUnits::Dimension::Angle& viewAngle() const;
    float range() const;

    bool look(bool force=false);

    double distance(const Vector &point) const;

    bool contains(const Vector &point) const;
    bool contains(const Sphere &sphere) const;

    bool viewChanged() const;
    bool sizeChanged() const;
    bool needRepaint() const;

public slots:
    
    void reset();
    void zoom(double delta);
    void spin(const SireUnits::Dimension::Angle &delta);
    void rotate(const SireUnits::Dimension::Angle &delta, const Vector &axis);    
    void translate(double dx, double dy);
    
    void lookAt(const Vector &lookat);
    
    void setSize(const QSize &size);

protected:
    void calculateViewVectors();

    /** The point at which the camera is located */
    Vector loc;
    
    /** The point in space that this camera is looking at
        (this is the point about which the camera view rotates) */
    Vector lookat;
    
    /** The normalised vector pointing straight ahead */
    Vector viewvec;
    
    /** The normalised vector pointing straight up */
    Vector upvec;
    
    /** The normalised vector pointing right */
    Vector sidevec;

    /** The viewing angle of the camera */
    SireUnits::Dimension::Angle viewang;

    /** The range of the camera */
    float rnge;
    
    /** The x and y offsets of the camera (translation delta) */
    double offsetx,offsety;
    
    /** The signed distance from the plane containing the 
        point 'lookat' and perpendicular to 'viewvec', to the 
        current camera position */
    double zoomdistance;
    
    /** The size of the canvas */
    QSize sz;
    
    /** Enum giving the indicies of the six planes */
    enum PLANEIDX{ _left=0, _right=1, _bottom=2, _top=3, _near=4, _far=5 };
    
    /** The six planes of the view frustrum (in world-space coordinates) */
    Plane fplanes[6];

    /** Whether or not the view has changed */
    bool viewchanged;
    
    /** Whether or not the size of the canvas has changed */
    bool sizechanged;
};

}

SIRE_END_HEADER

#endif
