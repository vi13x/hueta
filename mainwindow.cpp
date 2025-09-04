#include "mainwindow.h"
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Электронный дневник");
    resize(1100, 600);

    // Меню
    QMenu *fileMenu = menuBar()->addMenu("Файл");
    fileMenu->addAction("Выход", this, &QMainWindow::close);

    QMenu *helpMenu = menuBar()->addMenu("Помощь");
    helpMenu->addAction("О программе");

    // Тулбар
    QToolBar *toolBar = addToolBar("Главное меню");
    toolBar->addAction("Расписание");
    toolBar->addAction("Дневник");
    toolBar->addAction("Табель");
    toolBar->addAction("Посещаемость");

    // Вкладки
    QTabWidget *tabs = new QTabWidget(this);

    // === Табель ===
    QWidget *diaryTab = new QWidget;
    QVBoxLayout *diaryLayout = new QVBoxLayout(diaryTab);

    QTableWidget *table = new QTableWidget(10, 7);
    table->setHorizontalHeaderLabels({"Предмет", "Оценки", "Средний балл",
                                      "1 четверть", "2 четверть", "3 четверть", "Год"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList subjects = {
        "Алгебра", "Геометрия", "Русский язык", "Литература", "История",
        "Биология", "География", "Физика", "Химия", "Информатика"
    };

    for (int i = 0; i < subjects.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(subjects[i]));
    }

    QList<QList<int>> grades = {
        {5, 4, 4, 5}, {4, 3, 4, 5}, {5, 5, 4, 5}, {3, 4, 4, 3},
        {4, 5, 5, 5}, {5, 5, 4, 5}, {3, 3, 4, 4}, {4, 4, 5, 5},
        {2, 3, 3, 4}, {5, 5, 5, 5}
    };

    for (int row = 0; row < grades.size(); row++) {
        double sum = 0;
        QString gradesStr;

        for (int g : grades[row]) {
            gradesStr += QString::number(g) + " ";
            sum += g;
        }

        QTableWidgetItem *gradesItem = new QTableWidgetItem(gradesStr.trimmed());
        gradesItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 1, gradesItem);

        double avg = sum / grades[row].size();
        QTableWidgetItem *avgItem = new QTableWidgetItem(QString::number(avg, 'f', 2));
        avgItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 2, avgItem);

        table->setItem(row, 3, new QTableWidgetItem("5"));
        table->setItem(row, 4, new QTableWidgetItem("4"));
        table->setItem(row, 5, new QTableWidgetItem("5"));
        table->setItem(row, 6, new QTableWidgetItem("5"));
    }

    diaryLayout->addWidget(table);
    tabs->addTab(diaryTab, "Табель");

    // === Расписание ===
    QWidget *scheduleTab = new QWidget;
    QVBoxLayout *scheduleLayout = new QVBoxLayout(scheduleTab);
    scheduleLayout->addWidget(new QLabel("Здесь будет расписание уроков"));
    tabs->addTab(scheduleTab, "Расписание");

    // === Сообщения ===
    QWidget *messagesTab = new QWidget;
    QVBoxLayout *messagesLayout = new QVBoxLayout(messagesTab);
    messagesLayout->addWidget(new QLabel("Сообщения от учителей и администрации школы"));
    tabs->addTab(messagesTab, "Сообщения");

    setCentralWidget(tabs);
    statusBar()->showMessage("Готово");
}

MainWindow::~MainWindow() {}
