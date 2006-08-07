#ifndef SPIER_BUTTON_H
#define SPIER_BUTTON_H

#include "textlabel.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This is a button (obvious...)
 
@author Christopher Woods
*/
class SPIER_EXPORT ButtonBase : public TextLabelBase
{

friend class Wrapper<ButtonBase>;

Q_OBJECT

public:
    ~ButtonBase();

signals:
    void clicked(bool checked=false);

protected:
    ButtonBase(GLWidget parent=GLWidget());

    void paintWidget(QPainter &painter);

    void mouseClickEvent(QMouseEvent *e);

};

typedef Wrapper<ButtonBase> Button;

}

SIRE_END_HEADER

#endif
