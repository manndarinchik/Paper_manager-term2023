#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "BaseWindow.h"
#include "PSQLinterface.h"
#include "ItemViewWindow.h"

class MainWindow : public BaseWindow
{
    Q_OBJECT

public:
    MainWindow(PSQLInterface* psqli);

private:
    QTabWidget *tabsWidget;
    QWidget *pn_tab;
    QWidget *ar_tab;
    QWidget *pr_tab;
    QWidget *cn_tab;
    QTableView *pn_table;
    QTableView *ar_table;
    QTableView *pr_table;
    QTableView *cn_table;
    QPushButton *addBtn;

    QTableView* create_table(QWidget* parent); 
    void itemview_requested(QTableView* table);
    void create_itemview(int id = -1);
    void bind_itemview_window(ItemViewWindow* window);
    void update_table(QTableView* table, QString query, QStringList labels);

};
#endif // MAINWINDOW_H
