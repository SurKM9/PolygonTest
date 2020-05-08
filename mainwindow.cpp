// qt
#include <QGraphicsView>

// local
#include "mainwindow.h"
#include "customscene.h"



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QGraphicsView* view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    m_scene = new CustomScene(view);

    view->setScene(m_scene);

    setCentralWidget(view);

    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

