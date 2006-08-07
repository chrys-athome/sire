#ifndef SPIER_TEXTLABEL_H
#define SPIER_TEXTLABEL_H

#include "wrapper.hpp"
#include "glwidgetbase.h"

#include <QTextOption>

class QPixmap;
class QPicture;

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This is a label GLWidget, used to display text (:-)
 
@author Christopher Woods
*/
class SPIER_EXPORT TextLabelBase : public GLWidgetBase
{

friend class Wrapper<TextLabelBase>;

Q_OBJECT

public:
    ~TextLabelBase();

    void setAlignment(Qt::Alignment align);
    Qt::Alignment alignment() const;
    
    void setWordWrap(bool on);
    bool wordWrap() const;

    QString text() const;
    QPixmap pixmap() const;
    QPicture picture() const;

public slots:

    void setText(const QString &text);
    void setPicture(const QPicture &picture);
    void setPixmap(const QPixmap &pixmap);

protected:
    TextLabelBase(GLWidget parent = GLWidget());

    void paintWidget(QPainter &painter);
    
    void clearImage();
    
    /** The text displayed in the label */
    QString txt;
    
    /** The font of the label */
    QFont labelfont;
    
    /** Text options */
    QTextOption txtopts;
    
    /** The pixmap that may be displayed in the label */
    QPixmap *pxmap;
    
    /** The picture that may be displayed in the label */
    QPicture *pcture;

    /** The border size */
    qreal bordersize;
    
    /** The rectangle that will contain the text/image */
    QRectF labelrect;
    
    /** The preferred size of the label */
    QSizeF preferredsize;
};

typedef Wrapper<TextLabelBase> TextLabel;

}

SIRE_END_HEADER

#endif
