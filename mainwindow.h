#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include "datastore.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void onRegister();
    void onLogin();
    void onTeacherAdminLogin();
private:
    DataStore ds;
    QLabel *welcomeLabel;
    QPushButton *regBtn;
    QPushButton *loginBtn;
    QPushButton *teacherAdminBtn;
};
