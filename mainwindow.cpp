// qt
#include <QGraphicsView>

// local
#include "customscene.h"
#include "customview.h"
#include "interactiveobject.h"
#include "mainwindow.h"
#include "polygonitem.h"
#include "rectangleitem.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_view = new CustomView(this);
    m_scene = new CustomScene(m_view);

    m_scene->setSceneRect(0, 0, 640, 480);

    QPixmap pix = QPixmap::fromImage(QImage(":/images/resources/download_2.jpg"));

    m_scene->addPixmap(pix);

    RectangleItem* dummyRect = new RectangleItem;
    dummyRect->updateRect({50, 50, 300, 300});
    m_scene->addItem(dummyRect);

    PolygonItem* dummyPoly = new PolygonItem;
    for( const QPointF& point : std::initializer_list<QPointF> { {250., 400.}, {400., 100.}, {550., 400.}})
    dummyPoly->appendPoint(point);
    m_scene->addItem(dummyPoly);

    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
    connect(m_view, &CustomView::zoomChanged, this, [this](qreal /*percent*/)
    {
        for (auto item : m_scene->selectedItems())
        {
            if (auto iObj = qobject_cast<InteractiveObject*>(item->toGraphicsObject()))
            {
                iObj->updateGripPoints();
            }
        }
    });

    m_view->setScene(m_scene);

    setCentralWidget(m_view);

    resize(800, 600);
}



MainWindow::~MainWindow() {}
