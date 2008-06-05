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

#include <Python.h>
#include <boost/python.hpp>

#include <Qt>
#include <QString>

#ifdef USE_QTGUI
#include <QTextDocument>
#endif

#include "sireglobal.h"

using namespace boost::python;
using namespace Qt;

/** Dummy class used to create a Qt scope */
class _Qt
{
public:
    _Qt()
    {}
    
    ~_Qt()
    {}
};

void wrap_qCritical( const char *msg )
{
    qCritical(msg);
}

void wrap_qDebug( const char *msg )
{
    qDebug(msg);
}

void wrap_qFatal( const char *msg )
{
    qFatal(msg);
}

void wrap_qWarning( const char *msg )
{
    qWarning(msg);
}

#ifdef USE_QTGUI
QString wrap_convertFromPlainText2(const QString &plain, Qt::WhiteSpaceMode mode)
{
    return convertFromPlainText(plain, mode);
}

QString wrap_convertFromPlainText1(const QString &plain)
{
    return convertFromPlainText(plain);
}
#endif

void  
SIREQT_EXPORT
export_QtGlobal()
{
    def("qAbs", &qAbs<double>);
    def("qAbs", &qAbs<int>);

    def("qBound", &qBound<double>, return_value_policy<copy_const_reference>());
    def("qBound", &qBound<int>, return_value_policy<copy_const_reference>());

    def("qCritical", &wrap_qCritical);
    def("qDebug", &wrap_qDebug);
    def("qFatal", &wrap_qFatal);
    def("qWarning", &wrap_qWarning);
    
    //def("qInstallMsgHandler", &qInstallMsgHandler);
    
    #if defined(Q_OS_MAC)
    def("qMacVersion", &qMacVersion);
    #endif
    
    def("qMax", &qMax<double>, return_value_policy<copy_const_reference>());
    def("qMax", &qMax<int>, return_value_policy<copy_const_reference>());
    def("qMin", &qMin<double>, return_value_policy<copy_const_reference>());
    def("qMin", &qMin<int>, return_value_policy<copy_const_reference>());
    
    def("qRound64", &qRound64);
    def("qRound", &qRound);
    def("qVersion", &qVersion);
    
    #ifdef USE_QTGUI
    def("convertFromPlainText", &wrap_convertFromPlainText2);
    def("convertFromPlainText", &wrap_convertFromPlainText1);
    def("escape", &escape);
    def("mightBeRichText", &mightBeRichText);
    #endif

    scope Qt_scope = class_<_Qt, boost::noncopyable>("Qt", no_init);
    
/*    typedef qint8
    typedef qint16
    typedef qint32
    typedef qint64
    typedef qlonglong
    typedef qreal
    typedef quint8
    typedef quint16
    typedef quint32
    typedef quint64
    typedef qulonglong
    typedef uchar
    typedef uint
    typedef ulong
    typedef ushort     */
        
//     enum_<QtMsgType>("QtMsgType")
//         .value(QtDebugMsg, "QtDebugMsg")
//         .value(QtWarningMsg, "QtWarningMsg")
//         .value(QtCriticalMsg, "QtCriticalMsg")
//         .value(QtFatalMsg, "QtFatalMsg")
//         .value(QtSystemMsg, "QtSystemMsg")
//     ;

    enum_<Qt::AlignmentFlag>("AlignmentFlag")
        .value( "AlignLeft", Qt::AlignLeft )
        .value( "AlignRight", Qt::AlignRight )
        .value( "AlignHCenter", Qt::AlignHCenter )
        .value( "AlignJustify", Qt::AlignJustify )
        .value( "AlignTop", Qt::AlignTop )
        .value( "AlignBottom", Qt::AlignBottom )
        .value( "AlignVCenter", Qt::AlignVCenter )
        .value( "AlignCenter", Qt::AlignCenter )   
        .value( "AlignAbsolute", Qt::AlignAbsolute )
        .value( "AlignLeading", Qt::AlignLeading )
        .value( "AlignTrailing", Qt::AlignTrailing )
        .value( "AlignHorizontal_Mask", Qt::AlignHorizontal_Mask )
        .value( "AlignVertical_Mask", Qt::AlignVertical_Mask )
    ;
    
    implicitly_convertible<int,Qt::Alignment>();
    implicitly_convertible<Qt::AlignmentFlag,Qt::Alignment>();

    enum_<Qt::AnchorAttribute>("AnchorAttribute")
        .value( "AnchorName", Qt::AnchorName )
        .value( "AnchorHref", Qt::AnchorHref )
    ;
    
    enum_<Qt::ArrowType>("ArrowType")
        .value( "NoArrow", Qt::NoArrow )
        .value( "UpArrow", Qt::UpArrow )
        .value( "DownArrow", Qt::DownArrow )
        .value( "LeftArrow", Qt::LeftArrow )
        .value( "RightArrow", Qt::RightArrow )
    ;
    
    enum_<Qt::AspectRatioMode>("AspectRatioMode")
        .value( "IgnoreAspectRatio", Qt::IgnoreAspectRatio )
        .value( "KeepAspectRatio", Qt::KeepAspectRatio )
        .value( "KeepAspectRatioByExpanding", Qt::KeepAspectRatioByExpanding )    
    ;

    enum_<Qt::BGMode>("BGMode")
        .value( "TransparentMode", Qt::TransparentMode )
        .value( "OpaqueMode", Qt::OpaqueMode )
    ;
    
    enum_<Qt::BrushStyle>("BrushStyle")
        .value( "NoBrush", Qt::NoBrush )
        .value( "SolidPattern", Qt::SolidPattern )
        .value( "Dense1Pattern", Qt::Dense1Pattern )
        .value( "Dense2Pattern", Qt::Dense2Pattern )
        .value( "Dense3Pattern", Qt::Dense3Pattern )
        .value( "Dense4Pattern", Qt::Dense4Pattern )
        .value( "Dense5Pattern", Qt::Dense5Pattern )
        .value( "Dense6Pattern", Qt::Dense6Pattern )
        .value( "Dense7Pattern", Qt::Dense7Pattern )
        .value( "HorPattern", Qt::HorPattern )
        .value( "VerPattern", Qt::VerPattern )
        .value( "CrossPattern", Qt::CrossPattern )
        .value( "BDiagPattern", Qt::BDiagPattern )
        .value( "FDiagPattern", Qt::FDiagPattern )
        .value( "DiagCrossPattern", Qt::DiagCrossPattern )
        .value( "LinearGradientPattern", Qt::LinearGradientPattern )
        .value( "ConicalGradientPattern", Qt::ConicalGradientPattern )
        .value( "RadialGradientPattern", Qt::RadialGradientPattern )
        .value( "TexturePattern", Qt::TexturePattern )
    ;
    
    enum_<Qt::CaseSensitivity>("CaseSensitivity")
        .value( "CaseInsensitive", Qt::CaseInsensitive )
        .value( "CaseSensitive", Qt::CaseSensitive )
    ;

    enum_<Qt::CheckState>("CheckState")
        .value( "Unchecked", Qt::Unchecked )
        .value( "PartiallyChecked", Qt::PartiallyChecked )
        .value( "Checked", Qt::Checked )
    ;
    
    enum_<Qt::ClipOperation>("ClipOperation")
        .value( "NoClip", Qt::NoClip )
        .value( "ReplaceClip", Qt::ReplaceClip )
        .value( "IntersectClip", Qt::IntersectClip )
        .value( "UniteClip", Qt::UniteClip )    
    ;
    
    enum_<Qt::ConnectionType>("ConnectionType")
        .value( "DirectConnection", Qt::DirectConnection )
        .value( "QueuedConnection", Qt::QueuedConnection )
        .value( "AutoConnection", Qt::AutoConnection )
    ;
    
    enum_<Qt::ContextMenuPolicy>("ContextMenuPolicy")
        .value( "NoContextMenu", Qt::NoContextMenu )
        .value( "DefaultContextMenu", Qt::DefaultContextMenu )
        .value( "ActionsContextMenu", Qt::ActionsContextMenu )
        .value( "CustomContextMenu", Qt::CustomContextMenu )    
    ;
    
    enum_<Qt::Corner>("Corner")
        .value( "TopLeftCorner", Qt::TopLeftCorner )
        .value( "TopRightCorner", Qt::TopRightCorner )
        .value( "BottomLeftCorner", Qt::BottomLeftCorner )
        .value( "BottomRightCorner", Qt::BottomRightCorner )
    ;
    
    enum_<Qt::CursorShape>("CursorShape")
        .value( "ArrowCursor", Qt::ArrowCursor )
        .value( "UpArrowCursor", Qt::UpArrowCursor )
        .value( "CrossCursor", Qt::CrossCursor )
        .value( "WaitCursor", Qt::WaitCursor )
        .value( "IBeamCursor", Qt::IBeamCursor )
        .value( "SizeVerCursor", Qt::SizeVerCursor )
        .value( "SizeHorCursor", Qt::SizeHorCursor )
        .value( "SizeFDiagCursor", Qt::SizeFDiagCursor )
        .value( "SizeBDiagCursor", Qt::SizeBDiagCursor )
        .value( "SizeAllCursor", Qt::SizeAllCursor )
        .value( "BlankCursor", Qt::BlankCursor )
        .value( "SplitVCursor", Qt::SplitVCursor )
        .value( "SplitHCursor", Qt::SplitHCursor )
        .value( "PointingHandCursor", Qt::PointingHandCursor )
        .value( "ForbiddenCursor", Qt::ForbiddenCursor )
        .value( "WhatsThisCursor", Qt::WhatsThisCursor )
        .value( "BusyCursor", Qt::BusyCursor )
        .value( "BitmapCursor", Qt::BitmapCursor )
    ;
    
    enum_<Qt::DateFormat>("DataFormat")
        .value( "TextDate", Qt::TextDate )
        .value( "ISODate", Qt::ISODate )
        .value( "LocalDate", Qt::LocalDate )
    ;
    
    enum_<Qt::DayOfWeek>("DayOfWeek")
        .value( "Monday", Qt::Monday )
        .value( "Tuesday", Qt::Tuesday )
        .value( "Wednesday", Qt::Wednesday )
        .value( "Thursday", Qt::Thursday )
        .value( "Friday", Qt::Friday )
        .value( "Saturday", Qt::Saturday )
        .value( "Sunday", Qt::Sunday )    
    ;
    
    enum_<Qt::DockWidgetArea>("DockWidgetArea")
        .value( "LeftDockWidgetArea", Qt::LeftDockWidgetArea )
        .value( "RightDockWidgetArea", Qt::RightDockWidgetArea )
        .value( "TopDockWidgetArea", Qt::TopDockWidgetArea )
        .value( "BottomDockWidgetArea", Qt::BottomDockWidgetArea )
        .value( "AllDockWidgetAreas", Qt::AllDockWidgetAreas )
    ;
    
    implicitly_convertible<int, Qt::DockWidgetAreas>();
    implicitly_convertible<Qt::DockWidgetArea, Qt::DockWidgetAreas>();

    enum_<Qt::DropAction>("DropAction")
        .value( "CopyAction", Qt::CopyAction )
        .value( "MoveAction", Qt::MoveAction )
        .value( "LinkAction", Qt::LinkAction )
        .value( "ActionMask", Qt::ActionMask )
        .value( "TargetMoveAction", Qt::TargetMoveAction )
        .value( "IgnoreAction", Qt::IgnoreAction )
    ;
    
    implicitly_convertible<int, Qt::DropActions>();
    implicitly_convertible<Qt::DropAction, Qt::DropActions>();
    
    enum_<Qt::FillRule>("FillRule")
        .value( "OddEvenFill", Qt::OddEvenFill )
        .value( "WindingFill", Qt::WindingFill )
    ;
    
    enum_<Qt::FocusPolicy>("FocusPolicy")
        .value( "TabFocus", Qt::TabFocus )
        .value( "ClickFocus", Qt::ClickFocus )
        .value( "StrongFocus", Qt::StrongFocus )
        .value( "WheelFocus", Qt::WheelFocus )
        .value( "NoFocus", Qt::NoFocus )    
    ;
    
    enum_<Qt::FocusReason>("FocusReason")
        .value( "MouseFocusReason", Qt::MouseFocusReason )
        .value( "TabFocusReason", Qt::TabFocusReason )
        .value( "BacktabFocusReason", Qt::BacktabFocusReason )
        .value( "ActiveWindowFocusReason", Qt::ActiveWindowFocusReason )
        .value( "PopupFocusReason", Qt::PopupFocusReason )
        .value( "ShortcutFocusReason", Qt::ShortcutFocusReason )
        .value( "MenuBarFocusReason", Qt::MenuBarFocusReason )
        .value( "OtherFocusReason", Qt::OtherFocusReason )    
    ;
    
    enum_<Qt::GlobalColor>("GlobalColor")
        .value( "white", Qt::white )
        .value( "black", Qt::black )
        .value( "red", Qt::red )
        .value( "darkRed", Qt::darkRed )
        .value( "green", Qt::green )
        .value( "darkGreen", Qt::darkGreen )
        .value( "blue", Qt::blue )
        .value( "darkBlue", Qt::darkBlue )
        .value( "cyan", Qt::cyan )
        .value( "darkCyan", Qt::darkCyan )
        .value( "magenta", Qt::magenta )
        .value( "darkMagenta", Qt::darkMagenta )
        .value( "yellow", Qt::yellow )
        .value( "darkYellow", Qt::darkYellow )
        .value( "gray", Qt::gray )
        .value( "darkGray", Qt::darkGray )
        .value( "lightGray", Qt::lightGray )
        .value( "transparent", Qt::transparent )
        .value( "color0", Qt::color0 )
        .value( "color1", Qt::color1 )    
    ;
    
    enum_<Qt::ImageConversionFlag>("ImageConversionFlag")
        .value( "AutoColor", Qt::AutoColor )
        .value( "ColorOnly", Qt::ColorOnly )
        .value( "MonoOnly", Qt::MonoOnly )
        .value( "DiffuseDither", Qt::DiffuseDither )
        .value( "OrderedDither", Qt::OrderedDither )
        .value( "ThresholdDither", Qt::ThresholdDither )
        .value( "ThresholdAlphaDither", Qt::ThresholdAlphaDither )
        .value( "OrderedAlphaDither", Qt::OrderedAlphaDither )
        .value( "DiffuseAlphaDither", Qt::DiffuseAlphaDither )
        .value( "PreferDither", Qt::PreferDither )
        .value( "AvoidDither", Qt::AvoidDither )
    ;
    
    implicitly_convertible<int, Qt::ImageConversionFlags>();
    implicitly_convertible<Qt::ImageConversionFlag, Qt::ImageConversionFlags>();
    
    enum_<Qt::InputMethodQuery>("InputMethodQuery")
        .value( "ImMicroFocus", Qt::ImMicroFocus )
        .value( "ImFont", Qt::ImFont )
        .value( "ImCursorPosition", Qt::ImCursorPosition )
        .value( "ImSurroundingText", Qt::ImSurroundingText )
        .value( "ImCurrentSelection", Qt::ImCurrentSelection )    
    ;
    
    enum_<Qt::ItemDataRole>("ItemDataRole")
        .value( "DisplayRole", Qt::DisplayRole )
        .value( "DecorationRole", Qt::DecorationRole )
        .value( "EditRole", Qt::EditRole )
        .value( "ToolTipRole", Qt::ToolTipRole )
        .value( "StatusTipRole", Qt::StatusTipRole )
        .value( "WhatsThisRole", Qt::WhatsThisRole )
        .value( "SizeHintRole", Qt::SizeHintRole )
        .value( "FontRole", Qt::FontRole )
        .value( "TextAlignmentRole", Qt::TextAlignmentRole )
        .value( "BackgroundColorRole", Qt::BackgroundColorRole )
        .value( "TextColorRole", Qt::TextColorRole )
        .value( "CheckStateRole", Qt::CheckStateRole )
        .value( "AccessibleTextRole", Qt::AccessibleTextRole )
        .value( "AccessibleDescriptionRole", Qt::AccessibleDescriptionRole )
        .value( "UserRole", Qt::UserRole )    
    ;
    
    enum_<Qt::ItemFlag>("ItemFlag")
        .value( "ItemIsSelectable", Qt::ItemIsSelectable )
        .value( "ItemIsEditable", Qt::ItemIsEditable )
        .value( "ItemIsDragEnabled", Qt::ItemIsDragEnabled )
        .value( "ItemIsDropEnabled", Qt::ItemIsDropEnabled )
        .value( "ItemIsUserCheckable", Qt::ItemIsUserCheckable )
        .value( "ItemIsEnabled", Qt::ItemIsEnabled )
        .value( "ItemIsTristate", Qt::ItemIsTristate )    
    ;
    
    implicitly_convertible<int, Qt::ItemFlags>();
    implicitly_convertible<Qt::ItemFlag, Qt::ItemFlags>();

    enum_<Qt::Key>("Key")
        .value( "Key_Escape", Qt::Key_Escape )
        .value( "Key_Tab", Qt::Key_Tab )
        .value( "Key_Backtab", Qt::Key_Backtab )
        .value( "Key_Backspace", Qt::Key_Backspace )
        .value( "Key_Return", Qt::Key_Return )
        .value( "Key_Enter", Qt::Key_Enter )
        .value( "Key_Insert", Qt::Key_Insert )
        .value( "Key_Delete", Qt::Key_Delete )
        .value( "Key_Pause", Qt::Key_Pause )
        .value( "Key_Print", Qt::Key_Print )
        .value( "Key_SysReq", Qt::Key_SysReq )
        .value( "Key_Clear", Qt::Key_Clear )
        .value( "Key_Home", Qt::Key_Home )
        .value( "Key_End", Qt::Key_End )
        .value( "Key_Left", Qt::Key_Left )
        .value( "Key_Up", Qt::Key_Up )
        .value( "Key_Right", Qt::Key_Right )
        .value( "Key_Down", Qt::Key_Down )
        .value( "Key_PageUp", Qt::Key_PageUp )
        .value( "Key_PageDown", Qt::Key_PageDown )
        .value( "Key_Shift", Qt::Key_Shift )
        .value( "Key_Control", Qt::Key_Control )
        .value( "Key_Meta", Qt::Key_Meta )
        .value( "Key_Alt", Qt::Key_Alt )
        .value( "Key_AltGr", Qt::Key_AltGr )
        .value( "Key_CapsLock", Qt::Key_CapsLock )
        .value( "Key_NumLock", Qt::Key_NumLock )
        .value( "Key_ScrollLock", Qt::Key_ScrollLock )
        .value( "Key_F1", Qt::Key_F1 )
        .value( "Key_F2", Qt::Key_F2 )
        .value( "Key_F3", Qt::Key_F3 )
        .value( "Key_F4", Qt::Key_F4 )
        .value( "Key_F5", Qt::Key_F5 )
        .value( "Key_F6", Qt::Key_F6 )
        .value( "Key_F7", Qt::Key_F7 )
        .value( "Key_F8", Qt::Key_F8 )
        .value( "Key_F9", Qt::Key_F9 )
        .value( "Key_F10", Qt::Key_F10 )
        .value( "Key_F11", Qt::Key_F11 )
        .value( "Key_F12", Qt::Key_F12 )
        .value( "Key_F13", Qt::Key_F13 )
        .value( "Key_F14", Qt::Key_F14 )
        .value( "Key_F15", Qt::Key_F15 )
        .value( "Key_F16", Qt::Key_F16 )
        .value( "Key_F17", Qt::Key_F17 )
        .value( "Key_F18", Qt::Key_F18 )
        .value( "Key_F19", Qt::Key_F19 )
        .value( "Key_F20", Qt::Key_F20 )
        .value( "Key_F21", Qt::Key_F21 )
        .value( "Key_F22", Qt::Key_F22 )
        .value( "Key_F23", Qt::Key_F23 )
        .value( "Key_F24", Qt::Key_F24 )
        .value( "Key_F25", Qt::Key_F25 )
        .value( "Key_F26", Qt::Key_F26 )
        .value( "Key_F27", Qt::Key_F27 )
        .value( "Key_F28", Qt::Key_F28 )
        .value( "Key_F29", Qt::Key_F29 )
        .value( "Key_F30", Qt::Key_F30 )
        .value( "Key_F31", Qt::Key_F31 )
        .value( "Key_F32", Qt::Key_F32 )
        .value( "Key_F33", Qt::Key_F33 )
        .value( "Key_F34", Qt::Key_F34 )
        .value( "Key_F35", Qt::Key_F35 )
        .value( "Key_Super_L", Qt::Key_Super_L )
        .value( "Key_Super_R", Qt::Key_Super_R )
        .value( "Key_Menu", Qt::Key_Menu )
        .value( "Key_Hyper_L", Qt::Key_Hyper_L )
        .value( "Key_Hyper_R", Qt::Key_Hyper_R )
        .value( "Key_Help", Qt::Key_Help )
        .value( "Key_Direction_L", Qt::Key_Direction_L )
        .value( "Key_Direction_R", Qt::Key_Direction_R )
        .value( "Key_Space", Qt::Key_Space )
        .value( "Key_Any", Qt::Key_Any )
        .value( "Key_Exclam", Qt::Key_Exclam )
        .value( "Key_QuoteDbl", Qt::Key_QuoteDbl )
        .value( "Key_NumberSign", Qt::Key_NumberSign )
        .value( "Key_Dollar", Qt::Key_Dollar )
        .value( "Key_Percent", Qt::Key_Percent )
        .value( "Key_Ampersand", Qt::Key_Ampersand )
        .value( "Key_Apostrophe", Qt::Key_Apostrophe )
        .value( "Key_ParenLeft", Qt::Key_ParenLeft )
        .value( "Key_ParenRight", Qt::Key_ParenRight )
        .value( "Key_Asterisk", Qt::Key_Asterisk )
        .value( "Key_Plus", Qt::Key_Plus )
        .value( "Key_Comma", Qt::Key_Comma )
        .value( "Key_Minus", Qt::Key_Minus )
        .value( "Key_Period", Qt::Key_Period )
        .value( "Key_Slash", Qt::Key_Slash )
        .value( "Key_0", Qt::Key_0 )
        .value( "Key_1", Qt::Key_1 )
        .value( "Key_2", Qt::Key_2 )
        .value( "Key_3", Qt::Key_3 )
        .value( "Key_4", Qt::Key_4 )
        .value( "Key_5", Qt::Key_5 )
        .value( "Key_6", Qt::Key_6 )
        .value( "Key_7", Qt::Key_7 )
        .value( "Key_8", Qt::Key_8 )
        .value( "Key_9", Qt::Key_9 )
        .value( "Key_Colon", Qt::Key_Colon )
        .value( "Key_Semicolon", Qt::Key_Semicolon )
        .value( "Key_Less", Qt::Key_Less )
        .value( "Key_Equal", Qt::Key_Equal )
        .value( "Key_Greater", Qt::Key_Greater )
        .value( "Key_Question", Qt::Key_Question )
        .value( "Key_At", Qt::Key_At )
        .value( "Key_A", Qt::Key_A )
        .value( "Key_B", Qt::Key_B )
        .value( "Key_C", Qt::Key_C )
        .value( "Key_D", Qt::Key_D )
        .value( "Key_E", Qt::Key_E )
        .value( "Key_F", Qt::Key_F )
        .value( "Key_G", Qt::Key_G )
        .value( "Key_H", Qt::Key_H )
        .value( "Key_I", Qt::Key_I )
        .value( "Key_J", Qt::Key_J )
        .value( "Key_K", Qt::Key_K )
        .value( "Key_L", Qt::Key_L )
        .value( "Key_M", Qt::Key_M )
        .value( "Key_N", Qt::Key_N )
        .value( "Key_O", Qt::Key_O )
        .value( "Key_P", Qt::Key_P )
        .value( "Key_Q", Qt::Key_Q )
        .value( "Key_R", Qt::Key_R )
        .value( "Key_S", Qt::Key_S )
        .value( "Key_T", Qt::Key_T )
        .value( "Key_U", Qt::Key_U )
        .value( "Key_V", Qt::Key_V )
        .value( "Key_W", Qt::Key_W )
        .value( "Key_X", Qt::Key_X )
        .value( "Key_Y", Qt::Key_Y )
        .value( "Key_Z", Qt::Key_Z )
        .value( "Key_BracketLeft", Qt::Key_BracketLeft )
        .value( "Key_Backslash", Qt::Key_Backslash )
        .value( "Key_BracketRight", Qt::Key_BracketRight )
        .value( "Key_AsciiCircum", Qt::Key_AsciiCircum )
        .value( "Key_Underscore", Qt::Key_Underscore )
        .value( "Key_QuoteLeft", Qt::Key_QuoteLeft )
        .value( "Key_BraceLeft", Qt::Key_BraceLeft )
        .value( "Key_Bar", Qt::Key_Bar )
        .value( "Key_BraceRight", Qt::Key_BraceRight )
        .value( "Key_AsciiTilde", Qt::Key_AsciiTilde )
        .value( "Key_nobreakspace", Qt::Key_nobreakspace )
        .value( "Key_exclamdown", Qt::Key_exclamdown )
        .value( "Key_cent", Qt::Key_cent )
        .value( "Key_sterling", Qt::Key_sterling )
        .value( "Key_currency", Qt::Key_currency )
        .value( "Key_yen", Qt::Key_yen )
        .value( "Key_brokenbar", Qt::Key_brokenbar )
        .value( "Key_section", Qt::Key_section )
        .value( "Key_diaeresis", Qt::Key_diaeresis )
        .value( "Key_copyright", Qt::Key_copyright )
        .value( "Key_ordfeminine", Qt::Key_ordfeminine )
        .value( "Key_guillemotleft", Qt::Key_guillemotleft )
        .value( "Key_notsign", Qt::Key_notsign )
        .value( "Key_hyphen", Qt::Key_hyphen )
        .value( "Key_registered", Qt::Key_registered )
        .value( "Key_macron", Qt::Key_macron )
        .value( "Key_degree", Qt::Key_degree )
        .value( "Key_plusminus", Qt::Key_plusminus )
        .value( "Key_twosuperior", Qt::Key_twosuperior )
        .value( "Key_threesuperior", Qt::Key_threesuperior )
        .value( "Key_acute", Qt::Key_acute )
        .value( "Key_mu", Qt::Key_mu )
        .value( "Key_paragraph", Qt::Key_paragraph )
        .value( "Key_periodcentered", Qt::Key_periodcentered )
        .value( "Key_cedilla", Qt::Key_cedilla )
        .value( "Key_onesuperior", Qt::Key_onesuperior )
        .value( "Key_masculine", Qt::Key_masculine )
        .value( "Key_guillemotright", Qt::Key_guillemotright )
        .value( "Key_onequarter", Qt::Key_onequarter )
        .value( "Key_onehalf", Qt::Key_onehalf )
        .value( "Key_threequarters", Qt::Key_threequarters )
        .value( "Key_questiondown", Qt::Key_questiondown )
        .value( "Key_Agrave", Qt::Key_Agrave )
        .value( "Key_Aacute", Qt::Key_Aacute )
        .value( "Key_Acircumflex", Qt::Key_Acircumflex )
        .value( "Key_Atilde", Qt::Key_Atilde )
        .value( "Key_Adiaeresis", Qt::Key_Adiaeresis )
        .value( "Key_Aring", Qt::Key_Aring )
        .value( "Key_AE", Qt::Key_AE )
        .value( "Key_Ccedilla", Qt::Key_Ccedilla )
        .value( "Key_Egrave", Qt::Key_Egrave )
        .value( "Key_Eacute", Qt::Key_Eacute )
        .value( "Key_Ecircumflex", Qt::Key_Ecircumflex )
        .value( "Key_Ediaeresis", Qt::Key_Ediaeresis )
        .value( "Key_Igrave", Qt::Key_Igrave )
        .value( "Key_Iacute", Qt::Key_Iacute )
        .value( "Key_Icircumflex", Qt::Key_Icircumflex )
        .value( "Key_Idiaeresis", Qt::Key_Idiaeresis )
        .value( "Key_ETH", Qt::Key_ETH )
        .value( "Key_Ntilde", Qt::Key_Ntilde )
        .value( "Key_Ograve", Qt::Key_Ograve )
        .value( "Key_Oacute", Qt::Key_Oacute )
        .value( "Key_Ocircumflex", Qt::Key_Ocircumflex )
        .value( "Key_Otilde", Qt::Key_Otilde )
        .value( "Key_Odiaeresis", Qt::Key_Odiaeresis )
        .value( "Key_multiply", Qt::Key_multiply )
        .value( "Key_Ooblique", Qt::Key_Ooblique )
        .value( "Key_Ugrave", Qt::Key_Ugrave )
        .value( "Key_Uacute", Qt::Key_Uacute )
        .value( "Key_Ucircumflex", Qt::Key_Ucircumflex )
        .value( "Key_Udiaeresis", Qt::Key_Udiaeresis )
        .value( "Key_Yacute", Qt::Key_Yacute )
        .value( "Key_THORN", Qt::Key_THORN )
        .value( "Key_ssharp", Qt::Key_ssharp )
        .value( "Key_division", Qt::Key_division )
        .value( "Key_ydiaeresis", Qt::Key_ydiaeresis )
        .value( "Key_Multi_key", Qt::Key_Multi_key )
        .value( "Key_Codeinput", Qt::Key_Codeinput )
        .value( "Key_SingleCandidate", Qt::Key_SingleCandidate )
        .value( "Key_MultipleCandidate", Qt::Key_MultipleCandidate )
        .value( "Key_PreviousCandidate", Qt::Key_PreviousCandidate )
        .value( "Key_Mode_switch", Qt::Key_Mode_switch )
        .value( "Key_Kanji", Qt::Key_Kanji )
        .value( "Key_Muhenkan", Qt::Key_Muhenkan )
        .value( "Key_Henkan", Qt::Key_Henkan )
        .value( "Key_Romaji", Qt::Key_Romaji )
        .value( "Key_Hiragana", Qt::Key_Hiragana )
        .value( "Key_Katakana", Qt::Key_Katakana )
        .value( "Key_Hiragana_Katakana", Qt::Key_Hiragana_Katakana )
        .value( "Key_Zenkaku", Qt::Key_Zenkaku )
        .value( "Key_Hankaku", Qt::Key_Hankaku )
        .value( "Key_Zenkaku_Hankaku", Qt::Key_Zenkaku_Hankaku )
        .value( "Key_Touroku", Qt::Key_Touroku )
        .value( "Key_Massyo", Qt::Key_Massyo )
        .value( "Key_Kana_Lock", Qt::Key_Kana_Lock )
        .value( "Key_Kana_Shift", Qt::Key_Kana_Shift )
        .value( "Key_Eisu_Shift", Qt::Key_Eisu_Shift )
        .value( "Key_Eisu_toggle", Qt::Key_Eisu_toggle )
        .value( "Key_Hangul", Qt::Key_Hangul )
        .value( "Key_Hangul_Start", Qt::Key_Hangul_Start )
        .value( "Key_Hangul_End", Qt::Key_Hangul_End )
        .value( "Key_Hangul_Hanja", Qt::Key_Hangul_Hanja )
        .value( "Key_Hangul_Jamo", Qt::Key_Hangul_Jamo )
        .value( "Key_Hangul_Romaja", Qt::Key_Hangul_Romaja )
        .value( "Key_Hangul_Jeonja", Qt::Key_Hangul_Jeonja )
        .value( "Key_Hangul_Banja", Qt::Key_Hangul_Banja )
        .value( "Key_Hangul_PreHanja", Qt::Key_Hangul_PreHanja )
        .value( "Key_Hangul_PostHanja", Qt::Key_Hangul_PostHanja )
        .value( "Key_Hangul_Special", Qt::Key_Hangul_Special )
        .value( "Key_Dead_Grave", Qt::Key_Dead_Grave )
        .value( "Key_Dead_Acute", Qt::Key_Dead_Acute )
        .value( "Key_Dead_Circumflex", Qt::Key_Dead_Circumflex )
        .value( "Key_Dead_Tilde", Qt::Key_Dead_Tilde )
        .value( "Key_Dead_Macron", Qt::Key_Dead_Macron )
        .value( "Key_Dead_Breve", Qt::Key_Dead_Breve )
        .value( "Key_Dead_Abovedot", Qt::Key_Dead_Abovedot )
        .value( "Key_Dead_Diaeresis", Qt::Key_Dead_Diaeresis )
        .value( "Key_Dead_Abovering", Qt::Key_Dead_Abovering )
        .value( "Key_Dead_Doubleacute", Qt::Key_Dead_Doubleacute )
        .value( "Key_Dead_Caron", Qt::Key_Dead_Caron )
        .value( "Key_Dead_Cedilla", Qt::Key_Dead_Cedilla )
        .value( "Key_Dead_Ogonek", Qt::Key_Dead_Ogonek )
        .value( "Key_Dead_Iota", Qt::Key_Dead_Iota )
        .value( "Key_Dead_Voiced_Sound", Qt::Key_Dead_Voiced_Sound )
        .value( "Key_Dead_Semivoiced_Sound", Qt::Key_Dead_Semivoiced_Sound )
        .value( "Key_Dead_Belowdot", Qt::Key_Dead_Belowdot )
        .value( "Key_Dead_Hook", Qt::Key_Dead_Hook )
        .value( "Key_Dead_Horn", Qt::Key_Dead_Horn )
        .value( "Key_Back", Qt::Key_Back )
        .value( "Key_Forward", Qt::Key_Forward )
        .value( "Key_Stop", Qt::Key_Stop )
        .value( "Key_Refresh", Qt::Key_Refresh )
        .value( "Key_VolumeDown", Qt::Key_VolumeDown )
        .value( "Key_VolumeMute", Qt::Key_VolumeMute )
        .value( "Key_VolumeUp", Qt::Key_VolumeUp )
        .value( "Key_BassBoost", Qt::Key_BassBoost )
        .value( "Key_BassUp", Qt::Key_BassUp )
        .value( "Key_BassDown", Qt::Key_BassDown )
        .value( "Key_TrebleUp", Qt::Key_TrebleUp )
        .value( "Key_TrebleDown", Qt::Key_TrebleDown )
        .value( "Key_MediaPlay", Qt::Key_MediaPlay )
        .value( "Key_MediaStop", Qt::Key_MediaStop )
        .value( "Key_MediaPrevious", Qt::Key_MediaPrevious )
        .value( "Key_MediaNext", Qt::Key_MediaNext )
        .value( "Key_MediaRecord", Qt::Key_MediaRecord )
        .value( "Key_HomePage", Qt::Key_HomePage )
        .value( "Key_Favorites", Qt::Key_Favorites )
        .value( "Key_Search", Qt::Key_Search )
        .value( "Key_Standby", Qt::Key_Standby )
        .value( "Key_OpenUrl", Qt::Key_OpenUrl )
        .value( "Key_LaunchMail", Qt::Key_LaunchMail )
        .value( "Key_LaunchMedia", Qt::Key_LaunchMedia )
        .value( "Key_Launch0", Qt::Key_Launch0 )
        .value( "Key_Launch1", Qt::Key_Launch1 )
        .value( "Key_Launch2", Qt::Key_Launch2 )
        .value( "Key_Launch3", Qt::Key_Launch3 )
        .value( "Key_Launch4", Qt::Key_Launch4 )
        .value( "Key_Launch5", Qt::Key_Launch5 )
        .value( "Key_Launch6", Qt::Key_Launch6 )
        .value( "Key_Launch7", Qt::Key_Launch7 )
        .value( "Key_Launch8", Qt::Key_Launch8 )
        .value( "Key_Launch9", Qt::Key_Launch9 )
        .value( "Key_LaunchA", Qt::Key_LaunchA )
        .value( "Key_LaunchB", Qt::Key_LaunchB )
        .value( "Key_LaunchC", Qt::Key_LaunchC )
        .value( "Key_LaunchD", Qt::Key_LaunchD )
        .value( "Key_LaunchE", Qt::Key_LaunchE )
        .value( "Key_LaunchF", Qt::Key_LaunchF )
        .value( "Key_MediaLast", Qt::Key_MediaLast )
        .value( "Key_unknown", Qt::Key_unknown )
        .value( "Key_Call", Qt::Key_Call )
        .value( "Key_Context1", Qt::Key_Context1 )
        .value( "Key_Context2", Qt::Key_Context2 )
        .value( "Key_Context3", Qt::Key_Context3 )
        .value( "Key_Context4", Qt::Key_Context4 )
        .value( "Key_Flip", Qt::Key_Flip )
        .value( "Key_Hangup", Qt::Key_Hangup )
        .value( "Key_No", Qt::Key_No )
        .value( "Key_Select", Qt::Key_Select )
        .value( "Key_Yes", Qt::Key_Yes )
    ;
    
    enum_<Qt::KeyboardModifier>("KeyboardModifier")
        .value( "NoModifier", Qt::NoModifier )
        .value( "ShiftModifier", Qt::ShiftModifier )
        .value( "ControlModifier", Qt::ControlModifier )
        .value( "AltModifier", Qt::AltModifier )
        .value( "MetaModifier", Qt::MetaModifier )
        .value( "KeypadModifier", Qt::KeypadModifier )
    ;
    
    implicitly_convertible<int, Qt::KeyboardModifiers>();
    implicitly_convertible<Qt::KeyboardModifier, Qt::KeyboardModifiers>();
    
    enum_<Qt::LayoutDirection>("LayoutDirection")
        .value( "LeftToRight", Qt::LeftToRight )
        .value( "RightToLeft", Qt::RightToLeft )
    ;
    
    enum_<Qt::MatchFlag>("MatchFlag")
        .value( "MatchExactly", Qt::MatchExactly )
        .value( "MatchContains", Qt::MatchContains )
        .value( "MatchStartsWith", Qt::MatchStartsWith )
        .value( "MatchEndsWith", Qt::MatchEndsWith )
        .value( "MatchCaseSensitive", Qt::MatchCaseSensitive )
        .value( "MatchRegExp", Qt::MatchRegExp )
        .value( "MatchWildcard", Qt::MatchWildcard )
        .value( "MatchWrap", Qt::MatchWrap )
        .value( "MatchRecursive", Qt::MatchRecursive )
    ;
    
    implicitly_convertible<int, Qt::MatchFlags>();
    implicitly_convertible<Qt::MatchFlag, Qt::MatchFlags>();

    enum_<Qt::Modifier>("Modifier")
        .value( "SHIFT", Qt::SHIFT )
        .value( "META", Qt::META )
        .value( "CTRL", Qt::CTRL )
        .value( "ALT", Qt::ALT )
        .value( "UNICODE_ACCEL", Qt::UNICODE_ACCEL )
    ;
    
    enum_<Qt::MouseButton>("MouseButton")
        .value( "NoButton", Qt::NoButton )
        .value( "LeftButton", Qt::LeftButton )
        .value( "RightButton", Qt::RightButton )
        .value( "MidButton", Qt::MidButton )
        .value( "XButton1", Qt::XButton1 )
        .value( "XButton2", Qt::XButton2 )    
    ;
    
    implicitly_convertible<int, Qt::MouseButtons>();
    implicitly_convertible<Qt::MouseButton, Qt::MouseButtons>();
    
    enum_<Qt::Orientation>("Orientation")
        .value( "Horizontal", Qt::Horizontal )
        .value( "Vertical", Qt::Vertical )
    ;
    
    implicitly_convertible<int, Qt::Orientations>();
    implicitly_convertible<Qt::Orientation, Qt::Orientations>();

    enum_<Qt::PenCapStyle>("PenCapStyle")
        .value( "FlatCap", Qt::FlatCap )
        .value( "SquareCap", Qt::SquareCap )
        .value( "RoundCap", Qt::RoundCap )
    ;
    
    enum_<Qt::PenJoinStyle>("PenJoinStyle")
        .value( "MiterJoin", Qt::MiterJoin )
        .value( "BevelJoin", Qt::BevelJoin )
        .value( "RoundJoin", Qt::RoundJoin )
    ;
    
    enum_<Qt::PenStyle>("PenStyle")
        .value( "NoPen", Qt::NoPen )
        .value( "SolidLine", Qt::SolidLine )
        .value( "DashLine", Qt::DashLine )
        .value( "DotLine", Qt::DotLine )
        .value( "DashDotLine", Qt::DashDotLine )
        .value( "DashDotDotLine", Qt::DashDotDotLine )
        .value( "CustomDashLine", Qt::CustomDashLine )
    ;
    
    enum_<Qt::ScrollBarPolicy>("ScrollBarPolicy")
        .value( "ScrollBarAsNeeded", Qt::ScrollBarAsNeeded )
        .value( "ScrollBarAlwaysOff", Qt::ScrollBarAlwaysOff )
        .value( "ScrollBarAlwaysOn", Qt::ScrollBarAlwaysOn )
    ;
    
    enum_<Qt::ShortcutContext>("ShortcutContext")
        .value( "WidgetShortcut", Qt::WidgetShortcut )
        .value( "WindowShortcut", Qt::WindowShortcut )
        .value( "ApplicationShortcut", Qt::ApplicationShortcut )
    ;
    
    enum_<Qt::SortOrder>("SortOrder")
        .value( "AscendingOrder", Qt::AscendingOrder )
        .value( "DescendingOrder", Qt::DescendingOrder )
    ;
    
    enum_<Qt::TextElideMode>("TextElideMode")
        .value( "ElideLeft", Qt::ElideLeft )
        .value( "ElideRight", Qt::ElideRight )
        .value( "ElideMiddle", Qt::ElideMiddle )
    ;
    
    enum_<Qt::TextFlag>("TextFlag")
        .value( "TextSingleLine", Qt::TextSingleLine )
        .value( "TextDontClip", Qt::TextDontClip )
        .value( "TextExpandTabs", Qt::TextExpandTabs )
        .value( "TextShowMnemonic", Qt::TextShowMnemonic )
        .value( "TextWordWrap", Qt::TextWordWrap )
        .value( "TextWrapAnywhere", Qt::TextWrapAnywhere )
        .value( "TextHideMnemonic", Qt::TextHideMnemonic )
        .value( "TextDontPrint", Qt::TextDontPrint )
        .value( "TextIncludeTrailingSpaces", Qt::TextIncludeTrailingSpaces )
    ;

    enum_<Qt::TextFormat>("TextFormat")
        .value( "PlainText", Qt::PlainText )
        .value( "RichText", Qt::RichText )
        .value( "AutoText", Qt::AutoText )
        .value( "LogText", Qt::LogText )
    ;
    
    enum_<Qt::TimeSpec>("TimeSpec")
        .value( "LocalTime", Qt::LocalTime )
        .value( "UTC", Qt::UTC )
    ;
    
    enum_<Qt::ToolBarArea>("ToolBarArea")
        .value( "LeftToolBarArea", Qt::LeftToolBarArea )
        .value( "RightToolBarArea", Qt::RightToolBarArea )
        .value( "TopToolBarArea", Qt::TopToolBarArea )
        .value( "BottomToolBarArea", Qt::BottomToolBarArea )
        .value( "AllToolBarAreas", Qt::AllToolBarAreas )    
    ;
    
    implicitly_convertible<int, Qt::ToolBarAreas>();
    implicitly_convertible<Qt::ToolBarArea, Qt::ToolBarAreas>();

    enum_<Qt::ToolButtonStyle>("ToolButtonStyle")
        .value( "ToolButtonIconOnly", Qt::ToolButtonIconOnly )
        .value( "ToolButtonTextOnly", Qt::ToolButtonTextOnly )
        .value( "ToolButtonTextBesideIcon", Qt::ToolButtonTextBesideIcon )
        .value( "ToolButtonTextUnderIcon", Qt::ToolButtonTextUnderIcon )
    ;
    
    enum_<Qt::TransformationMode>("TransformationMode")
        .value( "FastTransformation", Qt::FastTransformation )
        .value( "SmoothTransformation", Qt::SmoothTransformation )
    ;
    
    enum_<Qt::UIEffect>("UIEffect")
        .value( "UI_General", Qt::UI_General )
        .value( "UI_AnimateMenu", Qt::UI_AnimateMenu )
        .value( "UI_FadeMenu", Qt::UI_FadeMenu )
        .value( "UI_AnimateCombo", Qt::UI_AnimateCombo )
        .value( "UI_AnimateTooltip", Qt::UI_AnimateTooltip )
        .value( "UI_FadeTooltip", Qt::UI_FadeTooltip )
        .value( "UI_AnimateToolBox", Qt::UI_AnimateToolBox )
    ;
    
    enum_<Qt::WidgetAttribute>("WidgetAttribute")
        .value( "WA_AcceptDrops", Qt::WA_AcceptDrops )
        .value( "WA_DeleteOnClose", Qt::WA_DeleteOnClose )
        .value( "WA_QuitOnClose", Qt::WA_QuitOnClose )
        .value( "WA_KeyCompression", Qt::WA_KeyCompression )
        .value( "WA_PendingMoveEvent", Qt::WA_PendingMoveEvent )
        .value( "WA_PendingResizeEvent", Qt::WA_PendingResizeEvent )
        .value( "WA_UnderMouse", Qt::WA_UnderMouse )
        .value( "WA_Disabled", Qt::WA_Disabled )
        .value( "WA_ContentsPropagated", Qt::WA_ContentsPropagated )
        .value( "WA_ForceDisabled", Qt::WA_ForceDisabled )
        .value( "WA_SetPalette", Qt::WA_SetPalette )
        .value( "WA_SetFont", Qt::WA_SetFont )
        .value( "WA_SetCursor", Qt::WA_SetCursor )
        .value( "WA_NoBackground", Qt::WA_NoBackground )
        .value( "WA_OpaquePaintEvent", Qt::WA_OpaquePaintEvent )
        .value( "WA_NoSystemBackground", Qt::WA_NoSystemBackground )
        .value( "WA_UpdatesDisabled", Qt::WA_UpdatesDisabled )
        .value( "WA_ForceUpdatesDisabled", Qt::WA_ForceUpdatesDisabled )
        .value( "WA_StaticContents", Qt::WA_StaticContents )
        .value( "WA_PaintOnScreen", Qt::WA_PaintOnScreen )
        .value( "WA_PaintUnclipped", Qt::WA_PaintUnclipped )
        .value( "WA_PaintOutsidePaintEvent", Qt::WA_PaintOutsidePaintEvent )
        .value( "WA_Hover", Qt::WA_Hover )
        .value( "WA_WindowModified", Qt::WA_WindowModified )
        .value( "WA_Resized", Qt::WA_Resized )
        .value( "WA_Moved", Qt::WA_Moved )
        .value( "WA_Mapped", Qt::WA_Mapped )
        .value( "WA_NoMouseReplay", Qt::WA_NoMouseReplay )
        .value( "WA_OutsideWSRange", Qt::WA_OutsideWSRange )
        .value( "WA_MacMetalStyle", Qt::WA_MacMetalStyle )
        .value( "WA_CustomWhatsThis", Qt::WA_CustomWhatsThis )
        .value( "WA_LayoutOnEntireRect", Qt::WA_LayoutOnEntireRect )
        .value( "WA_NoChildEventsForParent", Qt::WA_NoChildEventsForParent )
        .value( "WA_NoChildEventsFromChildren", Qt::WA_NoChildEventsFromChildren )
        .value( "WA_MouseTracking", Qt::WA_MouseTracking )
        .value( "WA_GroupLeader", Qt::WA_GroupLeader )
        .value( "WA_KeyboardFocusChange", Qt::WA_KeyboardFocusChange )
        .value( "WA_WindowPropagation", Qt::WA_WindowPropagation )
        .value( "WA_InputMethodEnabled", Qt::WA_InputMethodEnabled )
        .value( "WA_MacNoClickThrough", Qt::WA_MacNoClickThrough )
        .value( "WA_MouseNoMask", Qt::WA_MouseNoMask )
        .value( "WA_NoMousePropagation", Qt::WA_NoMousePropagation )
        .value( "WA_RightToLeft", Qt::WA_RightToLeft )
        .value( "WA_ShowModal", Qt::WA_ShowModal )
    ;
    
    enum_<Qt::WindowModality>("WindowModality")
        .value( "NonModal", Qt::NonModal )
        .value( "WindowModal", Qt::WindowModal )
        .value( "ApplicationModal", Qt::ApplicationModal )
    ;
    
    enum_<Qt::WindowState>("WindowState")
        .value( "WindowNoState", Qt::WindowNoState )
        .value( "WindowMinimized", Qt::WindowMinimized )
        .value( "WindowMaximized", Qt::WindowMaximized )
        .value( "WindowFullScreen", Qt::WindowFullScreen )
        .value( "WindowActive", Qt::WindowActive )
    ;
    
    implicitly_convertible<int, Qt::WindowStates>();
    implicitly_convertible<Qt::WindowState, Qt::WindowStates>();
  
    enum_<Qt::WindowType>("WindowType")
        .value( "Widget", Qt::Widget )
        .value( "Window", Qt::Window )
        .value( "Dialog", Qt::Dialog )
        .value( "Sheet", Qt::Sheet )
        .value( "Drawer", Qt::Drawer )
        .value( "Popup", Qt::Popup )
        .value( "Tool", Qt::Tool )
        .value( "ToolTip", Qt::ToolTip )
        .value( "SplashScreen", Qt::SplashScreen )
        .value( "Desktop", Qt::Desktop )
        .value( "SubWindow", Qt::SubWindow )
        .value( "MSWindowsFixedSizeDialogHint", Qt::MSWindowsFixedSizeDialogHint )
        .value( "MSWindowsOwnDC", Qt::MSWindowsOwnDC )
        .value( "X11BypassWindowManagerHint", Qt::X11BypassWindowManagerHint )
        .value( "FramelessWindowHint", Qt::FramelessWindowHint )
        .value( "WindowTitleHint", Qt::WindowTitleHint )
        .value( "WindowSystemMenuHint", Qt::WindowSystemMenuHint )
        .value( "WindowMinimizeButtonHint", Qt::WindowMinimizeButtonHint )
        .value( "WindowMaximizeButtonHint", Qt::WindowMaximizeButtonHint )
        .value( "WindowMinMaxButtonsHint", Qt::WindowMinMaxButtonsHint )
        .value( "WindowContextHelpButtonHint", Qt::WindowContextHelpButtonHint )
        .value( "WindowShadeButtonHint", Qt::WindowShadeButtonHint )
        .value( "WindowStaysOnTopHint", Qt::WindowStaysOnTopHint )
        .value( "WindowType_Mask", Qt::WindowType_Mask )
    
    ;

    implicitly_convertible<int, Qt::WindowFlags>();
    implicitly_convertible<Qt::WindowType, Qt::WindowFlags>();

    #ifdef USE_QTGUI
    enum_<Qt::WhiteSpaceMode>("WhiteSpaceMode")
        .value( "WhiteSpaceNormal", Qt::WhiteSpaceNormal )
        .value( "WhiteSpacePre", Qt::WhiteSpacePre )
        .value( "WhiteSpaceNoWrap", Qt::WhiteSpaceNoWrap )
    ;
    
    enum_<Qt::HitTestAccuracy>("HitTestAccuracy")
        .value( "ExactHit", Qt::ExactHit )
        .value( "FuzzyHit", Qt::FuzzyHit )
    ;
    #endif

}
