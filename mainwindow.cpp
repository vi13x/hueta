#include "mainwindow.h"
#include "registrationdialog.h"
#include "logindialog.h"
#include "studentview.h"
#include "teacherview.h"
#include "adminview.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("SchoolApp — приветствие");
    resize(600, 400);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *lay = new QVBoxLayout(central);

    welcomeLabel = new QLabel("<h1>иди нахуй далбаеб</h1>\nПожалуйста, зарегистрируйтесь или войдите.", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    lay->addWidget(welcomeLabel);

    regBtn = new QPushButton("Регистрация (ученик)", this);
    loginBtn = new QPushButton("Войти (ученик)", this);
    teacherAdminBtn = new QPushButton("Вход для учителя / администратора", this);

    lay->addWidget(regBtn);
    lay->addWidget(loginBtn);
    lay->addWidget(teacherAdminBtn);
    lay->addStretch();

    connect(regBtn, &QPushButton::clicked, this, &MainWindow::onRegister);
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLogin);
    connect(teacherAdminBtn, &QPushButton::clicked, this, &MainWindow::onTeacherAdminLogin);
}

void MainWindow::onRegister() {
    RegistrationDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        User u = dlg.user();
        if (ds.addStudent(u)) {
            QMessageBox::information(this, "OK", "Регистрация прошла успешно. Теперь войдите.");
        } else {
            QMessageBox::warning(this, "Ошибка", "Пользователь с таким именем уже существует.");
        }
    }
}

void MainWindow::onLogin() {
    LoginDialog dlg(LoginDialog::Role::Student, this);
    if (dlg.exec() == QDialog::Accepted) {
        QString username = dlg.username();
        QString password = dlg.password();
        if (ds.checkStudentCredentials(username, password)) {
            StudentView *sv = new StudentView(username);
            sv->setAttribute(Qt::WA_DeleteOnClose);
            sv->show();
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверные учётные данные ученика.");
        }
    }
}

void MainWindow::onTeacherAdminLogin() {
    LoginDialog dlg(LoginDialog::Role::TeacherOrAdmin, this);
    if (dlg.exec() == QDialog::Accepted) {
        QString username = dlg.username();
        QString password = dlg.password();
        if (ds.checkTeacherCredentials(username, password)) {
            TeacherView *tv = new TeacherView(username);
            tv->setAttribute(Qt::WA_DeleteOnClose);
            tv->show();
        } else if (ds.checkAdminCredentials(username, password)) {
            AdminView *av = new AdminView(username);
            av->setAttribute(Qt::WA_DeleteOnClose);
            av->show();
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверные учётные данные (учитель/админ).");
        }
    }
}
