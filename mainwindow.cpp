#include "mainwindow.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QPushButton>
#include <QListWidget>

using namespace QtCharts;

MainWindow::MainWindow(const User& u, DataStore* store, QWidget* parent)
    : QMainWindow(parent), m_user(u), m_store(store)
{
    setWindowTitle("Электронный дневник - " + u.fullName);

    m_tabs = new QTabWidget(this);
    setCentralWidget(m_tabs);

    // Расписание есть у всех
    m_tabs->addTab(createSchedulePage(), "🏫 Расписание");

    if (u.role == Role::Student) {
        setupStudentUI();
    } else if (u.role == Role::Teacher) {
        setupTeacherUI();
    } else if (u.role == Role::Admin) {
        setupAdminUI();
    }
}

void MainWindow::setupStudentUI() {
    m_tabs->addTab(createStudentStatsPage(), "📊 Статистика");
}

void MainWindow::setupTeacherUI() {
    m_tabs->addTab(createTeacherMarksPage(), "✍ Оценки");
}

void MainWindow::setupAdminUI() {
    m_tabs->addTab(createAdminPage(), "⚙ Администрирование");
}

// ====== Pages ======

QWidget* MainWindow::createSchedulePage() {
    QWidget* w = new QWidget(this);
    auto layout = new QVBoxLayout(w);

    QLabel* title = new QLabel("Расписание на неделю", w);
    title->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(title);

    QTableWidget* table = new QTableWidget(6, 5, w); // 6 уроков × 5 дней
    table->setHorizontalHeaderLabels({"Понедельник", "Вторник", "Среда", "Четверг", "Пятница"});
    table->setVerticalHeaderLabels({"1 урок", "2 урок", "3 урок", "4 урок", "5 урок", "6 урок"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Примерное расписание
    QString lessons[6][5] = {
        {"Математика","Русский","История","Английский","Физика"},
        {"Физ-ра","География","Химия","Литература","Информатика"},
        {"Биология","Математика","Обществозн.","Английский","Физ-ра"},
        {"Русский","Физика","Литература","История","Математика"},
        {"Информатика","Биология","Английский","Физика","География"},
        {"Химия","Математика","Физ-ра","Русский","Литература"}
    };

    for (int r=0; r<6; r++) {
        for (int c=0; c<5; c++) {
            table->setItem(r,c,new QTableWidgetItem(lessons[r][c]));
        }
    }

    layout->addWidget(table);
    return w;
}

QWidget* MainWindow::createStudentStatsPage() {
    QWidget* w = new QWidget(this);
    auto layout = new QVBoxLayout(w);

    QLabel* title = new QLabel("Моя успеваемость", w);
    title->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(title);

    QLineSeries* series = new QLineSeries();
    series->append(1, 7);
    series->append(2, 8);
    series->append(3, 6);
    series->append(4, 9);
    series->append(5, 10);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Средний балл по четвертям");

    QCategoryAxis* axisX = new QCategoryAxis();
    axisX->append("I", 1);
    axisX->append("II", 2);
    axisX->append("III", 3);
    axisX->append("IV", 4);
    chart->setAxisX(axisX, series);

    QCategoryAxis* axisY = new QCategoryAxis();
    for (int i=1;i<=10;i++) axisY->append(QString::number(i), i);
    chart->setAxisY(axisY, series);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    return w;
}

QWidget* MainWindow::createTeacherMarksPage() {
    QWidget* w = new QWidget(this);
    auto layout = new QVBoxLayout(w);

    QLabel* label = new QLabel("Выставление оценок и пропусков", w);
    label->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(label);

    QListWidget* classList = new QListWidget(w);
    classList->addItems({"9А", "9Б", "10А", "10Б"});
    layout->addWidget(new QLabel("Выберите класс:"));
    layout->addWidget(classList);

    QTableWidget* marks = new QTableWidget(5, 5, w);
    marks->setHorizontalHeaderLabels({"Ученик","Оценка","Пропуск","Комментарий","Дата"});
    marks->verticalHeader()->hide();
    marks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    marks->setItem(0,0,new QTableWidgetItem("Иванов Иван"));
    marks->setItem(1,0,new QTableWidgetItem("Петров Петр"));
    marks->setItem(2,0,new QTableWidgetItem("Сидоров Сидр"));

    layout->addWidget(marks);
    QPushButton* saveBtn = new QPushButton("Сохранить", w);
    layout->addWidget(saveBtn);

    return w;
}

QWidget* MainWindow::createAdminPage() {
    QWidget* w = new QWidget(this);
    auto layout = new QVBoxLayout(w);

    QLabel* label = new QLabel("Управление пользователями", w);
    label->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(label);

    QListWidget* users = new QListWidget(w);
    for (auto& u : m_store->allUsers()) {
        users->addItem(u.fullName + " (" + u.username + ")");
    }
    layout->addWidget(users);

    QPushButton* addBtn = new QPushButton("Добавить пользователя", w);
    QPushButton* delBtn = new QPushButton("Удалить выбранного", w);
    layout->addWidget(addBtn);
    layout->addWidget(delBtn);

    return w;
}
