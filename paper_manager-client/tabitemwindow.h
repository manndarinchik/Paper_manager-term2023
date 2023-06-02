#ifndef TABITEMWINDOW_H
#define TABITEMWINDOW_H
#include "basewindow.h"
#include <QComboBox>
#include <QDateEdit>
#include <QSqlQueryModel>
#include <QTableView>

// class ItemViewWindow : public BaseWindow
// {
//     Q_OBJECT

// public:
//     ItemViewWindow(PSQLInterface* psqli, QWidget *parent);

// public slots:
//     virtual void init();

// };

class PublicationViewWindow : public BaseWindow
{
    Q_OBJECT
private:
    QLineEdit *newName;
    QComboBox *newType;
    QDateEdit *newDate;
    QTableView *newAuthorsDisplay;
    QTableView *newCompilationsDisplay;
    int newPublisher;
    QLineEdit *newPublisherDisplay;
    std::vector<unsigned int> newAuthors;
    std::vector<unsigned int> newCompilations;
    QSqlQueryModel* model;

public:
    PublicationViewWindow(PSQLInterface* psqli, QWidget *parent, int publicationID = -1, bool canEdit=false);
    ~PublicationViewWindow(){};
private:
    void populate_with_db_data(int publicationID);
    void disable_fields();
    void update_authors_display();
    void update_compilations_display();

private slots:
    void submit();
    void add_author(int authorID){};
    void add_compilation(int compilationID){};
};

#endif //TABITEMWINDOW_H