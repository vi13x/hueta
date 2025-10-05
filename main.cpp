#include <QApplication>
#include <QDebug>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Academic Ledger");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Academic Ledger Team");
    
    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Load custom stylesheet
    QFile styleFile("brutal_style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream styleStream(&styleFile);
        app.setStyleSheet(styleStream.readAll());
        styleFile.close();
    }
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    qDebug() << "Academic Ledger запущен";
    return app.exec();
}
