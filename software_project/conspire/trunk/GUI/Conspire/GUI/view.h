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
#include <QDialog>

#include "Conspire/conspire.h"
#include "Conspire/option.h"

CONSPIRE_BEGIN_HEADER

class QLineEdit;
class QToolButton;
class QPushButton;

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
        
    signals:
        void setOption(QString key, QString value);

        void addOption(QString key);
        void removeOption(QString key);
    };

    /** This class provides the base of all entry widgets.
        These are widgets used to get data from the user */
    class CONSPIRE_EXPORT EntryView : public QWidget
    {
        Q_OBJECT
        
    public:
        EntryView(Obj value, int index, bool allow_multiple, QWidget *parent=0);
        ~EntryView();

        Obj value() const;
        int index() const;

        bool allowMultiple() const;

        void setValue(Obj new_value);

        static EntryView* build(Option option, QWidget *parent=0);

        virtual void update(Option option)=0;

    public slots:
        void add();
        void remove();

    signals:
        void edited(Obj new_value);
        void edited(Obj new_value, int index);

        void added();
        void removed();
        void removed(int index);
        
    protected:
        void updateValue(Obj new_value);
        
    private:
        Obj val;
        int idx;
        bool allow_multiple;
    };

    /** This class provides a holder for a single EntryView. This
        holds the name of the option, and optionally buttons to 
        allow the option to be deleted, or additional values to be
        added */
    class CONSPIRE_EXPORT EntryViewHolder : public QWidget
    {
        Q_OBJECT
    
    public:
        EntryViewHolder(QString label, EntryView *view, QWidget *parent=0);
        ~EntryViewHolder();
        
        void setLabel(QString label);
        
        EntryView* view();
        
        void setAddable(bool can_add);
        void setRemovable(bool can_remove);
        
        bool addable() const;
        bool removable() const;
        
    signals:
        void clickedAdd();
        void clickedDelete();
        
    private:
        QLabel *label;
        EntryView *v;
        QToolButton *add_button;
        QToolButton *del_button;
    };

    /** This class provides a group of entry widgets. This is used
        for multiple-value options */
    class CONSPIRE_EXPORT EntryViewGroup : public QWidget
    {
        Q_OBJECT
    
    public:
        EntryViewGroup(Option option, QWidget *parent=0);
        ~EntryViewGroup();
        
        bool allowMultiple() const;
        
        Obj value() const;
        Obj value(int index) const;
        
        void update(Option option);
        
    signals:
        void edited(Obj new_value);
        void edited(Obj new_value, int index);

        void added();
        void removed();
        void removed(int index);
        
    private slots:
        void showHelp() const;
    
    private:
        QHash<int,EntryViewHolder*> *views;

        QWidget *group_holder;

        QString help_text;
        bool allow_multiple;
    };

    /** This class is used to allow the user to enter
        textual information */
    class CONSPIRE_EXPORT StringValueView : public EntryView
    {
        Q_OBJECT
        
    public:
        StringValueView(Option option, QWidget *parent=0);
        ~StringValueView();

        void update(Option option);

    protected slots:
        void textChanged();
        
    private:
        QLineEdit *edit;
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
        void edited(Obj obj);
        void edited(Obj obj, int index);

        void added();
        void removed();
        void removed(int index);

    private:
        QString k;
        EntryViewGroup *editor;
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
    
    private slots:
        void add();
    
    private:
        /** This set of views for each option key */
        QHash<QString,OptionView*> *views;
        
        /** The button used to add new options */
        QPushButton *add_button;
        
        /** The list of addable options, together with their help */
        QHash<QString,QString> addable;
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

    /** This class holds a dialog of options that can be added */
    class CONSPIRE_EXPORT AddOptionDialog : public QDialog
    {
        Q_OBJECT
    
    public:
        AddOptionDialog(QString key, QHash<QString,QString> options, 
                        QWidget *parent=0);
        ~AddOptionDialog();
        
    private slots:
        void add();
        
    signals:
        void addOption(QString option);
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
        void setOption(QString key, QString value);
        void addOption(QString key);
        void removeOption(QString key);

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
