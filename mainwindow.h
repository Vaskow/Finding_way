#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sceneview.h"
#include "renderscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_GenerateScene_clicked();

  private:
    void saveState(const QPoint &posWindow);
    void restoreState(QPoint &posWindow);

    Ui::MainWindow *ui;
    SceneView *sceneView;
    RenderScene *scene;
};
#endif // MAINWINDOW_H
