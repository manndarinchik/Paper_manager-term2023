#include "MainWindow.h"
#include "PSQLinterface.h"
#include "ItemViewWindow.h"
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

MainWindow::MainWindow(PSQLInterface *psqli) 
    : BaseWindow(psqli, "Публикации"){
    this->resize(800, 500);

    tabsWidget = new QTabWidget();
    
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
                this, [=](){create_itemview(-1);});
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
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(table, &QTableView::doubleClicked,
            this, [=](){itemview_requested(table);});
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



void MainWindow::itemview_requested(QTableView* table){
    int id = table->selectionModel()->selectedRows(0)[0].data().toInt();
    create_itemview(id);
}

void MainWindow::create_itemview(int id){
    switch (this->tabsWidget->currentIndex())
    {
    case 0:
        bind_itemview_window(new PublicationViewWindow(db, this, id));
        break;
    case 1:
        bind_itemview_window(new AuthorViewWindow(db, this, id));
        break;
    // case 2:
    //     show_itemview_window(pn_table, new PublisherViewWindow(db, this));
    //     break;
    // case 3:
    //     show_itemview_window(pn_table, new CompilationViewWindow(db, this));
    //     break;
    default:
        break;
    }
}

void MainWindow::bind_itemview_window(ItemViewWindow* window){
    disable();
    connect(window, &PublicationViewWindow::window_closed, this, [=](){
        this->enable();
        this->update_tab_table(this->tabsWidget->currentIndex());
        delete window;
    });
}
