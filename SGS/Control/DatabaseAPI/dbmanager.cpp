/************************************
 *
 * File Name: dbmanager.cpp
 * Purpose  : This class is a stub
 *
 * Authors:
 *      yklam
 *      kdehkhan
 *
 * Revision: 1.0
 *
 ***********************************/

#include <iostream>
#include "dbManager.h"

using namespace std;

/*======================================================================= *
 * ===*********************DB CONNECTIONS**********************========== *
 * ====================================================================== */
dbManager::dbManager()
{
    connectionString = QString("DRIVER={SQL SERVER};" "SERVER=cypress.csil.sfu.ca;" "UID=c275g07;" "PWD=672t2hAjL7YN6mEh");
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(connectionString);
    db.open();
}

dbManager::dbManager(QString connectionName)
{
    connectionString = QString(connectionName);
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(connectionString);
    db.open();
}

dbManager::~dbManager(){
    db.close();
    QString connection = db.connectionName();
    db.removeDatabase(connection);
}

/*======================================================================= *
 * ===*********************INSERT FUNCTIONS********************========== *
 * ====================================================================== */

//Account
bool dbManager::insertAccount(QString employeeID, QString firstName, QString middleName, QString lastName, QString userName, QString password, bool isPasswordTemp, bool islocked, bool isAdminAssist, bool isSystemAdmin, bool isAdmin, bool isTA, bool isInstructor){
    QSqlQuery query(db);
    query.prepare("INSERT INTO Account (employeeID, firstName, middleName, lastName, userName, password, isPasswordTemp, islocked, isAdminAssist, isSystemAdmin, isAdmin, isTA, isInstructor)"
                  "VALUES(:employeeID,:firstName,:middleName,:lastName,:userName,:password,:isPasswordTemp,:islocked,:isAdminAssist,:isSystemAdmin,:isAdmin,:isTA,:isInstructor)");
    query.bindValue(":employeeID",employeeID);
    query.bindValue(":firstName",firstName);
    query.bindValue(":middleName",middleName);
    query.bindValue(":lastName",lastName);
    query.bindValue(":userName",userName);
    query.bindValue(":password",password);
    query.bindValue(":isPasswordTemp",isPasswordTemp);
    query.bindValue(":islocked",islocked);
    query.bindValue(":isAdminAssist",isAdminAssist);
    query.bindValue(":isSystemAdmin",isSystemAdmin);
    query.bindValue(":isAdmin",isAdmin);
    query.bindValue(":isTA",isTA);
    query.bindValue(":isInstructor",isInstructor);
    query.exec();
    return db.isOpen();
}

//TA
bool dbManager::insertTA( int accountID, int courseID){
     QSqlQuery query(db);
     query.prepare("INSERT INTO TA (accountID, courseID)"
                   "VALUES(:accountID,:courseID)");
     query.bindValue(":accountID",accountID);
     query.bindValue(":courseID",courseID);
     query.exec();
    return db.isOpen();

}

//Students
bool dbManager::insertStudentToCourse(int courseID, QString studentID, QString studentName){
    QSqlQuery query(db);
    query.prepare("INSERT INTO Student(courseID,studentID,studentName)"
                    "VALUES(:courseID,:studentID,:studentName)");
    query.bindValue(":courseID",courseID);
    query.bindValue(":studentID",studentID);
    query.bindValue(":studentName",studentName);
    query.exec();
    return db.isOpen();
}

//Course
bool dbManager::insertCourse(QString courseNumber, QString startDate, QString endDate, QString courseName, QString instructorEmployeeID){
    bool status;
    QString query = "INSERT INTO Course(courseNumber,startDate,endDate,courseName,instructorEmployeeID)VALUES ('"
            + courseNumber + "', '" + startDate + "', '" + endDate +"', '" + courseName + "', '" + instructorEmployeeID + "')";
    status = insertQuerytoDatabase(query);
    return status;
}

