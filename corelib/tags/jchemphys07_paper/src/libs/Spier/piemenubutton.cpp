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

// #include "piemenubutton.h"
// #include "piemenu.h"
// 
// #include "SireError/exceptions.h"
// #include "SireStream/txtstream.h"
// 
// using namespace Spier;
// using namespace SireStream;
// 
// PieMenuButton::PieMenuButton(PieMenu *parent, QAction *action) 
//               : GLWidget(parent), isfocussed(false)
// {
//     if (not parent)
//         throw SireError::nullptr_error(QObject::tr(
//               "Cannot create a PieMenuButton with a null PieMenu parent!"), CODELOC);
// 
//     save the action
//     actn = action;
// 
//     save the pointer to the parent
//     menuparent = parent;
// 
//     this->setSize(QSize(300,300));
//     this->setCenter(QPoint(500,500));
//     
//     strtang = Angle::degrees(10.0);
//     piesize = Angle::degrees(-90.0);
// }
// 
// PieMenuButton::~PieMenuButton()
// {}
// 
// /** Paint this pie slice */
// void PieMenuButton::paint( QPainter &painter )
// {
//     menuparent->paint(painter, this);
// }
// 
// void PieMenuButton::setSlice(const Angle &strt, const Angle &size)
// {
//     strtang = strt.small();  // strtang will lie between -180 < strtang <= 180
//     piesize = size;
// }
// 
// bool PieMenuButton::contains( const QPoint &point ) const
// {
//     if (not GLWidget::contains(point))
//         return false;
// 
//     is the point contained within the ellipse?
//     If so (x-x0)^2/w^2 +(y-y0)^2/h^2 <= 1
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
// GLWidget* PieMenuButton::widgetAt(const QPoint &point)
// {
//     if (this->contains(point))
//         return this;
//     else
//         return 0;
// }
// 
// /** The focus has entered this button */
// void PieMenuButton::focusInEvent(QFocusEvent*)
// {
//     isfocussed = true;
//     this->update();
// }
// 
// /** The focus has left this button */
// void PieMenuButton::focusOutEvent(QFocusEvent*)
// {
//     isfocussed = false;
//     this->update();
// }
// 
// /** Clicked in the button! */
// void PieMenuButton::mouseClickEvent(QMouseEvent*)
// {
//     if (actn)
//         actn->trigger();
// }
