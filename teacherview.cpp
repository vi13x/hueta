#include "teacherview.h"
#include "datastore.h"
#include "studentview.h" // for Student definition used in loadStudents()
#include <QHeaderView>
#include <QMessageBox>

Teacher::Teacher(const QString& username, const QString& password)
    : User(username, password, UserRole::Teacher) {
}

QStringList Teacher::getPermissions() const {
    return {"view_all_marks", "add_marks", "edit_marks", "view_students"};
}

TeacherView::TeacherView(QWidget *parent)
    : QWidget(parent), currentTeacher(nullptr) {
    setupUI();
}

void TeacherView::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    welcomeLabel = new QLabel("Добро пожаловать, преподаватель!");
    welcomeLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(welcomeLabel);
    
    // Add mark section
    QHBoxLayout *addMarkLayout = new QHBoxLayout();
    addMarkLayout->addWidget(new QLabel("Студент:"));
    studentCombo = new QComboBox();
    addMarkLayout->addWidget(studentCombo);
    
    addMarkLayout->addWidget(new QLabel("Предмет:"));
    subjectCombo = new QComboBox();
    subjectCombo->addItems({"Математика", "Физика", "Химия", "Информатика", "Русский язык"});
    addMarkLayout->addWidget(subjectCombo);
    
    addMarkLayout->addWidget(new QLabel("Оценка:"));
    markSpinBox = new QSpinBox();
    markSpinBox->setRange(1, 5);
    markSpinBox->setValue(5);
    addMarkLayout->addWidget(markSpinBox);
    
    addMarkButton = new QPushButton("Добавить оценку");
    connect(addMarkButton, &QPushButton::clicked, this, &TeacherView::onAddMarkClicked);
    addMarkLayout->addWidget(addMarkButton);
    
    mainLayout->addLayout(addMarkLayout);
    
    marksTable = new QTableWidget();
    marksTable->setColumnCount(3);
    marksTable->setHorizontalHeaderLabels({"Студент", "Предмет", "Оценки"});
    marksTable->horizontalHeader()->setStretchLastSection(true);
    marksTable->setAlternatingRowColors(true);
    mainLayout->addWidget(marksTable);
    
    refreshButton = new QPushButton("Обновить");
    connect(refreshButton, &QPushButton::clicked, this, &TeacherView::onRefreshClicked);
    mainLayout->addWidget(refreshButton);
}

void TeacherView::setTeacher(std::shared_ptr<Teacher> teacher) {
    currentTeacher = teacher;
    if (currentTeacher) {
        welcomeLabel->setText("Добро пожаловать, " + currentTeacher->getUsername() + "!");
        loadStudents();
        loadMarks();
    }
}

void TeacherView::loadStudents() {
    studentCombo->clear();
    auto students = DataStore::getInstance().getStudents();
    for (auto& student : students) {
        studentCombo->addItem(student->getUsername());
    }
}

void TeacherView::loadMarks() {
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

void TeacherView::refreshData() {
    loadStudents();
    loadMarks();
}

void TeacherView::onRefreshClicked() {
    refreshData();
}

void TeacherView::onAddMarkClicked() {
    if (studentCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите студента");
        return;
    }
    
    QString student = studentCombo->currentText();
    QString subject = subjectCombo->currentText();
    int mark = markSpinBox->value();
    
    DataStore::getInstance().addMark(student, subject, mark);
    loadMarks();
    
    QMessageBox::information(this, "Успех", "Оценка добавлена");
}
