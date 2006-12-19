#ifndef SIREFF_PARAMETERMAP_H
#define SIREFF_PARAMETERMAP_H

#include <QHash>
#include <QString>
#include <QList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

class ParameterSource;

class SIREFF_EXPORT ParameterName
{
public:
    ParameterName(const QString &name = QString::null);
    ParameterName(const QString &name, const QString &defaultsource);
    
    ParameterName(const ParameterName &other);
    
    ~ParameterName();
    
    ParameterSource operator==(const QString &source) const;

    bool isValid() const;
    
    const QString& name() const;
    const QString& defaultSource() const;

private:
    /** The name of the parameter */
    QString param_name;
    
    /** The default source of the parameter */
    QString default_source;
};

class SIREFF_EXPORT ParameterSource : public ParameterName
{

friend class ParameterName;

public:
    ParameterSource();
    
    ParameterSource(const ParameterSource &other);
    
    ~ParameterSource();
    
    bool isValid() const;
    bool isDefault() const;
    
    const QString& source() const;
    
protected:
    ParameterSource(const ParameterName &name, const QString &value);
    
private:
    /** The name of the parameter */
    QString param_name;
    /** The source of the parameter (property name) */
    QString param_source;
};

class SIREFF_EXPORT ParameterMap
{
public:
    ParameterMap();
    
    ParameterMap(const ParameterSource &source);
    ParameterMap(const QList<ParameterSource> &sources);
    
    ParameterMap(const ParameterMap &other);
    
    ~ParameterMap();
    
    ParameterMap operator&&(const ParameterMap &other) const;
    
    QString source(const ParameterName &param) const;
    
private:
    void add(const ParameterSource &source);
    
    /** Hash mapping the name of one type of 
        parameters to the property containing
        those parameters */
    QHash<QString,QString> map; 
};

}

SIRE_END_HEADER

#endif
