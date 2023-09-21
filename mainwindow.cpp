#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sceneview.h"
#include "searchvertex.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sceneView = nullptr;
    scene = nullptr;

    QPoint pos;

    restoreState(pos);
    this->move(pos);
}

MainWindow::~MainWindow()
{
    saveState(this->pos());

    if(scene)
        delete scene;
    if(sceneView)
        delete sceneView;

    delete ui;
}


void MainWindow::on_GenerateScene_clicked()
{
    if (sceneView != nullptr)
    {
        delete sceneView;
        sceneView = nullptr;
    }

    if (scene != nullptr)
    {
        delete scene;
        scene = nullptr;
    }

    sceneView = new SceneView();
    ui->gridLayout->addWidget(sceneView);

    scene = new RenderScene(ui->widthField->value(), ui->heigthField->value());
    sceneView->setScene(scene);

    scene->initialize();
}

void MainWindow::saveState(const QPoint &posWindow)
{
    QSettings settings("SettingWindow", "Find_way");
    settings.beginGroup("PositionWindow");
        settings.setValue("position", posWindow);
    settings.endGroup();
}

void MainWindow::restoreState(QPoint &posWindow)
{
    QSettings settings("SettingWindow", "Find_way");
    settings.beginGroup("PositionWindow");
        QPoint savedPoint = settings.value("position", QPoint(400, 300)).toPoint();
        posWindow.setX(savedPoint.x());
        posWindow.setY(savedPoint.y());

    settings.endGroup();
}


