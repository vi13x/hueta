#pragma once
#include <QWidget>
#include <QString>

class QListWidget;
class QPushButton;

class TeacherView : public QWidget {
    Q_OBJECT
public:
    TeacherView(const QString &username, QWidget *parent = nullptr);
private slots:
    void onClassSelected();
    void onStudentClicked();
    void onLogout();
private:
    QString username;
    QListWidget *classList;
    QListWidget *studentList;
    QPushButton *logoutBtn;
};
