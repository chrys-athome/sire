#ifndef SPIER_VIEWER_H
#define SPIER_VIEWER_H

#include <QGLWidget>

#include <QList>

#include "viewobj.h"
#include "materialmgr.h"
#include "meshmgr.h"
#include "glcanvas.h"
#include "mouseclickhelper.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMol::EditMol;

/**
This class is the Viewer, in which EditMols are viewed!
 
@author Christopher Woods
*/
class SPIER_EXPORT Viewer : public QGLWidget
{

Q_OBJECT

public:
    Viewer(QWidget *parent=0);
    ~Viewer();

    MaterialMgr& materialLibrary();
    MeshMgr& meshLibrary();

    int nbuffers() const;

    GLCanvas& canvas();
    const GLCanvas& canvas() const;

public slots:
    
    void setBuffer(int i);
    void nextBuffer();
    void prevBuffer();
    
    void display(const EditMol &mol);
    void clear();
    
    void setTranslationMode();
    void setRotationMode();
    void centerSelection();

signals:
    void viewingBuffer(int);

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    void paintEvent(QPaintEvent *e);

    /** The library of materials that are used in this viewer */
    MaterialMgr matlib;
    
    /** The library of meshes that are used in this viewer */
    MeshMgr meshlib;
    
    /** A helper object that is used to spot mouse click events */
    MouseClickHelper clickhelper;
    
    /** The current canvas controlling the rendering of this widget */
    GLCanvas *cnvs;
    
    /** The list of available buffers */
    QList<GLCanvas*> buffs;
    
    /** The id of the currently viewed buffer */
    int buffid;
};

/** Return a reference to the material library */
inline MaterialMgr& Viewer::materialLibrary()
{
    return matlib;
}

/** Return a reference to the mesh library */
inline MeshMgr& Viewer::meshLibrary()
{
    return meshlib;
}

/** Return a reference to the current canvas being displayed */
inline GLCanvas& Viewer::canvas()
{
    return *cnvs;
}

/** Return a const reference to the current canvas being displayed */
inline const GLCanvas& Viewer::canvas() const
{
    return *cnvs;
}

}

SIRE_END_HEADER

#endif
