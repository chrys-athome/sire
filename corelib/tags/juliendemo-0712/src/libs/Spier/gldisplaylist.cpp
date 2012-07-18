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

#include "gldisplaylist.h"
#include "glrendercontext.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace Spier;
using namespace SireStream;

static const RegisterMetaType<GLDisplayList> r_gldisplist;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLDisplayList &gldisplist)
{
    writeHeader(ds, r_gldisplist, 1);
    
    SharedDataStream sds(ds);
    sds << gldisplist.render_func;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds, GLDisplayList &gldisplist)
{
    VersionID v = readHeader(ds, r_gldisplist);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> gldisplist.render_func;
    }
    else
        throw version_error(v, "1", r_gldisplist, CODELOC);
        
    return ds;
}

/** Constructor */
GLDisplayList::GLDisplayList()
{}

/** Construct to render the passed function */
GLDisplayList::GLDisplayList(const GLRenderFunction &renderfunc)
              : render_func(renderfunc)
{}

/** Copy constructor */
GLDisplayList::GLDisplayList(const GLDisplayList &other)
              : render_func(other.render_func)
{}

/** Destructor */
GLDisplayList::~GLDisplayList()
{
    if (not render_func.isNull())
    {
        if (render_func.unique())
        {
            qDebug() << "Deleting the display list" << render_func->what();
            //we don't need this display list any more
            GLRenderContext::deleteList(*this);
        }
    }
}

/** Copy assignment operator */
GLDisplayList& GLDisplayList::operator=(const GLDisplayList &other)
{
    render_func = other.render_func;
    return *this;
}

/** Comparison operator */
bool GLDisplayList::operator==(const GLDisplayList &other) const
{
    return render_func == other.render_func;
}

/** Comparison operator */
bool GLDisplayList::operator!=(const GLDisplayList &other) const
{
    return render_func != other.render_func;
}

/** Render the contents of this display list on the context 'render_context' */
void GLDisplayList::render(GLRenderContext &render_context) const
{
    render_context.render(*this);
}

/** Is this an empty list? */
bool GLDisplayList::isEmpty() const
{
    return render_func.isNull();
}

/** Return the render function for this display list */
const GLRenderFunction& GLDisplayList::renderFunction() const
{
    return render_func;
}
