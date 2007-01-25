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
#include <QChar>
#include <wchar.h>

#include "SirePy/str.hpp"
#include "SirePy/pycontainer.hpp"

#include "qdatastream.hpp"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;


namespace SirePy
{

template<>
QString get_string_representation<QChar>(const QChar &qchar)
{
    return QString(qchar);
}

}

/** Wrap the QChar class */
void  
SIREQT_EXPORT
export_QChar()
{
    scope QChar_scope = class_<QChar>("QChar", init<>())
        .def( init<uchar, uchar>() )
        .def( init<ushort>() )
        .def( init<short>() )
        .def( init<uint>() )
        .def( init<int>() )
        
        .def( self == self )
        .def( self != self )
        .def( self > self )
        .def( self >= self )
        .def( self < self )
        .def( self <= self )
        
        .def( "__rlshift__", &__rlshift__QDataStream<QChar>, 
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<QChar>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "__str__", &__str__<QChar> )
        
        .def( "category", &QChar::category )
        .def( "cell", &QChar::cell )
        .def( "combiningClass", &QChar::combiningClass )
        .def( "decomposition", &QChar::decomposition )
        .def( "decompositionTag", &QChar::decompositionTag )
        .def( "digitValue", &QChar::digitValue )
        .def( "direction", &QChar::direction )
        .def( "hasMirrored", &QChar::hasMirrored )
        .def( "isDigit", &QChar::isDigit )
        .def( "isLetter", &QChar::isLetter )
        .def( "isLetterOrNumber", &QChar::isLetterOrNumber )
        .def( "isLower", &QChar::isLower )
        .def( "isMark", &QChar::isMark )
        .def( "isNull", &QChar::isNull )
        .def( "isNumber", &QChar::isNumber )
        .def( "isPrint", &QChar::isPrint )
        .def( "isPunct", &QChar::isPunct )
        .def( "isSpace", &QChar::isSpace )
        .def( "isSymbol", &QChar::isSymbol )
        .def( "isUpper", &QChar::isUpper )
        //.def( "joining", &QChar::joining ) //preliminary function - subject to change
        .def( "mirroredChar", &QChar::mirroredChar )
        .def( "row", &QChar::row )
        .def( "toAscii", &QChar::toAscii )
        .def( "toLatin1", &QChar::toLatin1 )
        .def( "toLower", &QChar::toLower )
        .def( "toUpper", &QChar::toUpper )
        .def( "unicode", (const ushort (QChar::*)() const)
                                  &QChar::unicode )
        .def( "unicodeVersion", &QChar::unicodeVersion )
        .def( "fromAscii", &QChar::fromAscii ).staticmethod("fromAscii")
        .def( "fromLatin1", &QChar::fromLatin1 ).staticmethod("fromLatin1")
    ;

    implicitly_convertible<QChar::SpecialCharacter, QChar>();

    enum_<QChar::Category>("Category")
        .value( "Mark_NonSpacing", QChar::Mark_NonSpacing )
        .value( "Mark_SpacingCombining", QChar::Mark_SpacingCombining )
        .value( "Mark_Enclosing", QChar::Mark_Enclosing )
        .value( "Number_DecimalDigit", QChar::Number_DecimalDigit )
        .value( "Number_Letter", QChar::Number_Letter )
        .value( "Number_Other", QChar::Number_Other )
        .value( "Separator_Space", QChar::Separator_Space )
        .value( "Separator_Line", QChar::Separator_Line )
        .value( "Separator_Paragraph", QChar::Separator_Paragraph )
        .value( "Other_Control", QChar::Other_Control )
        .value( "Other_Format", QChar::Other_Format )
        .value( "Other_Surrogate", QChar::Other_Surrogate )
        .value( "Other_PrivateUse", QChar::Other_PrivateUse )
        .value( "Other_NotAssigned", QChar::Other_NotAssigned )
        .value( "Letter_Uppercase", QChar::Letter_Uppercase )
        .value( "Letter_Lowercase", QChar::Letter_Lowercase )
        .value( "Letter_Titlecase", QChar::Letter_Titlecase )
        .value( "Letter_Modifier", QChar::Letter_Modifier )
        .value( "Letter_Other", QChar::Letter_Other )
        .value( "Punctuation_Connector", QChar::Punctuation_Connector )
        .value( "Punctuation_Dash", QChar::Punctuation_Dash )
        .value( "Punctuation_Open", QChar::Punctuation_Open )
        .value( "Punctuation_Close", QChar::Punctuation_Close )
        .value( "Punctuation_InitialQuote", QChar::Punctuation_InitialQuote )
        .value( "Punctuation_FinalQuote", QChar::Punctuation_FinalQuote )
        .value( "Punctuation_Other", QChar::Punctuation_Other )
        .value( "Symbol_Math", QChar::Symbol_Math )
        .value( "Symbol_Currency", QChar::Symbol_Currency )
        .value( "Symbol_Modifier", QChar::Symbol_Modifier )
        .value( "Symbol_Other", QChar::Symbol_Other )
        .value( "NoCategory", QChar::NoCategory )
    ;

    enum_<QChar::Decomposition>("Decomposition")
        .value( "NoDecomposition", QChar::NoDecomposition )
        .value( "Canonical", QChar::Canonical )
        .value( "Circle", QChar::Circle )
        .value( "Compat", QChar::Compat )
        .value( "Final", QChar::Final )
        .value( "Font", QChar::Font )
        .value( "Fraction", QChar::Fraction )
        .value( "Initial", QChar::Initial )
        .value( "Isolated", QChar::Isolated )
        .value( "Medial", QChar::Medial )
        .value( "Narrow", QChar::Narrow )
        .value( "NoBreak", QChar::NoBreak )
        .value( "Small", QChar::Small )
        .value( "Square", QChar::Square )
        .value( "Sub", QChar::Sub )
        .value( "Super", QChar::Super )
        .value( "Vertical", QChar::Vertical )
        .value( "Wide", QChar::Wide )
    ;
    
    enum_<QChar::Direction>("Direction")
        .value( "DirAL", QChar::DirAL )
        .value( "DirAN", QChar::DirAN )
        .value( "DirB", QChar::DirB )
        .value( "DirBN", QChar::DirBN )
        .value( "DirCS", QChar::DirCS )
        .value( "DirEN", QChar::DirEN )
        .value( "DirES", QChar::DirES )
        .value( "DirET", QChar::DirET )
        .value( "DirL", QChar::DirL )
        .value( "DirLRE", QChar::DirLRE )
        .value( "DirLRO", QChar::DirLRO )
        .value( "DirNSM", QChar::DirNSM )
        .value( "DirON", QChar::DirON )
        .value( "DirPDF", QChar::DirPDF )
        .value( "DirR", QChar::DirR )
        .value( "DirRLE", QChar::DirRLE )
        .value( "DirRLO", QChar::DirRLO )
        .value( "DirS", QChar::DirS )
        .value( "DirWS", QChar::DirWS )
    ;
  
    enum_<QChar::Joining>("Joining")
        .value( "Center", QChar::Center )
        .value( "Dual", QChar::Dual )
        .value( "OtherJoining", QChar::OtherJoining )
        .value( "Right", QChar::Right )
    ;
    
    enum_<QChar::SpecialCharacter>("SpecialCharacter")
        .value( "Null", QChar::Null )
        .value( "Nbsp", QChar::Nbsp )
        .value( "ReplacementCharacter", QChar::ReplacementCharacter )
        .value( "ObjectReplacementCharacter", QChar::ObjectReplacementCharacter )
        .value( "ByteOrderMark", QChar::ByteOrderMark )
        .value( "ByteOrderSwapped", QChar::ByteOrderSwapped )
        .value( "ParagraphSeparator", QChar::ParagraphSeparator )
        .value( "LineSeparator", QChar::LineSeparator )
    ;
    
    enum_<QChar::UnicodeVersion>("UnicodeVersion")
        .value( "Unicode_1_1", QChar::Unicode_1_1 )
        .value( "Unicode_2_0", QChar::Unicode_2_0 )
        .value( "Unicode_2_1_2", QChar::Unicode_2_1_2 )
        .value( "Unicode_3_0", QChar::Unicode_3_0 )
        .value( "Unicode_3_1", QChar::Unicode_3_1 )
        .value( "Unicode_3_2", QChar::Unicode_3_2 )
        .value( "Unicode_4_0", QChar::Unicode_4_0 )
        .value( "Unicode_Unassigned", QChar::Unicode_Unassigned )
    ;
}
