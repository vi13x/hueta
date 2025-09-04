#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRandomGenerator>

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

// --- Student ---
void MainWindow::setupStudentUI(){
    m_studentStack = new QStackedWidget();
    QWidget* schedulePage = createStudentPage();
    QWidget* marksPage = createStudentMarksPage();
    m_studentStack->addWidget(schedulePage);
    m_studentStack->addWidget(marksPage);

    QPushButton* toMarks = new QPushButton("📊 Мои оценки", schedulePage);
    schedulePage->layout()->addWidget(toMarks);
    connect(toMarks, &QPushButton::clicked, this, &MainWindow::showStudentMarksPage);

    QPushButton* back = new QPushButton("⬅ Назад", marksPage);
    marksPage->layout()->addWidget(back);
    connect(back, &QPushButton::clicked, this, &MainWindow::showStudentSchedulePage);

    QPushButton* logout = new QPushButton("Выйти", schedulePage);
    schedulePage->layout()->addWidget(logout);
    connect(logout, &QPushButton::clicked, [this](){ close(); });

    m_tabs->addTab(m_studentStack, "👨‍🎓 Личный кабинет");
}

void MainWindow::showStudentMarksPage(){ if(m_studentStack) m_studentStack->setCurrentIndex(1); }
void MainWindow::showStudentSchedulePage(){ if(m_studentStack) m_studentStack->setCurrentIndex(0); }

QWidget* MainWindow::createStudentPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    QLabel* info = new QLabel(QString("ФИО: %1\nКласс: %2")
                                  .arg(m_user.fullName).arg(m_user.classId), w);
    layout->addWidget(info);
    return w;
}

QWidget* MainWindow::createStudentMarksPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    QTableWidget* marks = new QTableWidget(0,3,w);
    marks->setHorizontalHeaderLabels({"Предмет","Оценка","Комментарий"});
    marks->verticalHeader()->hide();
    marks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(auto& g : m_user.grades){
        int r = marks->rowCount();
        marks->insertRow(r);
        marks->setItem(r,0,new QTableWidgetItem(g.subject));
        marks->setItem(r,1,new QTableWidgetItem(QString::number(g.mark)));
        marks->setItem(r,2,new QTableWidgetItem(g.comment));
    }
    layout->addWidget(marks);
    return w;
}

// --- Teacher ---
void MainWindow::setupTeacherUI(){ m_tabs->addTab(createTeacherMarksPage(), "✍ Оценки"); }

QWidget* MainWindow::createTeacherMarksPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    m_classList = new QListWidget(w);
    for(auto c : m_store->allClasses()) m_classList->addItem(c);
    layout->addWidget(m_classList);

    m_teacherMarksTable = new QTableWidget(0,5,w);
    m_teacherMarksTable->setHorizontalHeaderLabels({"Ученик","Оценка","Пропуск","Комментарий","Дата"});
    m_teacherMarksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(m_teacherMarksTable);

    connect(m_classList, &QListWidget::currentTextChanged, [=](const QString &className){
        m_teacherMarksTable->clearContents();
        m_teacherMarksTable->setRowCount(0);

        QStringList students = m_store->studentsInClass(className);
        for(int i=0; i<students.size(); i++){
            m_teacherMarksTable->insertRow(i);
            m_teacherMarksTable->setItem(i,0,new QTableWidgetItem(students[i]));
        }
    });

    return w;
}

// --- Admin ---
void MainWindow::setupAdminUI(){ m_tabs->addTab(createAdminPage(), "⚙ Администрирование"); }

QWidget* MainWindow::createAdminPage(){
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QPushButton* addBtn = new QPushButton("Добавить нового студента", w);
    layout->addWidget(addBtn);

    connect(addBtn, &QPushButton::clicked, [=](){
        User u;
        u.fullName = "Новый студент";
        u.username = "user" + QString::number(QRandomGenerator::global()->bounded(1000));
        u.role = Role::Student;
        u.classId = 1;
        m_store->addUser(u);
    });

    return w;
}