//Grading (TA/TM/Instructor/Administrator)
bool dbManager::assignStudentGrade(QString studentID, int rubricItemID, QString earnedGrade){
    bool status = db.isOpen();
    bool exist;
    QSqlQuery query(db);
    query.prepare("IF EXISTS (SELECT * FROM Grade WHERE studentID = (:studentID) AND rubricItemID = (:rubricItemID)) SELECT 1 ELSE SELECT 0");
    query.bindValue(":studentID",studentID);
    query.bindValue(":rubricItemID",rubricItemID);
    query.exec();
    query.first();
    exist = query.value(0).toBool();

    if (exist){
        QSqlQuery query1(db);
        query1.prepare("UPDATE Grade SET earnedGrade = (:earnedGrade) WHERE studentID = (:studentID) AND rubricItemID = (:rubricItemID)");
        query1.bindValue(":studentID",studentID);
        query1.bindValue(":rubricItemID",rubricItemID);
        query1.bindValue(":earnedGrade",earnedGrade);
        query1.exec();
    }
    else{
        QSqlQuery query2(db);
        query2.prepare("INSERT INTO Grade(studentID,rubricItemID,earnedGrade)"
                        "VALUES(:studentID,:rubricItemID,:earnedGrade)");
        query2.bindValue(":studentID",studentID);
        query2.bindValue(":rubricItemID",rubricItemID);
        query2.bindValue(":earnedGrade",earnedGrade);
        query2.exec();
    }
    return status;
}

//Grading Record
bool dbManager::insertGradingRecord(int activityID, QString studentID, QString lastGradedRole, QString userName){
    bool status = db.isOpen();
    bool exist;
    QSqlQuery query(db);
    query.prepare("IF EXISTS (SELECT * FROM GradingRecord WHERE activityID = (:activityID) AND studentID = (:studentID)) SELECT 1 ELSE SELECT 0");
    query.bindValue(":activityID",activityID);
    query.bindValue(":studentID",studentID);
    query.exec();
    query.first();
    exist = query.value(0).toBool();

    if (exist){
        QSqlQuery query1(db);
        query1.prepare("UPDATE GradingRecord SET lastGradedRole = (:lastGradedRole), userName = (:userName) WHERE activityID = (:activityID) AND studentID = (:studentID)");
        query1.bindValue(":lastGradedRole",lastGradedRole);
        query1.bindValue(":userName",userName);
        query1.bindValue(":activityID",activityID);
        query1.bindValue(":studentID",studentID);
        query1.exec();
    }
    else{
        QSqlQuery query2(db);
        query2.prepare("INSERT INTO GradingRecord(activityID,studentID,lastGradedRole,userName)"
                        "VALUES(:activityID,:studentID,:lastGradedRole,:userName)");
        query2.bindValue(":activityID",activityID);
        query2.bindValue(":studentID",studentID);
        query2.bindValue(":lastGradedRole",lastGradedRole);
        query2.bindValue(":userName",userName);
        query2.exec();
    }
    return status;
}

//Activity
int dbManager::insertActivity(int courseID, QString activityName, QString activityType, QString dueDateTime, QString language, QString pathToSubmissions, int numberOfTests, QString pathToSolution){
    bool success = db.isOpen();
    int activityID;
    QSqlQuery query(db);
    query.prepare("INSERT INTO Activity (courseID,activityName,activityType,dueDateTime,language,pathToSubmissions,numberOfTests,pathToSolution)"
                  "VALUES(:courseID,:activityName,:activityType,:dueDateTime,:language,:pathToSubmissions,:numberOfTests,:pathToSolution)");
    query.bindValue(":courseID",courseID);
    query.bindValue(":activityName",activityName);
    query.bindValue(":activityType",activityType);
    query.bindValue(":dueDateTime",dueDateTime);
    query.bindValue(":language",language);
    query.bindValue(":pathToSubmissions",pathToSubmissions);
    query.bindValue(":numberOfTests",numberOfTests);
    query.bindValue(":pathToSolution",pathToSolution);
    query.exec();

    if (success){
        activityID = getActivityID(courseID, activityName);
    }
    else{
        activityID = -1;
    }
    return activityID;
}

//Bonus and Penalty
int dbManager::insertBonusPenalty(int activityID, int bonusPenaltyTypeID, int hours, int amount){
    bool success = db.isOpen();
    int ID;
    QSqlQuery query(db);
    query.prepare("INSERT INTO BonusPenalty(activityID,bonusPenaltyTypeID,hours,amount)"
                  "VALUES(:activityID,:bonusPenaltyTypeID,:hours,:amount)");
    query.bindValue(":activityID",activityID);
    query.bindValue(":bonusPenaltyTypeID",bonusPenaltyTypeID);
    query.bindValue(":hours",hours);
    query.bindValue(":amount",amount);
    query.exec();

    if (success){
        QSqlQuery query1(db);
        query1.prepare("SELECT * FROM BonusPenalty WHERE activityID = (:activityID) AND bonusPenaltyTypeID = (:bonusPenaltyTypeID)");
        query1.bindValue(":activityID",activityID);
        query1.bindValue(":bonusPenaltyTypeID",bonusPenaltyTypeID);
        query1.exec();
        query1.first();
        ID = query1.value("bonusPenaltyID").toInt();
    }
    else{
        ID = -1;
    }
    return ID;
}

