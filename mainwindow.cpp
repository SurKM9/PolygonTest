// qt
#include <QGraphicsView>

// local
#include "mainwindow.h"
#include "customscene.h"
#include "customview.h"



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_view = new CustomView(this);
    m_scene = new CustomScene(m_view);

    m_scene->setSceneRect(0, 0, 640, 480);

    QPixmap pix = QPixmap::fromImage(QImage(":/images/resources/download_2.jpg"));

    m_scene->addPixmap(pix);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);

    m_view->setScene(m_scene);

    setCentralWidget(m_view);

    resize(800, 600);
}



MainWindow::~MainWindow()
{
}

