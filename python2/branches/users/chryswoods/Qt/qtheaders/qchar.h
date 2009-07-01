
#ifndef GCCXML_PARSE_QCHAR_H
#define GCCXML_PARSE_QCHAR_H

#ifdef GCCXML_PARSE

class QString;
class QDataStream;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

class QChar 
{
public:
    enum SpecialCharacter {
        Null = 0x0000,
        Nbsp = 0x00a0,
        ReplacementCharacter = 0xfffd,
        ObjectReplacementCharacter = 0xfffc,
        ByteOrderMark = 0xfeff,
        ByteOrderSwapped = 0xfffe,
        ParagraphSeparator = 0x2029,
        LineSeparator = 0x2028
    };

    enum Category
    {
        NoCategory,

        Mark_NonSpacing,          //   Mn
        Mark_SpacingCombining,    //   Mc
        Mark_Enclosing,           //   Me

        Number_DecimalDigit,      //   Nd
        Number_Letter,            //   Nl
        Number_Other,             //   No

        Separator_Space,          //   Zs
        Separator_Line,           //   Zl
        Separator_Paragraph,      //   Zp

        Other_Control,            //   Cc
        Other_Format,             //   Cf
        Other_Surrogate,          //   Cs
        Other_PrivateUse,         //   Co
        Other_NotAssigned,        //   Cn

        Letter_Uppercase,         //   Lu
        Letter_Lowercase,         //   Ll
        Letter_Titlecase,         //   Lt
        Letter_Modifier,          //   Lm
        Letter_Other,             //   Lo

        Punctuation_Connector,    //   Pc
        Punctuation_Dash,         //   Pd
        Punctuation_Open,         //   Ps
        Punctuation_Close,        //   Pe
        Punctuation_InitialQuote, //   Pi
        Punctuation_FinalQuote,   //   Pf
        Punctuation_Other,        //   Po

        Symbol_Math,              //   Sm
        Symbol_Currency,          //   Sc
        Symbol_Modifier,          //   Sk
        Symbol_Other,             //   So

        Punctuation_Dask = Punctuation_Dash // oops
    };

    enum Direction
    {
        DirL, DirR, DirEN, DirES, DirET, DirAN, DirCS, DirB, DirS, DirWS, DirON,
        DirLRE, DirLRO, DirAL, DirRLE, DirRLO, DirPDF, DirNSM, DirBN
    };

    enum Decomposition
    {
        NoDecomposition,
        Canonical,
        Font,
        NoBreak,
        Initial,
        Medial,
        Final,
        Isolated,
        Circle,
        Super,
        Sub,
        Vertical,
        Wide,
        Narrow,
        Small,
        Square,
        Compat,
        Fraction
    };

    enum Joining
    {
        OtherJoining, Dual, Right, Center
    };

    enum CombiningClass
    {
        Combining_BelowLeftAttached       = 200,
        Combining_BelowAttached           = 202,
        Combining_BelowRightAttached      = 204,
        Combining_LeftAttached            = 208,
        Combining_RightAttached           = 210,
        Combining_AboveLeftAttached       = 212,
        Combining_AboveAttached           = 214,
        Combining_AboveRightAttached      = 216,

        Combining_BelowLeft               = 218,
        Combining_Below                   = 220,
        Combining_BelowRight              = 222,
        Combining_Left                    = 224,
        Combining_Right                   = 226,
        Combining_AboveLeft               = 228,
        Combining_Above                   = 230,
        Combining_AboveRight              = 232,

        Combining_DoubleBelow             = 233,
        Combining_DoubleAbove             = 234,
        Combining_IotaSubscript           = 240
    };

    enum UnicodeVersion {
        Unicode_Unassigned,
        Unicode_1_1,
        Unicode_2_0,
        Unicode_2_1_2,
        Unicode_3_0,
        Unicode_3_1,
        Unicode_3_2,
        Unicode_4_0,
        Unicode_4_1,
        Unicode_5_0
    };


    QChar();
    QChar(uchar c, uchar r);
    QChar(ushort rc);
    QChar(short rc);
    QChar(uint rc);
    QChar(int rc);

    int digitValue() const;
    QChar toLower() const;
    QChar toUpper() const;

    bool hasMirrored() const;
    bool isLower() const; 
    bool isUpper() const;

    QChar mirroredChar() const;
    QString decomposition() const;
    unsigned char combiningClass() const;

    char toAscii() const;
    char toLatin1() const;
    ushort unicode() const;

    static QChar fromAscii(char c);
    static QChar fromLatin1(char c);

    bool isNull() const;
    bool isPrint() const;
    bool isPunct() const;
    bool isSpace() const;
    bool isMark() const;
    bool isLetter() const;
    bool isNumber() const;
    bool isLetterOrNumber() const;
    bool isDigit() const;
    bool isSymbol() const;

    uchar cell() const;
    uchar row() const;
    void setCell(uchar cell);
    void setRow(uchar row);
};

bool operator==(QChar c1, QChar c2);
bool operator!=(QChar c1, QChar c2);
bool operator<=(QChar c1, QChar c2);
bool operator>=(QChar c1, QChar c2);
bool operator<(QChar c1, QChar c2);
bool operator>(QChar c1, QChar c2);

QDataStream &operator<<(QDataStream &, const QChar &);
QDataStream &operator>>(QDataStream &, QChar &);

#else // ifdef GCCXML_PARSE
#include <QChar>

#endif // GCCXML_PARSE

#endif // GCCXML_PARSE_QCHAR_H
