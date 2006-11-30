#ifndef SIREMOL_ERRORS_H
#define SIREMOL_ERRORS_H

#include "SireError/exception.h"

SIRE_BEGIN_HEADER

/**
This file contains the exceptions that can be thrown by the SireMol library.

@author Christopher Woods
*/

namespace SireMol
{

/** This is the base class of all SireMol errors */
class SIREMOL_EXPORT siremol_error : public SireError::exception
{
public:
    siremol_error() : exception()
    {}

    siremol_error(QString err, QString place = QString::null) : exception(err,place)
    {}

    siremol_error(const siremol_error &other) : exception(other)
    {}

    ~siremol_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::siremol_error";
    }
};


/** This exception is thrown when a request is made of a non-existant atom

    @author Christopher Woods
*/
class SIREMOL_EXPORT missing_atom : public siremol_error
{
public:
    missing_atom() : siremol_error()
    {}

    missing_atom(QString err, QString place = QString::null)
              : siremol_error(err,place)
    {}

    missing_atom(const missing_atom &other) : siremol_error(other)
    {}

    ~missing_atom() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::missing_atom";
    }
};

/** This exception is thrown when a request is made of a non-existant group

    @author Christopher Woods
*/
class SIREMOL_EXPORT missing_group : public siremol_error
{
public:
    missing_group() : siremol_error()
    {}

    missing_group(QString err, QString place = QString::null)
              : siremol_error(err,place)
    {}

    missing_group(const missing_group &other) : siremol_error(other)
    {}

    ~missing_group() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::missing_group";
    }
};

/** This exception is thrown when a new atom with an existing ID is added
    to a set.

    @author Christopher Woods
*/
class SIREMOL_EXPORT duplicate_atom : public siremol_error
{
public:
    duplicate_atom() : siremol_error()
    {}

    duplicate_atom(QString err, QString place = QString::null)
                : siremol_error(err,place)
    {}

    duplicate_atom(const duplicate_atom &other) : siremol_error(other)
    {}

    ~duplicate_atom() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::duplicate_atom";
    }
};

/** This exception is thrown when an action is requested of a non-existant molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT missing_molecule : public siremol_error
{
public:
    missing_molecule() : siremol_error()
    {}

    missing_molecule(QString err, QString place = QString::null)
                    : siremol_error(err,place)
    {}

    missing_molecule(const missing_molecule &other) : siremol_error(other)
    {}

    ~missing_molecule() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::missing_molecule";
    }
};

/** This exception is thrown when an action is requested of a non-existant residue

    @author Christopher Woods
*/
class SIREMOL_EXPORT missing_residue : public siremol_error
{
public:
    missing_residue() : siremol_error()
    {}

    missing_residue(QString err, QString place = QString::null)
                    : siremol_error(err,place)
    {}

    missing_residue(const missing_residue &other) : siremol_error(other)
    {}

    ~missing_residue() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::missing_residue";
    }
};

/** This exception is thrown when an action is requested of a non-existant CutGroup

    @author Christopher Woods
*/
class SIREMOL_EXPORT missing_cutgroup : public siremol_error
{
public:
    missing_cutgroup() : siremol_error()
    {}

    missing_cutgroup(QString err, QString place = QString::null)
                    : siremol_error(err,place)
    {}

    missing_cutgroup(const missing_cutgroup &other) : siremol_error(other)
    {}

    ~missing_cutgroup() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::missing_cutgroup";
    }
};

/** This exception is thrown when a new residue is added to a set that already
    contains a residue with the same ID

    @author Christopher Woods
*/
class SIREMOL_EXPORT duplicate_residue : public siremol_error
{
public:
    duplicate_residue() : siremol_error()
    {}

    duplicate_residue(QString err, QString place = QString::null)
                  : siremol_error(err,place)
    {}

    duplicate_residue(const duplicate_residue &other) : siremol_error(other)
    {}

    ~duplicate_residue() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::duplicate_residue";
    }
};

/** This exception is thrown when a new molecule is added to a set that already
    contains a molecule with the same ID

    @author Christopher Woods
*/
class SIREMOL_EXPORT duplicate_molecule : public siremol_error
{
public:
    duplicate_molecule() : siremol_error()
    {}

    duplicate_molecule(QString err, QString place = QString::null)
                  : siremol_error(err,place)
    {}

    duplicate_molecule(const duplicate_molecule &other) : siremol_error(other)
    {}

    ~duplicate_molecule() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::duplicate_molecule";
    }
};

/** This exception is thrown when a problem is detected when applying
    a molecule or residue template.

    @author Christopher Woods
*/
class SIREMOL_EXPORT template_error : public siremol_error
{
public:
    template_error() : siremol_error()
    {}

    template_error(QString err, QString place = QString::null)
                  : siremol_error(err,place)
    {}

    template_error(const template_error &other) : siremol_error(other)
    {}

    ~template_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::template_error";
    }
};

/** This exception is thrown when a problem is detected with anchors on moving
    atoms

    @author Christopher Woods
*/
class SIREMOL_EXPORT anchor_error : public siremol_error
{
public:
    anchor_error() : siremol_error()
    {}

    anchor_error(QString err, QString place = QString::null)
                  : siremol_error(err,place)
    {}

    anchor_error(const anchor_error &other) : siremol_error(other)
    {}

    ~anchor_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::anchor_error";
    }
};

/** This exception is thrown when a ring is detected when trying to split
    a molecule into two

    @author Christopher Woods
*/
class SIREMOL_EXPORT ring_error : public siremol_error
{
public:
    ring_error() : siremol_error()
    {}

    ring_error(QString err, QString place = QString::null)
                  : siremol_error(err,place)
    {}

    ring_error(const ring_error &other) : siremol_error(other)
    {}

    ~ring_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::ring_error";
    }
};

/** This exception is thrown when an incompatible molecule is used

    @author Christopher Woods
*/
class SIREMOL_EXPORT incompatible_molecule : public siremol_error
{
public:
    incompatible_molecule() : siremol_error()
    {}

    incompatible_molecule(QString err, QString place = QString::null)
                          : siremol_error(err,place)
    {}

    incompatible_molecule(const incompatible_molecule &other)
                          : siremol_error(other)
    {}

    ~incompatible_molecule() throw()
    {}

    const char* what() const throw()
    {
        return "SireMol::incompatible_molecule";
    }
};

}

Q_DECLARE_METATYPE(SireMol::siremol_error)
Q_DECLARE_METATYPE(SireMol::missing_atom)
Q_DECLARE_METATYPE(SireMol::missing_group)
Q_DECLARE_METATYPE(SireMol::duplicate_atom)
Q_DECLARE_METATYPE(SireMol::missing_residue)
Q_DECLARE_METATYPE(SireMol::missing_cutgroup)
Q_DECLARE_METATYPE(SireMol::duplicate_residue)
Q_DECLARE_METATYPE(SireMol::duplicate_molecule)
Q_DECLARE_METATYPE(SireMol::template_error)
Q_DECLARE_METATYPE(SireMol::anchor_error)
Q_DECLARE_METATYPE(SireMol::ring_error)
Q_DECLARE_METATYPE(SireMol::incompatible_molecule)

SIRE_END_HEADER

#endif
