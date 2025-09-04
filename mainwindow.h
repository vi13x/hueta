#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QStackedWidget>
#include <QTableWidget>
#include "datastore.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(User u, DataStore* store, QWidget* parent = nullptr);

private:
    void setupStudentUI();
    void setupTeacherUI();
    void setupAdminUI();

    QWidget* createStudentPage();
    QWidget* createStudentMarksPage();
    QWidget* createTeacherMarksPage();
    QWidget* createAdminPage();

    void showStudentMarksPage();
    void showStudentSchedulePage();
    void refreshTeacherMarksPage();

    QTabWidget* m_tabs;
    QStackedWidget* m_studentStack;
    QTableWidget* m_teacherMarksTable;
    QListWidget* m_classList;

    User m_user;
    DataStore* m_store;
};
