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
    /** This is the base class of View types that represent
        views of Conspire options
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT View : public QLabel
    {
        Q_OBJECT
        
    public:
        View(QWidget *parent=0);
        View(const Options &options, QWidget *parent=0);
        
        ~View();
    };

    /** This class holds the view of an individual Option object */
    class CONSPIRE_EXPORT OptionView : public QWidget
    {
        Q_OBJECT
        
    public:
        OptionView(QWidget *parent=0);
        OptionView(const Option &option, QWidget *parent=0);
        
        ~OptionView();
        
        Option option() const;

    public slots:
        void childUpdated(Option option);
    
    private slots:
        void helpClicked() const;
        void edited();
        
    signals:
        void updatedOption(Option option);
        void updatedOption(Options options);

    private:
        /** The option managed by this view */
        Option opt;
        
        QLineEdit *edit;
    };

    /** This class holds the view of the Options object */
    class CONSPIRE_EXPORT OptionsView : public QWidget
    {
        Q_OBJECT
        
    public:
        OptionsView(QWidget *parent=0);
        OptionsView(const Options &options, QWidget *parent=0);
        
        ~OptionsView();
        
        Options options() const;
    
    public slots:
        void childUpdated(Option option);
        void childUpdated(Options options);
    
    signals:
        void updatedOption(Option option);
        void updatedOption(Options options);
    
    private:
        /** This options managed by this view */
        Options opts;
    };

    /** This class holds a command that changes the Options state
        from old_state to new_state */
    class CONSPIRE_EXPORT OptionsCommand
    {
    public:
        OptionsCommand();
        OptionsCommand(Options old_state, Options new_state);
        
        OptionsCommand(const OptionsCommand &other);
        
        ~OptionsCommand();
        
        OptionsCommand& operator=(const OptionsCommand &other);
        
        QString changedText() const;
        
        Options oldState() const;
        Options newState() const;
        
    private:
        Options old_state;
        Options new_state;
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
        
        QString text() const;
        
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
        void updated(Options options);

        void undo();
        void redo();

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
        
        /** Undo stack */
        QUndoStack *undo_stack;
    };
}

CONSPIRE_END_HEADER

#endif
