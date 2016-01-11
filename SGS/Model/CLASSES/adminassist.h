/************************************
 *
 * File Name: adminassist.h
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

#ifndef ADMINASSIST_H
#define ADMINASSIST_H

//include statements
#include <Qvector>
#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include "account.h"
#include "course.h"
#include "Control/DatabaseAPI/dbmanager.h"

class AdminAssist: public Account
{
public:
    AdminAssist();
    bool createCourse(QString courseName, QString courseNumber, QString courseStartDate, QString courseEndDate, QString instructorID);
    void copyCourse(QString courseID);
    bool modifyCourse(QString _courseID, QString _courseName, QString _courseNum, QString _startDate, QString _endDate, QString _instructorID);
    bool deleteCourse(int _courseID);
    bool assignOneTATMToCourse(QString employeeID,int courseID);
    bool assignOneStdToCourse(int courseID, QString stdName, QString stdID);
    bool clearListTA(QString courseID);
    bool clearListStd(QString courseID);
    bool checkemployeeIDexist(QString employeeID);
    bool IsInstructor(QString employeeID);
    bool IsTA(QString employeeID);
    QString searchName(QString employeeID);
    QString getCourseID(QString _courseNum, QString _courseName, QString _startDate, QString _endDate);
    bool courseContainAct(int _courseID);
};

#endif // ADMINASSIST_H
