#include "adminview.h"
#include "datastore.h"
#include "studentview.h"
#include "teacherview.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

Admin::Admin(const QString& username, const QString& password)
    : User(username, password, UserRole::Admin) {
}

QStringList Admin::getPermissions() const {
    return {"view_all_users", "add_users", "delete_users", "view_all_marks", "edit_all_marks"};
}

AdminView::AdminView(QWidget *parent)
    : QWidget(parent), currentAdmin(nullptr) {
    setupUI();
}

void AdminView::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    welcomeLabel = new QLabel("Добро пожаловать, администратор!");
    welcomeLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(welcomeLabel);
    
    tabWidget = new QTabWidget();
    
    // Users tab
    QWidget *usersTab = new QWidget();
    QVBoxLayout *usersLayout = new QVBoxLayout(usersTab);
    
    // Add user section
    QHBoxLayout *addUserLayout = new QHBoxLayout();
    addUserLayout->addWidget(new QLabel("Имя пользователя:"));
    usernameEdit = new QLineEdit();
    addUserLayout->addWidget(usernameEdit);
    
    addUserLayout->addWidget(new QLabel("Пароль:"));
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    addUserLayout->addWidget(passwordEdit);
    
    addUserLayout->addWidget(new QLabel("Роль:"));
    roleCombo = new QComboBox();
    roleCombo->addItems({"Студент", "Преподаватель", "Администратор"});
    addUserLayout->addWidget(roleCombo);
    
    addUserButton = new QPushButton("Добавить пользователя");
    connect(addUserButton, &QPushButton::clicked, this, &AdminView::onAddUserClicked);
    addUserLayout->addWidget(addUserButton);
    
    deleteUserButton = new QPushButton("Удалить пользователя");
    connect(deleteUserButton, &QPushButton::clicked, this, &AdminView::onDeleteUserClicked);
    addUserLayout->addWidget(deleteUserButton);
    
    usersLayout->addLayout(addUserLayout);
    
    usersTable = new QTableWidget();
    usersTable->setColumnCount(3);
    usersTable->setHorizontalHeaderLabels({"Имя пользователя", "Роль", "Пароль"});
    usersTable->horizontalHeader()->setStretchLastSection(true);
    usersTable->setAlternatingRowColors(true);
    usersLayout->addWidget(usersTable);
    
    tabWidget->addTab(usersTab, "Пользователи");
    
    // Marks tab
    QWidget *marksTab = new QWidget();
    QVBoxLayout *marksLayout = new QVBoxLayout(marksTab);
    
    marksTable = new QTableWidget();
    marksTable->setColumnCount(3);
    marksTable->setHorizontalHeaderLabels({"Студент", "Предмет", "Оценки"});
    marksTable->horizontalHeader()->setStretchLastSection(true);
    marksTable->setAlternatingRowColors(true);
    marksLayout->addWidget(marksTable);
    
    tabWidget->addTab(marksTab, "Оценки");
    
    mainLayout->addWidget(tabWidget);
    
    refreshButton = new QPushButton("Обновить");
    connect(refreshButton, &QPushButton::clicked, this, &AdminView::onRefreshClicked);
    mainLayout->addWidget(refreshButton);
}

void AdminView::setAdmin(std::shared_ptr<Admin> admin) {
    currentAdmin = admin;
    if (currentAdmin) {
        welcomeLabel->setText("Добро пожаловать, " + currentAdmin->getUsername() + "!");
        loadUsers();
        loadMarks();
    }
}

void AdminView::loadUsers() {
    auto users = DataStore::getInstance().getAllUsers();
    usersTable->setRowCount(users.size());
    
    for (int i = 0; i < users.size(); ++i) {
        auto user = users[i];
        usersTable->setItem(i, 0, new QTableWidgetItem(user->getUsername()));
        usersTable->setItem(i, 1, new QTableWidgetItem(user->getRoleString()));
        usersTable->setItem(i, 2, new QTableWidgetItem("***")); // Don't show password
    }
}

void AdminView::loadMarks() {
    auto allMarks = DataStore::getInstance().getAllMarks();
    
    int totalRows = 0;
    for (auto& studentMarks : allMarks) {
        totalRows += studentMarks.size();
    }
    
    marksTable->setRowCount(totalRows);
    
    int row = 0;
    for (auto studentIt = allMarks.begin(); studentIt != allMarks.end(); ++studentIt) {
        for (auto subjectIt = studentIt.value().begin(); subjectIt != studentIt.value().end(); ++subjectIt) {
            marksTable->setItem(row, 0, new QTableWidgetItem(studentIt.key()));
            marksTable->setItem(row, 1, new QTableWidgetItem(subjectIt.key()));
            
            QString marksStr;
            for (int mark : subjectIt.value()) {
                if (!marksStr.isEmpty()) marksStr += ", ";
                marksStr += QString::number(mark);
            }
            marksTable->setItem(row, 2, new QTableWidgetItem(marksStr));
            row++;
        }
    }
}

void AdminView::refreshData() {
    loadUsers();
    loadMarks();
}

void AdminView::onRefreshClicked() {
    refreshData();
}

void AdminView::onAddUserClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();
    
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }
    
    UserRole role;
    switch (roleCombo->currentIndex()) {
        case 0: role = UserRole::Student; break;
        case 1: role = UserRole::Teacher; break;
        case 2: role = UserRole::Admin; break;
        default: role = UserRole::Student; break;
    }
    
    std::shared_ptr<User> user;
    switch (role) {
        case UserRole::Student:
            user = std::make_shared<Student>(username, password);
            break;
        case UserRole::Teacher:
            user = std::make_shared<Teacher>(username, password);
            break;
        case UserRole::Admin:
            user = std::make_shared<Admin>(username, password);
            break;
    }
    
    if (DataStore::getInstance().addUser(user)) {
        QMessageBox::information(this, "Успех", "Пользователь добавлен");
        usernameEdit->clear();
        passwordEdit->clear();
        loadUsers();
    } else {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким именем уже существует");
    }
}

void AdminView::onDeleteUserClicked() {
    int currentRow = usersTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя для удаления");
        return;
    }
    
    QString username = usersTable->item(currentRow, 0)->text();
    
    int ret = QMessageBox::question(this, "Подтверждение", 
                                   "Вы уверены, что хотите удалить пользователя " + username + "?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        // Simple implementation - in real app would need proper user deletion
        QMessageBox::information(this, "Информация", "Функция удаления пользователей будет реализована в полной версии");
    }
}
