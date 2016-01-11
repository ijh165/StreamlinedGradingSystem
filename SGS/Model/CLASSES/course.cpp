/************************************
 *
 * File Name: course.cpp
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

#include "course.h"
#include <QDebug>

#define DATE_FORMAT "yyyy-MM-dd"

// Call the default destructor
Course::Course()
{
    //return model;
}

Course::Course(QString _courseID)
{
    QSqlQueryModel * model = db.getCourseInfo(_courseID);
    courseID = _courseID;
    courseNumber = model->record(0).value("courseNumber").toString();
    courseName = model->record(0).value("courseName").toString();
    startDate = QDate::fromString(model->record(0).value("startDate").toString(), DATE_FORMAT);
    endDate = QDate::fromString(model->record(0).value("endDate").toString(), DATE_FORMAT);
    instructorEmployeeID = model->record(0).value("instructorEmployeeID").toString();

    //add Instructor Name

    QSqlQueryModel * modelInst = db.getAllUserInfo_EID(instructorEmployeeID);

    QString firstName = modelInst->record(0).value("firstName").toString();
    QString midName = modelInst->record(0).value("midName").toString();
    QString lastName = modelInst->record(0).value("lastName").toString();

    if(midName.size()!=0)
        instructorName=firstName + " " + midName + " " + lastName;
    else
        instructorName=firstName + " " + lastName;


//    QSqlQueryModel * madara = db.getListTA(courseID);
//    int i = madara->rowCount();
//    qDebug() << i;
//     //tatmListEID.append( madara->record(0).value("employeeID").toString());
//     //qDebug() << tatmListEID[0];

//    for(int i = 0 ; i < madara->rowCount() ; i++)
//    {
//        tatmListEID.append(madara->record(i).value("employeeID").toString());

//        qDebug()<<tatmListEID[i];
//    }

//    //qDebug()<<tatmListEID[0];
}

Course::~Course()
{

}

QString Course::getcourseID(){
    return courseID;
}

QString Course::getcourseName(){
    return courseName;
}

QString Course::getcourseNumber(){
    return courseNumber;
}

QDate Course::getstartDate(){
    return startDate;
}

QDate Course::getendDate(){
    return endDate;
}

QString Course::getinstructorEmployeeID(){
    return instructorEmployeeID;
}

QString Course::getinstructorName(){
    return instructorName;
}

QSqlQueryModel* Course::getListTA(QString _courseID)
{
    //dbManager db;
    //QSqlQueryModel* TAmodel = db.getListTA(_courseID);

    dbManager* db = new dbManager();
    QSqlQueryModel* TAmodel = db->getListTA(_courseID);

    return TAmodel;


}

QSqlQueryModel* Course::getListStd(QString _courseID)
{
//    dbManager db;
//    QSqlQueryModel* stdModel = db.getListStudent(_courseID.toInt());

    dbManager* db = new dbManager();
    QSqlQueryModel* stdModel = db->getListStudent( _courseID.toInt() );

    return stdModel;

}

void Course::setcourseID(QString course_ID)
{
    courseID = course_ID;
}

void Course::setcourseName(QString course_Name)
{
    courseName = course_Name;
}

void Course::setcourseNumber(QString course_Number)
{
    courseNumber = course_Number;
}

void Course::setstartDate(QDate start_Date)
{
    startDate = start_Date;
}

void Course::setendDate(QDate end_Date)
{
    endDate = end_Date;
}

void Course::setinstructorEmployeeID(QString instructor_EmployeeID)
{
    Course::instructorEmployeeID = instructor_EmployeeID;
}

void Course::setinstructorName(QString instructor_Name)
{
    Course::instructorName = instructor_Name;
}
