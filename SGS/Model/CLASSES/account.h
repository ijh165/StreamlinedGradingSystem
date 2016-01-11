/************************************
 *
 * File Name: account.h
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

#ifndef ACCOUNT_H
#define ACCOUNT_H

//include statements
#include <QString>

class Account
{

private:

    //roles
    bool isSysAdmin;
    bool isAdminAssist;
    bool isAdmin;
    bool isInstructor;
    bool isTA;
    bool isBlocked;

    //block toggle boolean
    bool isNewAccount;

protected:

    //account info
    QString firstName;
    QString midName;
    QString lastName;
    QString userID;
    QString employeeID;
    QString password;
    int accountID;


public:

    //constructors and destructors
    Account();
    Account(QString _userID);
    Account(QString _employeeID, int stub);
    Account(QString _firstName,
            QString _midName,
            QString _lastName,
            QString _userID,
            QString _employeeID,
            QString _password,
            bool _isTA,
            bool _isInstructor,
            bool _isAdmin,
            bool _isAdminAssist,
            bool _isSysAdmin);
    ~Account();

    //setters
    void setFirstName(QString _firstName);
    void setMidName(QString _midName);
    void setLastName(QString _lastName);
    void setUserID(QString _userID);
    void setEmployeeID(QString _employeeID);
    void setPassword(QString _password);

    //role setters
    void setIsSysAdmin(bool bool_val);
    void setIsAdminAssist(bool bool_val);
    void setIsAdmin(bool bool_val);
    void setIsInstructor(bool bool_val);
    void setIsTA(bool bool_val);
    void setIsBlocked(bool bool_val);
    void setIsNewAccount(bool bVal);

    //getters
    QString getFirstName();
    QString getMidName();
    QString getLastName();
    QString getUserID();
    QString getEmployeeID();
    QString getPassword();
    QString getCourseName(int RecordNumber);
    QString getCourseNumber(int RecordNumber);
    QString getStartDate(int RecordNumber);
    QString getEndDate(int RecordNumber);
    bool isAccountSysAdmin();
    bool isAccountAdminAssist();
    bool isAccountAdmin();
    bool isAccountInstructor();
    bool isAccountTA();
    bool isAccountFirstLogin();
    bool isAccountBlocked();
    int getAccountID();

    //misc functions
    int getcourseNumCount();
    int getactivityNumCount();
    QString searchmyCourse(int recordNumber);
    void searchmyActivity(int rowNumber, int recordNumber,QString &ActivityName,QString &ActivityType);


    int retrieveCourseID(QString courseName, QString courseNumber, QString startDate, QString endDate);

    void getownActivityInfo_Data(int rowNumberofCourseList,QString activityName,
                                             QString &dueDateTime, QString &pathToSolution ,QString &pathToSubmissions,QString &language, QString &ActivityID, QString &courseID);

};

#endif // ACCOUNT_H
