#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "datastore.h"

class QTabWidget;
class QTableWidget;
class QChartView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(const User& u, DataStore* store, QWidget* parent=nullptr);

private:
    User m_user;
    DataStore* m_store;
    QTabWidget* m_tabs;

    void setupStudentUI();
    void setupTeacherUI();
    void setupAdminUI();

    QWidget* createSchedulePage();
    QWidget* createTeacherMarksPage();
    QWidget* createAdminPage();
};

#endif
