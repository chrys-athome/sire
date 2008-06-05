#ifndef SPIER_CANVASUIPTR_H
#define SPIER_CANVASUIPTR_H

#include <QPointer>

namespace Spier
{

class CanvasUI;

/** Use a Qt guarded pointer class, which is automatically set to null
    if the pointed to CanvasUI is deleted */
typedef QPointer<CanvasUI> CanvasUIPtr;

}

#endif
