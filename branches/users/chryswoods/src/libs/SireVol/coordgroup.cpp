/** 
  * @file
  *
  * C++ Implementation: CoordGroup, CoordGroupEditor and CoordGroupPvt
  *
  * Description: 
  * Implementation of the public CoordGroup and CoordGroupEditor classes,
  * and the private CoordGroupPvt class.
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedData>

#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

#include "coordgroup.h"

using namespace SireMaths;

namespace SireVol 
{

/** Private class which holds the data for CoordGroup and CoordGroupEditor

    @author Christopher Woods
*/
class CoordGroupPvt : public QSharedData
{
public:
    CoordGroupPvt();
    CoordGroupPvt(int size);
    CoordGroupPvt(int size, const Vector &value);
    
    CoordGroupPvt(const CoordGroupPvt &other);
    
    ~CoordGroupPvt();
    
    const Vector& at(int i) const;
    
    const Vector& operator[](int i) const;
    Vector& operator[](int i);
    
    bool isNull() const;
    
    void clear();
    
    int count() const;
    int size() const;
    
    const AABox& aaBox() const;
    
    const Vector* constData() const;
    const Vector* data() const;
    Vector* data();
    
    void translate(const Vector &delta);
    void rotate(const Vector &point, const Quaternion &quat);
    void rotate(const Vector &point, const Matrix &rotmat);
    
    void translateAndUpdate(const Vector &delta);
    void rotateAndUpdate(const Vector &point, const Quaternion &quat);
    void rotateAndUpdate(const Vector &point, const Matrix &rotmat);
    
private:
    /** Pointer to the array of coordinates */
    Vector *coords;
    /** The number of coordinates in the group */
    int sz;
    
    /** The AABox which should completely enclose all of the points */
    AABox aabox;
};

}

/////////////
///////////// Implementation of CoordGroupPvt
/////////////

CoordGroupPvt::CoordGroupPvt()
{}

CoordGroupPvt::~CoordGroupPvt()
{}

/////////////
///////////// Implementation of CoordGroup
/////////////

CoordGroup::CoordGroup()
{}

CoordGroup::~CoordGroup()
{}

/////////////
///////////// Implementation of CoordGroupEditor
/////////////

CoordGroupEditor::CoordGroupEditor()
{}

CoordGroupEditor::~CoordGroupEditor()
{}
