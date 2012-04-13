/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/GUI/optionswidget.h"
#include "Conspire/GUI/addwidget.h"
#include "Conspire/GUI/optionseditview.h"
#include "Conspire/GUI/optionscommand.h"
#include "Conspire/GUI/mainbar.h"
#include "Conspire/GUI/optionwidget.h"
#include "Conspire/GUI/exceptionwidget.h"

#include <QVBoxLayout>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QUndoStack>
#include <QLabel>
#include <QPushButton>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

using namespace Conspire;

/////////
///////// Implementation of NewOptionsWidget
/////////

/** Constructor */
NewOptionsWidget::NewOptionsWidget(QGraphicsItem *parent) : PageWidget(parent)
{
    build();
}

/** Construct, passing in a top-level options object */
NewOptionsWidget::NewOptionsWidget(Options options, QGraphicsItem *parent)
                 : PageWidget(parent)
{
    build();
    setOptions(options);
}

/** Construct, passing in an Opitons object that is a sub-options object,
    with root key 'root_key' */
NewOptionsWidget::NewOptionsWidget(Options options, QString root_key,
                                   QGraphicsItem *parent)
                 : PageWidget(parent)
{
    build();
    setOptions(options, root_key);
}

/** Destructor */
NewOptionsWidget::~NewOptionsWidget()
{}

/** Return the options object being edited by this view */
Options NewOptionsWidget::options() const
{
    return opts;
}

/** Return the root key of the options object being edited. This is
    null if this view is editing the top-level options object */
QString NewOptionsWidget::rootKey() const
{
    return root_key;
}

/** This function is called when the options object is updated */
void NewOptionsWidget::update(Options options)
{
    if (not root_key.isNull())
    {
        try
        {
            Option opt = options.getNestedOption(root_key);
            options = opt.value().asA<Options>();
        }
        catch(...)
        {
            conspireDebug() << "CANNOT FIND" << root_key << "IN OPTIONS!";
            options = Options();
        }
    }
    
    //create one button for each option...
    opts = options;
    
    QStringList keys = opts.keysAndIndiciesWithValue();
    
    //make sure that there are as many buttons in the layout as options
    QGraphicsLinearLayout *l = dynamic_cast<QGraphicsLinearLayout*>(this->layout());
    
    if (not l)
    {
        conspireDebug() << "INVALID LAYOUT!!!";
        throw Conspire::program_bug( Conspire::tr(
                "How do we have an invalid layout???"), CODELOC );
    }
    
    if (l->count() > keys.count())
    {
        for (int i = l->count()-1; i >= keys.count(); --i)
        {
            QGraphicsLayoutItem *item = l->itemAt(i);
            l->removeAt(i);
            buttons.removeAt(i);
            delete item;
        }
    }
    else if (l->count() < keys.count())
    {
        for (int i = l->count(); i < keys.count(); ++i)
        {
            QPushButton *b = new QPushButton();
            connect(b, SIGNAL(clicked()), mapper, SLOT(map()));
            buttons.append(b);
            QGraphicsProxyWidget *b_proxy = new QGraphicsProxyWidget(this);
            b_proxy->setWidget(b);
            l->insertItem(i, b_proxy);
        }
    }
    
    for (int i=0; i<keys.count(); ++i)
    {
        QAbstractButton *b = buttons[i];
        Option opt = opts.getNestedOption(keys[i]);
        
        if (opt.hasSubOptions())
        {
            b->setText( QString("%1 ----->").arg(keys[i]) );
        }
        else
        {
            b->setText( QString("%1 == %2").arg(keys[i], opt.value().toString()) );
        }
        
        mapper->removeMappings(b);
        mapper->setMapping(b, keys[i]);
    }
}

/** Slot called when one of the options is clicked */
void NewOptionsWidget::clicked(const QString &key)
{
    try
    {
        //create a new OptionsWidget to display the suboptions
        conspireDebug() << "CLICKED" << key;
        Option opt = opts.getNestedOption(key);
        
        if (opt.hasSubOptions())
        {
            QString root = key;
                
            if (not root_key.isNull())
                root.prepend(".").prepend(root_key);

            PagePointer p(new NewOptionsWidget(opt.value().asA<Options>(), root));
            
            emit( push(p) );
        }
        else
        {
            PagePointer p(new OptionWidget(opt, root_key));
            
            emit( push(p) );
        }
    }
    catch(const Conspire::Exception &e)
    {
        conspireDebug() << "EXCEPTION" << e.toString();
        
        PagePointer p(new ExceptionWidget( Conspire::tr(
                "An error occurred when you clicked on the option \"%1\".")
                    .arg(key), e ));
                    
        emit( push(p) );
    }
}

