
#include "materialmgr.h"
#include "material.h"

using namespace Spier;
using namespace SireMol;

MaterialMgr::MaterialMgr() 
{}

MaterialMgr::~MaterialMgr()
{}

MaterialPtr MaterialMgr::getMaterial(const char *name)
{
    return this->getMaterial(QString(name));
}

MaterialPtr MaterialMgr::getMaterial(const QString &name)
{
    if (mats.contains(name))
        return mats[name];
    else
    {
        //create a default material (orange)
        MaterialPtr mat( new Material() );
        
        mat->setMaterial( GLMaterial( Color(255, 162, 13) ) );
        
        mats.insert(name, mat);
        
        return mat;
    }
}

MaterialPtr MaterialMgr::getMaterial(const Element &elmnt)
{
    if (mats.contains(elmnt.name()))
        return mats[elmnt.name()];
    else
    {
        //create a new material that is based on the 
        //color specified by the element
        MaterialPtr mat( new Material() );
        
        mat->setMaterial( GLMaterial(Vector(elmnt.red(), elmnt.green(), elmnt.blue())) );
        
        mats.insert(elmnt.name(), mat);
        
        return mat;
    }
}
