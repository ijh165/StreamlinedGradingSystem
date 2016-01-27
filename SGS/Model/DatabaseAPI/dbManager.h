/************************************
 *
 * File Name: dbmanager.h
 * Purpose  : This is the data abstraction layer of the SGS system
 *            It manages all the DB queries and return data to the associated models.
 *
 * Authors:
 *      yklam
 *      kdehkhan
 *
 * Revision: 1.0
 *
 ***********************************/

#ifndef DBMANAGER
#define DBMANAGER
#include <QString>
#include <QtSql>

using namespace std;

class dbManager
{
public:

/*======================================================================= *
 * ===*********************DB CONNECTIONS**********************========== *
 * ====================================================================== */

    dbManager();
    dbManager(QString connectionName);
    ~dbManager();

/*======================================================================= *
 * ===*********************INSERT FUNCTIONS********************========== *
 * ====================================================================== */

    //Account
    bool insertAccount(QString employeeID, QString firstName, QString middleName, QString lastName, QString userName, QString password, bool isPasswordTemp, bool islocked, bool isAdminAssist, bool isSystemAdmin, bool isAdmin, bool isTA, bool isInstructor);

    //TA
    bool insertTA( int accountID, int courseID);

    //Students
    bool insertStudentToCourse(int courseID, QString studentID, QString studentName);

    //Student Submission

    //Course
    bool insertCourse(QString courseNumber, QString startDate, QString endDate, QString courseName, QString instructorEmployeeID);

    //Grading (TA/TM/Instructor/Administrator)
    bool assignStudentGrade(QString studentID, int rubricItemID, QString earnedGrade);          //Assign the grade of a rubric item to a student
    //------------------------------------------------------------------------------------------(Can be used for update as well and it will replace the old grade)

    //Grading Record
    bool insertGradingRecord(int activityID, QString studentID, QString lastGradedRole, QString userName);  //Store role information of the last marker of the activity

    //Activity
    int insertActivity(int courseID, QString activityName, QString activityType, QString dueDateTime, QString language, QString pathToSubmissions, int numberOfTests, QString pathToSolution);      //Returns activityID after activity creation

    //Bonus and Late Penalty
    int insertBonusPenalty(int activityID, int bonusPenaltyTypeID, int hours, int amount);
    int insertBonusPenaltyType(QString type);

    //Rubric
    bool insertRubricItem(int activityID, int rubricItemNumber, QString rubricItemText, int rubricItemValue);

    //Multiple Choice
    bool insertMCQItem(int activityID, int questionNumber, QString response, int weight);

    //Programming
    int insertProgrammingTest(int activityID, int testNumber, int numberOfInputFiles, int numberOfOutputFiles, QString consoleInputPath, QString consoleOutputPath, QString environment);    //Returns testID after the test info is inserted
    int insertFileType(QString type);   //Returns fileTypeID after insertion
    int insertFilePath(int activityID, int fileTypeID, int testNumber, QString pathToFile); //Returns fileID after the path is inserted

/*======================================================================= *
 * ===*********************GET FUNCTIONS***********************========== *
 * ====================================================================== */

    //Account
    QString retrieveUserPW(QString userName);               //Returns user password by userName
    int retrieveAccountID(QString employeeID);              //Returns accountID by employeeID
    bool employeeIDexists(QString employeeID);              //Returns the existence of an account by employeeID
    bool userNameExists(QString userName);                  //Returns the existence of a userName by userName
    bool userNameExists_EID(QString employeeID);            //Returns the existence of a userName by employeeID
    QSqlQueryModel* getAllUserInfo(QString userName);       //Returns all columns in the Account table by userName
    QSqlQueryModel* getAllUserInfo_EID(QString employeeID); //Returns all columns in the Account table by employeeID
    QSqlQueryModel* getListAccount();                       //Returns (employeeID | firstName | lastName | userName | islocked)
    int countAccountList();                                 //Returns number of account current in the database
    bool isFirstLogin(QString userName);                    //Returns if the account is initialized
    bool isBlocked(QString userName);                       //Returns if the account is blocked
    int attemptCount(QString userName);                     //Returns log-in attempt count of an account

    //Students
    QSqlQueryModel* getListStudent(int courseID);           //Returns all students associated with the course

    //Course
    bool activityAttached(int courseID);                    //Returns if any activity is attached to the course (For prior deletion of a course)
    int retrieveCourseID(QString courseName, QString courseNumber, QString startDate, QString endDate);
    int getNumberOfAssignedCourses(QString employeeID);     //Returns number of assigned course to an instructor
    QSqlQueryModel* getCourseInfo(QString courseID);        //Returns all course INFO
    QSqlQueryModel* getListInstructor(QString courseID);    //Returns instructor who's assigned to a course
    QSqlQueryModel* getListTA(QString courseID);            //Returns (employeeID | firstName | middleName | lastName)
    QSqlQueryModel* getAssignedCourses(QString employeeID); //Returns (courseName | courseNumber | startDate | endDate)
    QSqlQueryModel* getAssignedCoursesTATM(QString employeeID);//(FOR TA/TM) Returns (courseName | courseNumber | startDate | endDate)
    QSqlQueryModel* getListOfAllCourses();                  //Returns all the courses in the course table
    int countCourseList();                                  //Returns number of account current in the database

