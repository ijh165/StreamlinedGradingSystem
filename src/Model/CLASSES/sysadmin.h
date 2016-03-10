/************************************
 *
 * File Name: sysadmin.h
 * Purpose  : This class handles system admin functionality
 *
 * Authors:
 *      ihoo
 *
 * Revision: 1.0
 *
 ***********************************/

#ifndef SYSADMIN_H
#define SYSADMIN_H

//include statements
#include "account.h"
#include <QString>

class SysAdmin: public Account
{

public:

    //constructors and destructors
    SysAdmin();
    SysAdmin(QString _firstName,
             QString _midName,
             QString _lastName,
             QString _userID,
             QString _employeeID,
             QString _password);

    //member functions
    bool createNewAccount(QString _firstName,
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
    bool modifyAccount(int _accountID, //update changes in database
                       QString _firstName,
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
    bool deleteAccount(QString _employeeID);
    bool blockAccount(QString userID);
    bool unblockAcount(QString userID);
    int retrieveAccID(QString _employeeID);

    //check for existing username
    bool userNameExists(QString userName);
    bool employeeIDExists(QString employeeID);

};

#endif // SYSADMIN_H
