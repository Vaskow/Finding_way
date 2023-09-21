#include "renderscene.h"
#include "graphicssquarewallitem.h"
#include <QMessageBox>
#include <QThread>

RenderScene::RenderScene(int _widthScene, int _heigthScene) :
    widthScene(_widthScene), heigthScene(_heigthScene)
{
    queueResetState = ResetState::ResetStartState;

    int maxIndVert = widthScene * heigthScene;
    int amountWall = 1;
    if (maxIndVert >= 8) {amountWall = maxIndVert / 4;}

    for(int i = 0; i < amountWall; ++i)
    {
        wallVertices.insert(qrand() % maxIndVert);
    }
    //Построение списка связанности графа
    searchVert = new SearchVertexRectGraph();
    searchVert->buildAdjacListGraph(widthScene, heigthScene, wallVertices);

    paintMainPath = false;
    threadGetPath = nullptr;

    textItemStart = new QGraphicsSimpleTextItem();
    QFont font = QFont("Arial", 10, QFont::Normal);
    textItemStart->setFont(font);
    textItemStart->setText("A");

    textItemLast = new QGraphicsSimpleTextItem();
    textItemLast->setFont(font);
    textItemLast->setText("B");

    penForMainPath = new QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    penForPossPath = new QPen(Qt::blue, 1, Qt::DotLine, Qt::SquareCap, Qt::MiterJoin);

    qRegisterMetaType<QVector<int>>("QVector<int>");
}

RenderScene::~RenderScene()
{
    if (threadGetPath)
    {
        searchVert->setRunProcGetPath(false);
        threadGetPath->quit();
        threadGetPath->wait();
    }

    clear();
    delete penForMainPath;
    delete penForPossPath;
}

void RenderScene::initialize()
{
    if (widthScene == 1 || heigthScene == 1)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Пояснение");
        msgBox.setText("Нельзя задавать одномерную длину");
        msgBox.exec();
        return;
    }

    int cntSquarItem = 0;
    int indSquare = 0;
    set<int>::iterator iterWallVert = wallVertices.begin();

    for(int y = 0; y < heigthScene * sideSquare; y += sideSquare)
    {
        for(int x = 0; x < widthScene * sideSquare; x += sideSquare)
        {
            if (indSquare  == *iterWallVert)
            {
                iterWallVert++;
                GraphicsSquareWallItem* sqWallItem = new GraphicsSquareWallItem(x, y, sideSquare);
                addItem(sqWallItem);
            }
            else
            {
                GraphicsSquareItem* sqItem = new GraphicsSquareItem(x, y, sideSquare, indSquare);
                addItem(sqItem);
                cntSquarItem++;
                squarItmOnScene.push_back(sqItem);
            }

            indSquare++;
            indSqItmOnScene.push_back(cntSquarItem);
        }
    }
}

void RenderScene::makeThreadGetPath()
{
    threadGetPath = new QThread();
    searchVert->moveToThread(threadGetPath);

    // При запуске потока выполняется получение пути для заданных вершин
    QObject::connect(threadGetPath, &QThread::started, searchVert, &SearchVertexRectGraph::processGetPath);
    // Соединение для отрисовки получаемого пути
    QObject::connect(searchVert, &SearchVertexRectGraph::paintPathSquare, this, &RenderScene::paintPathSquare);
    // По сигналу finished отправляем команду на завершение потока
    QObject::connect(searchVert, &SearchVertexRectGraph::finished, threadGetPath, &QThread::quit);
    // По сигналу finished удаляем объект поиска пути
    QObject::connect(searchVert, &SearchVertexRectGraph::finished, searchVert, &QObject::deleteLater);
    // Когда закончит работу поток, удаляем и его
    QObject::connect(threadGetPath, &QThread::finished, threadGetPath, &QObject::deleteLater);

    searchVert->setRunProcGetPath(true);

    threadGetPath->start();
}

void RenderScene::paintPathSquare(QVector<int> path)
{
    if (paintMainPath)
    {
        paintMainPathSquare(path);
    }
    else
    {
        paintPossiblPathSquare(path);
    }
}