/** Actually build the widget */
void NewOptionsWidget::build()
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout( ::Qt::Vertical, this );
    this->setLayout(l);
    
    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString&)), this, SLOT(clicked(const QString&)));
}

/** Set the options and root_key used by this object */
void NewOptionsWidget::setOptions(Options options, QString key)
{
    root_key = key;
    opts = Options();
    this->update(options);
}

/////////
///////// Implementation of OptionsWidget
/////////

/** Constructor */
OptionsWidget::OptionsWidget(QWidget *parent) : QGraphicsView(parent)
{
    build();
}

/** Construct to edit the passed options */
OptionsWidget::OptionsWidget(Options options, QWidget *parent)
              : QGraphicsView(parent)
{
    build();
    setOptions(options);
}

/** Destructor */
OptionsWidget::~OptionsWidget()
{}

/** Return the current state of the options being edited */
Options OptionsWidget::options() const
{
    return opts;
}

/** Set the options object to be edited */
void OptionsWidget::setOptions(Options options)
{
    try
    {
        if (top_view)
            top_view->update(options);
        else
        {
            top_view = new NewOptionsWidget(options);
            this->scene()->addItem(top_view);

            top_view->setGeometry(0, 0, viewport()->width(), viewport()->height());
            top_view->setZValue(0);
            
            connect(top_view.data(), SIGNAL(push(PagePointer)), 
                    this, SLOT(push(PagePointer)));
            connect(top_view.data(), SIGNAL(pop(bool)), 
                    this, SLOT(pop(bool)));
            connect(top_view.data(), SIGNAL(add(QString)), 
                    this, SLOT(add(QString)));
            connect(top_view.data(), SIGNAL(remove(QString)), 
                    this, SLOT(remove(QString)));
            connect(top_view.data(), SIGNAL(update(QString,Obj)), 
                    this, SLOT(update(QString,Obj)));
        }
    }
    catch(const Conspire::Exception &e)
    {
        PagePointer error( new ExceptionWidget( Conspire::tr(
                "An error occurred when trying to set the global options "
                "object."), e ) );
                
        emit( push(error) );
        return;
    }

    //now go through an update active pages with the new
    //version of the options
    try
    {
        if (current_view and (current_view != top_view))
        {
            current_view->update(options);

            if (current_view->isBroken())
                current_view = PagePointer();
        }
    }
    catch(...)
    {
        //this broke the current view...
        current_view = PagePointer();
    }

    if (not view_history.isEmpty())
    {
        QMutableVectorIterator<PagePointer> it(view_history);
        
        while (it.hasNext())
        {
            PagePointer page = it.next();
            
            if (page != top_view)
            {
                try
                {
                    page->update(options);
                    
                    if (page->isBroken())
                        it.remove();
                }
                catch(...)
                {
                    //this broke this page
                    it.remove();
                }
            }
        }
    }
    
    if (not view_future.isEmpty())
    {
        QMutableVectorIterator<PagePointer> it(view_future);
        
        while (it.hasNext())
        {
            PagePointer page = it.next();
            
            if (page != top_view)
            {
                try
                {
                    page->update(options);
                    
                    if (page->isBroken())
                        it.remove();
                }
                catch(...)
                {
                    //this broke the page
                    it.remove();
                }
            }
        }
    }

    opts = options;

    if (not current_view)
        home(true);
}

void OptionsWidget::add(QString key)
{
    conspireDebug() << "OptionsWidget::add(" << key << ")";

    Options old_opts = opts;

    try
    {
        undo_stack->push( new OptionsAddCommand(this, key) );
    }
    catch(const Conspire::Exception &e)
    {
        setOptions(old_opts);

        PagePointer error( new ExceptionWidget( Conspire::tr(
                "An error occurred when trying to add the new value \"%1\".")
                .arg(key), e ) );
                
        emit( push(error) );
    }
}

void OptionsWidget::remove(QString key)
{
    Options old_opts = opts;

    try
    {
        undo_stack->push( new OptionsRemoveCommand(this, key) );
    }
    catch(const Conspire::Exception &e)
    {
        setOptions(old_opts);

        PagePointer error( new ExceptionWidget( Conspire::tr(
                "An error occurred when trying to remove the value \"%1\".")
                .arg(key), e ) );
                
        emit( push(error) );
    }
}

