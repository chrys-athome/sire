#ifndef SPIER_GLMATERIAL_H
#define SPIER_GLMATERIAL_H

#include <QSharedDataPointer>

#include "SireMaths/vector.h"

#include "color.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Vector;

class GLMaterialData;

/**
This class abstract an openGL material (the collection of diffuse, ambient, specular, shininess and emission properties, set via the glMaterial command).
 
@author Christopher Woods
*/
class SPIER_EXPORT GLMaterial
{
public:
    GLMaterial();
    GLMaterial(const Vector &basecolor);
    GLMaterial(const Vector &ambient, const Vector &diffuse, const Vector &specular,
               const Vector &emission, double shininess, double opacity=1.0);
    
    GLMaterial(const Color &basecolor);
    GLMaterial(const Color &ambient, const Color &diffuse, const Color &specular, 
               const Color &emission, double shininess);
    
    GLMaterial(const GLMaterial &other);
    
    ~GLMaterial();

    GLMaterial& operator=(const GLMaterial &other);

    void setAmbient(const Vector &color, double opacity=1.0);
    void setDiffuse(const Vector &color, double opacity=1.0);
    void setAmbientDiffuse(const Vector &color, double opacity=1.0);
    
    void setSpecular(const Vector &color, double opacity=1.0);
    void setEmission(const Vector &color, double opacity=1.0);

    void setAmbient(const Color &color);
    void setDiffuse(const Color &color);
    void setAmbientDiffuse(const Color &color);
    
    void setSpecular(const Color &color);
    void setEmission(const Color &color);

    void setShininess(double shininess);

    void bind() const;

    bool operator==(const GLMaterial &other) const;
    bool operator!=(const GLMaterial &other) const;

private:
    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<GLMaterialData> d;
};

}

SIRE_END_HEADER

#endif
