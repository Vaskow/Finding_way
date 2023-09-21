#include "searchvertex.h"
#include <queue>
#include <algorithm>

SearchVertexRectGraph::SearchVertexRectGraph()
{
    cntVertexWidth = 0;
    cntVertexHeight = 0;
    indexStartVert = -1;
    indexLastVert = -1;
    needGetPath = false;
    runProcGetPath = false;
}

SearchVertexRectGraph::~SearchVertexRectGraph()
{
}

void SearchVertexRectGraph::buildAdjacListGraph(int _cntVertexWidth, int _cntVertexHeight, set<int> &wallVert)
{
    cntVertexWidth = _cntVertexWidth;
    cntVertexHeight = _cntVertexHeight;
    int countVertGraph = cntVertexWidth * cntVertexHeight;
    int vertAbove = 0;
    int vertBelow = 0;
    int neighbour = 0;
    int numVertex = 0;
    adjacVert.resize(countVertGraph);

    for (int numHeight = 0; numHeight < cntVertexHeight; ++numHeight)
    {
        for(int numWidth = 0; numWidth < cntVertexWidth; ++numWidth)
        {
            if (numVertex < countVertGraph)
            {
                if (wallVert.count(numVertex)) //поиск вершины в списке непроходимых вершин
                {
                    numVertex++;
                    continue;
                }
            }

            //Соседи слева и справа
            if (numWidth != cntVertexWidth - 1) //вершина не на правом краю графа
            {
                neighbour = numVertex + 1;
                adjacVert[numVertex].push_back(neighbour);
                if (numWidth >= 1)
                {
                    neighbour = numVertex - 1;
                    adjacVert[numVertex].push_back(neighbour);
                }
            }
            else
            {
                adjacVert[numVertex].push_back(numVertex - 1);
            }

            //Соседи сверху и снизу
            if (numHeight >= 1 && numHeight != cntVertexHeight - 1)
            {
                vertAbove = numVertex - cntVertexWidth;
                vertBelow = numVertex + cntVertexWidth;
                adjacVert[numVertex].push_back(vertAbove);
                adjacVert[numVertex].push_back(vertBelow);
            }
            else if (numHeight == 0)
            {
                vertBelow = numVertex + cntVertexWidth;
                adjacVert[numVertex].push_back(vertBelow);
            }
            else if (numHeight == cntVertexHeight - 1)
            {
                vertAbove = numVertex - cntVertexWidth;
                adjacVert[numVertex].push_back(vertAbove);
            }

            numVertex++;
        }
    }
}

void SearchVertexRectGraph::setIndexVertPath(unsigned int _indexStartVert, unsigned int _indexLastVert)
{
    indexStartVert = _indexStartVert;
    indexLastVert = _indexLastVert;
    needGetPath = true; //состояние для получения нового пути
}

void SearchVertexRectGraph::setRunProcGetPath(bool run)
{
    if (runProcGetPath == run)
        return;

    runProcGetPath = run;
}

void SearchVertexRectGraph::processGetPath()
{
    if (adjacVert.empty() || indexStartVert == -1 || indexLastVert == -1)
    {
        emit finished();
        qDebug("Неправильно организована работа с SearchVertexRectGraph");
        return;
    }
    pair<QVector<int>, QVector<int>> pairBfs;
    QVector<int> path;

    while (runProcGetPath)
    {
        if (needGetPath)
        {
            pairBfs = bfs(indexStartVert);
            path = getPath(pairBfs.second, indexLastVert);
            emit paintPathSquare(path);
            needGetPath = false;
        }
    }

    emit finished();

    return;
}

pair<QVector<int>, QVector<int>> SearchVertexRectGraph::bfs(int startVert)
{
    const int INF = 1e9;
    QVector<int> dist(adjacVert.size(), INF); //вектор кратчайших расстояний от startVert
    //Вектор, где вершина, из которой пришли, хранится в i-й вершине. Для стартовой вершины "-1"
    QVector<int> from(adjacVert.size(), -1);
    queue<int> queUntravelAdjVert; //очередь непройденных смежных вершин

    if (adjacVert.size() == 0)
    {
        return {dist, from};
    }

    dist[startVert] = 0;
    queUntravelAdjVert.push(startVert);

    while (!queUntravelAdjVert.empty())
    {
        int curVert = queUntravelAdjVert.front();
        queUntravelAdjVert.pop();
        for (int to : adjacVert[curVert]) //просмотр смежных вершин для вершины curVert
        {
            if (dist[to] > dist[curVert] + 1) //смежная вершина не была посещена (INF)
            {
                dist[to] = dist[curVert] + 1; //находится на "1" дальше curVert
                from[to] = curVert;
                queUntravelAdjVert.push(to);
            }
        }
    }
    return {dist, from};
}

QVector<int> SearchVertexRectGraph::getPath(QVector<int> &from, int finish)
{
    QVector<int> path;
    for (int v = finish; v != -1; v = from[v])
        path.push_back(v);
    reverse(path.begin(), path.end()); //разворачиваем путь от конечной вершины
    return path;
}