int dbManager::insertBonusPenaltyType(QString type){
    bool success = db.isOpen();
    int TypeID;
    QSqlQuery query(db);
    query.prepare("INSERT INTO BonusPenaltyType (type) VALUES(:type)");
    query.bindValue(":type",type);
    query.exec();

    if (success){
        QSqlQuery query1(db);
        query1.prepare("SELECT * FROM BonusPenaltyType WHERE type = (:type)");
        query1.bindValue(":type",type);
        query1.exec();
        query1.first();
        TypeID = query1.value("bonusPenaltyTypeID").toInt();
    }
    else{
        TypeID = -1;
    }
    return TypeID;
}

//Rubric
bool dbManager::insertRubricItem(int activityID, int rubricItemNumber, QString rubricItemText, int rubricItemValue){
    QSqlQuery query(db);
    query.prepare("INSERT INTO RubricItem(activityID,rubricItemNumber,rubricItemText,rubricItemValue)"
                    "VALUES(:activityID,:rubricItemNumber,:rubricItemText,:rubricItemValue)");
    query.bindValue(":activityID",activityID);
    query.bindValue(":rubricItemNumber",rubricItemNumber);
    query.bindValue(":rubricItemText",rubricItemText);
    query.bindValue(":rubricItemValue",rubricItemValue);
    query.exec();
    return db.isOpen();
}

//Multiple Choice
bool dbManager::insertMCQItem(int activityID, int questionNumber, QString response, int weight){
    QSqlQuery query(db);
    query.prepare("INSERT INTO [AnswerKeyResponse](activityID,questionNumber,response,weight)"
                    "VALUES(:activityID,:questionNumber,:response,:weight)");
    query.bindValue(":activityID",activityID);
    query.bindValue(":questionNumber",questionNumber);
    query.bindValue(":response",response);
    query.bindValue(":weight",weight);
    query.exec();
    return db.isOpen();
}

//Programming
int dbManager::insertProgrammingTest(int activityID, int testNumber, int numberOfInputFiles, int numberOfOutputFiles, QString consoleInputPath, QString consoleOutputPath, QString environment){
    bool success = db.isOpen();
    int testID;
    QSqlQuery query(db);
    query.prepare("INSERT INTO ProgrammingTest(activityID,testNumber,numberOfInputFiles,numberOfOutputFiles,consoleInputPath,consoleOutputPath,environment)"
                  "VALUES(:activityID,:testNumber,:numberOfInputFiles,:numberOfOutputFiles,:consoleInputPath,:consoleOutputPath,:environment)");
    query.bindValue(":activityID",activityID);
    query.bindValue(":testNumber",testNumber);
    query.bindValue(":numberOfInputFiles",numberOfInputFiles);
    query.bindValue(":numberOfOutputFiles",numberOfOutputFiles);
    query.bindValue(":consoleInputPath",consoleInputPath);
    query.bindValue(":consoleOutputPath",consoleOutputPath);
    query.bindValue(":environment",environment);
    query.exec();

    if (success){
        QSqlQuery query1(db);
        query1.prepare("SELECT * FROM ProgrammingTest WHERE activityID = (:activityID) AND testNumber = (:testNumber)");
        query1.bindValue(":activityID",activityID);
        query1.bindValue(":testNumber",testNumber);
        query1.exec();
        query1.first();
        testID = query1.value("testID").toInt();
    }
    else{
        testID = -1;
    }
    return testID;
}

int dbManager::insertFileType(QString type){
    bool success = db.isOpen();
    int fileTypeID;
    QSqlQuery query(db);
    query.prepare("INSERT INTO TestFileType (type) VALUES(:type)");
    query.bindValue(":type",type);
    query.exec();

    if (success){
        QSqlQuery query1(db);
        query1.prepare("SELECT * FROM TestFileType WHERE type = (:type)");
        query1.bindValue(":type",type);
        query1.exec();
        query1.first();
        fileTypeID = query1.value("fileTypeID").toInt();
    }
    else{
        fileTypeID = -1;
    }
    return fileTypeID;
}

