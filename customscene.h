#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

// qt
#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>

// forward declaration
class PolygonItem;



class CustomScene : public QGraphicsScene
{

    public:

        explicit CustomScene(QObject* parent = nullptr);

    protected:

        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    private:

//        QPointF m_origPoint;
        PolygonItem* m_polygon;
//        QPainterPath m_path;
        QVector<QPointF> m_points;
        void storePoint(const QPointF& point);
        void previewPoint(const QPointF& point);
        void updatePolygon();

};

#endif // CUSTOMSCENE_H
