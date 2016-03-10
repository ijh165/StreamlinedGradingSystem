/************************************
 *
 * File Name: account.cpp
 * Purpose  : This class handles account model and account related action (when user is logged in)
 *
 * Authors:
 *      ihoo
 *      rca71
 *      cyrusc
 *      jhoi
 *      ftran
 *
 * Revision: 1.0
 *
 ***********************************/

#include "account.h"
#include <QDebug>

Account::Account()
{
    firstName='\0';
    midName='\0';
    lastName='\0';
    userID='\0';
    employeeID='\0';
    password='\0';
    isTA=false;
    isInstructor=false;
    isAdmin=false;
    isAdminAssist=false;
    isSysAdmin=false;
    isNewAccount = false;
}
/* Constructor to initialize account object from database given user ID
 * Note: caller need to verify that user exist */
Account::Account(QString _userID)
{
  dbManager db;
  QSqlQueryModel* AccountModel = db.getAllUserInfo(_userID);
  accountID = AccountModel->record(0).value("accountID").toInt();
  firstName = AccountModel->record(0).value("firstName").toString();
  midName = AccountModel->record(0).value("middleName").toString();;
  lastName = AccountModel->record(0).value("lastName").toString();;
  userID = _userID;
  employeeID = AccountModel->record(0).value("employeeID").toString();
  password = AccountModel->record(0).value("password").toString();
  isTA = AccountModel->record(0).value("isTA").toBool();
  isInstructor = AccountModel->record(0).value("isInstructor").toBool();
  isAdmin = AccountModel->record(0).value("isAdmin").toBool();
  isAdminAssist = AccountModel->record(0).value("isAdminAssist").toBool();
  isSysAdmin = AccountModel->record(0).value("isSystemAdmin").toBool();
  isNewAccount = AccountModel->record(0).value("isPasswordTemp").toBool();

  delete AccountModel;
}

Account::Account(QString _employeeID, int stub) //stub is useless variable reason why I add this is so that I can overload the constructor
{
  (void)stub;
  dbManager db;
  QSqlQueryModel* AccountModel = db.getAllUserInfo_EID(_employeeID);
  accountID = AccountModel->record(0).value("accountID").toInt();
  firstName = AccountModel->record(0).value("firstName").toString();
  midName = AccountModel->record(0).value("middleName").toString();
  lastName = AccountModel->record(0).value("lastName").toString();
  userID = AccountModel->record(0).value("userName").toString();
  employeeID = _employeeID;
  password = AccountModel->record(0).value("password").toString();
  isTA = AccountModel->record(0).value("isTA").toBool();
  isInstructor = AccountModel->record(0).value("isInstructor").toBool();
  isAdmin = AccountModel->record(0).value("isAdmin").toBool();
  isAdminAssist = AccountModel->record(0).value("isAdminAssist").toBool();
  isSysAdmin = AccountModel->record(0).value("isSystemAdmin").toBool();
  isNewAccount = AccountModel->record(0).value("isPasswordTemp").toBool();

  stub=-999; //this is useless variable reason why I add this is so that I can overload the constructor

  delete AccountModel;
}

Account::Account(QString _firstName,
        QString _midName,
        QString _lastName,
        QString _userID,
        QString _employeeID,
        QString _password,
        bool _isTA,
        bool _isInstructor,
        bool _isAdmin,
        bool _isAdminAssist,
        bool _isSysAdmin)
{
    firstName=_firstName;
    midName=_midName;
    lastName=_lastName;
    userID=_userID;
    employeeID=_employeeID;
    password=_password;
    isTA=_isTA;
    isInstructor=_isInstructor;
    isAdmin=_isAdmin;
    isAdminAssist=_isAdminAssist;
    isSysAdmin=_isSysAdmin;

}

Account::~Account()
{
}

//setters
void Account::setFirstName(QString _firstName)
{
    firstName=_firstName;
}

void Account::setMidName(QString _midName)
{
    midName=_midName;
}

void Account::setLastName(QString _lastName)
{
    lastName=_lastName;
}

void Account::setUserID(QString _userID)
{
    userID=_userID;
}

void Account::setEmployeeID(QString _employeeID)
{
    employeeID=_employeeID;
}

void Account::setPassword(QString _password)
{
    password=_password;
}

void Account::setIsNewAccount(bool bVal)
{
  isNewAccount = bVal;
}

