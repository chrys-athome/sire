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

#include "ThirdPartyGUI/glew.h"

#include "glcanvas.h"
#include "viewer.h"
#include "viewobj.h"
#include "camera.h"
#include "cpkrep.h"
#include "vdwrep.h"
#include "materialmgr.h"
#include "viewmode.h"
#include "moldisplaydata.h"
#include "molselectid.h"
#include "canvasui.h"

#include "SireMol/editmol.h"

#include <QMouseEvent>
#include <QCursor>
#include <QPoint>
#include <QThread>
#include <QApplication>
#include <QPainter>
#include <QDebug>

using namespace Spier;
using namespace SireMol;

template<>
ViewObj* set_indexer(const ViewObjPtr &viewobj)
{
    return viewobj.get();
}

/** Construct the GLCanvas */
GLCanvas::GLCanvas(Viewer *parent)
       : QObject(parent),
         prnt(parent), cam( new Camera() ), 
         needrepaint(true), back_buffer_ready(false),
         sceneidx(0), bgidx(0), isrepainting(false)
{
    current_interpreter = new ViewMode(this);
}

/** Destructor */
GLCanvas::~GLCanvas()
{
    if (bgidx)
        glDeleteLists(bgidx,1);
        
    if (sceneidx)
        glDeleteLists(sceneidx,1);
        
    delete cam;
}

/** Reset the canvas back to defaults */
void GLCanvas::reset()
{
    curitem = SelectID();
    selectobjs.clear();
    needrepaint = true;
    
    cam->reset();
    
    mols.clear();
    dispobjs.clear();
    matstate.clear();

    if (bgidx)
    {
        glDeleteLists(bgidx,1);
        bgidx = 0;
    }
        
    if (sceneidx)
    {
        glDeleteLists(sceneidx,1);
        sceneidx = 0;
    }
}

/** Return a reference to the material library */
MaterialMgr& GLCanvas::materialLibrary()
{
    return prnt->materialLibrary();
}

/** Return a reference to the mesh library */
MeshMgr& GLCanvas::meshLibrary()
{
    return prnt->meshLibrary();
}

/** Return a reference to the openGL context used by this GLCanvas */
const QGLContext& GLCanvas::context() const
{
    return *(prnt->context());
}

/** Return the openGL format (QGLFormat) associated with this GLCanvas */
QGLFormat GLCanvas::format() const
{
    return prnt->format();
}

/** Tell the canvas to repaint */
void GLCanvas::updateGL()
{
    if (not isrepainting)
        //there is no need to repaint the canvas
        //if it is already in the process of being repainted
        prnt->updateGL();
}

/** Set the cursor shape */
void GLCanvas::setCursor(Qt::CursorShape shape)
{
    prnt->setCursor(shape);
}

/** Set the user-interface that is drawn over the top of the scene */
void GLCanvas::setUI(const CanvasUIPtr &uiptr)
{
    //do nothing if this is the same as current
    if (uiptr == canvasui)
        return;
        
    //tell the old UI that it has lost this canvas
    if (canvasui)
        canvasui->setCanvas(0);
        
    canvasui = uiptr;
    
    if (canvasui)
    {
        canvasui->setCanvas(this);
        
        connect(this,SIGNAL(itemHighlighted(const SelectID&)),
                canvasui,SLOT(itemHighlighted(const SelectID&)));
                
        connect(this,SIGNAL(selectionChanged(const SelectList&)),
                canvasui,SLOT(selectionChanged(const SelectList&)));
    }
}

/** Display the molecule pointed to by 'molptr' */
void GLCanvas::display(const EditMol &mol)
{
    MolDisplayDataPtr moldata = MolDisplayData::create(this, mol);
    mols.insert(&(moldata->molecule()), moldata);
}

/** Center on the center of this list of molecules (note that none of these
    molecules have to have been loaded for this to work) */
void GLCanvas::center(const EditMolList &mols)
{
    AtomSet atms;
    
    int nmols = 0;
    Vector minvec,maxvec;
    
    for (int i=0; i<mols.count(); ++i)
    {
        Vector molcenter = mols[i].center();
        nmols ++;
           
        if (nmols == 1)
        {
            minvec = molcenter;
            maxvec = molcenter;
        }
        else
        {
            minvec.setMin(molcenter);
            maxvec.setMax(molcenter);
        }
    }
    
    if (nmols > 0)
        this->center( 0.5*(minvec+maxvec) );
}

/** Center the display on the passed molecule (note that the molecule 
    does not need to have been loaded or have been displayed for this to work)
*/
void GLCanvas::center(const EditMol &mol)
{
    this->center(mol.center());
}

