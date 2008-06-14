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

#include <QFontDialog>
#include <QApplication>
#include <QAction>

#include "canvasui.h"
#include "glcanvas.h"
#include "viewer.h"
#include "glwidgetbase.h"
#include "widgetanimator.h"
#include "textlabel.h"
#include "molselectid.h"

#include "SireUnits/units.h"
#include "SireMaths/angle.h"

#include "SireBase/mutablesetiterator.hpp"

#include "SireError/errors.h"

using namespace Spier;
using namespace SireBase;

CanvasUI::CanvasUI(QObject *parent) 
         : QObject(parent), cnvs(0), label_font("Courier", 10, QFont::Bold), needrepaint(true)
{
    hlightlabel = TextLabel::create();
    hlightlabel->setPosition(15,15);
    hlightlabel->hide();
    
    this->addWidget(hlightlabel);

    selectlabel = TextLabel::create();
    selectlabel->setAlignment(Qt::AlignJustify);
    selectlabel->setPosition(15,50);
    selectlabel->hide();
    
    this->addWidget(selectlabel);
}

CanvasUI::~CanvasUI()
{}

/** Repaint the contents of the UI. */
void CanvasUI::update()
{
    //can only update if we have a canvas, and it is not 
    //in the process of being repainted
    if (cnvs and not cnvs->isRepainting())
    {
        needrepaint = true;
        cnvs->updateGL();
    }
}

/** Return the painter used to draw this UI. This will be undefined if 
    this UI is not associated with a canvas (isValid() returns false) */
QPainter& CanvasUI::canvasPainter()
{
    BOOST_ASSERT(cnvs != 0);
    
    return cnvs->painter();
}

/** Add the set of widgets to this canvasui */
void CanvasUI::addWidgets(GLWidgetSet widgets)
{
    int n = widgets.count();
    for (int i=0; i<n; ++i)
        this->addWidget(widgets.at(i));
}

/** Add a widget to this canvasui */
void CanvasUI::addWidget(GLWidget widget)
{
    widgts.insert(widget);
    
    if (widget)
        connect(widget,SIGNAL(needsUpdate()),this,SLOT(update()));
}

/** Set the canvas on which this UI will be drawn */
void CanvasUI::setCanvas(GLCanvas *canvas)
{
    //do nothing if there is no change in canvas
    if (canvas == cnvs)
        return;

    //tell the old canvas that it has lost its UI
    if (cnvs)
        cnvs->setUI( CanvasUIPtr() );

    cnvs = canvas;
    
    if (cnvs)
    {
        //reparent this QObject
        this->setParent( cnvs->parent() );
        cnvs->setUI( CanvasUIPtr(this) );
    }
    else
    {
        //this UI has no parent - should it be deleted...
        this->setParent(0);
    }
}

/** Paint this UI on the canvas */
void CanvasUI::paint()
{
    if (not isValid())
        return;

    QPainter &painter = this->canvasPainter();
    
    //draw all of the widgets in the UI
    MutableSetIterator<GLWidgetBase*,GLWidget> it(widgts);
    
    while(it.hasNext())
    {
        it.next();
        it.value()->paint(painter);
    }
    
    needrepaint = false;
}

/** Slot called whenever an item is highlighted on the canvas */
void CanvasUI::itemHighlighted(const SelectID &item)
{
    if (item.isNull())
        hlightlabel->hide();
    else
    {
        hlightlabel->setText(item.toString());
        hlightlabel->show();
    }
}

