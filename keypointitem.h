#ifndef KEYPOINT_ITEM_H
#define KEYPOINT_ITEM_H

// qt
#include <QGraphicsPathItem>

// local
#include "itemtypes.h"
#include "interactiveobject.h"


class QGraphicsPathItem;
class KeyPointItem : public InteractiveObject
{

    public:

        explicit KeyPointItem(QGraphicsItem* parentItem = nullptr);

        enum
        {
            Type = UserType + static_cast<int>(myitems::Type::ItemKeyPoint)
        };

        int type() const override
        {
            return Type;
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;


        QRectF boundingRect() const override;

        void updateGripPoints() override;
        QVector<QPointF> localPoints() const;
        QPainterPath shape() const override;

    protected:

        void onManualMoveStart(GripPoint* gp, const QPointF& at) override;


        void onManualMoveProgress(GripPoint* gp, const QPointF& from, const QPointF& to) override;


        void onManualMoveFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt) override;


        void onManualResizeStart(GripPoint* gp, const QPointF& at) override;


        void onManualResizeProgress(GripPoint* gp, const QPointF& from, const QPointF& to) override;


        void onManualResizeFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt) override;


        void initGripPoints() override;

        void updatePoint(int at, const QPointF& scenePoint);

    protected Q_SLOTS:

        void onSelectionChanged(bool isSelected) override;

    private:

        void updateBoundingRect();

        QGraphicsPathItem* m_item = nullptr;
        QVector<QPointF> m_points;
};
#endif // KEYPOINT_ITEM_H
