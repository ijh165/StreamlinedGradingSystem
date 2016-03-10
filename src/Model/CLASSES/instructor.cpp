/************************************
 *
 * File Name: instructor.cpp
 * Purpose  : This class handles instructor actions
 *
 * Authors:
 *      ftran
 *      phoi
 *      ihoo
 *      rca71
 *      cyrusc
 *      yangjaey
 *      salkhali
 *
 * Revision: 1.0
 *
 ***********************************/

#include "instructor.h"
#include <QDebug>

Instructor::Instructor()
{
    firstName='\0';
    midName='\0';
    lastName='\0';
    userID='\0';
    employeeID='\0';
    password='\0';
}

Instructor::Instructor(QString _firstName,
                       QString _midName,
                       QString _lastName,
                       QString _userID,
                       QString _employeeID,
                       QString _password)
{
    firstName=_firstName;
    midName=_midName;
    lastName=_lastName;
    userID=_userID;
    employeeID=_employeeID;
    password=_password;
}

Instructor::~Instructor()
{
}


//create act
int Instructor::createActivity(QString activityName, QString activityType, QString dueDateTime,
                                QString language, QString pathToSubmissions, int numberOfTests,
                                QString pathToSolution, int courseID)
{
    dbManager db;
    int rv;
    rv = db.insertActivity(courseID,activityName,activityType,dueDateTime,language,pathToSubmissions,numberOfTests,pathToSolution);
    return rv;
}


//modify act
void Instructor::modifyActivity(QString activityID, QString courseID,QString activityName,QString activityType, QString dueDateTime,
                                QString language, QString pathToSubmissions, QString numberOfTests,
                                QString pathToSolution)
{
    dbManager db;

    db.updateActivity(activityID,courseID,activityName,activityType,dueDateTime,language,pathToSubmissions,numberOfTests,pathToSolution);
}


//insert rubric item
void Instructor::insertRubricItem(QString activityName, QString courseName, QString courseNumber,
                              QString startDate, QString endDate, int rubricItemNumber, QString rubricItemText, int rubricItemValue)
{
    dbManager db;
    int courseID = db.retrieveCourseID(courseName, courseNumber,startDate, endDate);
    QString cid = QString::number(courseID);
    QSqlQueryModel *activityModel = db.getActivityInfo(cid,activityName);
    int activityID = activityModel->record(0).value("activityID").toInt();
    db.insertRubricItem(activityID, rubricItemNumber, rubricItemText, rubricItemValue);

}


//delete act
bool Instructor::deleteActivity(int actID, QString _actType)
{
    dbManager db;

    bool ret;

//    if( _actType == ACTIVITY_TYPE_MC )
//        ret=db.deleteAllMCQItems(actID);
//    else
//        ret=db.deleteAllRubricItems(actID);

    ret=db.deleteAllGrades(actID);

    ret=db.deleteActivity(actID);

    return ret;
}


void Instructor::storeTestCasetoDb(int activityID,int testNumber, int numberOfInputFile, int numberOfOutputFile, QString environment,QString consoleInputPath,QString consoleOutputPath)
{
  dbManager db;
  // wipe current TestCase items for this activityID

  db.insertProgrammingTest(activityID,testNumber,numberOfInputFile,numberOfOutputFile,consoleInputPath, consoleOutputPath,environment);

  //store test case files to db

}
