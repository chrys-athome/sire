#ifndef SPIER_GLCANVAS_H
#define SPIER_GLCANVAS_H

#include <QGLWidget>
#include <QList>
#include <QTime>
#include <QWaitCondition>
#include <QMutex>
#include <QPainter>
#include <QFont>
#include <QObject>

#include "selectid.h"
#include "materialstate.h"
#include "viewobjptr.h"
#include "inputinterpreter.h"
#include "moldisplaydataptr.h"
#include "canvasuiptr.h"

#include "SireMol/editmolset.h"
#include "SireMol/atomset.h"
#include "SireBase/set.hpp"

SIRE_BEGIN_HEADER

namespace Spier
{

class Viewer;
class MaterialMgr;
class MeshMgr;
class Camera;
class MolDisplayData;

using SireMol::EditMol;
using SireMol::EditMolList;
using SireMol::AtomSet;

typedef SireBase::Set<ViewObj*, ViewObjPtr> ViewObjSet;
typedef QList<SelectID> SelectList;

/**
This is an abstraction of an openGL canvas - this is the area within which openGL drawing operations are performed. This allows for multiple GLCanvases to be swapped within a single Viewer, thus allowing fast view switching (e.g. to view different molecules, or to change render modes).
 
@author Christopher Woods
*/
class SPIER_EXPORT GLCanvas : public QObject
{

Q_OBJECT

friend class Viewer;

public:
    GLCanvas(Viewer *parent);
    ~GLCanvas();

    void display(const EditMol &mol);
    void bind(const MaterialPtr &material);

    void pushAttrib();
    void popAttrib();

    MaterialMgr& materialLibrary();
    MeshMgr& meshLibrary();

    void encodeIndex(quint32 index);
    qint32 decodeIndex(int x, int y) const;

    void reset();

    void clear();
    void updateGL();

    bool isRepainting() const;

    void addToDisplay(ViewObjPtr viewobj);
    void removeFromDisplay(const ViewObjPtr &viewobj);

    const QGLContext& context() const;
    QGLFormat format() const;
    
    const Camera& camera() const;
    Camera& camera();

    const SelectID& highlighted() const;
    const SelectList& selected() const;
    
    void addSelected(SelectID obj);
    void setSelected(SelectID obj);
    void deselect(SelectID obj);
    void toggleSelected(SelectID obj);
    void clearSelection();

    bool selected(const SelectID &obj) const;

    void printSelected() const;

    QPainter& painter();

    void setMoveMode(bool on);
    bool moveMode() const;
    
    Viewer* parent();
    const Viewer* parent() const;

    void setUI(const CanvasUIPtr &uiptr);

public slots:
    
    void setTranslationMode();
    void setRotationMode();
    void centerSelection();
    void center();
    void center(const EditMol &mol);
    void center(const EditMolList &mols);
    void center(const AtomSet &atoms);
    void center(const SireMaths::Vector &point);

signals:

    void itemHighlighted(const SelectID &item);
    void selectionChanged(const SelectList &selectobjs);

protected:

    void initialize();
    void paintGL();
    void resizeGL(int w, int h);
    void setCursor(Qt::CursorShape cursor);

    void paintBackground();
    void fillBackBuffer();
    SelectID getObjectUnderMouse();
    void paintUI();

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseClickEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    
    void setSceneState();
    
    void saveRenderState();
    void restoreRenderState();

    void checkError(QString codeloc);
    void clearError();

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    void paintEvent(QPaintEvent *e);

    qint32 decodeColorToIndex(uchar red, uchar green, uchar blue) const;

    void cleanSelectedList();

    /** The parent viewer of this canvas - this cannot change
        during the lifetime of this canvas */
    Viewer *prnt;

    /** The camera used to view the scene */
    Camera *cam;

    /** The current input interpreter */
    InputInterpreter current_interpreter;

    /** The current CanvasUI (user interface) */
    CanvasUIPtr canvasui;

    /** Whether or not the camera view has changed since the last render */
    bool needrepaint;

    /** Whether or not the back buffer contains the selection info */
    bool back_buffer_ready;

    /** The currently highlighted item */
    SelectID curitem;

    /** The list of currently selected items */
    SelectList selectobjs;

    /** Whether or not we are in camera moving mode */
    bool movemode;

    /** The list of molecules that are displayed in this viewer */
    QHash<const EditMol*, MolDisplayDataPtr> mols;

    /** The set of displayed objects */
    ViewObjSet dispobjs;

    /** QPainter used to paint the user interface on the canvas */
    QPainter pntr;

    /** Timer used to slow the rendering down to a maximum of 20 fps */
    QTime rendertimer;

    /** The manager of the material state of the viewer */
    MaterialState matstate;
    
    /** openGL display list containing the state changes used to draw
        the scene */
    GLuint sceneidx;
    
    /** openGL display list containing the background */
    GLuint bgidx;
    
    /** Whether or not the canvas is in the process of being repainted */
    bool isrepainting;
    
    /** Mutex to ensure that rendering is serialised */
    QMutex rendermutex;
    
    /** Wait condition to slow rendering down to 20 fps (50ms render) */
    QWaitCondition renderwait;
};

/** Completely clear the screen - note that this can only be called from within 
    paintGL (or within a function that is called from paintGL) */
inline void GLCanvas::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/** Return the parent Viewer of this canvas */
inline Viewer* GLCanvas::parent()
{
    return prnt;
}

/** Return a reference to the camera currently being used to view the scene */
inline const Camera& GLCanvas::camera() const
{
    return *cam;
}

/** Return a non-const reference to the camera currently being used to view 
    the scene */
inline Camera& GLCanvas::camera()
{
    return *cam;
}

/** Return the parent Viewer of this canvas */
inline const Viewer* GLCanvas::parent() const
{
    return prnt;
}

/** Set whether or not camera moving mode is on. When camera moving mode is
    on the renderer will use shortcuts to improve the rendering speed
    (e.g. selection mode is turned off). This is all to improve the smoothness
    of the user experience when a view is manipulated. */
inline void GLCanvas::setMoveMode(bool on)
{
    movemode = on;
    
    if (movemode)
        this->setCursor(Qt::BlankCursor);
    else
        this->setCursor(Qt::ArrowCursor);
}

/** Return whether or not the canvas is in move mode */
inline bool GLCanvas::moveMode() const
{
    return movemode;
}

/** Return the painter used to draw to the canvas */
inline QPainter& GLCanvas::painter()
{
    return pntr;
}

/** Return whether or not the canvas is in the process of being repainted */
inline bool GLCanvas::isRepainting() const
{
    return isrepainting;
}

}

SIRE_END_HEADER

#endif
