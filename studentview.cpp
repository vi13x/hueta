#include "studentview.h"
#include "datastore.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFrame>
#include <QHBoxLayout>
#include <QDate>
#include <QMessageBox>

StudentView::StudentView(const QString &username, QWidget *parent) : QWidget(parent), username(username) {
    setWindowTitle(QString("👨‍🎓 Студент: %1").arg(username));
    resize(1000, 700);
    setMinimumSize(800, 600);

    // Устанавливаем стили
    setStyleSheet(
        "QWidget { background-color: #2b2b2b; color: white; } "
        "QTabWidget::pane { border: 1px solid #555; background-color: #3c3c3c; } "
        "QTabBar::tab { background-color: #4a4a4a; color: white; padding: 10px 20px; margin-right: 2px; } "
        "QTabBar::tab:selected { background-color: #667eea; } "
        "QTabBar::tab:hover { background-color: #5a5a5a; } "
        "QTableWidget { background-color: #3c3c3c; gridline-color: #555; } "
        "QTableWidget::item { padding: 8px; border-bottom: 1px solid #555; } "
        "QTableWidget::item:selected { background-color: #667eea; } "
        "QHeaderView::section { background-color: #4a4a4a; color: white; padding: 10px; border: none; } "
        "QPushButton { background-color: #667eea; color: white; border: none; padding: 10px 20px; border-radius: 5px; font-weight: bold; } "
        "QPushButton:hover { background-color: #5a6fd8; } "
        "QPushButton:pressed { background-color: #4c63d2; } "
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Заголовок
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setStyleSheet("QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); border-radius: 10px; padding: 15px; }");
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    QLabel *userLbl = new QLabel(QString("👨‍🎓 Добро пожаловать, %1!").arg(username), this);
    userLbl->setStyleSheet("QLabel { color: white; font-size: 24px; font-weight: bold; }");
    headerLayout->addWidget(userLbl);
    headerLayout->addStretch();
    
    QLabel *dateLabel = new QLabel(QDate::currentDate().toString("dd.MM.yyyy"), this);
    dateLabel->setStyleSheet("QLabel { color: rgba(255, 255, 255, 0.8); font-size: 16px; }");
    headerLayout->addWidget(dateLabel);
    
    mainLayout->addWidget(headerFrame);

    // Вкладки
    tabs = new QTabWidget(this);
    tabs->setTabPosition(QTabWidget::North);

    // Расписание
    QWidget *scheduleWidget = new QWidget();
    QVBoxLayout *scheduleLayout = new QVBoxLayout(scheduleWidget);
    
    QTableWidget *scheduleTable = new QTableWidget(7, 2, this);
    scheduleTable->setHorizontalHeaderLabels(QStringList() << "День недели" << "Предметы");
    scheduleTable->verticalHeader()->setVisible(false);
    scheduleTable->horizontalHeader()->setStretchLastSection(true);
    scheduleTable->setAlternatingRowColors(true);
    scheduleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Загружаем расписание из DataStore
    DataStore ds;
    auto scheduleLines = ds.loadSchedule();
    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
    
    for (int i = 0; i < 7; ++i) {
        QTableWidgetItem *dayItem = new QTableWidgetItem(days[i]);
        dayItem->setFlags(dayItem->flags() & ~Qt::ItemIsEditable);
        dayItem->setBackground(QColor(0x4a4a4a));
        scheduleTable->setItem(i, 0, dayItem);
        
        QString subjects = "Выходной";
        if (i < scheduleLines.size() && !scheduleLines[i].isEmpty()) {
            subjects = scheduleLines[i];
        }
        QTableWidgetItem *subjectItem = new QTableWidgetItem(subjects);
        subjectItem->setFlags(subjectItem->flags() & ~Qt::ItemIsEditable);
        scheduleTable->setItem(i, 1, subjectItem);
    }
    
    scheduleLayout->addWidget(scheduleTable);
    tabs->addTab(scheduleWidget, "📅 Расписание");

    // Успеваемость
    QWidget *gradesWidget = new QWidget();
    QVBoxLayout *gradesLayout = new QVBoxLayout(gradesWidget);
    
    QTableWidget *gradesTable = new QTableWidget(0, 4, this);
    gradesTable->setHorizontalHeaderLabels(QStringList() << "Предмет" << "Оценка" << "Дата" << "Учитель");
    gradesTable->verticalHeader()->setVisible(false);
    gradesTable->horizontalHeader()->setStretchLastSection(true);
    gradesTable->setAlternatingRowColors(true);
    gradesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Загружаем оценки из DataStore
    auto grades = ds.loadGrades();
    for (const QString &gradeLine : grades) {
        QStringList parts = gradeLine.split(',');
        if (parts.size() >= 4) {
            int row = gradesTable->rowCount();
            gradesTable->insertRow(row);
            
            gradesTable->setItem(row, 0, new QTableWidgetItem(parts[2])); // Предмет
            gradesTable->setItem(row, 1, new QTableWidgetItem(parts[4])); // Оценка
            gradesTable->setItem(row, 2, new QTableWidgetItem(parts[3])); // Дата
            gradesTable->setItem(row, 3, new QTableWidgetItem("Учитель")); // Учитель
        }
    }
    
    gradesLayout->addWidget(gradesTable);
    tabs->addTab(gradesWidget, "📊 Успеваемость");

    // Статистика
    QWidget *statsWidget = new QWidget();
    QVBoxLayout *statsLayout = new QVBoxLayout(statsWidget);
    
    QLabel *statsLabel = new QLabel("📈 Статистика успеваемости", this);
    statsLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; margin: 10px; }");
    statsLayout->addWidget(statsLabel);
    
    // Здесь можно добавить графики или диаграммы
    QLabel *statsInfo = new QLabel("Средний балл: 8.5\nЛучший предмет: Математика\nКоличество оценок: 15", this);
    statsInfo->setStyleSheet("QLabel { font-size: 14px; padding: 20px; background-color: rgba(255, 255, 255, 0.1); border-radius: 10px; }");
    statsLayout->addWidget(statsInfo);
    
    tabs->addTab(statsWidget, "📈 Статистика");

    mainLayout->addWidget(tabs);

    // Кнопки
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    backBtn = new QPushButton("🔙 Назад", this);
    logoutBtn = new QPushButton("🚪 Выйти из аккаунта", this);
    
    buttonsLayout->addWidget(backBtn);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(logoutBtn);
    
    mainLayout->addLayout(buttonsLayout);

    connect(logoutBtn, &QPushButton::clicked, this, &StudentView::onLogout);
    connect(backBtn, &QPushButton::clicked, this, &StudentView::onBack);
}

void StudentView::onLogout() {
    close();
}

void StudentView::onBack() {
    // просто закрываем — пользователь вернётся к главному окну
    close();
}
