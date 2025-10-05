#include "mainwindow.h"
#include "logindialog.h"
#include "datastore.h"
#include "studentview.h"
#include "teacherview.h"
#include "adminview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentUser(nullptr), loginDialog(nullptr) {
    setupUI();
    setWindowTitle("Academic Ledger - Система управления академическими записями");
    setMinimumSize(800, 600);
    
    // Initialize demo data
    DataStore::getInstance().initDemoData();
}

MainWindow::~MainWindow() {
    if (loginDialog) {
        delete loginDialog;
    }
}

void MainWindow::setupUI() {
    setupMenuBar();
    setupStatusBar();
    
    // Create stacked widget for different views
    stackedWidget = new QStackedWidget();
    setCentralWidget(stackedWidget);
    
    // Welcome widget
    welcomeWidget = new QWidget();
    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeWidget);
    
    QLabel *welcomeLabel = new QLabel("Добро пожаловать в Academic Ledger");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 50px;");
    welcomeLayout->addWidget(welcomeLabel);
    
    QLabel *infoLabel = new QLabel("Система управления академическими записями\n\n"
                                  "Для начала работы войдите в систему или зарегистрируйтесь");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 14px; margin: 20px;");
    welcomeLayout->addWidget(infoLabel);
    
    QPushButton *loginButton = new QPushButton("Войти в систему");
    loginButton->setStyleSheet("font-size: 16px; padding: 10px; margin: 20px;");
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLogin);
    welcomeLayout->addWidget(loginButton);
    
    stackedWidget->addWidget(welcomeWidget);
    
    // Create user views
    studentView = new StudentView();
    teacherView = new TeacherView();
    adminView = new AdminView();
    
    stackedWidget->addWidget(studentView);
    stackedWidget->addWidget(teacherView);
    stackedWidget->addWidget(adminView);
    
    // Show welcome screen initially
    stackedWidget->setCurrentWidget(welcomeWidget);
}

void MainWindow::setupMenuBar() {
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("Файл");
    
    loginAction = new QAction("Войти", this);
    loginAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(loginAction, &QAction::triggered, this, &MainWindow::onLogin);
    fileMenu->addAction(loginAction);
    
    logoutAction = new QAction("Выйти", this);
    logoutAction->setShortcut(QKeySequence("Ctrl+Shift+L"));
    logoutAction->setEnabled(false);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogout);
    fileMenu->addAction(logoutAction);
    
    fileMenu->addSeparator();
    
    exitAction = new QAction("Выход", this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);
    fileMenu->addAction(exitAction);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("Справка");
    
    aboutAction = new QAction("О программе", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupStatusBar() {
    statusBar()->showMessage("Готов к работе");
}

void MainWindow::onLogin() {
    if (!loginDialog) {
        loginDialog = new LoginDialog(this);
    }
    
    if (loginDialog->exec() == QDialog::Accepted) {
        currentUser = loginDialog->getAuthenticatedUser();
        if (currentUser) {
            showUserView();
            statusBar()->showMessage("Вход выполнен: " + currentUser->getUsername());
        }
    }
}

void MainWindow::onLogout() {
    clearCurrentView();
    currentUser = nullptr;
    stackedWidget->setCurrentWidget(welcomeWidget);
    statusBar()->showMessage("Выход выполнен");
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "О программе",
                      "Academic Ledger v1.0\n\n"
                      "Система управления академическими записями\n"
                      "Разработано для управления оценками студентов,\n"
                      "преподавателями и администраторами.\n\n"
                      "© 2024 Academic Ledger Team");
}

void MainWindow::showUserView() {
    if (!currentUser) return;
    
    switch (currentUser->getRole()) {
        case UserRole::Student: {
            auto student = std::dynamic_pointer_cast<Student>(currentUser);
            if (student) {
                studentView->setStudent(student);
                stackedWidget->setCurrentWidget(studentView);
            }
            break;
        }
        case UserRole::Teacher: {
            auto teacher = std::dynamic_pointer_cast<Teacher>(currentUser);
            if (teacher) {
                teacherView->setTeacher(teacher);
                stackedWidget->setCurrentWidget(teacherView);
            }
            break;
        }
        case UserRole::Admin: {
            auto admin = std::dynamic_pointer_cast<Admin>(currentUser);
            if (admin) {
                adminView->setAdmin(admin);
                stackedWidget->setCurrentWidget(adminView);
            }
            break;
        }
    }
    
    // Update menu actions
    loginAction->setEnabled(false);
    logoutAction->setEnabled(true);
}

void MainWindow::clearCurrentView() {
    // Update menu actions
    loginAction->setEnabled(true);
    logoutAction->setEnabled(false);
}