/** Center the display on the visible representations */
void GLCanvas::center()
{
    QHashIterator<const EditMol*,MolDisplayDataPtr> it(mols);
    
    Vector minvec,maxvec;
    
    int nmols = 0;
    
    while( it.hasNext() )
    {
        it.next();
        
        const MolDisplayDataPtr &moldata = it.value();
            
        Vector molcenter = moldata->molecule().center();
        
        nmols++;
        if (nmols == 1)
        {
            minvec = molcenter;
            maxvec = molcenter;
        }
        else
        {
            minvec.setMin(molcenter);
            maxvec.setMax(molcenter);
        }
    }
    
    if (nmols > 0)
        this->center( 0.5*(minvec+maxvec) );
}

/** Center on the point 'point' */
void GLCanvas::center(const Vector &point)
{
    cam->lookAt( point );
    updateGL();
}

/** Add the ViewObj to the list of objects to draw */
void GLCanvas::addToDisplay(ViewObjPtr dispobj)
{
    //do nothing if we are already displaying this object...
    ViewObj *ptr = dispobj.get();
    
    if (ptr == 0 or dispobjs.has_key(ptr))
        return;
        
    //add this object to the set of displayed objects
    dispobjs.insert(dispobj);
    
    //reparent this display object so that it is a child of this canvas
    ptr->setCanvas(this);
}

/** Remove the ViewObj from the display */
void GLCanvas::removeFromDisplay(const ViewObjPtr &dispobj)
{
    if (dispobj.get() != 0 and dispobjs.contains(dispobj))
    {
        dispobjs.remove(dispobj);
        dispobj->orphan();
    }
}

/** Initialise the openGL context to view this canvas - this can be 
    called several times, as it is called whenever the Viewer switches
    over to use this GLCanvas */
void GLCanvas::initialize()
{}

/** Bind the material to this openGL context - note that this material
    must not have been bound to any other openGL contexts */
void GLCanvas::bind(const MaterialPtr &material)
{
    matstate.bind(material);
}

/** Push all openGL attributes onto the stack */
void GLCanvas::pushAttrib()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
}

/** Pop all openGL attributes off of the stack (note that this may clear the 
    current material...) */
void GLCanvas::popAttrib()
{
    glPopAttrib();
    
    //we need to clear the material state as it may have been cleared by the 
    //popping of the openGL attributes
    matstate.clear();
}

/** Encode the 32bit index into a 24bit colour. Obviously this will only
    encode the index correctly if it is less than 2^24 */
void GLCanvas::encodeIndex(quint32 index)
{
    //add 1 to the index, as color 0 is no selection
    index += 1000;
    
    //this works for 24bit colour - can do 16bit by multiplying index by 256 here,
    //then dividing by 256 in the decode
    if (index >= 16777216)
        throw SireError::invalid_arg(QObject::tr(
            "Index is too large for 24bit colour mode! (%1)").arg(index));
            
    uchar r = index % 256;
    uchar g = (index/256) % 256;
    uchar b = (index/65536) % 256;
            
    glColor3ub( r, g, b );
}

/** Return the index encoded by the colour with the passed components,
    or -1 if no index is encoded. */
qint32 GLCanvas::decodeColorToIndex(uchar red, uchar green, uchar blue) const
{
    quint32 r = red;
    quint32 g = green;
    quint32 b = blue;

    qint32 idx = r + (g * 256) + (b * 65536) - 1000;

    return idx;
}

/** Set the render state so we can render the scene */
void GLCanvas::setSceneState()
{
    if (sceneidx)
        glCallList(sceneidx);
    else
    {
        sceneidx = glGenLists(1);
                
        if (sceneidx)
            glNewList(sceneidx, GL_COMPILE_AND_EXECUTE);
                
        glClearColor(0.9,0.9,0.9,1.0);
           
        glEnable(GL_DEPTH_TEST);
           
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
      
        glShadeModel(GL_SMOOTH);
      
        GLfloat globalamb[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalamb);
    
        GLfloat lightpos[] = { 150.0f, 150.0f, 0.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogf(GL_FOG_START,0.0);
        glFogf(GL_FOG_END,300.0);
        glFogf(GL_FOG_DENSITY,0.01);
    
        GLfloat fogcolor[] = { 0.5f, 0.5f, 0.5f, 0.0f };
        glFogfv(GL_FOG_COLOR, fogcolor);
    
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    
        glEnable(GL_TEXTURE_2D);
       
        this->clear();
       
        if (sceneidx)
            glEndList();
    }
            
    cam->look(true);
}

/** Save the current render state */
void GLCanvas::saveRenderState()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    pushAttrib();
}

