
#include "smarts.h"

using namespace SireDB;

/** Null constructor */
Smarts::Smarts()
{}

/** Copy constructor */
Smarts::Smarts(const Smarts&)
{}

/** Destructor */
Smarts::~Smarts()
{}

/** Comparison operator */
bool Smarts::operator==(const Smarts&) const
{
    return true;
}

/** Return the Smarts string */
QString Smarts::toString() const
{
    return "Smarts";
}

/** Is the string empty - of course it is! */
bool Smarts::isEmpty() const
{
    return true;
}
