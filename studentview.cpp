#include "studentview.h"
#include "datastore.h"
#include <QHeaderView>

Student::Student(const QString& username, const QString& password)
    : User(username, password, UserRole::Student) {
}

QStringList Student::getPermissions() const {
    return {"view_own_marks", "view_own_profile"};
}

StudentView::StudentView(QWidget *parent)
    : QWidget(parent), currentStudent(nullptr) {
    setupUI();
}

void StudentView::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    welcomeLabel = new QLabel("Добро пожаловать, студент!");
    welcomeLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(welcomeLabel);
    
    marksTable = new QTableWidget();
    marksTable->setColumnCount(2);
    marksTable->setHorizontalHeaderLabels({"Предмет", "Оценки"});
    marksTable->horizontalHeader()->setStretchLastSection(true);
    marksTable->setAlternatingRowColors(true);
    mainLayout->addWidget(marksTable);
    
    refreshButton = new QPushButton("Обновить");
    connect(refreshButton, &QPushButton::clicked, this, &StudentView::onRefreshClicked);
    mainLayout->addWidget(refreshButton);
}

void StudentView::setStudent(std::shared_ptr<Student> student) {
    currentStudent = student;
    if (currentStudent) {
        welcomeLabel->setText("Добро пожаловать, " + currentStudent->getUsername() + "!");
        loadMarks();
    }
}

void StudentView::loadMarks() {
    if (!currentStudent) return;
    
    auto marks = DataStore::getInstance().getStudentMarks(currentStudent->getUsername());
    marksTable->setRowCount(marks.size());
    
    int row = 0;
    for (auto it = marks.begin(); it != marks.end(); ++it) {
        marksTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        
        QString marksStr;
        for (int mark : it.value()) {
            if (!marksStr.isEmpty()) marksStr += ", ";
            marksStr += QString::number(mark);
        }
        marksTable->setItem(row, 1, new QTableWidgetItem(marksStr));
        row++;
    }
}

void StudentView::refreshMarks() {
    loadMarks();
}

void StudentView::onRefreshClicked() {
    refreshMarks();
}
