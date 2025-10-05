#ifndef DATASTORE_H
#define DATASTORE_H

#include "user.h"
#include <QList>
#include <QString>
#include <QMap>
#include <memory>

class Student;
class Teacher;
class Admin;

class DataStore {
public:
    static DataStore& getInstance();
    
    // User management
    bool addUser(std::shared_ptr<User> user);
    std::shared_ptr<User> findUser(const QString& username);
    bool authenticateUser(const QString& username, const QString& password);
    QList<std::shared_ptr<User>> getAllUsers() const;
    
    // Student specific
    QList<std::shared_ptr<Student>> getStudents() const;
    std::shared_ptr<Student> findStudent(const QString& username);
    
    // Teacher specific
    QList<std::shared_ptr<Teacher>> getTeachers() const;
    std::shared_ptr<Teacher> findTeacher(const QString& username);
    
    // Admin specific
    QList<std::shared_ptr<Admin>> getAdmins() const;
    std::shared_ptr<Admin> findAdmin(const QString& username);
    
    // Data persistence
    void saveData();
    void loadData();
    void initDemoData();
    
    // Marks management
    void addMark(const QString& studentUsername, const QString& subject, int mark);
    QMap<QString, QList<int>> getStudentMarks(const QString& studentUsername) const;
    QMap<QString, QMap<QString, QList<int>>> getAllMarks() const;

private:
    DataStore() = default;
    ~DataStore() = default;
    DataStore(const DataStore&) = delete;
    DataStore& operator=(const DataStore&) = delete;
    
    QList<std::shared_ptr<User>> users;
    QMap<QString, QMap<QString, QList<int>>> marks; // student -> subject -> marks
};

#endif // DATASTORE_H
