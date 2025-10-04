#include "teacherview.h"
#include "markdialog.h"
#include "datastore.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QDate>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSplitter>
TeacherView::TeacherView(const QString &username, QWidget *parent) : QWidget(parent), username(username) {
    setWindowTitle(QString("👨‍🏫 Учитель: %1").arg(username));
    resize(1200, 800);
    setMinimumSize(1000, 700);

    // Устанавливаем стили
    setStyleSheet(
        "QWidget { background-color: #2b2b2b; color: white; } "
        "QListWidget { background-color: #3c3c3c; border: 1px solid #555; border-radius: 5px; } "
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #555; } "
        "QListWidget::item:selected { background-color: #667eea; } "
        "QListWidget::item:hover { background-color: #4a4a4a; } "
        "QTableWidget { background-color: #3c3c3c; gridline-color: #555; } "
        "QTableWidget::item { padding: 8px; border-bottom: 1px solid #555; } "
        "QTableWidget::item:selected { background-color: #667eea; } "
        "QHeaderView::section { background-color: #4a4a4a; color: white; padding: 10px; border: none; } "
        "QPushButton { background-color: #667eea; color: white; border: none; padding: 10px 20px; border-radius: 5px; font-weight: bold; } "
        "QPushButton:hover { background-color: #5a6fd8; } "
        "QPushButton:pressed { background-color: #4c63d2; } "
        "QPushButton#gradeBtn { background-color: #4CAF50; } "
        "QPushButton#gradeBtn:hover { background-color: #45a049; } "
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Заголовок
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setStyleSheet("QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); border-radius: 10px; padding: 15px; }");
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    QLabel *userLbl = new QLabel(QString("👨‍🏫 Добро пожаловать, %1!").arg(username), this);
    userLbl->setStyleSheet("QLabel { color: white; font-size: 24px; font-weight: bold; }");
    headerLayout->addWidget(userLbl);
    headerLayout->addStretch();
    
    QLabel *dateLabel = new QLabel(QDate::currentDate().toString("dd.MM.yyyy"), this);
    dateLabel->setStyleSheet("QLabel { color: rgba(255, 255, 255, 0.8); font-size: 16px; }");
    headerLayout->addWidget(dateLabel);
    
    mainLayout->addWidget(headerFrame);

    // Основной контент
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Левая панель - классы и студенты
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    QLabel *classesLabel = new QLabel("📚 Классы:", this);
    classesLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; margin: 10px 0; }");
    leftLayout->addWidget(classesLabel);
    
    classList = new QListWidget(this);
    classList->addItem("1A");
    classList->addItem("1B");
    classList->addItem("2A");
    classList->addItem("2B");
    classList->addItem("3A");
    classList->addItem("3B");
    leftLayout->addWidget(classList);
    
    QLabel *studentsLabel = new QLabel("👥 Студенты:", this);
    studentsLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; margin: 10px 0; }");
    leftLayout->addWidget(studentsLabel);
    
    studentList = new QListWidget(this);
    studentList->addItem("Выберите класс для просмотра студентов");
    leftLayout->addWidget(studentList);
    
    QPushButton *gradeBtn = new QPushButton("📝 Поставить оценку", this);
    gradeBtn->setObjectName("gradeBtn");
    gradeBtn->setEnabled(false);
    leftLayout->addWidget(gradeBtn);
    
    mainSplitter->addWidget(leftPanel);
    
    // Правая панель - таблица оценок
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    QLabel *gradesLabel = new QLabel("📊 Оценки класса:", this);
    gradesLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; margin: 10px 0; }");
    rightLayout->addWidget(gradesLabel);
    
    gradesTable = new QTableWidget(0, 5, this);
    gradesTable->setHorizontalHeaderLabels(QStringList() << "Студент" << "Предмет" << "Оценка" << "Дата" << "Действия");
    gradesTable->verticalHeader()->setVisible(false);
    gradesTable->horizontalHeader()->setStretchLastSection(true);
    gradesTable->setAlternatingRowColors(true);
    gradesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    rightLayout->addWidget(gradesTable);
    
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setSizes({400, 800});
    
    mainLayout->addWidget(mainSplitter);

    // Кнопки
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    logoutBtn = new QPushButton("🚪 Выйти", this);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(logoutBtn);
    mainLayout->addLayout(buttonsLayout);

    connect(classList, &QListWidget::itemClicked, this, &TeacherView::onClassSelected);
    connect(studentList, &QListWidget::itemClicked, this, &TeacherView::onStudentClicked);
    connect(gradeBtn, &QPushButton::clicked, this, &TeacherView::onStudentClicked);
    connect(logoutBtn, &QPushButton::clicked, this, &TeacherView::onLogout);
    
    // Сохраняем ссылки на виджеты для использования в слотах
    this->gradeBtn = gradeBtn;
}

