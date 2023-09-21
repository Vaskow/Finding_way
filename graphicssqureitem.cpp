#include "graphicssqureitem.h"
#include <QDebug>

GraphicsSquareItem::GraphicsSquareItem(int x, int y, int width, int index)
{
    setPos(QPointF(x, y));
    itemBox = QRectF(0, 0, width, width);
    indexSquare = index;
}

GraphicsSquareItem::~GraphicsSquareItem()
{
    useStartState = false;
    useLastState = false;
}

void GraphicsSquareItem::resetStartState()
{
    if (activeStartState)
    {
        activeStartState = false;
        useStartState = false;
        curSquareColor = colorIdleState;
        update();
    }
}

void GraphicsSquareItem::resetLastState()
{
    if (activeLastState)
    {
        activeLastState = false;
        useLastState = false;
        curSquareColor = colorIdleState;
        update();
    }
}

int GraphicsSquareItem::getIndexSquar()
{
    return indexSquare;
}

QRectF GraphicsSquareItem::boundingRect() const
{
    return itemBox;
}

void GraphicsSquareItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(itemBox, curSquareColor);
    painter->drawRect(itemBox);
}

void GraphicsSquareItem::processLeftClick()
{
    if (!useStartState && !activeLastState)
    {
        activeStartState = true;
        curSquareColor = colorStartState;
        useStartState = true;
    }
    else if (!useLastState && !activeStartState)
    {
        activeLastState = true;
        curSquareColor = colorLastState;
        useLastState = true;
    }

    update();
}

