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
    QItemSelectionModel* m[4] = {
        pn_table->selectionModel(),
        ar_table->selectionModel(),
        pr_table->selectionModel(),
        cn_table->selectionModel()
    };
    QSqlQueryModel* model = nullptr;
    switch (index)
    {
    case 0:
        model = query_database("SELECT * FROM PUBLICATION_COMPOSITE");
        if (model != nullptr) pn_table->setModel(model);
        break;
    case 1:
        model = query_database("SELECT * FROM AUTHOR_COMPOSITE");
        if (model != nullptr) ar_table->setModel(model);
        break;
    case 2:
        model = query_database("SELECT * FROM PUBLISHER");
        if (model != nullptr) pr_table->setModel(model);
        break;
    case 3:
        model = query_database("SELECT * FROM COMPILATION_COMPOSITE");
        if (model != nullptr) cn_table->setModel(model);
        break;
    }

    for (int i = 0; i < 4; ++i) delete m[i];
}