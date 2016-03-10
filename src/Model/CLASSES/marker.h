/************************************
 *
 * File Name: marker.h
 * Purpose  : This class is stub
 *
 * Authors:
 *      ihoo
 *      rca71
 *
 * Revision: 1.0
 *
 ***********************************/

#ifndef MARKER_H
#define MARKER_H

//include statements
#include "account.h"
#include <QString>

class Marker: public Account
{

public:

    //constructors
    Marker();

    //member functions
    void updateGrade(QString courseName, QString activityName, QString stdID, QString groupName);

};

#endif // MARKER_H
