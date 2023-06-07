#include "ItemViewWindow.h"
#include "QuerySelectionWindow.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTableView>
#include <QString>
#include <QMenu>
#include <QDebug>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QGuiApplication>

// ---------------------
// -- LIST TABLE VIEW --
// ---------------------
ListTableView::ListTableView(PSQLInterface *psqli, QWidget *parent, QString base_query, QString filter_field, std::vector<unsigned int>* list)
    :QTableView(parent){
    this->base_query = base_query;
    this->filter_field = filter_field;
    this->db = psqli;
    this->list = list;
    horizontalHeader()->hide();
    verticalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, &QTableView::customContextMenuRequested,
            this, &ListTableView::show_context_menu);
    update_display();
}
void ListTableView::update_display(){
    delete model();
    QString query = ItemViewWindow::insert_array_into_query(
        QString("%1 WHERE %2 IN (").arg(base_query, filter_field), list, ")"
    );
    QSqlQueryModel* m = db->issue_query(query);
    setModel(m);
    setColumnHidden(0, true);
}
void ListTableView::show_context_menu(QPoint pos){
    qDebug() << "Item deletion requested at: ";
    QModelIndex cell = indexAt(pos).siblingAtColumn(0);
    qDebug() << cell.data().toUInt();
    QMenu *menu = new QMenu(this);  
    menu->addAction("Удалить", this, [=](){
        for (int i = 0; i < list->size(); ++i)
            if (list->at(i) == cell.data().toUInt()){
                list->erase(list->begin()+i);
                break;
            }
        update_display();
    });
    menu->exec(QCursor::pos());
}
QString ListTableView::exclusion_query(){
    QString query = base_query;
    if (list->size() != 0)
        query += ItemViewWindow::insert_array_into_query(QString(" WHERE %1 NOT IN (").arg(filter_field), list, ")");
    qDebug() << query;
    return query;
}
// --------------------------
// -- ITEM VIEW BASE CLASS --
// --------------------------
ItemViewWindow::ItemViewWindow(PSQLInterface* psqli, QWidget *parent, int itemID)
    : BaseWindow(psqli, "Новая запись", parent){
    if (itemID != -1)
        this->setWindowTitle("Редактировать запись");
    if (can_edit != -1)
        this->setWindowTitle("Просмотр записи");
    this->itemID = itemID;
}
QString ItemViewWindow::insert_array_into_query(QString base_query, std::vector<unsigned int>* list, QString filter_tail){
    QString query = base_query;
    for (int i = 0; i < list->size(); ++i)
        query += QString::number(list->at(i)) + (list->size() - i != 1 ? "," : "");
    query += filter_tail;
    return query;
}
QString ItemViewWindow::format_date(QDate d){
    return QString("'%1-%2-%3'").arg(QString::number(d.year()), QString::number(d.month()), QString::number(d.day()));
}
void ItemViewWindow::add_table_item(ListTableView* table){
    QuerySelectionWindow* item_selection = new QuerySelectionWindow(db, this, table->exclusion_query());
    disable();
    connect(item_selection, &QuerySelectionWindow::selection_confirmed, this, [=](QModelIndex s){
        table->list->push_back(s.data().toUInt());
        table->update_display();        
        item_selection->close();
    });
    connect(item_selection, &QuerySelectionWindow::window_closed, this, [=](){
        delete item_selection;
        enable();
    });
}
void ItemViewWindow::select_from_query(QPushButton* display, int* id, QString base_query, QString filter_field){
    QString query = base_query;
    if (*id != -1)
        query += QString(" WHERE %1 != %2").arg(filter_field, QString::number(*id));
    QuerySelectionWindow* item_selection = new QuerySelectionWindow(db, this, query);
    disable();
    connect(item_selection, &QuerySelectionWindow::selection_confirmed, this, [=](QModelIndex s){
        *id = s.siblingAtColumn(0).data().toInt();
        display->setText(s.siblingAtColumn(1).data().toString());
        item_selection->close();
    });
    connect(item_selection, &QuerySelectionWindow::window_closed, this, [=](){
        delete item_selection;
        enable();
    });
}
QHBoxLayout* ItemViewWindow::create_edit_buttons(){
        QHBoxLayout *btnL = new QHBoxLayout();
        // centralL->addLayout(btnL);
        // centralL->setAlignment(btnL, Qt::AlignCenter);
        QPushButton *finishBtn = new QPushButton(
            (itemID == -1) ? "Создать" : "Сохранить"
        );
        finishBtn->setStyleSheet("background-color: #5eba7d;");
        finishBtn->setMaximumWidth(100);
        QPushButton *abortBtn = new QPushButton("Отменить");
        abortBtn->setStyleSheet("background-color: #ba5e5e;");
        abortBtn->setMaximumWidth(100);

        connect(finishBtn, &QPushButton::clicked,
                this, &ItemViewWindow::submit);
        connect(abortBtn, &QPushButton::clicked,
                this, &ItemViewWindow::close);

        if (itemID != -1){
            QPushButton *deleteBtn = new QPushButton("Удалить");
            deleteBtn->setStyleSheet("background-color: #ba5e5e;");
            deleteBtn->setMaximumWidth(100);
            btnL->addWidget(deleteBtn); 
            btnL->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
            connect(deleteBtn, &QPushButton::clicked, 
                this, &ItemViewWindow::remove_item);
        }
        btnL->addWidget(finishBtn); 
        btnL->addWidget(abortBtn);

        return btnL;
}