void OptionsWidget::update(QString key, Obj object)
{
    Options old_opts = opts;

    try
    {
        undo_stack->push( new OptionsUpdateCommand(this, key, object.asA<Value>()) );
    }
    catch(const Conspire::Exception &e)
    {
        setOptions(old_opts);

        PagePointer error( new ExceptionWidget( Conspire::tr(
                "An error occurred when trying to set the value for \"%1\" to \"%2\".")
                .arg(key, object.toString()), e ) );
                
        emit( push(error) );
    }
}

void OptionsWidget::resizeEvent(QResizeEvent *event)
{
    if (event)
    {
        this->viewport()->resize(event->size());

        if (mainbar)
        {
            QPointF top_left = mapToScene(QRect(QPoint(0,0), QSize(2, 2)))
                                    .boundingRect().center();
                                    
            mainbar->setPos(top_left);
        }
    
        if (current_view)
        {
            current_view->resize(this->viewport()->size());

            setSceneRect( QRectF(0, 0, current_view->geometry().width(), 
                                       current_view->geometry().height()) );
        }
    }
}

void OptionsWidget::keyPressEvent(QKeyEvent *event)
{
    if (event)
    {
        if (event->key() == ::Qt::Key_Escape)
        {
            this->popView();
        }
        else
        {
            QGraphicsView::keyPressEvent(event);
        }
    }
}

/** Slot used to push a new page to the top of the view */
void OptionsWidget::push(PagePointer page)
{
    if (not page)
        return;

    this->scene()->addItem(page.data());

    this->pushView(page, true);
    
    connect(page.data(), SIGNAL(push(PagePointer)), this, SLOT(push(PagePointer)));
    connect(page.data(), SIGNAL(pop(bool)), this, SLOT(pop(bool)));
    connect(page.data(), SIGNAL(add(QString)), this, SLOT(add(QString)));
    connect(page.data(), SIGNAL(remove(QString)), this, SLOT(remove(QString)));
    connect(page.data(), SIGNAL(update(QString,Obj)), this, SLOT(update(QString,Obj)));
}

/** Slot used to pop a page from the view. If "forget_view" is true, then
    this page is not added to the view future */
void OptionsWidget::pop(bool forget_page)
{
    this->popView(forget_page);
}

/** Slot used to switch to the home view. This clears the view history and future
    if "clear_history" is true */
void OptionsWidget::home(bool clear_history)
{
    if (clear_history)
    {
        view_history.clear();
        view_future.clear();
        pop(true);
    }
    else
    {
        if (current_view != top_view)
        {
            push(top_view);
        }
    }
}

/** Switch to a new view, pushing the old view onto the stack */
void OptionsWidget::pushView(PagePointer new_view, bool clear_future)
{
    if (not new_view)
        return;

    if (clear_future)
        view_future.clear();

    if (current_view == new_view)
    {
        //there is no change
        return;
    }
    else if (current_view)
    {
        //animate the transition from the current view to the new view
        animateSwitch(current_view, new_view, true);
    }
    else
    {
        //there is no current view, so animate a zoom into the new view
        animateNew(new_view);
    }

    view_history.push(current_view);
        
    current_view = new_view;

    emit( canBackChanged( not view_history.isEmpty() ) );
    emit( canForwardChanged( not view_future.isEmpty() ) );
}

/** Switch back to the old view */
void OptionsWidget::popView(bool forget_view)
{
    PagePointer new_view;

    //get the last, non-broken view to display
    while (not view_history.isEmpty())
    {
        new_view = view_history.pop();
        
        if (new_view and not new_view->isBroken())
            break;
    }
    
    if (not new_view)
        new_view = top_view;

    if (not new_view)
    {
        //something has gone really wrong, as there is nothing to display...
        conspireDebug() << "SOMETHING IS REALLY WRONG!!!";
    }
    else if (current_view == new_view)
    {
        //there is no change
        return;
    }
    else if (current_view)
    {
        //animate the transition from the current view to the new view
        animateSwitch(current_view, new_view, false);
    }
    else
    {
        //there is no current view, so animate a zoom into the new view
        animateNew(new_view);
    }

    if (not forget_view)
        view_future.push(current_view);
        
    current_view = new_view;

    emit( canBackChanged( not view_history.isEmpty() ) );
    emit( canForwardChanged( not view_future.isEmpty() ) );
}

void OptionsWidget::animateDestroy(PagePointer old_view)
{
    if (not old_view)
        return;
        
    old_view->setOpacity(1.0);

    QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
    QPropertyAnimation *anim = new QPropertyAnimation(old_view.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);
    
    anim = new QPropertyAnimation(old_view.data(), "x");
    anim->setDuration(1000);
    anim->setStartValue(-2 * this->width());
    anim->setStartValue(0.0);
    anim->setEasingCurve(QEasingCurve::InOutBack);
    g->addAnimation(anim);

    g->start(QAbstractAnimation::DeleteWhenStopped);
}

