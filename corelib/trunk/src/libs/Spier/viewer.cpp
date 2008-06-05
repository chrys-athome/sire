
#include "ThirdPartyGUI/glew.h"

#include "viewer.h"
#include "canvasui.h"

#include <QMouseEvent>
#include <QPoint>
#include <QThread>
#include <QDebug>

using namespace Spier;
using namespace SireStream;
using namespace SireMol;

/** Small function to return the QGLFormat required by the Viewer */
QGLFormat getViewerFormat()
{
    QGLFormat format;
    
    //we want a 32bit double-buffered display with a depth buffer,
    //stencil buffer, alpha channel, uses direct rendering,
    //no overlay plane, and has a multisampling (for possible antialiasing)
    
    format.setDepth(true);
    format.setDoubleBuffer(true);
    format.setAlpha(true);
    format.setStencil(true);
    format.setDirectRendering(true);
    format.setOverlay(false);
    format.setSampleBuffers(true);
    format.setRgba(true);
    
    return format;
}

/** Construct the viewer widget */
Viewer::Viewer(QWidget *parent) 
       : QGLWidget(getViewerFormat(), parent)
{
    window()->setWindowTitle(QObject::tr("Spier Molecular Viewer"));
    this->setAutoBufferSwap(false);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
    
    //create four buffers
    for (int i=0; i<4; i++)
    {
        cnvs = new GLCanvas(this);
        //create a new CanvasUI which presents a mini-ui within the GLCanvas. 
        cnvs->setUI( CanvasUIPtr( new CanvasUI() ) );
    
        //append this canvas onto the list of buffers
        buffs.append(cnvs);
    }
    
    //by default, we will use buffer 1 (the first buffer)
    buffid = 1;
    cnvs = buffs[0];
}

/** Destructor */
Viewer::~Viewer()
{
    for (int i=0; i<buffs.count(); ++i)
        delete buffs[i];
        
    buffs.clear();
}

/** Display the molecule pointed to by 'molptr' in the current buffer */
void Viewer::display(const EditMol &molptr)
{
    cnvs->display(molptr);
}

/** View buffer 'i' - this does nothing if we are already viewing 
    this buffer, or if there is no such buffer. Note that 'i' begins counting from 1 */
void Viewer::setBuffer(int i)
{
    if (buffid == i or i <= 0 or i > buffs.count())
        return;
        
    buffid = i;
    cnvs = buffs[buffid-1];
    cnvs->needrepaint = true;
    cnvs->updateGL();
    
    emit viewingBuffer(buffid);
}
    
/** Return the number of buffers */
int Viewer::nbuffers() const
{
    return buffs.count();
}
    
/** View the next buffer */
void Viewer::nextBuffer()
{
    int newbuffer = buffid + 1;
    if (newbuffer > buffs.count())
        newbuffer = 1;
        
    this->setBuffer(newbuffer);
}

/** View the previous buffer */
void Viewer::prevBuffer()
{
    int newbuffer = buffid - 1;
    if (newbuffer <= 0)
        newbuffer = buffs.count();
        
    this->setBuffer(newbuffer);
}

/** Completely clear the current buffer */
void Viewer::clear()
{
    cnvs->reset();
    cnvs->updateGL();
}