int dbManager::insertFilePath(int activityID, int fileTypeID, int testNumber, QString pathToFile){
    int fileID;
    QSqlQuery query(db);
    query.prepare("INSERT INTO [File](activityID,fileTypeID,testNumber,pathToFile)"
                  "VALUES(:activityID,:fileTypeID,:testNumber,:pathToFile)");
    query.bindValue(":activityID",activityID);
    query.bindValue(":fileTypeID",fileTypeID);
    query.bindValue(":testNumber",testNumber);
    query.bindValue(":pathToFile",pathToFile);
    bool success = query.exec();

    if (success){
        QSqlQuery query1(db);
        query1.prepare("SELECT * FROM [File] WHERE (activityID = (:activityID) AND fileTypeID = (:fileTypeID) AND testNumber = (:testNumber) AND pathToFile = (:pathToFile))");
        query1.bindValue(":activityID",activityID);
        query1.bindValue(":fileTypeID",fileTypeID);
        query1.bindValue(":testNumber",testNumber);
        query1.bindValue(":pathToFile",pathToFile);
        query1.exec();
        query1.first();
        fileID = query1.value("fileID").toInt();
    }
    else{
        fileID = -1;
    }
    return fileID;
}

/*======================================================================= *
 * ===*********************GET FUNCTIONS***********************========== *
 * ====================================================================== */

//Account
QString dbManager::retrieveUserPW(QString userName){
    QString str = "SELECT password FROM Account WHERE userName = '" + userName + "'";
    QString pw;

    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        pw = query.value(0).toString();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return pw;
}

int dbManager::retrieveAccountID(QString employeeID){
    QString str = "SELECT accountID FROM Account WHERE employeeID = '" + employeeID + "'";
    int accID;

    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        accID = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return accID;
}

bool dbManager::employeeIDexists(QString employeeID){
    QString str = "IF EXISTS (SELECT * FROM Account WHERE employeeID = '" + employeeID + "' ) SELECT 'True' ELSE SELECT 'False' ";
    bool exist;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        exist = query.value(0).toBool();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return exist;
}

bool dbManager::userNameExists(QString userName){
    QString str = "IF EXISTS (SELECT * FROM Account WHERE userName = '" + userName + "' ) SELECT 'True' ELSE SELECT 'False' ";
    bool exist;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        exist = query.value(0).toBool();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return exist;
}

bool dbManager::userNameExists_EID(QString employeeID){
    QString str = "IF EXISTS (SELECT * FROM Account WHERE employeeID = '" + employeeID + "' ) SELECT 'True' ELSE SELECT 'False' ";
    bool exist;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        exist = query.value(0).toBool();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return exist;
}

