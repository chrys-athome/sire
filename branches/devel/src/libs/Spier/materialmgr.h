#ifndef SPIER_MATERIALMGR_H
#define SPIER_MATERIALMGR_H

#include <QHash>

#include "materialptr.h"

#include "SireMol/atom.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMol::Element;

class Material;
class GLMaterial;
class GLTexture;
class GLShader;

/**
This maintains a library of materials.
 
@author Christopher Woods
*/
class SPIER_EXPORT MaterialMgr
{
public:
    MaterialMgr();
    ~MaterialMgr();

    MaterialPtr getMaterial(const char *name);
    MaterialPtr getMaterial(const QString &name);
    MaterialPtr getMaterial(const Element &elmnt);

private:

    /** Hash that stores different materials indexed name */
    QHash<QString, MaterialPtr> mats;
};

}

SIRE_END_HEADER

#endif
