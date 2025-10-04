#include "mainwindow.h"
#include "registrationdialog.h"
#include "logindialog.h"
#include "studentview.h"
#include "teacherview.h"
#include "adminview.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QPalette>
#include <QApplication>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("📚 Электронный журнал");
    resize(800, 600);
    setMinimumSize(600, 500);

    // Устанавливаем современный стиль
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    // Создаем палитру для темной темы
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    QApplication::setPalette(darkPalette);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    
    // Основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Заголовок с иконкой
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setFrameStyle(QFrame::Box);
    headerFrame->setStyleSheet("QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); border-radius: 15px; padding: 20px; }");
    
    QVBoxLayout *headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setAlignment(Qt::AlignCenter);
    
    welcomeLabel = new QLabel("🎓 Добро пожаловать в Электронный журнал", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("QLabel { color: white; font-size: 28px; font-weight: bold; margin: 10px; }");
    
    QLabel *subtitleLabel = new QLabel("Выберите действие для продолжения", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("QLabel { color: rgba(255, 255, 255, 0.8); font-size: 16px; margin: 5px; }");
    
    headerLayout->addWidget(welcomeLabel);
    headerLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(headerFrame);

    // Кнопки в красивом стиле
    QFrame *buttonsFrame = new QFrame(this);
    buttonsFrame->setStyleSheet("QFrame { background: rgba(255, 255, 255, 0.1); border-radius: 15px; padding: 20px; }");
    
    QVBoxLayout *buttonsLayout = new QVBoxLayout(buttonsFrame);
    buttonsLayout->setSpacing(15);

    regBtn = new QPushButton("👤 Регистрация ученика", this);
    regBtn->setStyleSheet(
        "QPushButton { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4CAF50, stop:1 #45a049); "
        "border: none; "
        "color: white; "
        "font-size: 16px; "
        "font-weight: bold; "
        "padding: 15px; "
        "border-radius: 10px; "
        "min-height: 20px; "
        "} "
        "QPushButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5CBF60, stop:1 #4CAF50); "
        "transform: translateY(-2px); "
        "} "
        "QPushButton:pressed { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3d8b40, stop:1 #4CAF50); "
        "}"
    );

    loginBtn = new QPushButton("🔑 Вход для ученика", this);
    loginBtn->setStyleSheet(
        "QPushButton { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2196F3, stop:1 #1976D2); "
        "border: none; "
        "color: white; "
        "font-size: 16px; "
        "font-weight: bold; "
        "padding: 15px; "
        "border-radius: 10px; "
        "min-height: 20px; "
        "} "
        "QPushButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #42A5F5, stop:1 #2196F3); "
        "transform: translateY(-2px); "
        "} "
        "QPushButton:pressed { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #1976D2); "
        "}"
    );

    teacherAdminBtn = new QPushButton("👨‍🏫 Вход для учителя / администратора", this);
    teacherAdminBtn->setStyleSheet(
        "QPushButton { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF9800, stop:1 #F57C00); "
        "border: none; "
        "color: white; "
        "font-size: 16px; "
        "font-weight: bold; "
        "padding: 15px; "
        "border-radius: 10px; "
        "min-height: 20px; "
        "} "
        "QPushButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFB74D, stop:1 #FF9800); "
        "transform: translateY(-2px); "
        "} "
        "QPushButton:pressed { "
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E65100, stop:1 #F57C00); "
        "}"
    );

    buttonsLayout->addWidget(regBtn);
    buttonsLayout->addWidget(loginBtn);
    buttonsLayout->addWidget(teacherAdminBtn);
    
    mainLayout->addWidget(buttonsFrame);
    mainLayout->addStretch();

    // Подвал с информацией
    QLabel *footerLabel = new QLabel("© 2024 Электронный журнал | Версия 2.0", this);
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setStyleSheet("QLabel { color: rgba(255, 255, 255, 0.6); font-size: 12px; margin: 10px; }");
    mainLayout->addWidget(footerLabel);

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
