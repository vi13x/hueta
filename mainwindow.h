#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include "datastore.h" // твоя структура User, DataStore, Role

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(const User& u, DataStore* store, QWidget* parent = nullptr);

private:
    void setupStudentUI();
    void setupTeacherUI();
    void setupAdminUI();

    QWidget* createSchedulePage();
    QWidget* createTeacherMarksPage();
    QWidget* createAdminPage();

    QTabWidget* m_tabs;
    User m_user;
    DataStore* m_store;
};
