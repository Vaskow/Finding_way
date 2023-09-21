#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include <graphicssqureitem.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QVector>
#include <set>
#include "searchvertex.h"

class RenderScene : public QGraphicsScene
{
    Q_OBJECT
public:
    RenderScene(int widthScene, int heigthScene);
    ~RenderScene();

    void initialize();

private:
    int widthScene;  //ширина сцены в квадратах
    int heigthScene; //высота сцены в квадратах
    int sideSquare = 30;    //длина стороны квадрата
    int offsetDrawLetter;   //смещение для рисования букв на граничных точках

    GraphicsSquareItem* startWaySquare = nullptr;
    GraphicsSquareItem* lastWaySquare = nullptr;
    GraphicsSquareItem* possiblWaySquare = nullptr;
    QGraphicsSimpleTextItem *textItemStart;
    QGraphicsSimpleTextItem *textItemLast;

    enum class ResetState {ResetStartState, ResetLastState};
    ResetState queueResetState; //очередь сброса состояния выбранного квадрата

    set<int> wallVertices;      //вектор квадратов-стен
    QVector<int> wayFromLastVert;   //вектор квадратов-стен
    QVector<int> fromStartVert;     //вектор посещения для i-х вершин, построенный от lastWaySquare
    QVector<int> indSqItmOnScene;   //сдвиг вершин для получения проходимых элементов сцены по индексу
    QVector<GraphicsSquareItem*> squarItmOnScene;   //проходимые вершины на сцене

    SearchVertexRectGraph *searchVert; //объект для нахождения пути между вершинами

    QThread* threadGetPath; //поток, в котором будет находится путь между вершинами

    QList<QGraphicsLineItem*> listlinePath; //список объектов линий основного пути
    QList<QGraphicsLineItem*> listlinePossiblPath; //список объектов линий возможного пути
    QList<QGraphicsItem*> itemsMoveEvent;   //список получаемых объектов для события MoveEvent

    QPen* penForMainPath;
    QPen* penForPossPath;

    bool paintMainPath; //флаг для типа отрисовки пути между заданными вершинами (основной или возможный пути)

    void makeThreadGetPath(); //создание потока для нахождения путей между заданными вершинами
    void paintMainPathSquare(QVector<int> &path); //функция рисования основного пути между вершинами
    void paintPossiblPathSquare(QVector<int> &path); //функция рисования возможного пути между вершинами

private slots:
    void paintPathSquare(QVector<int> path); //общая функция рисования пути в зависимости от флага paintMainPath

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // RENDERSCENE_H
