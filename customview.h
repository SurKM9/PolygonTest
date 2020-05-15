#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

// qt
#include <QGraphicsView>
#include <QWheelEvent>

class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CustomView(QWidget *parent = nullptr);

signals:
    void zoomChanged(qreal percent);

protected:
    virtual void wheelEvent(QWheelEvent *event) override;

    // this one is to set the zoom level in "X% of original" way:
    // original level is 1.0 (100%)
    // setZoom(0.5) zoomed out x2 (50% of original)
    // setZoom(4) zoomed in x4 (400% of original)
    void setZoom(double percent);

    // these two are helpers to manage the limits of zoom levels.
    // may be usefull if you need things like "a combobox to quickly jump to certin scale", etc
    // providing appropriate setters, you can made the ranve be dynamic - may be usefull for some scenes.
    qreal minZoomLevel() const;
    qreal maxZoomLevel() const;

    // the value to change scale on each mouse wheel turn.
    // currently it's just hardcoded 5%, but you can adwance it
    // to de-/accelerate scaling near the scaling range edges.
    qreal zoomStepPercent() const;

private:
    qreal m_zoomPercent;
};

#endif // CUSTOMVIEW_H
