#ifndef POLYGONRESIZEHANDLE_H
#define POLYGONRESIZEHANDLE_H

// qt
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRectF>

// forward declaration
class PolygonItem;



class PolygonResizeHandle : public QGraphicsEllipseItem
{

    public:

        explicit PolygonResizeHandle(QGraphicsItem* parent = nullptr);

        virtual QRectF boundingRect() const override;

        void setIndex(uint32_t index);

        uint32_t index() const;

    protected:

        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    private:

        QRectF m_boundingRect;
        uint32_t m_index;
        PolygonItem* m_parent;
};
#endif // POLYGONRESIZEHANDLE_H