void RenderScene::paintMainPathSquare(QVector<int> &path)
{
    int centerX1 = 0; //координаты стартовой вершины
    int centerY1 = 0;
    int centerX2 = 0; //координаты конечной вершины
    int centerY2 = 0;
    int offsetCentr = 8; //смещение от центра при рисовании основного пути
    int ind1 = 0;
    int ind2 = 0;

    for(int indSq = 0; indSq < path.size()-1; ++indSq)
    {
        //Определяем индекс проходимой вершины на сцене
        ind1 = indSqItmOnScene[path[indSq]] - 1;
        ind2 = indSqItmOnScene[path[indSq+1]] - 1;
        centerX1 = squarItmOnScene[ind1]->x() + sideSquare / 2;
        centerY1 = squarItmOnScene[ind1]->y() + sideSquare / 2;
        centerX2 = squarItmOnScene[ind2]->x() + sideSquare / 2;
        centerY2 = squarItmOnScene[ind2]->y() + sideSquare / 2;

        //Корректировка линий, чтобы они не рисовались от центра
        if (indSq == 0) //для стартовой вершины
        {
            if (centerX2 > centerX1) centerX1 += offsetCentr;
            else if (centerX2 < centerX1) centerX1 -= offsetCentr;
            else if (centerY2 > centerY1) centerY1 += offsetCentr;
            else if (centerY2 < centerY1) centerY1 -= offsetCentr;

            if (path.size() == 2) //если между стартовой и конечной нет вершин
            {
                if (centerX1 > centerX2) centerX2 += offsetCentr;
                else if (centerX1 < centerX2) centerX2 -= offsetCentr;
                else if (centerY1 > centerY2) centerY2 += offsetCentr;
                else if (centerY1 < centerY2) centerY2 -= offsetCentr;
            }
        }
        else if (indSq == path.size()-2) //для конечной вершины
        {
            if (centerX1 > centerX2) centerX2 += offsetCentr;
            else if (centerX1 < centerX2) centerX2 -= offsetCentr;
            else if (centerY1 > centerY2) centerY2 += offsetCentr;
            else if (centerY1 < centerY2) centerY2 -= offsetCentr;
        }

        QGraphicsLineItem* lineItem = new QGraphicsLineItem(centerX1, centerY1,
                                                        centerX2, centerY2);
        lineItem->setPen(*penForMainPath);

        listlinePath.push_back(lineItem);
        addItem(lineItem);
    }
}

void RenderScene::paintPossiblPathSquare(QVector<int> &path)
{
    int centerX1 = 0; //координаты стартовой вершины
    int centerY1 = 0;
    int centerX2 = 0; //координаты конечной вершины
    int centerY2 = 0;
    int ind1 = 0;
    int ind2 = 0;

    for(int indSq = 0; indSq < path.size()-1; ++indSq)
    {
        //Определяем индекс проходимой вершины на сцене
        ind1 = indSqItmOnScene[path[indSq]] - 1;
        ind2 = indSqItmOnScene[path[indSq+1]] - 1;
        centerX1 = squarItmOnScene[ind1]->x() + sideSquare / 2;
        centerY1 = squarItmOnScene[ind1]->y() + sideSquare / 2;
        centerX2 = squarItmOnScene[ind2]->x() + sideSquare / 2;
        centerY2 = squarItmOnScene[ind2]->y() + sideSquare / 2;
        QGraphicsLineItem* lineItem = new QGraphicsLineItem(centerX1, centerY1,
                                                        centerX2, centerY2);
        lineItem->setPen(*penForPossPath);

        listlinePossiblPath.push_back(lineItem);
        addItem(lineItem);
    }
}


void RenderScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QList<QGraphicsItem*> itemsUnderCursor = items(event->scenePos());
        for(auto& it : itemsUnderCursor)
        {
            GraphicsSquareItem* sqItm = dynamic_cast<GraphicsSquareItem*>(it);
            if(sqItm)
            {
                if (startWaySquare && lastWaySquare)
                {
                    if (queueResetState == ResetState::ResetStartState)
                    {
                        if (sqItm == lastWaySquare)
                        {
                            QMessageBox msgBox;
                            msgBox.setWindowTitle("Пояснение");
                            msgBox.setText("Нельзя задавать точку A в точке B");
                            msgBox.exec();
                            break;
                        }
                        startWaySquare->resetStartState();
                        sqItm->processLeftClick();
                        startWaySquare = sqItm;
                        queueResetState = ResetState::ResetLastState;

                        //Добавление буквы
                        textItemStart->setPos(startWaySquare->x()+11, startWaySquare->y()+7);

                        for (auto itemLine : listlinePath) //удаление линий основного пути
                        {
                            removeItem(itemLine);
                        }
                        listlinePath.clear();
                    }
                    else if (queueResetState == ResetState::ResetLastState)
                    {
                        if (sqItm == startWaySquare)
                        {
                            QMessageBox msgBox;
                            msgBox.setWindowTitle("Пояснение");
                            msgBox.setText("Нельзя задавать точку B в точке A");
                            msgBox.exec();
                            break;
                        }
                        lastWaySquare->resetLastState();
                        sqItm->processLeftClick();
                        lastWaySquare = sqItm;
                        queueResetState = ResetState::ResetStartState;

                        //Добавление буквы
                        textItemLast->setPos(lastWaySquare->x()+11, lastWaySquare->y()+7);

                        paintMainPath = true; //отрисовка основного пути
                        //Установка стартовой и конечной точек для получение пути
                        searchVert->setIndexVertPath(startWaySquare->getIndexSquar(), lastWaySquare->getIndexSquar());
                    }
                }
                else if (!startWaySquare)
                {
                    sqItm->processLeftClick();
                    startWaySquare = sqItm;

                    //Добавление буквы
                    textItemStart->setPos(startWaySquare->x()+11, startWaySquare->y()+7);
                    addItem(textItemStart);
                }
                else if (!lastWaySquare)
                {
                    if (sqItm == startWaySquare)
                    {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("Пояснение");
                        msgBox.setText("Нельзя задавать точку B в точке A");
                        msgBox.exec();
                        break;
                    }
                    sqItm->processLeftClick();
                    lastWaySquare = sqItm;

                    //Добавление буквы
                    textItemLast->setPos(lastWaySquare->x()+11, lastWaySquare->y()+7);
                    addItem(textItemLast);

                    paintMainPath = true; //отрисовка основного пути
                    if (threadGetPath != nullptr)
                    {
                        //Установка стартовой и конечной точек для получение пути
                        searchVert->setIndexVertPath(startWaySquare->getIndexSquar(), lastWaySquare->getIndexSquar());
                    }
                    else
                    {
                        makeThreadGetPath(); //cоздание потока для получения пути
                        searchVert->setIndexVertPath(startWaySquare->getIndexSquar(), lastWaySquare->getIndexSquar());
                    }
                }

                break;
            }
        }
    }
}

void RenderScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    itemsMoveEvent = items(event->scenePos());

    if (itemsMoveEvent.isEmpty())
    {
        if (!listlinePossiblPath.isEmpty())
        {
            for (auto itemLine : listlinePossiblPath) //удаление линий возможного пути со сцены
            {
                removeItem(itemLine);
            }

            listlinePossiblPath.clear();
        }

        possiblWaySquare = nullptr;

        return;
    }

    for(auto& it : itemsMoveEvent)
    {
        GraphicsSquareItem* sqItm = dynamic_cast<GraphicsSquareItem*>(it);

        if(sqItm)
        {
            if (possiblWaySquare == sqItm)
            {
                break;
            }

            if (startWaySquare && startWaySquare != sqItm)
            {
                possiblWaySquare = sqItm;

                if (!listlinePossiblPath.isEmpty())
                {
                    for (auto itemLine : listlinePossiblPath) //удаление линий возможного пути
                    {
                        removeItem(itemLine);
                    }

                    listlinePossiblPath.clear();
                }

                paintMainPath = false; //отрисовка возможного пути
                if (threadGetPath != nullptr)
                {
                    //Установка стартовой и конечной точек для получение пути
                    searchVert->setIndexVertPath(startWaySquare->getIndexSquar(), possiblWaySquare->getIndexSquar());
                }
                else
                {
                    makeThreadGetPath(); //cоздание потока для получения пути
                    searchVert->setIndexVertPath(startWaySquare->getIndexSquar(), possiblWaySquare->getIndexSquar());
                }
            }

            break;
        }
        else
        {
            if (!listlinePossiblPath.isEmpty())
            {
                for (auto itemLine : listlinePossiblPath) //удаление линий возможного пути
                {
                    removeItem(itemLine);
                }

                listlinePossiblPath.clear();
            }
            possiblWaySquare = nullptr;
        }
    }
}
