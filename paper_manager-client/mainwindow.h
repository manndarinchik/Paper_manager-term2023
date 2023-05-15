#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "psqlinterface.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PSQLInterface* psqli, QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void init();

signals:


private:
    PSQLInterface* db;
};
#endif // MAINWINDOW_H
