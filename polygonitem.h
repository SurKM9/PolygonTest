#ifndef POLYGONITEM_H
#define POLYGONITEM_H

// qt
#include "interactiveobject.h"
#include "itemtypes.h"

#include <QGraphicsPathItem>

class QGraphicsPathItem;
class PolygonItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit PolygonItem(QGraphicsItem *parent = nullptr);
    ~PolygonItem() = default;

    enum
    {
        Type = UserType + static_cast<int>(myitems::Type::ItemPolygon)
    };
    int type() const override { return Type; }

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void initGripPoints() override;
    void updateGripPoints() override;

    QVector<QPointF> localPoints() const;
    void appendPoint(const QPointF &scenePoint);
    void updatePoint(int at, const QPointF &scenePoint);

protected:
    void onManualMoveStart(GripPoint *gp, const QPointF &at) override;
    void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt) override;

    void onManualResizeStart(GripPoint *gp, const QPointF &at) override;
    void onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualResizeFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt) override;

protected Q_SLOTS:
    void onSelectionChanged(bool isSelected) override;

private:
    void updateBoundingRect();

    QGraphicsPathItem *m_item = nullptr;
    QVector<QPointF> m_points;
};

#endif // POLYGONITEM_H
