/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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
