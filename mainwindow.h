#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// qt
#include <QMainWindow>

// forward declaration
class CustomScene;



class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:

        CustomScene* m_scene;
};
#endif // MAINWINDOW_H
