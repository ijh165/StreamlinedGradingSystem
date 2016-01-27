/************************************
 *
 * File Name: activity.h
 * Purpose  : This class handles activity model
 *
 * Authors:
 *      cyrusc
 *      ihoo
 *      jhoi
 *      ftran
 *
 * Revision: 1.0
 *
 ***********************************/

#ifndef ACTIVITY_H
#define ACTIVITY_H

//include statements
#include "Model/DatabaseAPI/dbmanager.h"
#include "Model/CLASSES/rubric.h"
#include "Model/CLASSES/testcase.h"
#include <QString>
#include <QDate>
#include <QTime>

#define ACTIVITY_TYPE_ESSAY             "Essay"
#define ACTIVITY_TYPE_PROBLEM_SET       "Problem Set"
#define ACTIVITY_TYPE_ESSAY_PROBLEM_SET "Essay/Problem Set"
#define ACTIVITY_TYPE_MC                "MC"
#define ACTIVITY_TYPE_PROGRAMMING       "Programming"

#define LANG_English "English"
#define LANG_French "French"
#define LANG_German "German"
#define LANG_Chinese "Chinese"
#define LANG_CXX "C++"
#define LANG_Java "Java"
#define LANG_Python "Python"

class Activity
{

protected:

    int activityID;
    QString actName;
    QString actType;
    QDate dueDate;
    QTime dueTime;
    QString pathToStdSubm; //std submission
    QString pathToSolnFile;
    QString actLanguage;
    int bonusDays;
    int penaltyDays;
    int bonusPercentPerDay;
    int penaltyPercentPerDay;
    Rubric rubric;
    bool rubricChanged;
    QVector<TestCase> TestCaseList;

public:

    Activity();
    ~Activity();
    Activity(QString courseID, QString activityName);
    Activity(int activityID);
    void setActName(QString activityName);
    void setActType(QString activityType);
    void setActivityID(int ID);
    void setDueDateAndTime(QDate dueDate, QTime dueTime);
    void setPathToStdSubm(QString path);
    void setPathToSolnFile(QString path);
    void setBonusDuration(int days);
    void setPenaltyDuration(int days);
    void setBonusPercentPerDay(int percent_int);
    void setPenaltyPercentPerDay(int percent_int);
    void addRubricItem(int rubricItemNum, QString rubricItemDesc, int rubricItemGrade);
    bool saveRubricToDb();
    bool isRubricEmpty();
    void clearRubric();
    bool getRubricFromDb(int ActivityID);
    bool getRuricItemList(QVector<rubricItem_t> &rubricList);
    bool removeRubricfromdB();
    void setRubricChanged(bool value);
    bool getTestCaseList(QVector <TestCase> &_testCaseList);
    bool getTestCasesProgrammingActivityfromdB();
    int checkActivityNameexist(QString activityName,int rowNumber,  QSqlQueryModel *sqlModel,QVector<QString> &tempCourseIDList);
    bool deleteTestCasesFromDb(int activityID);

    /* Attribute getter */
    QString getPathToStdSubmission();
    QString getPathToSolution();
    QString getActivityType();
    QString getActivityName();
    QDate getDueDate();
    QTime getDueTime();
    int getActivityID();


};

#endif // ACTIVITY_H
