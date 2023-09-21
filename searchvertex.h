#ifndef SEARCHVERTEX_H
#define SEARCHVERTEX_H
#include <QObject>
#include <QVector>
#include <set>
using namespace std;

class SearchVertexRectGraph : public QObject
{
    Q_OBJECT
public:
    SearchVertexRectGraph();
    ~SearchVertexRectGraph();
    //Построение списка смежности графа
    void buildAdjacListGraph(int cntVertexWidth, int cntVertexHeight, set<int> &wallVert);

    //Алгоритм поиска в ширину от стартовой вершины (startVert)
    pair<QVector<int>, QVector<int>> bfs(int startVert);
    //Получение пути до вершины finish из полученного в bfs() пути от startVert
    QVector<int> getPath(QVector<int> &from, int finish);
    //Установка вершин для работы алгоритма поиска пути
    void setIndexVertPath(unsigned int indexStartVert, unsigned int indexLastVert);
    //Установка состояния процесса поиска вершин
    void setRunProcGetPath(bool run);

signals:
    void finished();
    void paintPathSquare(QVector<int> from);

public slots:
    void processGetPath(); //функция поиска пути для выполнения в потоке

private:
    int cntVertexWidth;
    int cntVertexHeight;
    int indexStartVert;
    int indexLastVert;
    QVector<QVector<int>> adjacVert; //вектор вершин с векторами смежных вершин графа
    atomic_bool needGetPath; //флаг для получения нового пути после смены граничных вершин пути
    atomic_bool runProcGetPath; //запуск или остановка процесса получения пути
};

#endif // SEARCHVERTEX_H