/** Initialise the openGL context for this widget */
void Viewer::initializeGL()
{
    //see if how well the display matches what we want...
    QGLFormat fmt = this->format();
    
    if (fmt.alpha())
        qDebug() << QObject::tr("Alpha buffer available (size = %1)")
                                          .arg(fmt.alphaBufferSize());
    
    if (fmt.depth())
        qDebug() << QObject::tr("Depth buffer available (size = %1)")
                                                  .arg(fmt.depthBufferSize());
    
    if (fmt.stencil())
        qDebug() << QObject::tr("Stencil buffer available (size = %1)")
                                          .arg(fmt.stencilBufferSize());
        
    if (fmt.directRendering())
        qDebug() << QObject::tr("Using direct openGL rendering");
        
    if (fmt.doubleBuffer())
        qDebug() << QObject::tr("Using double buffering");
    else
        throw SireError::unsupported(QObject::tr(
            "The viewer needs to be able to create a double-buffered display. This does not "
            "appear to be supported on this platform."), CODELOC);
            
    if (fmt.hasOverlay())
        qDebug() << QObject::tr("Using an overlay despite a request not to!");
        
    if (fmt.sampleBuffers())
        qDebug() << QObject::tr("openGL antialiasing sample buffers are available! "
                                    "Number of samples = %1").arg(fmt.samples());
                                    
    if (fmt.rgba())
        qDebug() << QObject::tr("Using RGBA colour mode");
    else
        throw SireError::unsupported(QObject::tr(
            "This viewer needs to be run in RGBA colour mode, which does not appear to be "
            "supported on this platform."), CODELOC);

    GLint naux;
    glGetIntegerv(GL_AUX_BUFFERS, &naux);
    qDebug() << QObject::tr("There are %1 auxillary buffers.").arg(naux);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &naux);
    qDebug() << QObject::tr("Maximum texture size = %1x%2 pixels").arg(naux).arg(naux);

    //initialise the GLEW openGL extension manager
    GLenum err = glewInit();
    
    if (err != GLEW_OK)
    {
        qDebug() << QObject::tr("Error initalising the GLEW extension library!");
        qDebug() << QObject::tr("Error = \"%1\"").arg((const char*)glewGetErrorString(err));
    }

    cnvs->initialize();
}

/** Switch all canvases over to translation mode */
void Viewer::setTranslationMode()
{
    for (int i=0; i<buffs.count(); ++i)
        buffs[i]->setTranslationMode();
}

/** Switch all canvases over to rotation mode */
void Viewer::setRotationMode()
{
    for (int i=0; i<buffs.count(); ++i)
        buffs[i]->setRotationMode();
}

/** Center the current canvas on its current selection */
void Viewer::centerSelection()
{
    cnvs->centerSelection();
}

/** Paint the widget (render the scene) */
void Viewer::paintGL()
{
    cnvs->paintGL();
}

/** Resize the display */
void Viewer::resizeGL(int w, int h)
{
    //we need to resize all of the buffers
    for (int i=0; i<buffs.count(); ++i)
        buffs[i]->resizeGL(w,h);

    cnvs->updateGL();
}

/** Executed whenever a key is pressed when the mouse is in this viewer */
void Viewer::keyPressEvent(QKeyEvent *e)
{
    cnvs->keyPressEvent(e);
}

/** Executed whenever a key is pressed when the mouse is not in this viewer */
void Viewer::keyReleaseEvent(QKeyEvent *e)
{
    cnvs->keyReleaseEvent(e);
}

/** Executed when the mouse button is pressed */
void Viewer::mousePressEvent(QMouseEvent *e)
{
    clickhelper.press(e);
    cnvs->mousePressEvent(e);
}

/** Executed when the mouse button is released */
void Viewer::mouseReleaseEvent(QMouseEvent *e)
{
    QMouseEvent *clickevent = clickhelper.release(e);
    
    if (clickevent)
        cnvs->mouseClickEvent(e);
        
    cnvs->mouseReleaseEvent(e);
}

/** Executed when the mouse button is double-clicked */
void Viewer::mouseDoubleClickEvent(QMouseEvent *e)
{
    cnvs->mouseDoubleClickEvent(e);
}

/** Executed when the mouse is moved */
void Viewer::mouseMoveEvent(QMouseEvent *e)
{
    if (not clickhelper.move(e))
        cnvs->mouseMoveEvent(e);
}

/** Executed on mouse wheel events */
void Viewer::wheelEvent(QWheelEvent *e)
{
    cnvs->wheelEvent(e);
}

/** Executed whenever the viewer recieves a paint event */
void Viewer::paintEvent(QPaintEvent *e)
{
    cnvs->paintEvent(e);
}
