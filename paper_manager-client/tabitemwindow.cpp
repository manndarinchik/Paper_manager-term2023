#include "tabitemwindow.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTableView>
#include <QString>
#include <QDebug>
#include <QHeaderView>

PublicationViewWindow::PublicationViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID, bool canEdit)
    : BaseWindow(psqli, "Новая публикация", parent){
    if (publicationID != -1)
        this->setWindowTitle("Редактировать публикацию");
    newAuthors = {};
    newCompilations = {};
    newDate = new QDateEdit();
    newName = new QLineEdit();
    newType = new QComboBox();
    newPublisher = -1;
    model = nullptr;

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
    newPublisherDisplay = new QLineEdit();
    newPublisherDisplay->setReadOnly(true);

    newAuthorsDisplay = new QTableView();
    newAuthorsDisplay->horizontalHeader()->hide();
    newAuthorsDisplay->verticalHeader()->hide();
    newCompilationsDisplay = new QTableView();
    newCompilationsDisplay->horizontalHeader()->hide();
    newCompilationsDisplay->verticalHeader()->hide();

    form->addRow("Название", newName);
    form->addRow("Тип", newType);
    form->addRow("Дата", newDate);
    form->addRow("Издательство", newPublisherDisplay);
    authorsL->addWidget(newAuthorsDisplay);
    compilationsL->addWidget(newCompilationsDisplay);

    if (publicationID != -1)
        populate_with_db_data(publicationID);
    

    if (canEdit){
        QHBoxLayout *btnL = new QHBoxLayout();
        centralL->addLayout(btnL);
        centralL->setAlignment(btnL, Qt::AlignCenter);
        QPushButton *finishBtn = new QPushButton(
            (publicationID == -1) ? "Создать" : "Сохранить"
        );
        finishBtn->setStyleSheet("background-color: #5eba7d;");
        finishBtn->setMaximumWidth(100);
        QPushButton *abortBtn = new QPushButton("Отменить");
        abortBtn->setStyleSheet("background-color: #ba5e5e;");
        abortBtn->setMaximumWidth(100);
        btnL->addWidget(finishBtn);
        btnL->addWidget(abortBtn);

        QPushButton *addAuthor = new QPushButton("Добавить автора");
        QPushButton *addCompilation = new QPushButton("Добавить Сборник");
        authorsL->addWidget(addAuthor);
        compilationsL->addWidget(addCompilation);

        connect(finishBtn, &QPushButton::clicked,
                this, &PublicationViewWindow::submit);
        connect(abortBtn, &QPushButton::clicked,
                this, &PublicationViewWindow::close);

        newPublisherDisplay->setCursor(Qt::PointingHandCursor);

    }
    else {
        newName->setReadOnly(true);
        newDate->setReadOnly(true);
        newType->setDisabled(true);
    }

    show();
}

void PublicationViewWindow::populate_with_db_data(int id){
    model = query_database(QString("SELECT publicationID, name, type, publicationdate, publisherID FROM PUBLICATION where publicationID=%1").arg(id));
    
    newName->setText(model->data(model->index(0, 1)).toString());
    newType->setCurrentIndex(model->data(model->index(0, 2)).toInt());
    newDate->setDate(model->data(model->index(0, 3)).toDate());
    newPublisher = model->data(model->index(0, 4)).toInt();
    QSqlQueryModel* pubName = query_database(QString("SELECT fullname FROM PUBLISHER WHERE publisherID=%1").arg(newPublisher));
    newPublisherDisplay->setText(pubName->data(pubName->index(0, 0)).toString());
    delete pubName;

    QSqlQueryModel* authors = query_database(QString("SELECT authorID, name FROM get_publication_authors(%1)").arg(id));
    newAuthorsDisplay->setModel(authors);
    newAuthorsDisplay->setColumnHidden(0, true);
    QSqlQueryModel* compilations = query_database(QString("SELECT compilationID, name FROM get_publication_compilations(%1)").arg(id));
    newCompilationsDisplay->setModel(compilations);
    newCompilationsDisplay->setColumnHidden(0, true);

    for (int i = 0; i < authors->rowCount(); ++i)
        newAuthors.push_back(authors->data(authors->index(i, 0)).toUInt());
    for (int i = 0; i < compilations->rowCount(); ++i)
        newCompilations.push_back(compilations->data(compilations->index(i, 0)).toUInt());
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

}

void PublicationViewWindow::update_authors_display(){
    
}