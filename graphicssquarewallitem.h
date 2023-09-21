#ifndef GRAPHICSSQUAREWALLITEM_H
#define GRAPHICSSQUAREWALLITEM_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsSquareWallItem : public QGraphicsItem
{
public:
    GraphicsSquareWallItem(int x, int y, int width);
    ~GraphicsSquareWallItem();

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

  private:
    QRectF itemBox;
    QColor colorWallSquare = Qt::gray;
    QPen *blackPen;
    QBrush *blackBrush;
};

#endif // GRAPHICSSQUAREWALLITEM_H