// ----------------------
// -- PUBLICATION VIEW --
// ----------------------
PublicationViewWindow::PublicationViewWindow(PSQLInterface* psqli, QWidget *parent, int itemID)
    : ItemViewWindow(psqli, parent, itemID){
    newAuthors = {};
    newCompilations = {};
    newDate = new QDateEdit();
    newName = new QLineEdit();
    newType = new QComboBox();
    newPublisher = -1;

    QFormLayout *form = new QFormLayout();
    QHBoxLayout *tablesL = new QHBoxLayout();
    QVBoxLayout *authorsL = new QVBoxLayout();
    QVBoxLayout *compilationsL = new QVBoxLayout();
    tablesL->addLayout(authorsL);
    tablesL->addLayout(compilationsL);
    centralL->addLayout(form);
    centralL->addLayout(tablesL);
    
    newType->addItems({"Статья", "Препринт", "Многорафия", "Диссертаиця", "Патент", "Отчет"});
    newType->setCurrentIndex(-1);
    newDate->setDate(QDate::currentDate());
    newPublisherDisplay = new QPushButton();
    newPublisherDisplay->setStyleSheet("background: none;");

    newAuthorsDisplay = new ListTableView(db, this, "SELECT authorID, name FROM AUTHOR", "authorID", &newAuthors);
    newCompilationsDisplay = new ListTableView(db, this, "SELECT compilationID, name FROM COMPILATION", "compilationID", &newCompilations);

    form->addRow("Название", newName);
    form->addRow("Тип", newType);
    form->addRow("Дата", newDate);
    form->addRow("Издательство", newPublisherDisplay);
    authorsL->addWidget(newAuthorsDisplay);
    compilationsL->addWidget(newCompilationsDisplay);

    if (itemID != -1)
        populate_with_db_data();
    
    if (can_edit){
        QPushButton *addAuthor = new QPushButton("Добавить автора");
        QPushButton *addCompilation = new QPushButton("Добавить Сборник");
        authorsL->addWidget(addAuthor);
        compilationsL->addWidget(addCompilation);

        newAuthorsDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
        newCompilationsDisplay->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(addAuthor, &QPushButton::clicked, 
                this, [=](){this->add_table_item(newAuthorsDisplay);});
        connect(addCompilation, &QPushButton::clicked, 
                this, [=](){this->add_table_item(newCompilationsDisplay);});

        newPublisherDisplay->setCursor(Qt::PointingHandCursor);
        connect(newPublisherDisplay, &QPushButton::clicked,
                this, [=](){this->select_from_query(newPublisherDisplay, &newPublisher, 
                "SELECT publisherID, name FROM PUBLICATION", "publisherID");});
        
        centralL->addLayout(create_edit_buttons());
    }
    else {
        newName->setReadOnly(true);
        newDate->setReadOnly(true);
        newType->setDisabled(true);
        newPublisherDisplay->setDisabled(true);
    }

    show();
}
void PublicationViewWindow::populate_with_db_data(){
    QSqlQueryModel* model = query_database(QString("SELECT publicationID, name, type, publicationdate, publisherID FROM PUBLICATION where publicationID=%1").arg(itemID));
    
    newName->setText(model->data(model->index(0, 1)).toString());
    newType->setCurrentIndex(model->data(model->index(0, 2)).toInt());
    newDate->setDate(model->data(model->index(0, 3)).toDate()); 
    newPublisher = model->data(model->index(0, 4)).toInt();
    QSqlQueryModel* pubName = query_database(QString("SELECT fullname FROM PUBLISHER WHERE publisherID=%1").arg(newPublisher));
    newPublisherDisplay->setText(pubName->data(pubName->index(0, 0)).toString());
    delete pubName;

    QSqlQueryModel* authors = query_database(QString("SELECT authorID, name FROM get_publication_authors(%1)").arg(itemID));
    QSqlQueryModel* compilations = query_database(QString("SELECT compilationID, name FROM get_publication_compilations(%1)").arg(itemID));

    for (int i = 0; i < authors->rowCount(); ++i)
        newAuthors.push_back(authors->data(authors->index(i, 0)).toUInt());
    for (int i = 0; i < compilations->rowCount(); ++i)
        newCompilations.push_back(compilations->data(compilations->index(i, 0)).toUInt());
    delete authors; delete compilations; delete model;
    newAuthorsDisplay->update_display();
    newCompilationsDisplay->update_display();
}
void PublicationViewWindow::submit(){
    QString error_msg = "";
    if (newName->text().isEmpty())
        error_msg.append("Не указано название публикации.\n");
    if (newDate->date().isNull())
        error_msg.append("Не указана дата публикации.\n");
    if (newType->currentIndex() == -1)
        error_msg.append("Не указан тип публикации.\n");
    if (newPublisher == -1)
        error_msg.append("Не указан издатель.\n");
    if (newAuthors.empty())
        error_msg.append("Должен быть указан хотя бы один автор публикации.\n");
    
    if (!error_msg.isEmpty()){
        show_error(error_msg);
        return;
    }

    QString query; 
    QSqlQueryModel* model;
    if (itemID == -1){
        query = QString("SELECT * FROM add_publication('%1', %2, %3, %4); ").arg(
            newName->text(), QString::number(newType->currentIndex()), QString::number(newPublisher), format_date(newDate->date())
        );
        model = query_database(query);
        itemID = model->data(model->index(0,0)).toInt();
    } else 
        query = QString("SELECT edit_publication(%1, '%2', %3, %4, %5); ").arg(
            QString::number(itemID), newName->text(), QString::number(newType->currentIndex()), QString::number(newPublisher), format_date(newDate->date())
        );
        model = query_database(query);
    query = insert_array_into_query(
        QString("CALL set_publication_authors(%1, ARRAY[").arg(itemID), &newAuthors, "]::integer[]); " 
    )
    //  + insert_array_into_query(
    //     QString("CALL set_publication_compilations(%1, ARRAY[").arg(itemID), &newCompilations, "]::integer[]);" 
    // )
    ;
    model = query_database(query);
    delete model;
    close();
}
void PublicationViewWindow::remove_item(){
    query_database(QString("SELECT remove_publication(%1)").arg(itemID));
    close();
}


