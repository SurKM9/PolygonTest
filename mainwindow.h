#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// qt
#include <QMainWindow>

// forward declaration
class CustomScene;
class CustomView;



class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:

        CustomScene* m_scene;
        CustomView* m_view;
};
#endif // MAINWINDOW_H
