#ifndef SPIER_CANVASUI_H
#define SPIER_CANVASUI_H

#include <QObject>
#include <QPainter>
#include <QFont>
#include <QPointer>

#include <QWidget>
#include <QList>
#include <QImage>
#include <QTime>

#include "canvasuiptr.h"
#include "glwidget.h"
#include "glwidgetset.h"

#include "textlabel.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLCanvas;
class WidgetAnimator;
class SelectID;
typedef QList<SelectID> SelectList;

/**
This is a user-interface which is drawn on the GLCanvas itself, via a QPainter that can paint to openGL. This class is used to paint the user interface to the canvas, and to manage the sub-widgets.
 
@author Christopher Woods
*/
class SPIER_EXPORT CanvasUI : public QObject
{

Q_OBJECT

public:
    CanvasUI(QObject *parent=0);
    virtual ~CanvasUI();

    virtual void setCanvas(GLCanvas *canvas);

    bool needRepaint() const;

    virtual void paint();

    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseClickEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    
    bool isValid() const;
    
    const QFont& labelFont() const;
    
    void addWidget(GLWidget widget);
    void addWidgets(GLWidgetSet widgets);
    
public slots:

    void update();
    void changeLabelFont();
    void setLabelFont(const QFont &font);
    
    void itemHighlighted(const SelectID &item);
    void selectionChanged(const SelectList &selected);
    
protected:

    void timerEvent(QTimerEvent *e);

    QPainter& canvasPainter();

    GLCanvas* canvas();
    const GLCanvas* canvas() const;

private:

    /** Pointer to the canvas on which this UI is drawn */
    GLCanvas *cnvs;
    
    /** The currently focussed widget */
    GLWidget focuswidget;
    
    /** The font used to draw labels */
    QFont label_font;
    
    /** The set of widgets on this canvas */
    GLWidgetSet widgts;
    
    /** Whether or not the UI needs repainting */
    bool needrepaint;
    
    /** The list of animators */
    QList<WidgetAnimator*> anims;
    
    /** The text label that holds the information about the currently
        highlighted atom */
    TextLabel hlightlabel;

    /** The text label that holds information about the currently 
        selected items */
    TextLabel selectlabel;
};

/** Return whether this UI is valid (i.e. it has a canvas to draw on) */
inline bool CanvasUI::isValid() const
{
    return cnvs;
}

/** Return a pointer to the canvas on which this UI is being drawn. This pointer 
    is guaranteed to not be null, and should definitely not be deleted! */
inline GLCanvas* CanvasUI::canvas()
{
    return cnvs;
}

/** Return a pointer to the canvas on which this UI is being drawn. This pointer 
    is guaranteed to not be null, and should definitely not be deleted! */
inline const GLCanvas* CanvasUI::canvas() const
{
    return cnvs;
}

/** Whether or not the canvas needs to be repainted */
inline bool CanvasUI::needRepaint() const
{
    return needrepaint;
}

}

SIRE_END_HEADER

#endif
