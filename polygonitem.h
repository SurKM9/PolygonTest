#ifndef POLYGONITEM_H
#define POLYGONITEM_H

// qt
#include <QGraphicsPathItem>



class PolygonItem : public QGraphicsPathItem
{
    public:

        PolygonItem(const QPainterPath& path, QGraphicsItem* parent = nullptr);

        void updatePath(const QPainterPath& path);

    protected:

        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    private:

        QPainterPath m_path;
        QRectF m_boundingRect;
};
#endif // POLYGONITEM_H
