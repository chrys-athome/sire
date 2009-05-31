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

#ifndef SPIER_GLDISPLAYLIST_H
#define SPIER_GLDISPLAYLIST_H

#include <QGLWidget>
#include <QGLContext>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLDisplayList;
class GLDisplayListRecorder;

namespace detail
{
class GLDisplayListData;
}

/** This class holds an OpenGL display list. This display list
    will have been compiled for a specific OpenGL context
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLDisplayList
{
public:
    GLDisplayList();
    GLDisplayList(const GLDisplayList &other);
    
    ~GLDisplayList();
    
    GLDisplayList& operator=(const GLDisplayList &other);
    
    bool operator==(const GLDisplayList &other) const;
    bool operator!=(const GLDisplayList &other) const;
    
    bool play(const QGLContext *playing_context);
    
    void startRecording(const QGLContext *recording_context);
    void stopRecording();

    bool isEmpty() const;

    bool isValidFor(const QGLContext *render_context) const;

private:
    boost::shared_ptr<detail::GLDisplayListData> d;
};

/** This simple class is used to ensure that recording stops
    if an error occurs (RAII)
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLDisplayListRecorder : public boost::noncopyable
{
public:
    GLDisplayListRecorder();
    GLDisplayListRecorder(GLDisplayList &list, const QGLContext *context);
    
    ~GLDisplayListRecorder();
    
private:
    /** The GLDisplayList being recorded */
    GLDisplayList recording_list;
};

}

SIRE_END_HEADER

#endif
