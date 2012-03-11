#ifndef CONSPIRE_VIEW_H
#define CONSPIRE_VIEW_H
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

#include <QWidget>
#include <QLabel>
#include <QUndoCommand>

#include "Conspire/conspire.h"
#include "Conspire/option.h"

CONSPIRE_BEGIN_HEADER

class QLineEdit;

namespace Conspire
{
    class OptionsController;

    /** This class is the base class of views of Option objects */
    class CONSPIRE_EXPORT View : public QWidget
    {
        Q_OBJECT
        
    public:
        View(QWidget *parent=0);
        
        virtual ~View();
        
        View* parentView();
        const View* parentView() const;
        
        View* rootNode();
        const View* rootNode() const;
        
        virtual QString rootKey() const;
        
        virtual QString key() const;
        
    public slots:
        void updateOption(QString key, QString value);
        
    signals:
        void setOption(QString key, QString value);
    };

    /** This class holds the view of an individual Option object */
    class CONSPIRE_EXPORT OptionView : public View
    {
        Q_OBJECT
        
    public:
        OptionView(View *parent=0);
        OptionView(const Option &option, View *parent=0);
        
        ~OptionView();

        QString key() const;

        void build(const Option &option);
        void update(const Option &option);

    private slots:
        void helpClicked() const;
        void edited();

    private:
        /** The key managed by this view */
        QString k;
        
        /** The current textual representation of the value */
        QString v;
        
        /** The help text for this view */
        QString help_text;
        
        QLineEdit *edit;
    };

    /** This class holds the view of the Options object */
    class CONSPIRE_EXPORT OptionsView : public View
    {
        Q_OBJECT
        
    public:
        OptionsView(QWidget *parent=0);
        OptionsView(const Options &options, QWidget *parent=0);
        
        ~OptionsView();
    
        void build(const Options &options);
        void update(const Options &options);
    
    private:
        /** This set of views for each option key */
        QHash<QString,OptionView*> *views;
    };

    /** This class holds a command that changes the Options state
        from old_state to new_state */
    class CONSPIRE_EXPORT OptionsCommand
    {
    public:
        OptionsCommand();
        OptionsCommand(Options old_state, QString key, QString value);
        
        OptionsCommand(const OptionsCommand &other);
        
        ~OptionsCommand();
        
        OptionsCommand& operator=(const OptionsCommand &other);
        
        QString changedText() const;
        
        Options oldState() const;

        QString key() const;
        
        QString oldValue() const;
        QString newValue() const;
        
    private:
        Options old_state;
        QString k;
        QString v;
    };

    class OptionsControl;

    /** This class holds the commands used to change the options object.
        These work by calling the setNestedValue strings in the option */
    class CONSPIRE_EXPORT OptionsUndoCommand : public QUndoCommand
    {
    public:
        OptionsUndoCommand();
        OptionsUndoCommand(OptionsControl *parent,
                           OptionsCommand command);
                       
        ~OptionsUndoCommand();
        
        void redo();
        void undo();
        
    private:
        OptionsControl *control;
        OptionsCommand cmd;
    };

    /** This class holds the top-level overview of the options */
    class CONSPIRE_EXPORT OptionsControl : public QWidget
    {
        Q_OBJECT
        
    public:
        OptionsControl(QWidget *parent=0);
        OptionsControl(const Options &options, QWidget *parent=0);
        
        ~OptionsControl();
        
        Options options() const;
    
    public slots:
        void updateOption(QString key, QString value);

        void save();
        void load();

        void quit();
    
    protected:
        friend class OptionsUndoCommand;
        
        void undo(const OptionsCommand &command);
        void redo(const OptionsCommand &command);

    private:
        void applyCommand(const OptionsCommand &command);

        /** The current options object */
        Options opts;

        /** The top-level OptionsView */
        OptionsView *view;
        
        /** Undo stack */
        QUndoStack *undo_stack;
    };
}

CONSPIRE_END_HEADER

#endif
