#ifndef VSTVIEW_H
#define VSTVIEW_H

#include "objectview.h"

namespace View {

    class VstView : public ObjectView
    {
    public:
        VstView(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    };

}

#endif // VSTVIEW_H
