#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

// qt
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QPointF>

// forward declaration
class PolygonItem;
class RectangleItem;

class CustomScene : public QGraphicsScene
{

    public:
        explicit CustomScene(QObject* parent = nullptr);

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    private:
        PolygonItem* m_polygon;
        RectangleItem* m_rectangle;
        QPointF m_origPoint;
        void storePoint(const QPointF& point);
        void previewPoint(const QPointF& point);
};

#endif // CUSTOMSCENE_H