void TeacherView::onClassSelected() {
    QString cls = classList->currentItem()->text();
    studentList->clear();
    gradeBtn->setEnabled(false);
    
    // Загружаем студентов для выбранного класса
    if (cls == "1A") {
        studentList->addItem("Иванов Иван");
        studentList->addItem("Петров Петр");
        studentList->addItem("Сидоров Сидор");
        studentList->addItem("Козлова Анна");
    } else if (cls == "1B") {
        studentList->addItem("Анна Смирнова");
        studentList->addItem("Ольга Волкова");
        studentList->addItem("Мария Новикова");
    } else if (cls == "2A") {
        studentList->addItem("Алексей Морозов");
        studentList->addItem("Дмитрий Лебедев");
        studentList->addItem("Елена Соколова");
    } else if (cls == "2B") {
        studentList->addItem("Николай Попов");
        studentList->addItem("Татьяна Федорова");
    } else if (cls == "3A") {
        studentList->addItem("Андрей Морозов");
        studentList->addItem("Екатерина Волкова");
        studentList->addItem("Сергей Новиков");
    } else if (cls == "3B") {
        studentList->addItem("Наталья Соколова");
        studentList->addItem("Владимир Лебедев");
    }
    
    // Загружаем оценки для выбранного класса
    loadGradesForClass(cls);
}

void TeacherView::loadGradesForClass(const QString &className) {
    gradesTable->setRowCount(0);
    
    DataStore ds;
    auto grades = ds.loadGrades();
    
    for (const QString &gradeLine : grades) {
        QStringList parts = gradeLine.split(',');
        if (parts.size() >= 5 && parts[0] == className) {
            int row = gradesTable->rowCount();
            gradesTable->insertRow(row);
            
            gradesTable->setItem(row, 0, new QTableWidgetItem(parts[1])); // Студент
            gradesTable->setItem(row, 1, new QTableWidgetItem(parts[2])); // Предмет
            gradesTable->setItem(row, 2, new QTableWidgetItem(parts[4])); // Оценка
            gradesTable->setItem(row, 3, new QTableWidgetItem(parts[3])); // Дата
            
            QPushButton *editBtn = new QPushButton("✏️");
            editBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; border: none; padding: 5px; border-radius: 3px; }");
            gradesTable->setCellWidget(row, 4, editBtn);
        }
    }
}

void TeacherView::onStudentClicked() {
    if (!classList->currentItem()) return;
    
    QString cls = classList->currentItem()->text();
    QString student;
    
    // Проверяем, выбрал ли пользователь студента или нажал кнопку
    if (studentList->currentItem() && studentList->currentItem()->text() != "Выберите класс для просмотра студентов") {
        student = studentList->currentItem()->text();
    } else {
        // Если студент не выбран, показываем сообщение
        QMessageBox::information(this, "Информация", "Пожалуйста, выберите студента из списка");
        return;
    }

    // Открываем диалог для постановки оценки
    MarkDialog dlg(cls, student, this);
    if (dlg.exec() == QDialog::Accepted) {
        QString subject = dlg.subject();
        QString grade = dlg.grade();
        DataStore ds;
        ds.addGrade(cls, student, subject, QDate::currentDate().toString(Qt::ISODate), grade);
        
        // Обновляем таблицу оценок
        loadGradesForClass(cls);
        
        QMessageBox::information(this, "✅ Успешно", 
            QString("Поставлена оценка <b>%1</b> ученику <b>%2</b> по предмету <b>%3</b>")
            .arg(grade, student, subject));
    }
}

void TeacherView::onLogout() {
    close();
}
