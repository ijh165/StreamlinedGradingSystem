/************************************
 *
 * File Name: instructor.h
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

#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

//include statements
#include "marker.h"
#include "activity.h"
#include <QString>
#include <QDate>
#include <QTime>
#include <QHash>

class Instructor: public Marker
{
protected:

    //account info
    QString firstName;
    QString midName;
    QString lastName;
    QString userID;
    QString employeeID;
    QString password;

public:

    //constructors
    Instructor();
    Instructor(QString _firstName,
               QString _midName,
               QString _lastName,
               QString _userID,
               QString _employeeID,
               QString _password);
    ~Instructor();

    //member functions
    void createActivity(QString courseName,
                        QString courseNumber,
                        QString activityName,
                        QString activityType,
                        QDate dueDate,
                        QTime dueTime,
                        QString pathToStdSubmission,
                        QString pathToSolnFile);

//    Activity copyActivity();
    bool deleteActivity(int actID, QString _actType);
    void createRubricForAnAct(QString courseName,
                                QString courseNumber,
                                QString activityName,
                                QString activityType,
                                QString listOfCriterias[],
                                int listOfMaxPoints[]);
    void modifyRubricOfAnAct();
    int createActivity(QString activityName, QString activityType, QString dueDateTime,
                        QString language, QString pathToSubmissions, int numberOfTests,
                        QString pathToSolution, int courseID);
    void modifyActivity(QString activityID,QString courseID,QString activityName, QString activityType,
                        QString dueDateTime,QString language,
                        QString pathToSubmissions, QString numberOfTests,
                        QString pathToSolution);
    void insertRubricItem(QString activityName, QString courseName, QString courseNumber,
                      QString startDate, QString endDate, int rubricItemNumber, QString rubricItemText, int rubricItemValue);


    void storeTestCasetoDb(int activityID,int testNumber, int numberOfInputFile,int numberOfOutputFile,
                                            QString environment,QString consoleInputPath,QString consoleOutputPath);
};

#endif // INSTRUCTOR_H
