#ifndef SPIER_MATERIAL_H
#define SPIER_MATERIAL_H

#include "glmaterial.h"
#include "gltexture.h"
#include "glshader.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class abstracts a material, which represents the look of an object. The material is a combination of the openGL material (diffuse, ambient and specular properties - represented by GLMaterial), with the material's texture (represented by GLTexture) and the material's shader properties (represented by GLShader).
 
@author Christopher Woods
*/
class SPIER_EXPORT Material
{
public:
    Material();
    ~Material();

    bool operator==(const Material &other) const;
    bool operator!=(const Material &other) const;

    const GLMaterial& material() const;
    const GLTexture& texture() const;
    const GLShader& shader() const;
    
    void setMaterial(const GLMaterial &material);
    void setTexture(const GLTexture &texture);
    void setShader(const GLShader &shader);

protected:
    /** Pointer to the GLMaterial for this material */
    GLMaterial glmat;
    
    /** Pointer to the GLTexture for this material */
    GLTexture gltex;
    
    /** Pointer to the shader for this material */
    GLShader glshader;
};

/** Return a reference to the openGL material of this material */
inline const GLMaterial& Material::material() const
{
      return glmat;
}

/** Return a reference to the openGL texture of this material */
inline const GLTexture& Material::texture() const
{
      return gltex;
}

/** Return a reference to the openGL shader of this material */
inline const GLShader& Material::shader() const
{
      return glshader;
}

/** Set the openGL material of this material */
inline void Material::setMaterial(const GLMaterial &material)
{
    glmat = material;
}

/** Set the openGL material of this material */
inline void Material::setTexture(const GLTexture &texture)
{
    gltex = texture;
}

/** Set the openGL shader of this material */
inline void Material::setShader(const GLShader &shader)
{
    glshader = shader;
}

}

SIRE_END_HEADER

#endif
