
#include "widgetanimator.h"
#include "glwidgetbase.h"

#include "SireUnits/units.h"

using namespace Spier;

WidgetAnimator::WidgetAnimator(GLWidget widget) : wdgt(widget)
{
    //start the timer
    timer.start();
}

WidgetAnimator::~WidgetAnimator()
{}

void WidgetAnimator::animate()
{
    if (wdgt)
        wdgt->setRotation( 0.1 * timer.elapsed() * SireUnits::degrees );
}
