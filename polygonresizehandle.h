#ifndef POLYGONRESIZEHANDLE_H
#define POLYGONRESIZEHANDLE_H

// qt
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRectF>

// forward declaration
class PolygonItem;



class PolygonResizeHandle : public QGraphicsItem
{

    public:
    enum {Type = QGraphicsItem::UserType + 1};
    int type() const override {return PolygonResizeHandle::Type;}
        PolygonResizeHandle(QGraphicsItem* parent = nullptr);

        virtual QRectF boundingRect() const override;

        class HandleItem : public QGraphicsEllipseItem
        {
            public:
                enum {Type = QGraphicsItem::UserType + 2};
                int type() const override {return HandleItem::Type;}
                HandleItem(int index, const QPointF& pos, PolygonResizeHandle* parent = nullptr);

                int index() const;

            protected:

                virtual QVariant itemChange(GraphicsItemChange change,
                                            const QVariant& value) override;

            private:

                PolygonResizeHandle* m_parent;
                uint32_t m_index;
        };


    protected:

        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:

        void resize(int index, const QPointF& newPos);
        QRectF m_boundingRect;
        PolygonItem* m_parent;
        QVector<HandleItem*> m_handles;
};
#endif // POLYGONRESIZEHANDLE_H
