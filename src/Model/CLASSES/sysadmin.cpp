/************************************
 *
 * File Name: sysadmin.cpp
 * Purpose  : This class handles system admin functionality
 *
 * Authors:
 *      ihoo
 *
 * Revision: 1.0
 *
 ***********************************/

#include "sysadmin.h"

SysAdmin::SysAdmin()
{
  firstName='\0';
  midName='\0';
  lastName='\0';
  userID='\0';
  employeeID='\0';
  password='\0';
}

SysAdmin::SysAdmin(QString _firstName,
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

bool SysAdmin::createNewAccount(QString _firstName,
                             QString _midName,
                             QString _lastName,
                             QString _userID,
                             QString _employeeID,
                             QString _password,
                             bool _isTA,
                             bool _isInstructor,
                             bool _isAdmin,
                             bool _isAdminAssist,
                             bool _isSysAdmin)
{
    dbManager *db = new dbManager();
    return db->insertAccount(_employeeID,
                      _firstName,
                      _midName,
                      _lastName,
                      _userID,
                      _password,
                      true,
                      false,
                      _isAdminAssist,
                      _isSysAdmin,
                      _isAdmin,
                      _isTA,
                      _isInstructor);

}

bool SysAdmin::userNameExists(QString userName)
{
   dbManager *db = new dbManager();
   bool rv=db->userNameExists(userName);
   delete db;
   return rv;
}

bool SysAdmin::employeeIDExists(QString employeeID)
{
  dbManager db;
  bool rv = db.employeeIDexists(employeeID);
  return rv;
}

bool SysAdmin::modifyAccount(int _accountID,
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
                             bool _isSysAdmin)
{
    dbManager db;
    bool rv=db.updateUserInfo(_accountID, _firstName, _midName, _lastName, _userID, _password, _employeeID);
    rv=db.updateUserRole(_accountID, _isAdminAssist, _isAdmin, _isInstructor, _isSysAdmin, _isTA);
    return rv;
}

bool SysAdmin::deleteAccount(QString _employeeID)
{
    dbManager db;
    int accID=db.retrieveAccountID(_employeeID);
    bool rv=db.deleteAccount(accID);
    return rv;
}

bool SysAdmin::blockAccount(QString userID)
{
    dbManager db;
    bool rv = db.blockUserACC(userID,true);
    return rv;
}

bool SysAdmin::unblockAcount(QString userID)
{
    dbManager db;
    bool rv = db.blockUserACC(userID,false);
    return rv;
}

int SysAdmin::retrieveAccID(QString _employeeID)
{
    dbManager db;
    return db.retrieveAccountID(_employeeID);
}
