#include "psqlinterface.h"
#include <QtSql>

PSQLInterface::PSQLInterface(){}
PSQLInterface::~PSQLInterface(){
    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void PSQLInterface::connect_to_bd(QString login, QString pswd){
    db.close();
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("paper_manager_db");
    db.setHostName("localhost");
    db.setUserName(login);
    db.setPassword(pswd);
    qDebug() << "Connection status: " << db.open() << db.lastError(); 
    if (!db.open()){
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}

bool PSQLInterface::issue_query(QString query){
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(query);
}

bool PSQLInterface::open() {return db.open();}