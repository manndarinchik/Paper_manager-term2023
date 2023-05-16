#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "basewindow.h"
#include "psqlinterface.h"

class MainWindow : public BaseWindow
{
    Q_OBJECT

public:
    MainWindow(PSQLInterface* psqli);
public slots:
    void init();

signals:

private:
};
#endif // MAINWINDOW_H
