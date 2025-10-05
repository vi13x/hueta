#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <memory>

class User;
class StudentView;
class TeacherView;
class AdminView;
class LoginDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLogin();
    void onLogout();
    void onExit();
    void onAbout();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void showUserView();
    void clearCurrentView();

    QStackedWidget *stackedWidget;
    QWidget *welcomeWidget;
    StudentView *studentView;
    TeacherView *teacherView;
    AdminView *adminView;
    
    QAction *loginAction;
    QAction *logoutAction;
    QAction *exitAction;
    QAction *aboutAction;
    
    std::shared_ptr<User> currentUser;
    LoginDialog *loginDialog;
};

#endif // MAINWINDOW_H