    //Activity
    QSqlQueryModel* getListActivity(QString courseID);                          //Returns a list of activity belongs to a courseID
    int getActivityID(int courseID, QString activityName);                      //Returns activityID
    int activityExists(int courseID, QString actiivtyName);                     //Returns activityID if exists, returns -1 if it doesn't
    QSqlQueryModel* getActivityInfo(QString courseID, QString activityName);    //Returns activity info by courseID and activityName
    QSqlQueryModel* getActivityInfo_AID (int activityID);                       //Returns activity info by activityID

    //Grading (TA/TM/Instructor/Administrator)
    int getNumberOfAssignedActivity(QString employeeID);            //Returns number of assigned activities to a Marker
    QSqlQueryModel* getAssignedActivity(QString employeeID);        //FOR INSTRUCTOR GRADING (Shows only activities in the courses the instructor is teaching or taught)
    QSqlQueryModel* getAssignedActivityTATM(QString employeeID);    //FOR TATM GRADING (Shows only activities in the courses the TA/TM is tutoring)
    QSqlQueryModel* getStudentActivityGrade(int activityID, QString studentID);     //Returns rubricItemID and corresponding student grade associated with the activity

    //Multiple Choice
    QSqlQueryModel* getListMCQItems(QString activityID);        //Returns the set of MCQ answer keys of the activity

    //Rubric
    QSqlQueryModel* getRubric(QString activityID);              //Returns all Rubric items associated by the activityID

    //Programming
    QSqlQueryModel* getListOfProgrammingTestInfo(int activityID);   //Return list of (testNumber | numberOfInputFiles | numberOfOutputFiles | environment)
    QString getFileType(int fileTypeID);                            //Returns the file extension type of the file
    QString getFilePath(int fileID);                                //Returns the path to the file
    QSqlQueryModel* getAllFilePaths(int activityID);                //Returns all the paths to the test files associated to an activity

    //GradingRecord
    QSqlQueryModel* getGradingRecord(int activityID);           //Returns all the grades for a given activity.

/*======================================================================= *
 * ===*********************UPDATE FUNCTIONS********************========== *
 * ====================================================================== */

    //Account
    bool updateUserInfo(int accountID, QString firstName, QString middleName, QString lastName, QString userName, QString password, QString employeeID);
    bool updateUserRole(int accountID, bool isAdminAssist, bool isAdmin, bool isInstructor, bool isSystemAdmin, bool isTA);
    bool changeUserPW(QString password, QString userName);              //Change the password of an account
    bool blockUserACC(QString userName, bool islocked);                 //Locks an account by its userName
    bool setAccIsPasswordTemp(QString userName, bool isPasswordTemp);   //Set password of an account to be temporary
    bool increAttemptCtr(QString userName);     //Increment Log-in attempt counter by 1
    bool resetAttemptCtr(QString userName);     //Reset Log-in attempt counter to default ( = 0 )

    //Course
    bool updateCourseInfo(int courseID, QString courseName, QString courseNumber, QString startDate, QString endDate, QString instructorEmployeeID);
    bool insertOneInstToCourse(QString courseID, QString instructorEmployeeID); //Inserts or reassigns a new instructor to after a course is created

    //Activity
    bool updateActivity(QString activityID, QString courseID, QString activityName, QString activityType, QString dueDateTime, QString language, QString pathToSubmissions, QString numberOfTests, QString pathToSolution);


    //TestFileType
    bool updateTestFileType(QString fileTypeID,QString type);

    //File

    //Remember to name the File table as [File]
    bool updateFile(QString fileID,QString activityID, QString fileTypeID, QString testNumber, QString pathToFile, QString extension);

/*======================================================================= *
 * ===*********************DELETE FUNCTIONS********************========== *
 * ====================================================================== */

    //Account
    bool deleteAccount(int accountID);

    //TA
    bool deleteCourseTA(int courseID);          //Delete all TAs assigned to a course.

    //Students
    bool deleteStudentFromCourse(int courseID, QString studentID);  //Delete a specific student from a course.
    bool deleteAllStudentFromCourse(int courseID);                  //Delete all students associated to a course.

    //Course
    bool deleteCourse(int courseID);

    //Activity
    bool deleteActivity(int activityID);

    //Multiple Choice
    bool deleteAllMCQItems(int activityID);     //Delete all MCQ items associated with an activity.

    //Rubric
    bool deleteAllRubricItems(int activityID);  //Delete all rubric items associted with an activity.

    //Programming
    bool deleteAllTestFiles(int activityID);    //Delete all console test files and test files associated to an activity.

    //Grade
    bool deleteAllGrades(int activityID);       //Delete all the grades and grading record associated with an activity.

/*======================================================================= *
 * ===*********************PRIVATE FUNCTIONS*******************========== *
 * ====================================================================== */

private:
    QString connectionTemplate;
    QString connectionString;
    QSqlDatabase db;
    bool insertQuerytoDatabase(QString query);
};

#endif // DBMANAGER