QSqlQueryModel* dbManager::getAllUserInfo(QString userName){
    QString str = "SELECT * FROM Account WHERE userName = '" + userName + "'";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getAllUserInfo_EID(QString employeeID){
    QString str = "SELECT * FROM Account WHERE employeeID = '" + employeeID + "'";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getListAccount(){
    QString str = "SELECT accountID, employeeID, firstName, middleName, lastName, userName, islocked, attemptCtr FROM Account";
    QSqlQueryModel *accList;

    if(db.isOpen()){
        accList = new QSqlQueryModel;
        accList->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return accList;
}

int dbManager::countAccountList(){
    QString str = "SELECT COUNT(*) FROM Account";
    int count;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        count = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return count;
}

bool dbManager::isFirstLogin(QString userName){
    QString str = "SELECT isPasswordTemp FROM Account WHERE userName = '" + userName + "'";
    bool isTemp;

    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        isTemp = query.value(0).toBool();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return isTemp;
}

bool dbManager::isBlocked(QString userName){
    QString str = "SELECT islocked FROM Account WHERE userName = '" + userName + "'";
    bool isBlocked;

    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        isBlocked = query.value(0).toBool();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return isBlocked;
}

int dbManager::attemptCount(QString userName){
    QString str = "SELECT attemptCtr FROM Account WHERE userName = '" + userName + "'";
    int count;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        count = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return count;
}

//Students
QSqlQueryModel* dbManager::getListStudent(int courseID){
    QSqlQueryModel *ourModel;
    QSqlQuery query(db);
    query.prepare("SELECT studentID, studentName FROM Student WHERE courseID = (:courseID)");
    query.bindValue(":courseID",courseID);
    ourModel = new QSqlQueryModel;
    query.exec();
    ourModel->setQuery(query);
    return ourModel;
}

//Course
bool dbManager::activityAttached(int courseID){
    bool exists;
    QSqlQuery query(db);
    query.prepare("IF EXISTS (SELECT * FROM Activity WHERE courseID = (:courseID) ) SELECT 1 AS EXIST ELSE SELECT 0 AS EXIST");
    query.bindValue(":courseID",courseID);
    query.exec();
    query.first();
    exists = query.value(0).toBool();
    return exists;
}

int dbManager::retrieveCourseID(QString courseName, QString courseNumber, QString startDate, QString endDate){
    QString str = "SELECT courseID FROM Course WHERE courseName = '" + courseName + "' AND courseNumber = '" + courseNumber + "' AND startDate = '" + startDate + "' AND endDate = '"
            + endDate + "'";
    int cid;

    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        cid = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return cid;
}

int dbManager::getNumberOfAssignedCourses(QString employeeID){
    QString str = "SELECT COUNT(*) FROM Course WHERE instructorEmployeeID = '" + employeeID + "'";
    int count;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        count = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return count;
}

QSqlQueryModel* dbManager::getCourseInfo(QString courseID){
    QString str = "SELECT * FROM Course WHERE courseID = '" + courseID + "'";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getListInstructor(QString courseID){
    QString str = "SELECT A.employeeID, A.firstName, A.middleName, A.lastName FROM Course C, Account A "
                  "WHERE C.instructorEmployeeID = A.employeeID AND courseID = '" + courseID + "'";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getListTA(QString courseID){
    QString str = "SELECT A.employeeID, A.firstName, A.middleName, A.lastName FROM TA T, Account A "
                  "WHERE T.accountID = A.accountID AND courseID = '" + courseID + "'";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getAssignedCourses(QString employeeID){
    QString str = "SELECT * FROM Course WHERE instructorEmployeeID = '" + employeeID + "'";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getAssignedCoursesTATM(QString employeeID){
    QString str = "SELECT DISTINCT Course.courseID, Course.courseName, Course.courseNumber, Course.startDate, Course.endDate "
                  "FROM Course JOIN TA ON Course.courseID = TA.courseID JOIN Account ON Account.accountID = TA.accountID "
                  "WHERE Account.employeeID = '" + employeeID + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getListOfAllCourses(){
    QString str = "SELECT courseID, courseName, courseNumber, startDate, endDate, instructorEmployeeID FROM Course";
    QSqlQueryModel *ourModel;

    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

int dbManager::countCourseList(){
    QString str = "SELECT COUNT(*) FROM Course";
    int count;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        count = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return count;
}

//Activity
QSqlQueryModel* dbManager::getListActivity(QString courseID){
    QString str = "SELECT * FROM Activity WHERE courseID = '" + courseID + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

int dbManager::getActivityID(int courseID, QString activityName){
    int aid;
    QSqlQuery query(db);
    query.prepare("SELECT activityID FROM Activity WHERE courseID = (:courseID) AND activityName = (:activityName)");
    query.bindValue(":courseID",courseID);
    query.bindValue(":activityName",activityName);
    query.exec();
    query.first();
    aid = query.value("activityID").toInt();
    return aid;
}

int dbManager::activityExists(int courseID, QString activityName){
    bool status = db.isOpen();
    bool exist;
    QSqlQuery query(db);
    query.prepare("IF EXISTS (SELECT * FROM Activity WHERE courseID = (:courseID) AND activityName = (:activityName)) SELECT 1 ELSE SELECT 0");
    query.bindValue(":courseID",courseID);
    query.bindValue(":activityName",activityName);
    query.exec();
    query.first();
    exist = query.value(0).toBool();
    qDebug() << exist;
    if (exist){
        int aid;
        QSqlQuery query1(db);
        query1.prepare("SELECT activityID FROM Activity WHERE courseID = (:courseID) AND activityName = (:activityName)");
        query1.bindValue(":courseID",courseID);
        query1.bindValue(":activityName",activityName);
        query1.exec();
        query1.first();
        aid = query1.value("activityID").toInt();
        return aid;
    }
    else{
        return -1;
    }
    return status;
}

QSqlQueryModel* dbManager::getActivityInfo(QString courseID, QString activityName){
    QString str = "SELECT * FROM Activity WHERE courseID = '" + courseID + "' AND activityName = '" + activityName + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getActivityInfo_AID (int activityID){
    QSqlQueryModel *ourModel;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM Activity WHERE activityID = (:activityID)");
    query.bindValue(":activityID",activityID);
    ourModel = new QSqlQueryModel;
    query.exec();
    ourModel->setQuery(query);
    return ourModel;
}

//Grading (TA/TM/Instructor/Administrator)
int dbManager::getNumberOfAssignedActivity(QString employeeID){
    QString str = "SELECT COUNT(*) "
                  "FROM Course JOIN Activity ON Course.courseID = Activity.courseID JOIN Account ON Account.employeeID = Course.instructorEmployeeID "
                  "WHERE Account.employeeID = '" + employeeID + "'";
    int count;
    if(db.isOpen()){
        QSqlQuery query(str, db);
        query.first();
        count = query.value(0).toInt();
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return count;
}

QSqlQueryModel* dbManager::getAssignedActivity(QString employeeID){ //FOR INSTRUCTOR GRADING
    QString str = "SELECT Course.courseName, Course.courseNumber, Course.startDate, Course.endDate, Activity.activityName, Activity.activityType, Activity.language, Activity.dueDateTime "
                  "FROM Course JOIN Activity ON Course.courseID = Activity.courseID JOIN Account ON Account.employeeID = Course.instructorEmployeeID "
                  "WHERE Account.employeeID = '" + employeeID + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getAssignedActivityTATM(QString employeeID){ //FOR TATM GRADING
    QString str = "SELECT Course.courseName, Course.courseNumber, Course.startDate, Course.endDate, Activity.activityName, Activity.activityType, Activity.language, Activity.dueDateTime "
                  "FROM Course JOIN Activity ON Course.courseID = Activity.courseID JOIN TA ON Activity.courseID = TA.courseID JOIN Account ON Account.accountID = TA.accountID "
                  "WHERE Account.employeeID = '" + employeeID + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

QSqlQueryModel* dbManager::getStudentActivityGrade(int activityID, QString studentID){
    QSqlQueryModel *ourModel;
    QSqlQuery query(db);
    query.prepare("SELECT G.rubricItemID, R.rubricItemNumber, G.earnedGrade, R.rubricItemValue FROM Grade G, RubricItem R WHERE G.rubricItemID = R.rubricItemID AND G.studentID = :studentID AND R.activityID = :activityID");
    query.bindValue(":activityID",activityID);
    query.bindValue(":studentID",studentID);
    ourModel = new QSqlQueryModel;
    query.exec();
    ourModel->setQuery(query);
    return ourModel;
}


//Rubric
QSqlQueryModel* dbManager::getRubric(QString activityID){
    QString str = "SELECT * FROM dbo.RubricItem WHERE activityID = '" + activityID + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

//Essay & Problem Set

//Multiple Choice
QSqlQueryModel* dbManager::getListMCQItems(QString activityID){
    QString str = "SELECT * FROM dbo.[AnswerKeyResponse] WHERE activityID = '" + activityID + "'";
    QSqlQueryModel *ourModel;
    if(db.isOpen()){
        ourModel = new QSqlQueryModel;
        ourModel->setQuery(str, db);
    }
    else {
        qDebug() << "ERROR: Connection not open";
    }
    return ourModel;
}

//Programming
QSqlQueryModel* dbManager::getListOfProgrammingTestInfo(int activityID){
    QSqlQueryModel *ourModel;
    QSqlQuery query(db);
    query.prepare("SELECT testID, testNumber, numberOfInputFiles, numberOfOutputFiles, consoleInputPath, consoleOutputPath, environment FROM ProgrammingTest WHERE activityID = :activityID");
    query.bindValue(":activityID",activityID);
    ourModel = new QSqlQueryModel;
    query.exec();
    ourModel->setQuery(query);
    return ourModel;
}

QString dbManager::getFileType(int fileTypeID){
    QString type;
    QSqlQuery query(db);
    query.prepare("SELECT type FROM [TestFileType] WHERE fileTypeID = :fileTypeID");
    query.bindValue(":fileTypeID",fileTypeID);
    query.exec();
    query.first();
    type = query.value(0).toString();
    return type;
}

QString dbManager::getFilePath(int fileID){
    QString path;
    QSqlQuery query(db);
    query.prepare("SELECT pathToFile FROM [File] WHERE fileID = :fileID");
    query.bindValue(":fileID",fileID);
    query.exec();
    query.first();
    path = query.value(0).toString();
    return path;
}

QSqlQueryModel* dbManager::getAllFilePaths(int activityID){
    QSqlQueryModel *paths;
    QSqlQuery query(db);
    query.prepare("SELECT fileID, fileTypeID, testNumber, pathToFile FROM [File] WHERE activityID = :activityID");
    query.bindValue(":activityID",activityID);
    paths = new QSqlQueryModel;
    query.exec();
    paths->setQuery(query);
    return paths;
}

//GradingRecord
//Returns all the grades for a given activity.
QSqlQueryModel* dbManager::getGradingRecord(int activityID){
    QSqlQueryModel* grades;
    QSqlQuery query(db);
    query.prepare("SELECT [activityID], [studentID], [lastGradedRole], [userName] FROM [c275g07A].[dbo].[GradingRecord] WHERE activityID = :activityID");
    query.bindValue(":activityID",activityID);
    grades = new QSqlQueryModel;
    query.exec();
    grades->setQuery(query);
    return grades;
}
/*======================================================================= *
 * ===*********************UPDATE FUNCTIONS********************========== *
 * ====================================================================== */

//Account
bool dbManager::updateUserInfo(int accountID, QString firstName, QString middleName, QString lastName, QString userName, QString password, QString employeeID){
    QSqlQuery query(db);
    query.prepare("UPDATE Account SET firstName = (:firstName), middleName = (:middleName), lastName = (:lastName), userName = (:userName), [password] = (:password), employeeID = (:employeeID) WHERE accountID = (:accountID)");
    query.bindValue(":accountID",accountID);
    query.bindValue(":firstName",firstName);
    query.bindValue(":middleName",middleName);
    query.bindValue(":lastName",lastName);
    query.bindValue(":userName",userName);
    query.bindValue(":password",password);
    query.bindValue(":employeeID",employeeID);
    query.exec();
    return db.isOpen();
}

bool dbManager::updateUserRole(int accountID, bool isAdminAssist, bool isAdmin, bool isInstructor, bool isSystemAdmin, bool isTA){
    QSqlQuery query(db);
    query.prepare("UPDATE Account SET isAdminAssist = (:isAdminAssist), isAdmin = (:isAdmin), isInstructor = (:isInstructor), isSystemAdmin = (:isSystemAdmin), isTA = (:isTA) WHERE accountID = (:accountID)");
    query.bindValue(":accountID",accountID);
    query.bindValue(":isAdminAssist",isAdminAssist);
    query.bindValue(":isAdmin",isAdmin);
    query.bindValue(":isInstructor",isInstructor);
    query.bindValue(":isSystemAdmin",isSystemAdmin);
    query.bindValue(":isTA",isTA);
    query.exec();
    return db.isOpen();
}

bool dbManager::changeUserPW(QString password, QString userName){
    QString str = "UPDATE Account SET password = '" + password + "' WHERE userName = '" + userName + "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;
}

bool dbManager::blockUserACC(QString userName, bool islocked){
    QSqlQuery query(db);
    query.prepare("UPDATE Account SET islocked = (:islocked) WHERE userName = (:userName)");
    query.bindValue(":islocked",islocked);
    query.bindValue(":userName",userName);
    query.exec();
    return db.isOpen();
}

bool dbManager::setAccIsPasswordTemp(QString userName, bool isPasswordTemp){
    QSqlQuery query(db);
    query.prepare("UPDATE Account SET isPasswordTemp = (:isPasswordTemp) WHERE userName = (:userName)");
    query.bindValue(":isPasswordTemp",isPasswordTemp);
    query.bindValue(":userName",userName);
    query.exec();
    return db.isOpen();
}

bool dbManager::increAttemptCtr(QString userName){
    QString str = "UPDATE Account SET attemptCtr = attemptCtr + 1 WHERE userName = '" + userName + "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;
}

bool dbManager::resetAttemptCtr(QString userName){
    QString str = "UPDATE Account SET attemptCtr = DEFAULT WHERE userName = '" + userName + "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;
}

//Course
bool dbManager::updateCourseInfo(int courseID, QString courseName, QString courseNumber, QString startDate, QString endDate, QString instructorEmployeeID){
    QSqlQuery query(db);
    query.prepare("UPDATE Course SET courseName = (:courseName), courseNumber = (:courseNumber), startDate = (:startDate), endDate = (:endDate), instructorEmployeeID = (:instructorEmployeeID) WHERE courseID = (:courseID)");
    query.bindValue(":courseID",courseID);
    query.bindValue(":courseName",courseName);
    query.bindValue(":courseNumber",courseNumber);
    query.bindValue(":startDate",startDate);
    query.bindValue(":endDate",endDate);
    query.bindValue(":instructorEmployeeID",instructorEmployeeID);
    query.exec();
    return db.isOpen();
}

bool dbManager::insertOneInstToCourse(QString courseID, QString instructorEmployeeID){
    QString str = "UPDATE Course SET instructorEmployeeID = '"+ instructorEmployeeID + "' WHERE courseID = '" + courseID + "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;
}

//Activity
bool dbManager::updateActivity( QString activityID, QString courseID, QString activityName, QString activityType, QString dueDateTime, QString language, QString pathToSubmissions, QString numberOfTests, QString pathToSolution){
    QString str = "UPDATE Activity SET courseID = '" + courseID + "', activityName = '" + activityName + "', activityType = '" + activityType + "', dueDateTime = '" + dueDateTime + "', language = '"
            + language + "', pathToSubmissions = '" + pathToSubmissions +  "', numberOfTests = '" + numberOfTests + "', pathToSolution = '" + pathToSolution + "' WHERE activityID = '" + activityID + "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;
}


//TestFileType
bool dbManager::updateTestFileType(QString fileTypeID,QString type){
    QString str = "UPDATE TestFileType SET type = '" + type +
                    "'WHERE fileTypeID ='" + fileTypeID + "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;

}


//File
bool dbManager::updateFile(QString fileID,QString activityID, QString fileTypeID, QString testNumber, QString pathToFile, QString extension){
    QString str = "UPDATE [dbo].[File] SET activityID = '" + activityID + "', fileTypeID = '" + fileTypeID + "', testNumber = '" + testNumber + "', pathToFile = '" + pathToFile + "', extension = '" + extension + "'"
            "WHERE fileID = '" +fileID+ "'";
    bool success;
    if(db.isOpen()){
        success = insertQuerytoDatabase(str);
    } else {
        qDebug() << "ERROR: Connection not open";
    }
    return success;
}


/*======================================================================= *
 * ===*********************DELETE FUNCTIONS********************========== *
 * ====================================================================== */

//Account
bool dbManager::deleteAccount(int accountID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM Account WHERE accountID = (:accountID)");
    query.bindValue(":accountID",accountID);
    query.exec();
    return db.isOpen();
}

//TA
bool dbManager::deleteCourseTA(int courseID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM TA WHERE courseID = (:courseID)");
    query.bindValue(":courseID",courseID);
    query.exec();
    return db.isOpen();
}

//Student
bool dbManager::deleteStudentFromCourse(int courseID, QString studentID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM Student WHERE courseID = (:courseID) AND studentID = (:studentID)");
    query.bindValue(":courseID",courseID);
    query.bindValue(":studentID",studentID);
    query.exec();
    return db.isOpen();
}

bool dbManager::deleteAllStudentFromCourse(int courseID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM Student WHERE courseID = (:courseID)");
    query.bindValue(":courseID",courseID);
    query.exec();
    return db.isOpen();
}

//Course
bool dbManager::deleteCourse(int courseID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM Course WHERE courseID = (:courseID)");
    query.bindValue(":courseID",courseID);
    query.exec();
    return db.isOpen();
}

//Activity
bool dbManager::deleteActivity(int activityID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM Activity WHERE activityID = (:activityID)");
    query.bindValue(":activityID",activityID);
    query.exec();
    return db.isOpen();
}

//Multiple Choice
bool dbManager::deleteAllMCQItems(int activityID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM [AnswerKeyResponse] WHERE activityID = (:activityID)");
    query.bindValue(":activityID",activityID);
    query.exec();
    return db.isOpen();
}

//Rubric
bool dbManager::deleteAllRubricItems(int activityID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM RubricItem WHERE activityID = (:activityID)");
    query.bindValue(":activityID",activityID);
    query.exec();
    return db.isOpen();
}

//Programming
bool dbManager::deleteAllTestFiles(int activityID){
    bool status = db.isOpen();
    QSqlQuery query(db);
    query.prepare("DELETE FROM ProgrammingTest WHERE activityID = (:activityID) "
                  "DELETE FROM [File] WHERE activityID = (:activityID)");
    query.bindValue(":activityID",activityID);
    query.exec();
    return status;
}

//Grade
bool dbManager::deleteAllGrades(int activityID){
    bool status = db.isOpen();
    QSqlQuery query(db);
    query.prepare("DELETE FROM Grade WHERE rubricItemID IN ( SELECT rubricItemID FROM RubricItem WHERE activityID = (:activityID) ) "
                  "DELETE FROM GradingRecord WHERE activityID = (:activityID)");
    query.bindValue(":activityID",activityID);
    query.exec();
    return status;
}

/*======================================================================= *
 * ===*********************PRIVATE FUNCTIONS*******************========== *
 * ====================================================================== */

bool dbManager::insertQuerytoDatabase(QString query){
    bool insertSuccess = true;

    if(db.isOpen()){

        QSqlQueryModel model;
        model.setQuery(query, db);

    } else {
        qDebug() << "ERROR: Connection not open";
        insertSuccess = false;
    };
    return insertSuccess;
}
