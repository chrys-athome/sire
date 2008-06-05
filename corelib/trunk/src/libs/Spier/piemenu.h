// #ifndef SPIER_PIEMENU_H
// #define SPIER_PIEMENU_H
// 
// #include "glwidget.h"
// 
// #include "SireMaths/angle.h"
// 
// #include <QList>
// #include <QHash>
// #include <QPointer>
// #include <QPen>
// #include <QBrush>
// 
// namespace Spier
// {
// 
// class PieMenuButton;
// 
// typedef QPointer<PieMenuButton> PieMenuButtonPtr;
// 
// using SireMaths::Angle;
// 
// /**
// This collects together a group of PieMenuButtons into a single PieMenu. A PieMenu is a circle (or a part of a circle, or even an ellipse) that is subdivided into options.
//  
// @author Christopher Woods
// */
// class PieMenu : public GLWidget
// {
// 
// Q_OBJECT
// 
// friend class PieMenuButton;
// 
// public:
//     PieMenu(QObject *parent=0);
//     ~PieMenu();
// 
//     void add(QAction *action);
// 
//     void add(PieMenuButton *button);
//     void add(PieMenuButton *button, int id);
//     
//     PieMenuButton* button(int id) const;
//     
//     void clear();
//     
//     int id(PieMenuButton *button) const;
// 
//     QList<PieMenuButtonPtr> buttons() const;
// 
//     void setCenter(const QPoint &center);
//     void setSize(const QSize &size);
// 
//     bool contains(const QPoint &point) const;
//     GLWidget* widgetAt(const QPoint &point);
// 
// signals:
//     void buttonClicked(PieMenuButton *button);
//     void buttonClicked(int id);
// 
// protected:
// 
//     void paint(QPainter &painter);
// 
//     void paint(QPainter &painter, PieMenuButton *slice);
// 
//     void checkButtonsValid();
//     void arrangeButtons();
//     void calculateTextRect(PieMenuButton *slice);
// 
//     void setCanvasUI(CanvasUI *canvasui);
// 
//     /** The list of all of the buttons in this PieMenu */
//     QList<PieMenuButtonPtr> bttns;
//     /** The hash which can map buttons to integers */
//     QHash<PieMenuButton*,int> buttonhash;
// 
//     /** The start angle of the menu */
//     Angle strtang;
//     
//     /** The size of the menu */
//     Angle piesize;
// 
//     /** The brush used to paint all of the menu slices */
//     QBrush normalbrush;
//     QBrush focusbrush;
//     
//     /** The pens used to paint all of the menu slices */
//     QPen normalpen;
//     QPen focuspen;
// 
// };
// 
// }
// 
// #endif