/** Restore the render state (which must have been previously saved!) */
void GLCanvas::restoreRenderState()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    popAttrib();
}

/** Fill the back buffer with each ViewObj rendered as a single, different colour */
void GLCanvas::fillBackBuffer()
{
    //we render the scene to the back buffer, painting each object with 
    //a different colour
    
    cam->look(true);
            
    glDrawBuffer(GL_BACK);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    //disable antialiasing as it is not required, and it even messes with the 
    //colour selection!
    if (GLEW_ARB_multisample)
    {
        glDisable(GL_MULTISAMPLE_ARB);        
        
        if (GLEW_NV_multisample_filter_hint)
            //I need GL_FASTEST or I get poor results for colour picking
            glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);
    }
    
    glClearColor(0.0,0.0,0.0,0.0);
    this->clear();
    
    //render all objects to find the selected object...
    int nobjs = dispobjs.count();
    for (int i=0; i<nobjs; ++i)
    {
        this->encodeIndex(i);
        dispobjs.at(i)->selectDraw();
    }
    
    back_buffer_ready = true;
}

/** Find the object underneath the mouse. This assumes that the back buffer is already 
    filled with each ViewObj rendered using its encoded index value. */
SelectID GLCanvas::getObjectUnderMouse()
{
    if (not back_buffer_ready)
    {
        this->fillBackBuffer();
    }

    //we are in selection state whenever the back buffer is ready 
    //(we thus don't need to change the state again)

    QPoint mousepos = current_interpreter.pos();
    qint32 selected = decodeIndex(mousepos.x(), mousepos.y());
    
    if (selected >= 0 and selected < dispobjs.count())
    {
        //the mouse is over an object - we now need to select within the
        //object. The screen is cleared, and now only the object is drawn, with 
        //each separate part of the object drawn with a different colour.
        //(perhaps I could restrict rendering to only a small volume around
        //the current mouse position)
        
        this->clear();
    
        SelectID retval = dispobjs.at(selected)->select(mousepos.x(), mousepos.y());

        //the back buffer now no longer contains the whole-scene selection info
        back_buffer_ready = false;
        
        return retval;
    }
    else
        return SelectID();   
}

/** Paint the background (a pretty gradient) */
void GLCanvas::paintBackground()
{
    if (bgidx)
    {
        glCallList(bgidx);
    }
    else
    {
        bgidx = glGenLists(1);
        
        if (bgidx)
            glNewList(bgidx, GL_COMPILE_AND_EXECUTE);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
    
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    
//        pushAttrib();
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_FOG);
        
        if (GLEW_ARB_multisample)
            glDisable(GL_MULTISAMPLE_ARB);

        glBegin(GL_QUADS);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(0.0,0.0);
        glColor3f(0.0,0.0,0.5);
        glVertex2f(1.0,0.0);
        glColor3f(0.9,0.9,0.9);
        glVertex2f(1.0,1.0);
        glVertex2f(0.0,1.0);
        glEnd();
    
//        popAttrib();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        
        if (bgidx)
            glEndList();
    }
}

/** Paint the User interface (GL widget) layer */
void GLCanvas::paintUI()
{
    if (canvasui)
        canvasui->paint();
    
    clearError();
}

/** Clear the current error */
void GLCanvas::clearError()
{
    glGetError();
}

/** Check the status of the openGL error flag */
void GLCanvas::checkError(QString codeloc)
{
    GLenum err = glGetError();
    
    switch(err)
    {
        case GL_NO_ERROR:
            return;
        case GL_INVALID_ENUM:
            qDebug() << QObject::tr("%1: GL_INVALID_ENUM").arg(codeloc);
            return;
        case GL_INVALID_OPERATION:
            qDebug() << QObject::tr("%1: GL_INVALID_OPERATION").arg(codeloc);
            return;
        case GL_STACK_OVERFLOW:
            qDebug() << QObject::tr("%1: GL_STACK_OVERFLOW").arg(codeloc);
            return;
        case GL_STACK_UNDERFLOW:
            qDebug() << QObject::tr("%1: GL_STACK_UNDERFLOW").arg(codeloc);
            return;
        case GL_OUT_OF_MEMORY:
            qDebug() << QObject::tr("%1: GL_OUT_OF_MEMORY").arg(codeloc);
            return;
    }
}

