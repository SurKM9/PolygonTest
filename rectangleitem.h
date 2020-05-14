#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H
// qt
#include <QGraphicsRectItem>

// local
#include "interactiveobject.h"
#include "itemtypes.h"



class RectangleItem : public InteractiveObject
{
    public:

        explicit RectangleItem(QGraphicsItem* parent = nullptr);
        ~RectangleItem() = default;

        enum
        {
            Type = UserType + static_cast<int>(myitems::Type::ItemRectangle)
        };

        // QGraphicsItem interface
        virtual QRectF boundingRect() const override;
        virtual QPainterPath shape() const override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        int type() const override;

        virtual void updateGripPoints() override;
        virtual void initGripPoints() override;

        void updateRect(const QRectF& rect);
        QVector<QPointF> localPoints() const;

        // InteractiveObject interface
    protected:
        virtual void onManualMoveStart(GripPoint* gp, const QPointF& at) override;
        virtual void onManualMoveProgress(GripPoint* gp, const QPointF& from, const QPointF& to) override;
        virtual void onManualMoveFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt) override;
        virtual void onManualResizeStart(GripPoint* gp, const QPointF& at) override;
        virtual void onManualResizeProgress(GripPoint* gp, const QPointF& from, const QPointF& to) override;
        virtual void onManualResizeFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt) override;

    protected Q_SLOTS:

        void onSelectionChanged(bool isSelected) override;

    private:

        void updateBoundingRect();

        QGraphicsRectItem* m_item = nullptr;
        QRectF m_rect;
        QPointF m_initPoint;

        // InteractiveObject interface
    public:

};

#endif // RECTANGLEITEM_H
