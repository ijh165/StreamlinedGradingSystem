/************************************
 *
 * File Name: adminassist.cpp
 * Purpose  : This class handles admistrative assistant functionality
 *
 * Authors:
 *      cyrusc
 *      ihoo
 *      rca71
 *
 * Revision: 1.0
 *
 ***********************************/

#include "adminassist.h"

AdminAssist::AdminAssist()
{
    firstName='\0';
    midName='\0';
    lastName='\0';
    userID='\0';
    employeeID='\0';
    password='\0';
}

bool AdminAssist::createCourse(QString courseName, QString courseNumber, QString courseStartDate, QString courseEndDate, QString instructorID)
{
    dbManager *db = new dbManager();

    bool rv=db->insertCourse(courseNumber, courseStartDate, courseEndDate, courseName, instructorID);
    delete db;
    return rv;
}

bool AdminAssist::assignOneTATMToCourse(QString employeeID,int courseID)
{
    dbManager db;
    int AccountID = db.retrieveAccountID(employeeID);
    bool rv = db.insertTA(AccountID,courseID);
    return rv;
}

bool AdminAssist::assignOneStdToCourse(int courseID, QString stdName, QString stdID)
{
    dbManager db;
    bool rv=db.insertStudentToCourse(courseID, stdID, stdName);
    return rv;
}

//update course info into database
bool AdminAssist::modifyCourse(QString _courseID, QString _courseName, QString _courseNum, QString _startDate, QString _endDate, QString _instructorID)
{
    dbManager* db = new dbManager();
    bool rv=db->updateCourseInfo(_courseID.toInt(), _courseName, _courseNum, _startDate,  _endDate, _instructorID);
    return rv;
}

//clear TA list of a course in database
bool AdminAssist::clearListTA(QString courseID)
{
    dbManager db;
    bool rv = db.deleteCourseTA(courseID.toInt());
    return rv;
}

//clear Std list of a course in database
bool AdminAssist::clearListStd(QString courseID)
{
    dbManager db;
    bool rv = db.deleteAllStudentFromCourse(courseID.toInt());
    return rv;
}

/*==============================================================================================================*/

//Function for checking employeeID existing
bool AdminAssist::checkemployeeIDexist(QString employeeID)
{
  bool rv = true;
  dbManager db;
  if(db.userNameExists_EID(employeeID))
  {
     rv = true;
  }
  else
  {
     rv = false;
  }
  return rv;
}

//Function for check the employee has instructor role
bool AdminAssist::IsInstructor(QString employeeID){
  bool rv = true;
  dbManager db;
  QSqlQueryModel *employee = db.getAllUserInfo_EID(employeeID);
  if(employee->record(0).value("isInstructor").toBool())
  {
     rv = true;
  }
  else
  {
     rv = false;
  }
  return rv;
}

//Function for check the employee has TA role
bool AdminAssist::IsTA(QString employeeID){
  bool rv = true;
  dbManager db;
  QSqlQueryModel *employee = db.getAllUserInfo_EID(employeeID);
  if(employee->record(0).value("isTA").toBool())
  {
     rv = true;
  }
  else
  {
     rv = false;
  }
  return rv;
}

//Function for search the name for employeeID
QString AdminAssist::searchName(QString employeeID)
{
  dbManager db;
  QSqlQueryModel *employee = db.getAllUserInfo_EID(employeeID);
  QString Name =  employee->record(0).value("firstName").toString() + " " + employee->record(0).value("lastName").toString();
  return Name;
}

//function to get course ID
QString AdminAssist::getCourseID(QString _courseNum, QString _courseName, QString _startDate, QString _endDate)
{
    int _courseID=retrieveCourseID(_courseName,_courseNum,_startDate,_endDate);
    return QString::number(_courseID);
}

//function to delete course
bool AdminAssist::deleteCourse(int _courseID)
{
    dbManager* db=new dbManager();
    bool rv=db->deleteCourse(_courseID);
    return rv;
}

//function to check whether course contains activity
bool AdminAssist::courseContainAct(int _courseID)
{
    dbManager* db=new dbManager();
    return db->activityAttached(_courseID);
}