void OptionsWidget::animateNew(PagePointer new_view)
{
    if (not new_view or (new_view == current_view))
    {
        return;
    }
    
    new_view->setOpacity(0);
    new_view->resize(this->viewport()->size());

    new_view->resize(this->viewport()->size());
    
    setSceneRect( QRectF(0, 0, new_view->geometry().width(), 
                               new_view->geometry().height()) );
    
    QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
    QPropertyAnimation *anim = new QPropertyAnimation(new_view.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    g->addAnimation(anim);
    
    anim = new QPropertyAnimation(new_view.data(), "x");
    anim->setDuration(1000);
    anim->setStartValue(2 * this->width());
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InOutBack);
    g->addAnimation(anim);

    g->start(QAbstractAnimation::DeleteWhenStopped);
}

void OptionsWidget::animateSwitch(PagePointer old_view, PagePointer new_view, 
                                  bool move_forwards)
{
    if (not new_view)
    {
        animateDestroy(old_view);
        return;
    }
    else if (not old_view)
    {
        animateNew(new_view);
        return;
    }

    //make sure that the two views have the correct size
    new_view->setOpacity(0);
    
    new_view->resize(this->viewport()->size());
    
    setSceneRect( QRectF(0, 0, new_view->geometry().width(), 
                               new_view->geometry().height()) );

    old_view->resize(this->viewport()->size());

    QParallelAnimationGroup *g = new QParallelAnimationGroup();

    QPropertyAnimation *anim = new QPropertyAnimation(old_view.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);

    anim = new QPropertyAnimation(new_view.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    g->addAnimation(anim);

    if (move_forwards)
    {
        anim = new QPropertyAnimation(old_view.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(0.0);
        anim->setEndValue(-2 * this->width());
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_view.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(2 * this->width());
        anim->setEndValue(0.0);
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
    }
    else
    {
        anim = new QPropertyAnimation(old_view.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(0.0);
        anim->setEndValue(2 * this->width());
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_view.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(-2 * this->width());
        anim->setEndValue(0.0);
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
    }

    g->start(QAbstractAnimation::DeleteWhenStopped);
}

/** Internal function used to construct the widget */
void OptionsWidget::build()
{
    this->setMinimumSize(600,400);

    this->setScene( new QGraphicsScene(this) );

    undo_stack = new QUndoStack(this);

    connect(undo_stack, SIGNAL(canUndoChanged(bool)),
            this, SIGNAL(canUndoChanged(bool)));
    connect(undo_stack, SIGNAL(canRedoChanged(bool)),
            this, SIGNAL(canRedoChanged(bool)));

    mainbar = new MainBar();
    mainbar->setPos(0, 0);
    mainbar->setZValue(1);

    this->scene()->addItem(mainbar);

    connect(mainbar, SIGNAL(undo()), this, SLOT(undo()));
    connect(mainbar, SIGNAL(redo()), this, SLOT(redo()));
    connect(mainbar, SIGNAL(back()), this, SLOT(back()));
    connect(mainbar, SIGNAL(forward()), this, SLOT(forward()));
    
    connect(this, SIGNAL(canBackChanged(bool)), mainbar, SLOT(canBackChanged(bool)));
    connect(this, SIGNAL(canForwardChanged(bool)),
            mainbar, SLOT(canForwardChanged(bool)));
    
    connect(this, SIGNAL(canUndoChanged(bool)),
            mainbar, SLOT(canUndoChanged(bool)));
    connect(this, SIGNAL(canRedoChanged(bool)),
            mainbar, SLOT(canRedoChanged(bool)));
}

/** Called to undo the last action */
void OptionsWidget::undo()
{
    if (undo_stack->canUndo())
        undo_stack->undo();
}

/** Called to redo the last undone action */
void OptionsWidget::redo()
{
    if (undo_stack->canRedo())
        undo_stack->redo();
}

/** Called to return back to the previous screen */
void OptionsWidget::back()
{
    this->popView(false);
}

/** Called to move forward to the next screen */
void OptionsWidget::forward()
{
    if (view_future.isEmpty())
        return;
        
    PagePointer page = view_future.pop();
    
    if (not page or page->isBroken())
    {
        //this widget has died - try to get another one
        this->forward();
        return;
    }
    
    this->pushView(page, false);
}
