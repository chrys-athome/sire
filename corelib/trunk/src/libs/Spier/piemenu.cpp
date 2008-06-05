
// #include "piemenu.h"
// #include "piemenubutton.h"
// 
// #include "SireUnits/units.h"
// #include "SireStream/txtstream.h"
// 
// using namespace Spier;
// using namespace SireStream;
// 
// PieMenu::PieMenu(QObject *parent) : GLWidget(parent)
// {
//     strtang = -135.0 * SireUnits::degrees;
//     piesize = 270.0 * SireUnits::degrees;
// }
// 
// PieMenu::~PieMenu()
// {}
// 
// /** Set the canvasui on which this menu is drawn - this sets the canvas for all 
//     child buttons */
// void PieMenu::setCanvasUI(CanvasUI *canvasui)
// {
//     int nbttns = bttns.count();
//     for (int i=0; i<nbttns; ++i)
//         bttns[i]->setCanvasUI(canvasui);
//        
//     GLWidget::setCanvasUI(canvasui);
// }
// 
// /** Add a button to the menu */
// void PieMenu::add(PieMenuButton *button)
// {
//     if (button)
//     {
//         bttns.append( PieMenuButtonPtr(button) ); 
//         button->setCanvasUI(cnvsui);
//         arrangeButtons();
//     }
// }
// 
// /** Add a button to the menu, associated with ID 'id' */
// void PieMenu::add(PieMenuButton *button, int id)
// {
//     buttonhash.insert(button,id);
//     this->add(button);
// }
//     
// /** Add an action to this menu */
// void PieMenu::add(QAction *action)
// {
//     if (action)
//     {
//         PieMenuButton *button = new PieMenuButton(this, action);
//         this->add(button);
//     }
// }
// 
// /** Return the button with the ID = id (or 0 if there is no such button) */
// PieMenuButton* PieMenu::button(int id) const
// {
//     QHashIterator<PieMenuButton*,int> it(buttonhash);
//     
//     while( it.hasNext() )
//     {
//         it.next();
//         
//         if (it.value() == id)
//             return it.key();
//     }
//     
//     return 0;
// }
//     
// /** Clear all buttons from this menu */
// void PieMenu::clear()
// {
//     bttns.clear();
//     buttonhash.clear();
// }
//     
// /** Return the ID of the button, or -1 if no such button exists */
// int PieMenu::id(PieMenuButton *button) const
// {
//     if (buttonhash.contains(button))
//         return buttonhash[button];
//     else
//         return -1;
// }
// 
// /** Return a list of all of the buttons in this menu */
// QList<PieMenuButtonPtr> PieMenu::buttons() const
// {
//     return bttns;
// }
// 
// /** Set the position of the center of this menu */
// void PieMenu::setCenter(const QPoint &center)
// {
//     int nbttns = bttns.count();
//     for (int i=0; i<nbttns; ++i)
//         bttns[i]->setCenter(center);
//         
//     GLWidget::setCenter(center);
// }
// 
// /** Set the size of this menu */
// void PieMenu::setSize(const QSize &size)
// {
//     GLWidget::setSize(size);
//     this->arrangeButtons();
// }
// 
// /** Paint the menu */
// void PieMenu::paint(QPainter &painter)
// {
//     draw a nice menu background?
//     
//     draw all of the buttons
//     int nbttns = bttns.count();
//     for (int i=0; i<nbttns; ++i)
//     {
//         PieMenuButtonPtr button = bttns.at(i);
//         if (button)
//             this->paint(painter, button);
//     }
// }
// 
// /** Check that the buttons are all valid (not null) */
// void PieMenu::checkButtonsValid()
// {
//     QMutableListIterator<PieMenuButtonPtr> it(bttns);
//     
//     bool chg = false;
//     
//     while (it.hasNext())
//     {
//         PieMenuButtonPtr ptr = it.next();
//         
//         if (ptr.isNull())
//         {
//             it.remove();
//             chg = true;
//         }
//     }
// }
// 
// /** Calculate and set the text rectangle for the pie menu slice */
// void PieMenu::calculateTextRect(PieMenuButton *slice)
// {
//     if (not slice)
//         return;
//         
//     calculate the center of the slice
//     double a = boundrect.width() * 0.5;
//     double b = boundrect.height() * 0.5;
//     
//     double tan_t = std::tan( slice->strtang + 0.5*slice->piesize );
//     double q = std::atan( 0.5*a*tan_t / (0.5*b) );
//     
//     QPointF cent( a * std::sin(q) , -b * std::cos(q) );
//     
//     QRect textrect;
//     textrect.setTopLeft( QPoint(cent.x()-50, cent.y()-25) );
//     textrect.setBottomRight( QPoint(cent.x()+50, cent.y()+25) );
//     
//     textrect.translate(boundrect.center() + QPoint(textrect.width()/2, textrect.height()/2));
//     
//     slice->buttonrect = textrect;
// }
// 
// /** Arrange the buttons within the limits of this menu */
// void PieMenu::arrangeButtons()
// {
//     checkButtonsValid();
//     
//     int nbttns = bttns.count();
//     
//     if (nbttns == 0)
//         return;
//     else if (nbttns == 1)
//     {
//         PieMenuButton *button = bttns[0];
//         
//         button->setRect(boundrect);
//         button->setSlice( strtang, piesize );
//         calculateTextRect(button);
//     }
//     else
//     {
//         get the size of each pie slice
//         Angle sz = piesize / nbttns;
//   
//         we will put a 5 degree buffer either side of the slice
//         Angle buffer = 5.0 * SireUnits::degrees;
//   
//         for (int i=0; i<nbttns; ++i)
//         {
//             PieMenuButton *button = bttns[i];
//             button->setRect(boundrect);
//             button->setSlice(strtang + 0.5*buffer + i*sz, sz - buffer);
//             calculateTextRect(button);
//         }
//     }
//     
//     set the color gradients
//     QRadialGradient grad(boundrect.center(), 0.3*(boundrect.height()+boundrect.width()));
//     grad.setColorAt(0.0, QColor(100, 0, 0, 150));
//     grad.setColorAt(1.0, QColor(255, 255, 255, 75));
//     
//     normalbrush = QBrush(grad);
//     
//     grad.setColorAt(0.0, QColor(255, 0, 0, 255));
//     grad.setColorAt(1.0, QColor(255, 255, 255, 100));
//     
//     focusbrush = QBrush(grad);
//     
//     normalpen = QPen(Qt::NoPen);
//     focuspen = QPen( QColor(0, 0, 0, 100) );
// }
// 
// /** Return whether or not the point 'point' is within the pie menu */
// bool PieMenu::contains(const QPoint &point) const
// {
//     if (not GLWidget::contains(point))
//         return false;
// 
//     is the point contained within the ellipse?
//     If so (x-x0)^2/w^2 +(y-y0)^2/h^2 <= 1
//     return true;
//     
//     get the center of the ellipse
//     QPoint cent = boundrect.center();
//     
//     get the vector from the center to the point
//     QPoint del = point - cent;
// 
//     if ( SireMaths::pow_2(del.x())/SireMaths::pow_2(0.5*boundrect.width()) + 
//          SireMaths::pow_2(del.y())/SireMaths::pow_2(0.5*boundrect.height()) > 1.0)
//         return false;
// 
//     ok - the point is definitely within the ellipse. Now calculate the angle
//     between the point and the y axis, and see if this lies within the pie slice
//     Angle ang(del.x(), -del.y());
//     
//     this angle will lie between -180 < ang <= 180 
//     Get the difference between this angle and strtang (which also lies in this range)
//     Angle diff = strtang.smallDifference(ang);
//     
//     Angle zero(0.0);
//     
//     return (piesize > zero and diff > zero and diff <= piesize)  or
//            (piesize < zero and diff < zero and diff >= piesize);
// }
// 
// /** Return the widget that contains the point 'point', or no widget if there 
//     is no such widget */
// GLWidget* PieMenu::widgetAt(const QPoint &point)
// {
//     if (not this->contains(point))
//         return 0;
//         
//     loop over all of the buttons, and find the one that holds this point..
//     int nbttns = bttns.count();
//     for (int i=0; i<nbttns; ++i)
//     {
//         PieMenuButtonPtr ptr = bttns.at(i);
//         if (ptr)
//         {
//             GLWidget *widget = ptr->widgetAt(point);
//             if (widget)
//                 return widget;
//         }
//     }
//     
//     return 0;
// }
// 
// /** Paint the specified pie slice - we do it here as the PieMenu determines
//     the look and feel of the pie slice */
// void PieMenu::paint( QPainter &painter, PieMenuButton *slice )
// {
//     if (not slice)
//         return;
// 
//     if (slice->focussed())
//     {
//         painter.setBrush(focusbrush);
//         painter.setPen(focuspen);
//     }
//     else
//     {
//         painter.setBrush(normalbrush);
//         painter.setPen(normalpen);
//     }
// 
//     if (slice->piesize.isZero())
//         draw a circle/ellipse
//         painter.drawEllipse(slice->boundrect);
//     else
//     {
//         convert the angle to Qt units ( 0 = 3 o'clock, positive anti-clockwise )
//         int strt = int(16.0 * (Angle::degrees(90.0) - slice->strtang).toDegrees());
//         int sz = -int(16.0 * slice->piesize.toDegrees());
// 
//         painter.drawPie(slice->boundrect, strt, sz);
//         
//         painter.setPen( QPen(QColor(0,0,0)) );
//         painter.drawText(slice->buttonrect, slice->actn->text());
//     }
// }
