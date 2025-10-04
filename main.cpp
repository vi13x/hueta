#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    // Загружаем брутальный стиль
    QFile styleFile("brutal_style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream styleStream(&styleFile);
        QString style = styleStream.readAll();
        a.setStyleSheet(style);
        styleFile.close();
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
