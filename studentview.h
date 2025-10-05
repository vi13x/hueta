#ifndef STUDENTVIEW_H
#define STUDENTVIEW_H

#include "user.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>

class Student : public User {
public:
    Student(const QString& username, const QString& password);
    QStringList getPermissions() const override;
};

class StudentView : public QWidget {
    Q_OBJECT

public:
    explicit StudentView(QWidget *parent = nullptr);
    void setStudent(std::shared_ptr<Student> student);
    void refreshMarks();

private slots:
    void onRefreshClicked();

private:
    void setupUI();
    void loadMarks();

    std::shared_ptr<Student> currentStudent;
    QVBoxLayout *mainLayout;
    QLabel *welcomeLabel;
    QTableWidget *marksTable;
    QPushButton *refreshButton;
};

#endif // STUDENTVIEW_H
