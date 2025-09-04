#include "datastore.h"

DataStore::DataStore(QObject* parent) : QObject(parent) {}

bool DataStore::load() { return true; }
bool DataStore::save() const { return true; }

bool DataStore::addUser(const User& u) {
    if(userExists(u.username)) return false;
    m_users.append(u);
    emit changed();
    return true;
}

bool DataStore::removeUser(const QString& username) {
    for(int i=0;i<m_users.size();i++){
        if(m_users[i].username==username) {
            m_users.removeAt(i);
            emit changed();
            return true;
        }
    }
    return false;
}

bool DataStore::userExists(const QString& username) const {
    for(auto& u : m_users) if(u.username==username) return true;
    return false;
}

User DataStore::getUser(const QString& username) const {
    for(auto& u : m_users) if(u.username==username) return u;
    return User{};
}

QList<User> DataStore::allUsers() const { return m_users; }

QString DataStore::hash(const QString& password) { return password; }
bool DataStore::verify(const QString& username, const QString& password, User* u) const {
    for(auto& user : m_users){
        if(user.username==username && user.passwordHash==password){
            if(u) *u=user;
            return true;
        }
    }
    return false;
}

void DataStore::addMark(const QString& username, const QString& subject, int mark, const QDate& date){
    for(auto& user : m_users){
        if(user.username==username){
            user.grades.append({subject, mark, "", date});
            emit changed();
            return;
        }
    }
}

int DataStore::getMarks(const QString& username) const {
    for(auto& user : m_users){
        if(user.username==username) return user.grades.size();
    }
    return 0;
}

void DataStore::setAbsent(const QString&, const QDate&, bool){}
bool DataStore::isAbsent(const QString&, const QDate&) const { return false; }

QStringList DataStore::getSchedule(int) const { return {}; }
void DataStore::setSchedule(int, const QStringList&){}

QStringList DataStore::allClasses() const {
    QSet<int> classSet;
    for(auto& u : m_users) if(u.role==Role::Student) classSet.insert(u.classId);
    QStringList classes;
    for(int c : classSet) classes.append(QString::number(c));
    return classes;
}

QStringList DataStore::studentsInClass(const QString& className) const {
    QStringList list;
    int cid = className.toInt();
    for(auto& u : m_users) if(u.role==Role::Student && u.classId==cid) list.append(u.fullName);
    return list;
}
