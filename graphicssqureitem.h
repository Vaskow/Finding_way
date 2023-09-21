#ifndef GRAPHICSSQUREITEM_H
#define GRAPHICSSQUREITEM_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsSquareItem : public QGraphicsItem
{
  public:
    GraphicsSquareItem(int x, int y, int width, int index);
    ~GraphicsSquareItem();

    void resetStartState();
    void resetLastState();
    int getIndexSquar();

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    void processLeftClick();

  private:
    QRectF itemBox;

    QColor colorIdleState = Qt::gray;
    QColor colorStartState = Qt::green;
    QColor colorLastState = Qt::darkRed;

    QColor curSquareColor = colorIdleState;
    bool activeStartState = false;
    bool activeLastState = false;
    int indexSquare;

    static inline bool useStartState{};
    static inline bool useLastState{};
};

#endif // GRAPHICSSQUREITEM_H
