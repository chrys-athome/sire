
#include "material.h"
#include "materialmgr.h"

using namespace Spier;

Material::Material()
{}

Material::~Material()
{}

/** Test for equality of two materials */
bool Material::operator==(const Material &other) const
{
    return (glmat == other.glmat and gltex == other.gltex and glshader == other.glshader);
}

/** Test for inequality */
bool Material::operator!=(const Material &other) const
{
    return (glmat != other.glmat or gltex != other.gltex or glshader != other.glshader);
}