/** Slot called whenever the list of selected objects on the canvas changes */
void CanvasUI::selectionChanged(const SelectList &selected)
{
    if (selected.isEmpty())
        selectlabel->hide();
    else
    {
        QString labeltext;
        if (selected.count() == 1)
            labeltext = tr("Selected a single item.\n");
        else
            labeltext = tr("Select %1 items.\n").arg(selected.count());
    
        QList<Atom> atoms;
        QList<int> idxs;
    
        for (int i=0; i<selected.count(); ++i)
        {
            labeltext += QString("%1: %2\n").arg(i+1).arg(selected[i].toString());
            
            const MolSelectID *atomselect = selected[i].atomSelection();
            if (atomselect)
            {
                atoms.append(atomselect->atom());
                idxs.append(i+1);
            }
        }
     
        if (atoms.count() == 2)
        {
            //calculate the interatomic distance and add that to the info
            double dist = Vector::distance(atoms[0], atoms[1]);
            labeltext += tr("Distance %1(%2)-%3(%4) = %5 A\n")
                          .arg(atoms[0].name()).arg(idxs[0])
                          .arg(atoms[1].name()).arg(idxs[1])
                          .arg(dist);
        }
        else if (atoms.count() == 3)
        {
            //calculate the interatomic angle and add that to the info
            SireMaths::Angle ang = Vector::angle(atoms[0], atoms[1], atoms[2]);
            labeltext += tr("Angle %1(%2)-%3(%4)-%5(%6) = %7 degrees\n")
                          .arg(atoms[0].name()).arg(idxs[0])
                          .arg(atoms[1].name()).arg(idxs[1])
                          .arg(atoms[2].name()).arg(idxs[2])
                          .arg(ang.toDegrees());
        }
        else if (atoms.count() == 4)
        {
            //calculate the interatomic dihedral and add that to the info
            SireMaths::Angle dih = Vector::dihedral(atoms[0],atoms[1],atoms[2],atoms[3]);
            labeltext += tr("Dihedral %1(%2)-%3(%4)-%5(%6)-%7(%8) = %9 degrees\n")
                          .arg(atoms[0].name()).arg(idxs[0])
                          .arg(atoms[1].name()).arg(idxs[1])
                          .arg(atoms[2].name()).arg(idxs[2])
                          .arg(atoms[3].name()).arg(idxs[3])
                          .arg(dih.toDegrees());
        }
     
        selectlabel->setText(labeltext);
     
        QSize sz = cnvs->parent()->size();
     
        selectlabel->setPosition(sz.width()-selectlabel->width()-10, 
                                 sz.height()-selectlabel->height()-10);
     
        selectlabel->show();
    }
}

/** Set the font used to draw labels */
void CanvasUI::setLabelFont(const QFont &font)
{
    label_font = font;
}

/** Open a dialog to change the label font */
void CanvasUI::changeLabelFont()
{
    //change the font
    bool ok;
    QFont newfont = QFontDialog::getFont(&ok, label_font, cnvs->parent());
    if (ok)
        label_font = newfont;
}

/** Update the currently focussed widget based on the position of the mouse
    (focus follows mouse) */
void CanvasUI::mouseMoveEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    //only change the focus if no mouse buttons are pressed
    if (e->buttons() == Qt::NoButton)
    {
        //find the widget under the mouse...
        GLWidget findwidget;
    
        MutableSetIterator<GLWidgetBase*,GLWidget> it(widgts);
        while (it.hasNext() )
        {
            it.next();
        
            findwidget = it.value()->widgetAt(e->pos());
          
            if (findwidget)
                break;
        }
    
        if (findwidget != focuswidget)
        {
            //there has been a change of widget focus...
            if (focuswidget.isValid())
            {
                QFocusEvent f(QEvent::FocusOut, Qt::MouseFocusReason);
                focuswidget->focusOutEvent( &f );
            }
        
            focuswidget = findwidget;
        
            if (focuswidget.isValid())
            {
                QFocusEvent f(QEvent::FocusIn, Qt::MouseFocusReason);
                focuswidget->focusInEvent( &f );
            }
        }
    
        if (findwidget.isValid())
            e->accept();
    }
}

/** Send the mouse press event to the widget under the mouse */
void CanvasUI::mousePressEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    mouseMoveEvent(e);
    
    if (focuswidget)
    {
        e->accept();
        focuswidget->mousePressEvent(e);
    }
        
}

void CanvasUI::mouseReleaseEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    mouseMoveEvent(e);
    
    if (focuswidget)
    {
        e->accept();
        focuswidget->mouseReleaseEvent(e);
    }
       
}

void CanvasUI::mouseClickEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    mouseMoveEvent(e);
    
    if (focuswidget)
    {
        e->accept();
        focuswidget->mouseClickEvent(e);
    }
}

void CanvasUI::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    mouseMoveEvent(e);
    
    if (focuswidget)
    {
        e->accept();
        focuswidget->mouseDoubleClickEvent(e);
    }
}

void CanvasUI::wheelEvent(QWheelEvent *e)
{
    if (!e)
        return;
        
    if (focuswidget)
    {
        e->accept();
        focuswidget->wheelEvent(e);
    }
}

void CanvasUI::keyPressEvent(QKeyEvent *e)
{
    if (!e)
        return;

    if (focuswidget)
    {
        e->accept();
        focuswidget->keyPressEvent(e);
    }
}

void CanvasUI::keyReleaseEvent(QKeyEvent *e)
{
    if (!e)
        return;

    if (focuswidget)
    {
        e->accept();
        focuswidget->keyReleaseEvent(e);
    }
}

void CanvasUI::timerEvent(QTimerEvent*)
{
    if (anims.isEmpty())
        return;
        
    int nanims = anims.count();
    for (int i=0; i<nanims; ++i)
        anims[i]->animate();

    this->update();
}
