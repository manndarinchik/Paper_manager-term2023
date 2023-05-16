#include "mainwindow.h"
#include "psqlinterface.h"
#include <QVBoxLayout>
#include <QTableView>

MainWindow::MainWindow(PSQLInterface *psqli) 
    : BaseWindow(psqli, "Публикации"){}

void MainWindow::init(){
    this->resize(800, 500);

    QTabWidget *tabsWidget = new QTabWidget();
    
    pn_tab = new QWidget();
    pn_table = create_table(pn_tab);
    tabsWidget->addTab(pn_tab, QString("Публикации"));

    ar_tab = new QWidget;
    ar_table = create_table(ar_tab);
    tabsWidget->addTab(ar_tab, QString("Авторы"));

    pr_tab = new QWidget;
    pr_table = create_table(pr_tab);
    tabsWidget->addTab(pr_tab, QString("Издательства"));

    cn_tab = new QWidget;
    cn_table = create_table(cn_tab);
    tabsWidget->addTab(cn_tab, QString("Сборники"));

    connect(tabsWidget, &QTabWidget::currentChanged,
            this, &MainWindow::update_tab_table);

    centralL->addWidget(tabsWidget);  

    show();
}


QTableView* MainWindow::create_table(QWidget* parent){
    QTableView* table = new QTableView();
    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(table);
    return table;
}

void MainWindow::update_tab_table(int index){
    switch (index)
    {
    case 0:
        pn_table->setModel(db->issue_query(QString("SELECT * FROM PUBLICATION_COMPOSITE"))); break;
    case 1:
        ar_table->setModel(db->issue_query(QString("SELECT * FROM AUTHOR_COMPOSITE"))); break;
    case 2:
        pr_table->setModel(db->issue_query(QString("SELECT * FROM PUBLISHER"))); break;
    case 3:
        cn_table->setModel(db->issue_query(QString("SELECT * FROM COMPILATION_COMPOSITE"))); break;
    
    default:
        break;
    }

}