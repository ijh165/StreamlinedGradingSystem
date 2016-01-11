/************************************
 *
 * File Name: course.h
 * Purpose  : This class is courses model
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

#ifndef COURSE_H
#define COURSE_H
#include "Control/DatabaseAPI/dbmanager.h"
#include <QDate>
#include <QString>


class Course
{
public:


    Course();
    Course(QString _courseID);
    ~Course();

    //get functions
    QString getcourseID();
    QString getcourseName();
    QString getcourseNumber();
    QDate getstartDate();
    QDate getendDate();
    QString getinstructorEmployeeID();
    QString getinstructorName();
    QSqlQueryModel* getListTA(QString _courseID);
    QSqlQueryModel* getListStd(QString _courseID);
    void setCourseID(QString value);

    //set functions
    void setcourseID(QString course_ID);
    void setcourseName(QString course_Name);
    void setcourseNumber(QString course_Number);
    void setstartDate(QDate start_Date);
    void setendDate(QDate end_Date);
    void setinstructorEmployeeID(QString instructor_EmployeeID);
    void setinstructorName(QString instructor_Name);

private:
    dbManager db;
    QString courseID;
    QString courseName;
    QString courseNumber;
    QDate startDate;
    QDate endDate;
    QString instructorEmployeeID;
    QString instructorName;


//    QVector<QString> tatmListEID;
//    QHash<QString,QString> studentList;
};

#endif // COURSE_H
