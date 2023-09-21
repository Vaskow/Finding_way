#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class SceneView : public QGraphicsView {
    Q_OBJECT
  public:
    explicit SceneView(QWidget *parent = nullptr);

protected:
    // Перегружаем событие прокрутки
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

};

#endif // SCENEVIEW_H
