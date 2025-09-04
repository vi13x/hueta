#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QMessageBox>

MainWindow::MainWindow(User u, DataStore* store, QWidget* parent)
    : QMainWindow(parent), m_user(u), m_store(store)
{
    setWindowTitle("Электронный дневник - " + u.fullName);
    resize(900,600);

    m_tabs = new QTabWidget(this);
    setCentralWidget(m_tabs);

    if(u.role==Role::Student) setupStudentUI();
    else if(u.role==Role::Teacher) setupTeacherUI();
    else if(u.role==Role::Admin) setupAdminUI();
}

// ---------------- STUDENT -----------------
void MainWindow::setupStudentUI(){
    m_studentStack = new QStackedWidget();

    QWidget* schedulePage = createStudentPage();
    QWidget* marksPage = createStudentMarksPage();

    m_studentStack->addWidget(schedulePage);
    m_studentStack->addWidget(marksPage);

    QPushButton* toMarksBtn = new QPushButton("📊 Мои оценки", schedulePage);
    schedulePage->layout()->addWidget(toMarksBtn);
    QObject::connect(toMarksBtn, &QPushButton::clicked, this, &MainWindow::showStudentMarksPage);

    QPushButton* backBtn = new QPushButton("⬅ Назад", marksPage);
    marksPage->layout()->addWidget(backBtn);
    QObject::connect(backBtn, &QPushButton::clicked, this, &MainWindow::showStudentSchedulePage);

    QPushButton* logoutBtn = new QPushButton("Выйти из аккаунта", schedulePage);
    schedulePage->layout()->addWidget(logoutBtn);
    QObject::connect(logoutBtn, &QPushButton::clicked, [this](){ close(); });

    m_tabs->addTab(m_studentStack, "👨‍🎓 Личный кабинет");
}

void MainWindow::showStudentMarksPage(){ if(m_studentStack) m_studentStack->setCurrentIndex(1);}
void MainWindow::showStudentSchedulePage(){ if(m_studentStack) m_studentStack->setCurrentIndex(0); }

// ---------------- STUDENT PAGES -----------------
QWidget* MainWindow::createStudentPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QLabel* title = new QLabel("Личный кабинет студента", w);
    title->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(title);

    QLabel* info = new QLabel(QString("ФИО: %1\nКласс: %2%3")
                                  .arg(m_user.fullName)
                                  .arg(m_user.classNumber)
                                  .arg(m_user.classLetter), w);
    layout->addWidget(info);

    QTableWidget* table = new QTableWidget(6,5,w);
    table->setHorizontalHeaderLabels({"Пн","Вт","Ср","Чт","Пт"});
    table->setVerticalHeaderLabels({"1","2","3","4","5","6"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    return w;
}

QWidget* MainWindow::createStudentMarksPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QLabel* title = new QLabel("Мои оценки", w);
    title->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(title);

    QTableWidget* marks = new QTableWidget(0,3,w);
    marks->setHorizontalHeaderLabels({"Предмет","Оценка","Комментарий"});
    marks->verticalHeader()->hide();
    marks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i=0;i<m_user.grades.size();i++){
        marks->insertRow(i);
        marks->setItem(i,0,new QTableWidgetItem(m_user.grades[i].subject));
        marks->setItem(i,1,new QTableWidgetItem(QString::number(m_user.grades[i].grade)));
        marks->setItem(i,2,new QTableWidgetItem(m_user.grades[i].comment));
    }
    layout->addWidget(marks);

    QPushButton* logoutBtn = new QPushButton("Выйти из аккаунта", w);
    layout->addWidget(logoutBtn);
    QObject::connect(logoutBtn, &QPushButton::clicked, [this](){ close(); });

    return w;
}

// ---------------- TEACHER -----------------
void MainWindow::setupTeacherUI(){ m_tabs->addTab(createTeacherMarksPage(), "✍ Оценки"); }

