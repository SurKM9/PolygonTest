#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// qt
#include <QMainWindow>

// forward declaration
class CustomScene;
class CustomView;
class KeyPointItem;



class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:

        CustomScene* m_scene;
        CustomView* m_view;
        KeyPointItem* m_keyPoint;
};
#endif // MAINWINDOW_H
