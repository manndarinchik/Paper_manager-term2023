#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QDebug>

int main(int argc, char **argv) {
 QApplication app(argc, argv);
 QLabel label("Hello, world!");
 label.show();
 qDebug() << "Hi!!!!!!!";
 return app.exec();
}