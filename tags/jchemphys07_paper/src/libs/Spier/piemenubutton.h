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

// #ifndef SPIER_PIEMENUBUTTON_H
// #define SPIER_PIEMENUBUTTON_H
// 
// #include <QPointer>
// #include <QAction>
// #include <QPen>
// #include <QBrush>
// 
// #include "glwidget.h"
// 
// #include "SireMaths/angle.h"
// 
// namespace Spier
// {
// 
// using SireMaths::Angle;
// 
// class PieMenu;
// 
// /**
// This class provides a button within a PieMenu. A PieMenu is a very efficient way to present a series of options to a user, as it allows the options to appear near the user, with a minimum of mouse movement necessary to select them. A PieMenu (properly implemented) can be much more efficient than standard nested menus.
//  
// @author Christopher Woods
// */
// class PieMenuButton : public GLWidget
// {
// 
// friend class PieMenu;
// 
// Q_OBJECT
// 
// public:
//     PieMenuButton(PieMenu *parent, QAction *action);
//     ~PieMenuButton();
// 
//     bool contains(const QPoint &point) const;
// 
//     bool focussed() const;
// 
//     void setSlice(const Angle &strt, const Angle &size);
// 
//     GLWidget* widgetAt(const QPoint &point);
// 
// protected:
// 
//     void paint(QPainter &painter);
// 
//     void focusInEvent(QFocusEvent *e);
//     void focusOutEvent(QFocusEvent *e);
//     
//     void mouseClickEvent(QMouseEvent *e);
//     
//     /** Whether or not this button is focussed */
//     bool isfocussed;
//     
//     /** The action associated with this button. */
//     QPointer<QAction> actn;
// 
//     /** The PieMenu parent */
//     PieMenu *menuparent;
// 
//     /** The starting angle of the pie slice. This has 0 degrees
//         at the 12 o'clock position */
//     Angle strtang;
//     
//     /** The size of the slice - positive angle is clockwise,  
//         while negative angle is anticlockwise */
//     Angle piesize;
// 
//     /** Rectangle in which the button text can be written */
//     QRect buttonrect;
// 
// };
// 
// /** Return whether or not this button is focussed */
// inline bool PieMenuButton::focussed() const
// {
//     return isfocussed;
// }
// 
// }
// 
// #endif
