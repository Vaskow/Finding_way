#include "sceneview.h"

SceneView::SceneView(QWidget *parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true); //включение обработки сообытия MouseMove
}

void SceneView::wheelEvent(QWheelEvent *event)
{
    const ViewportAnchor anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = event->angleDelta().y();
    qreal factor;
    if (angle > 0)
    {
        factor = 1.1;
    }
    else
    {
        factor = 0.9;
    }
    scale(factor, factor);
    setTransformationAnchor(anchor);
}
