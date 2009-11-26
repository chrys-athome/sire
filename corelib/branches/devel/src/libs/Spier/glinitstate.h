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

#ifndef SPIER_GLINITSTATE_H
#define SPIER_GLINITSTATE_H

#include "glrenderfunction.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLInitState;
class GLInitRenderState;
class GLInitSelectorState;
}

QDataStream& operator<<(QDataStream&, const Spier::GLInitState&);
QDataStream& operator>>(QDataStream&, Spier::GLInitState&);

QDataStream& operator<<(QDataStream&, const Spier::GLInitRenderState&);
QDataStream& operator>>(QDataStream&, Spier::GLInitRenderState&);

QDataStream& operator<<(QDataStream&, const Spier::GLInitSelectorState&);
QDataStream& operator>>(QDataStream&, Spier::GLInitSelectorState&);

namespace Spier
{

/** This is the base class of the render functions that
    initialise the state of the OpenGL context for rendering
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLInitState : public GLRenderFunction
{

friend QDataStream& ::operator<<(QDataStream&, const GLInitState&);
friend QDataStream& ::operator>>(QDataStream&, GLInitState&);

public:
    GLInitState();
    
    GLInitState(const GLInitState &other);
    
    virtual ~GLInitState();
    
    static const char* typeName()
    {
        return "Spier::GLInitState";
    }

    virtual GLInitState* clone() const=0;
};

/** This contains the OpenGL functions necessary to initialise
    a scene for normal rendering
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLInitRenderState
         : public SireBase::ConcreteProperty<GLInitRenderState,GLInitState>
{

friend QDataStream& ::operator<<(QDataStream&, const GLInitRenderState&);
friend QDataStream& ::operator>>(QDataStream&, GLInitRenderState&);

public:
    GLInitRenderState();
    
    GLInitRenderState(const GLInitRenderState &other);
    
    ~GLInitRenderState();
    
    GLInitRenderState& operator=(const GLInitRenderState &other);
    
    bool operator==(const GLInitRenderState &other) const;
    bool operator!=(const GLInitRenderState &other) const;
    
    static const char* typeName();
    
    void operator()() const;
};

/** This contains the OpenGL functions necessary to initialise
    a scene for selector rendering
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLInitSelectorState
         : public SireBase::ConcreteProperty<GLInitSelectorState,GLInitState>
{

friend QDataStream& ::operator<<(QDataStream&, const GLInitSelectorState&);
friend QDataStream& ::operator>>(QDataStream&, GLInitSelectorState&);

public:
    GLInitSelectorState();
    
    GLInitSelectorState(const GLInitSelectorState &other);
    
    ~GLInitSelectorState();
    
    GLInitSelectorState& operator=(const GLInitSelectorState &other);
    
    bool operator==(const GLInitSelectorState &other) const;
    bool operator!=(const GLInitSelectorState &other) const;
    
    static const char* typeName();
    
    void operator()() const;
};

}

Q_DECLARE_METATYPE( Spier::GLInitRenderState )
Q_DECLARE_METATYPE( Spier::GLInitSelectorState )

SIRE_EXPOSE_CLASS( Spier::GLInitState )
SIRE_EXPOSE_CLASS( Spier::GLInitRenderState )
SIRE_EXPOSE_CLASS( Spier::GLInitSelectorState )

SIRE_END_HEADER

#endif
