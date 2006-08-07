
#include "button.h"

#include <QMouseEvent>

using namespace Spier;

ButtonBase::ButtonBase(GLWidget parent) : TextLabelBase(parent)
{}

ButtonBase::~ButtonBase()
{}

/** Paint the button */
void ButtonBase::paintWidget(QPainter &painter)
{
    QBrush b(QColor(100,100,100,100));

    if (isFocussed())
        b = QBrush(QColor(200,200,200,200));

    painter.setBrush(b);
    painter.setPen(QColor(0,0,0,100));
    painter.drawPath(clippath);

    //now draw the text
    if (not txt.isNull())
    {
        painter.setPen(QColor(255,255,255,200));
        painter.setFont(labelfont);
        painter.drawText(labelrect, txt, txtopts);
    }
}

/** Emit a 'clicked' signal if the mouse is clicked within this button */
void ButtonBase::mouseClickEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    if (e->button() == Qt::LeftButton)
    {
        e->accept();
        emit clicked();
    }
}