void Account::setIsBlocked(bool bVal)
{
  isBlocked = bVal;
}

//getters
QString Account::getFirstName()
{
    return firstName;
}

QString Account::getMidName()
{
    return midName;
}

QString Account::getLastName()
{
    return lastName;
}

QString Account::getUserID()
{
    return userID;
}

QString Account::getEmployeeID()
{
    return employeeID;
}

QString Account::getPassword()
{
    return password;
}

bool Account::isAccountSysAdmin()
{
  return isSysAdmin;
}

bool Account::isAccountAdminAssist()
{
  return isAdminAssist;
}

bool Account::isAccountAdmin()
{
  return isAdmin;
}

bool Account::isAccountInstructor()
{
  return isInstructor;
}

bool Account::isAccountTA()
{
  return isTA;
}

bool Account::isAccountBlocked()
{
    return isBlocked;
}

bool Account::isAccountFirstLogin()
{
    qDebug() << "current account isNewAccount? " << isNewAccount;
    return isNewAccount;
}

int Account::getAccountID()
{
    return accountID;
}


/*=====================================MISC_FUNCTIONS==========================================*/



//retrieve course ID
int Account::retrieveCourseID(QString courseName, QString courseNumber, QString startDate, QString endDate)
{
  dbManager db;
  int CourseID = db.retrieveCourseID(courseName,courseNumber,startDate,endDate);
  return CourseID;
}

int Account::getcourseNumCount(){
    dbManager db;
    int courseNumCount = db.getNumberOfAssignedCourses(employeeID);
    return courseNumCount;
}

int Account::getactivityNumCount(){
    dbManager db;
    int courseNumCount = db.getNumberOfAssignedActivity(employeeID);
    return courseNumCount;
}
//......................................
QString Account::searchmyCourse(int recordNumber)
 {
  dbManager db;
  QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
  QString Course = /*CourseModel->record(recordNumber).value("courseName").toString() +*/
                   " " + CourseModel->record(recordNumber).value("courseNumber").toString() +
                   " From "+CourseModel->record(recordNumber).value("startDate").toString() +
                   " to "+CourseModel->record(recordNumber).value("endDate").toString();
  return Course;
  }

void Account::searchmyActivity(int rowNumber, int recordNumber,QString &ActivityName,QString &ActivityType)
 {
  dbManager db;

  QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
  QString courseID = CourseModel->record(rowNumber).value("courseID").toString();
  QSqlQueryModel* ActivityModel = db.getListActivity(courseID);
  ActivityName = ActivityModel->record(recordNumber).value("activityName").toString();
  ActivityType = ActivityModel->record(recordNumber).value("activityType").toString();
  }

//.......................

QString Account::getCourseName(int recordNumber){
    dbManager db;
    QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
    QString Name = CourseModel->record(recordNumber).value("courseName").toString();
    return Name;
}
QString Account::getCourseNumber(int recordNumber){
    dbManager db;
    QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
    QString Number = CourseModel->record(recordNumber).value("courseNumber").toString();
    return Number;
}
QString Account::getStartDate(int recordNumber){
    dbManager db;
    QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
    QString StartDate = CourseModel->record(recordNumber).value("startDate").toString();
    return StartDate;
}
QString Account::getEndDate(int recordNumber){
    dbManager db;
    QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
    QString EndDate = CourseModel->record(recordNumber).value("endDate").toString();
    return EndDate;
}
void Account::getownActivityInfo_Data(int rowNumberofCourseList,QString activityName,
                                         QString &dueDateTime, QString &pathToSolution ,QString &pathToSubmissions,QString &language, QString &ActivityID,QString &courseID){
    dbManager db;
    QSqlQueryModel* CourseModel = db.getAssignedCourses(employeeID);
    courseID = CourseModel->record(rowNumberofCourseList).value("courseID").toString();
    QSqlQueryModel* ActivityModel= db.getActivityInfo(courseID, activityName);
    dueDateTime = ActivityModel->record(0).value("dueDateTime").toString();
    pathToSolution = ActivityModel->record(0).value("pathToSolution").toString();
    pathToSubmissions = ActivityModel->record(0).value("pathToSubmissions").toString();
    language = ActivityModel->record(0).value("language").toString();
    ActivityID = ActivityModel->record(0).value("activityID").toString();

}

