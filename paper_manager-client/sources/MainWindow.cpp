#include "MainWindow.h"
#include "PSQLinterface.h"
#include "ItemViewWindow.h"
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

MainWindow::MainWindow(PSQLInterface *psqli) 
    : BaseWindow(psqli, "Публикации"){
    this->resize(800, 500);

    QTabWidget *tabsWidget = new QTabWidget();
    
    pn_tab = new QWidget();
    pn_table = create_table(pn_tab);
    tabsWidget->addTab(pn_tab, QString("Публикации"));
    if (can_edit)
        connect(pn_table, &QTableView::doubleClicked,
                this, &MainWindow::edit_publication);

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

    if (can_edit){
        addBtn = new QPushButton();
        addBtn->setText("+");
        QHBoxLayout* addBtnL = new QHBoxLayout();
        addBtn->setFixedSize(30, 30);
        addBtn->setStyleSheet("background-color: #5eba7d; color: white; font-size: 25px;");
        QSpacerItem* addBtnSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        addBtnL->addSpacerItem(addBtnSpacer);
        addBtnL->addWidget(addBtn);
        centralL->addLayout(addBtnL);
        connect(addBtn, &QPushButton::clicked,
                this, [=]{this->MainWindow::show_publication_window(-1);});
    }

    show();
    update_tab_table(0);
}

QTableView* MainWindow::create_table(QWidget* parent){
    QTableView* table = new QTableView();
    table->verticalHeader()->hide();
    table->setColumnHidden(0, true);
    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(table);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

    void MainWindow::show_publication_window(int id){
        PublicationViewWindow *newPublicationWindow = new PublicationViewWindow(db, this, id);
        disable();
        connect(newPublicationWindow, &PublicationViewWindow::window_closed,
                this, [=](){this->delete_child_window(newPublicationWindow); this->enable();});
    }

void MainWindow::delete_child_window(QMainWindow* ptr){
    delete ptr;
}

void MainWindow::edit_publication(const QModelIndex &index){
    QModelIndex selected_row = pn_table->selectionModel()->selectedRows(0)[0];
    show_publication_window(selected_row.data().toInt());
}