/** Paint the canvas (render the scene) */
void GLCanvas::paintGL()
{
    QMutexLocker lkr(&rendermutex);
    
    isrepainting = true;
    
    rendertimer.start();

    needrepaint = needrepaint or (canvasui and canvasui->needRepaint());

    //look through the camera
    if (cam->needRepaint())
    {
        needrepaint = true;
        back_buffer_ready = false;
    }

    if (movemode)
    {
        //we are in move mode, so no item is selected and we will not perform
        //back-buffer selection
        curitem = SelectID();
    }
    else
    {
        //we are not moving the camera, so we can see if we have changed
        //what we are highlighting/selecting
        
        //do the selection on the back buffer - this will return the ID of the currently
        //selected item
        SelectID selected = this->getObjectUnderMouse();
        
        //has the selection changed? If so, then we will need to re-render
        if (curitem != selected)
        {
            curitem = selected;
            needrepaint = true;
            emit itemHighlighted(curitem);
        }
        
        //uncomment below two lines to view picking in action
        //parent()->swapBuffers();
        //return;
    }
    
    //only draw the scene again if we have to...
    if (needrepaint)
    {
        needrepaint = false;
    
        //Start the QPainter (do this now as it clears the screen)
        pntr.begin(parent());
        pntr.setRenderHint(QPainter::Antialiasing, true);
        pntr.setRenderHint(QPainter::TextAntialiasing, true);
        
        if (GLEW_ARB_multisample)
        {
            glEnable(GL_MULTISAMPLE_ARB);
            
            if (GLEW_NV_multisample_filter_hint)
                glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);
        }
        
        //save the QPainter state
        this->saveRenderState();
        
        //now switch to the scene rendering state
        this->setSceneState();
        
        //paint the background
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        this->paintBackground();
        glPopAttrib();
                
        //highlight the selected object (if we are not in move mode)
        if (not movemode)
            curitem.highlight();
        
        //draw the rest of the scene
        int nobjs = dispobjs.count();
    
        for (int i=0; i<nobjs; ++i)
            dispobjs.at(i)->draw();
        
        //check for any openGL errors
        checkError(CODELOC);
        
        //draw the user-interface layer - we need to restore the QPainter state
        this->restoreRenderState();
        this->paintUI();
        
        //finish painting
        pntr.end();
        
        //the back buffer now no longer contains the whole-scene selection info
        back_buffer_ready = false;
        
        //send the rendered scene to the front buffer
        parent()->swapBuffers();
    }

    isrepainting = false;
    //make sure that all commands have gone to the renderer
    glFlush();
        
    int ms = rendertimer.elapsed();
    
    if (ms < 50)
        renderwait.wait( &rendermutex, 50-ms );
    else if (ms > 500)
        qDebug() << QObject::tr("Render time = %1").arg(ms);
}

/** Convert the colour the pixel at window coordinates x,y into an index */
qint32 GLCanvas::decodeIndex(int x, int y) const
{
    unsigned char rgb[3];
    
    glReadBuffer(GL_BACK);
    glReadPixels(x, parent()->height()-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);

    return decodeColorToIndex(rgb[0], rgb[1], rgb[2]);
}

/** Set the passed SelectID as the selected object. This clears the current
    selection so that only 'obj' is selected. */
void GLCanvas::setSelected(SelectID obj)
{
    this->clearSelection();
    this->addSelected(obj);
}

/** Add the passed SelectID to the list of selected object. */
void GLCanvas::addSelected(SelectID obj)
{
    obj.select();
    
    if (obj.selected())
    {
        selectobjs.append(obj);
        
        emit selectionChanged(selectobjs);
        
        needrepaint = true;
        updateGL();
    }
}

/** Toggle whether or not 'obj' is selected */
void GLCanvas::toggleSelected(SelectID obj)
{
    obj.toggle();
    
    if (obj.selected())
        selectobjs.append(obj);
    else
        this->cleanSelectedList();
    
    emit selectionChanged(selectobjs);
    
    needrepaint = true;
    updateGL();
}

/** Deselect the passed object */
void GLCanvas::deselect(SelectID obj)
{
    obj.deselect();
    
    //clean up the list of selected objects
    //(removes any now deselected objects from the selection list)
    this->cleanSelectedList();
    
    emit selectionChanged(selectobjs);
    
    needrepaint = true;
    updateGL();
}

/** Clear the list of selected objects */
void GLCanvas::clearSelection()
{
    //loop over each selected item and deselect it...
    foreach( SelectID obj, selectobjs )
    {
        obj.deselect();
    }
    
    selectobjs.clear();
    
    emit selectionChanged(selectobjs);
    
    needrepaint = true;
    updateGL();
}