QWidget* MainWindow::createTeacherMarksPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QLabel* label = new QLabel("Выставление оценок", w);
    label->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(label);

    m_classList = new QListWidget(w);
    m_classList->addItems(m_store->allClasses());
    layout->addWidget(new QLabel("Выберите класс:"));
    layout->addWidget(m_classList);

    m_teacherMarksTable = new QTableWidget(0,5,w);
    m_teacherMarksTable->setHorizontalHeaderLabels({"Ученик","Оценка","Пропуск","Комментарий","Дата"});
    m_teacherMarksTable->verticalHeader()->hide();
    m_teacherMarksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_teacherMarksTable);

    QPushButton* saveBtn = new QPushButton("Сохранить", w);
    layout->addWidget(saveBtn);

    QObject::connect(m_classList, &QListWidget::currentTextChanged, [=](const QString &className){
        m_teacherMarksTable->clearContents();
        m_teacherMarksTable->setRowCount(0);
        QStringList students = m_store->studentsInClass(className);
        for(int i=0;i<students.size();i++){
            m_teacherMarksTable->insertRow(i);
            m_teacherMarksTable->setItem(i,0,new QTableWidgetItem(students[i]));
        }
    });

    return w;
}

// ---------------- ADMIN -----------------
void MainWindow::setupAdminUI(){ m_tabs->addTab(createAdminPage(), "⚙ Администрирование"); }

QWidget* MainWindow::createAdminPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QLabel* label = new QLabel("Управление пользователями", w);
    label->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(label);

    QListWidget* users = new QListWidget(w);
    for(auto& u : m_store->allUsers())
        users->addItem(u.fullName + " (" + u.username + ")");
    layout->addWidget(users);

    QLineEdit* nameEdit = new QLineEdit(w); nameEdit->setPlaceholderText("ФИО");
    QLineEdit* usernameEdit = new QLineEdit(w); usernameEdit->setPlaceholderText("Логин");
    QLineEdit* classEdit = new QLineEdit(w); classEdit->setPlaceholderText("Класс (число)");
    QLineEdit* letterEdit = new QLineEdit(w); letterEdit->setPlaceholderText("Буква");
    layout->addWidget(nameEdit); layout->addWidget(usernameEdit);
    layout->addWidget(classEdit); layout->addWidget(letterEdit);

    QPushButton* addBtn = new QPushButton("Добавить пользователя", w);
    layout->addWidget(addBtn);
    QObject::connect(addBtn, &QPushButton::clicked, [=](){
        User newUser;
        newUser.fullName = nameEdit->text();
        newUser.username = usernameEdit->text();
        newUser.role = Role::Student;
        newUser.classNumber = classEdit->text().toInt();
        newUser.classLetter = letterEdit->text();
        m_store->addUser(newUser);
        users->addItem(newUser.fullName + " (" + newUser.username + ")");
    });

    QPushButton* delBtn = new QPushButton("Удалить выбранного", w);
    layout->addWidget(delBtn);
    QObject::connect(delBtn, &QPushButton::clicked, [=](){
        int idx = users->currentRow();
        if(idx>=0){
            QString username = m_store->allUsers()[idx].username;
            m_store->deleteUser(username);
            delete users->takeItem(idx);
        }
    });

    return w;
}

// ---------------- HELPER -----------------
void MainWindow::refreshTeacherMarksPage(){
    if(!m_classList || !m_teacherMarksTable) return;
    QString className = m_classList->currentItem() ? m_classList->currentItem()->text() : "";
    if(!className.isEmpty()){
        m_teacherMarksTable->clearContents();
        m_teacherMarksTable->setRowCount(0);
        QStringList students = m_store->studentsInClass(className);
        for(int i=0;i<students.size();i++){
            m_teacherMarksTable->insertRow(i);
            m_teacherMarksTable->setItem(i,0,new QTableWidgetItem(students[i]));
        }
    }
}
