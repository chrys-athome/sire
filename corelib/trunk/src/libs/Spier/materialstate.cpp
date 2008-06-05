
#include "materialstate.h"
#include "material.h"

using namespace Spier;

MaterialState::MaterialState() : forcebind(true)
{}

MaterialState::~MaterialState()
{}

/** Bind the passed material to the current openGL context. This will minimise
    the amount of work if the current material is similar or identical to the 
    passed material */
void MaterialState::bind(const MaterialPtr &material)
{
    if (material.get() == 0)
        return;
    
    //only bind the material if it has changed
    if (forcebind or current_material != material)
    {
        const GLMaterial &glmat = material->material();
        const GLTexture &gltex = material->texture();
        const GLShader &glshader = material->shader();
        
        const Material *cur_ptr = current_material.get();
        if (forcebind or cur_ptr == 0)
        {
            glmat.bind();
            gltex.bind();
            glshader.bind();
        }
        else
        {
            if (cur_ptr->material() != glmat)
                glmat.bind();
        
            if (cur_ptr->texture() != gltex)
                gltex.bind();
                    
            if (cur_ptr->shader() != glshader)
                glshader.bind();
        }
        
        current_material = material;
        forcebind = false;
    }
}

/** Clear information about the current state */
void MaterialState::clear()
{
    forcebind = true;
}