/** Clean the list of selected objects so that it definitely does not 
    contain any objects which are no longer selected. (this can occur
    when there are multiple representations referring to the same underlying
    object, e.g. multiple reps of a molecule. This means deselecting the atom
    with one rep will also deselect it for all reps) */
void GLCanvas::cleanSelectedList()
{
    //for now we will just remove deselected items. Eventually I will 
    //need to parse this list to get the set of available actions that can 
    //be applied to the selection...
    
    QMutableListIterator<SelectID> it(selectobjs);
    
    while (it.hasNext())
    {
        SelectID obj = it.next();
        
        if (not obj.selected())
            it.remove();
    }
}

/** Print all of the selected objects */
void GLCanvas::printSelected() const
{
    qDebug() << QObject::tr("\n****** Selected Objects ********");

    foreach( SelectID obj, selectobjs )
    {
        qDebug() << obj.toString();
    }
}

/** Return whether the SelectID 'obj' is selected. */
bool GLCanvas::selected(const SelectID &obj) const
{
    return obj.selected();
}

/** Resize the display */
void GLCanvas::resizeGL(int w, int h)
{
    cam->setSize(QSize(w,h));
}

/** Return the list of selected objects */
const SelectList& GLCanvas::selected() const
{
    return selectobjs;
}

/** Return the currently highlighted object */
const SelectID& GLCanvas::highlighted() const
{
    return curitem;
}

/** Switch over to translation mode */
void GLCanvas::setTranslationMode()
{
    current_interpreter.setTranslationMode();
}

/** Switch over to rotation mode */
void GLCanvas::setRotationMode()
{
    current_interpreter.setRotationMode();
}

/** Center the view on the currently selected items */
void GLCanvas::centerSelection()
{
    int n = selectobjs.count();
    if (n == 0)
        return;
        
    //get the center of the selected items...
    int nats = 0;
    Vector minvec,maxvec;
    
    for (int i=0; i<n; ++i)
    {
        SelectID item = selectobjs[i];
        const MolSelectID *atm = item.atomSelection();
            
        if (atm)
        {
            nats++;
            if (nats == 1)
            {
                minvec = atm->atom();
                maxvec = atm->atom();
            }
            else
            {
                minvec.setMin(atm->atom());
                maxvec.setMax(atm->atom());
            }
        }
    }
    
    if (nats > 0)
    {
        this->center( 0.5*(minvec+maxvec) );
    }
}

/** Center the selection on the set of Atoms */
void GLCanvas::center(const AtomSet &atoms)
{
    if (atoms.count() == 0)
        return;
        
    Vector minvec = atoms[0];
    Vector maxvec = minvec;
    
    for (int i=1; i<atoms.count(); ++i)
    {
        const Atom &atom = atoms[i];
    
        minvec.setMin(atom);
        maxvec.setMax(atom);
    }
    
    this->center( 0.5*(minvec+maxvec) );
}

/** Executed whenever the viewer recieves a paint event */
void GLCanvas::paintEvent(QPaintEvent *)
{
    needrepaint = true;
    updateGL();
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::mousePressEvent(QMouseEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->mousePressEvent(e);
        if (e->isAccepted())
            return;
    }

    current_interpreter.mousePressEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->mouseReleaseEvent(e);
        if (e->isAccepted())
            return;
    }
    
    current_interpreter.mouseReleaseEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::mouseMoveEvent(QMouseEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->mouseMoveEvent(e);
        if (e->isAccepted())
            return;
    }
    
    current_interpreter.mouseMoveEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::mouseClickEvent(QMouseEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->mouseClickEvent(e);
        if (e->isAccepted())
            return;
    }

    current_interpreter.mouseClickEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->mouseDoubleClickEvent(e);
        if (e->isAccepted())
            return;
    }

    current_interpreter.mouseDoubleClickEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::wheelEvent(QWheelEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->wheelEvent(e);
        if (e->isAccepted())
            return;
    }

    current_interpreter.wheelEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::keyPressEvent(QKeyEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->keyPressEvent(e);
        if (e->isAccepted())
            return;
    }

    current_interpreter.keyPressEvent(e);
}

/** Interpret this type of event using the current input interpreter */
void GLCanvas::keyReleaseEvent(QKeyEvent *e)
{
    if (!e)
        return;

    e->ignore();

    if (canvasui)
    {
        canvasui->keyReleaseEvent(e);
        if (e->isAccepted())
            return;
    }

    current_interpreter.keyReleaseEvent(e);
}
