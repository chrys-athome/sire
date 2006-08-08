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
