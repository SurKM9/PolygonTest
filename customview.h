#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

// qt
#include <QGraphicsView>
#include <QWheelEvent>



class CustomView : public QGraphicsView
{
    public:

        explicit CustomView(QWidget* parent = nullptr);

    protected:

        virtual void wheelEvent(QWheelEvent* event) override;
};

#endif // CUSTOMVIEW_H
