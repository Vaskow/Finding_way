#include "graphicssquarewallitem.h"

GraphicsSquareWallItem::GraphicsSquareWallItem(int x, int y, int width)
{
    setPos(QPointF(x, y));
    itemBox = QRectF(0, 0, width, width);
    blackPen = new QPen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    blackBrush = new QBrush(Qt::black, Qt::SolidPattern);
}

GraphicsSquareWallItem::~GraphicsSquareWallItem()
{
    delete blackPen;
    delete blackBrush;
}

QRectF GraphicsSquareWallItem::boundingRect() const
{
    return itemBox;
}

void GraphicsSquareWallItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(*blackPen);
    painter->setBrush(*blackBrush);
    painter->drawRect(itemBox);
}