// -----------------
// -- AUTHOR VIEW --
// -----------------
AuthorViewWindow::AuthorViewWindow(PSQLInterface* psqli, QWidget *parent, int itemID): ItemViewWindow(psqli, parent, itemID){
    newName = new QLineEdit();
    newDegree = new QLineEdit();
    newPublications = {};
    
    QFormLayout *form = new QFormLayout();
    form->addRow("Название:", newName);
    form->addRow("Ученая степень:", newDegree);
    centralL->addLayout(form);

    newPublicationsView = new ListTableView(psqli, this, "SELECT publicationID, name, publicationtype, date, publisher FROM PUBLICATION_COMPOSITE", "publicationID", &newPublications);
    
    if (itemID != -1)
        populate_with_db_data();

    if (can_edit){
        newPublicationsView->setContextMenuPolicy(Qt::CustomContextMenu);
        QPushButton* addPublication = new QPushButton("Добавить", this);
        connect(addPublication, &QPushButton::clicked,
                this, [=](){this->add_table_item(newPublicationsView);});
        form->addRow("Публикации автора", addPublication);
    } else {
        centralL->addWidget(new QLabel("Публикации автора"));
        newName->setReadOnly(true);
        newDegree->setReadOnly(true);
    }

    centralL->addWidget(newPublicationsView);

    if (can_edit)
        centralL->addLayout(create_edit_buttons());
    show();
}

void AuthorViewWindow::populate_with_db_data(){
    QSqlQueryModel* model = query_database(QString("SELECT * FROM AUTHOR WHERE authorID=%1;").arg(itemID));
    
    newName->setText(model->data(model->index(0, 1)).toString());
    newDegree->setText(model->data(model->index(0, 2)).toString());
    QSqlQueryModel* publications = query_database(QString("SELECT publicationID, name, publicationtype, date, publisher FROM get_authors_publications(%1)").arg(itemID));
    for (int i = 0; i < publications->rowCount(); ++i)
        newPublications.push_back(publications->data(publications->index(i, 0)).toUInt());
    
    delete publications; delete model;

    newPublicationsView->update_display();
}

void AuthorViewWindow::submit(){
    QString error_msg = "";
    if (newName->text().isEmpty())
        error_msg.append("Не указано имя автора.\n");
    if (newName->text().isEmpty())
        error_msg.append("Не указано ученая степень автора.\n");
    
    if (!error_msg.isEmpty()){
        show_error(error_msg);
        return;
    }

    if (itemID == -1){
        QSqlQueryModel* model = query_database(
            QString("SELECT * FROM add_author('%1', '%2'); ").arg(
                newName->text(), newDegree->text()
        ));
        itemID = model->data(model->index(0,0)).toInt();
        delete model;
    } else 
        delete query_database(
            QString("SELECT edit_author(%1, '%2', '%3'); ").arg(
                QString::number(itemID), newName->text(), newDegree->text()
        ));
    delete query_database(insert_array_into_query(
        QString("CALL set_authors_publications(%1, ARRAY[").arg(itemID), &newPublications, "]::integer[]); " 
    ));
    close();
}

void AuthorViewWindow::remove_item(){
    query_database(QString("SELECT remove_author(%1)").arg(itemID));
    close();
}