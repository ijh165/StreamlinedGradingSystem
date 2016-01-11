/**************
 *
 * File name: gui_interaction.cpp
 * Purpose  : This file mainly handles interaction between
 *            SGS gui with model and database
 *
 * Authors:
 *      ftran
 *      phoi
 *      ihoo
 *      rca71
 *      cyrusc
 *      yklam
 *      kdehghan
 *      yangjaey
 *      salkhali
 *
 * Revision: 1.0
 *
 **************/

#include "streamlinedgradingsystem.h"
#include "ui_streamlinedgradingsystem.h"
#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QFile>
#include <QtGui/QDesktopServices>

#include "Control/DatabaseAPI/dbmanager.h"
#include "Model/CLASSES/adminassist.h"
#include "Model/CLASSES/account.h"
#include "Model/CLASSES/sysadmin.h"
#include "Model/CLASSES/instructor.h"
#include "Model/CLASSES/testcase.h"


//Instructor wishes to export grades to csv. Credits to: http://stackoverflow.com/questions/4487799/export-data-from-qtablewidget-to-csv
void StreamlinedGradingSystem::exportGrades()
{
    QString filePath=QFileDialog::getSaveFileName(this,tr("Exporting grades as csv file..."),"C://","CSV (*.csv)");

    if( filePath=="" )
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Canceled exporting grades!");
        return;
    }

    QFile f( filePath );

    if (f.open(QFile::WriteOnly))
    {
        QTextStream data( &f );

        dbManager* db = new dbManager();

        int currRowCourse = ui->CourseList_Marker->currentRow();
        int currRowAct = ui->ActivityList_Marker->currentRow();
        if(currRowCourse<0 || currRowAct<0)
        {
            printLogMessage(ERROR_MESSAGE_TEXT,"Please select an activity!");
            return;
        }

        int selActID = (tempActivityIDList[currRowAct]).toInt();

        int numOfStd = ui->studentList_Marking->rowCount();

        for(int i = 0 ; i < numOfStd ; i++)
        {
            QString stdID = ui->studentList_Marking->item(i,0)->text();
            QSqlQueryModel* model = db->getStudentActivityGrade(selActID,stdID);
            int rowNum = model->rowCount();
            QString ithStdGrades;
            for(int j=0; j<rowNum; j++)
            {
                if(j==rowNum-1)
                {
                    ithStdGrades += model->record(j).value("earnedGrade").toString();
                    break;
                }
                ithStdGrades += model->record(j).value("earnedGrade").toString() + ",";
            }
            data << stdID + "," + ithStdGrades + "\n";
        }
    }

    printLogMessage(CONFIRM_MESSAGE_TEXT,"Exported to " + filePath);
}

/* validate password entered by user */
bool StreamlinedGradingSystem::validatePassword(QString username, QString password, bool &isFirstLogin)
{
#define MAX_LOGIN_FAIL 5
    dbManager db;
    bool rv;
    int consecutiveLoginFail = 0;

    /* check username and password against database */
    if (db.userNameExists(username) == true)
    {
        if (true == db.isBlocked(username))
        {
            rv = false;
            printLogMessage(ERROR_MESSAGE_TEXT, "Error: Account is locked, please contact System Administrator to unlock account");
        }
        else
        {
            if (password == db.retrieveUserPW(username))
            {
                //successful login
                db.resetAttemptCtr(username);
                printLogMessage(CONFIRM_MESSAGE_TEXT, "Login OK");
                currentUser = Account(username); //instantiate the current logged in user object
                isFirstLogin = currentUser.isAccountFirstLogin();
                if (isFirstLogin == true) printLogMessage(CONFIRM_MESSAGE_TEXT, "first login");
                rv = true;
            }
            else
            {
                rv = false;
                //increment fail login attempt counter
                db.increAttemptCtr(username);
                consecutiveLoginFail = db.attemptCount(username);
                if (consecutiveLoginFail <= MAX_LOGIN_FAIL)
                {
                    printLogMessage(ERROR_MESSAGE_TEXT, "Error: Invalid password. Account will be locked after 6 login failure.<br>\
                                    In case of forgotten password,  contact System Administrator");
                }
                else
                {
                    // lock user account
                    db.blockUserACC(username, true);
                    printLogMessage(ERROR_MESSAGE_TEXT, "Error: Failed to log in 6 times.<br>\
                                    Account has been locked,  contact System Administrator for a new password");
                }
            }
        }
    }
    else{
        printLogMessage(ERROR_MESSAGE_TEXT, "Error: Invalid username");
        rv = false;
    }

    return rv;
}

/* handle changing password from gui to database */
bool StreamlinedGradingSystem::changePassword(QString oldPassword, QString newPassword, QString reEnterPassword)
{
    bool rv;
    dbManager db;
    if ((oldPassword == currentUser.getPassword())&& (newPassword == reEnterPassword))
    {
        //check if new password is the same as old password
        if (oldPassword == newPassword)
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "New password cannot be the same as old password");
            rv = false;
        }
        else
        {
            //changeAccountPassword
            currentUser.setPassword(newPassword);
            db.changeUserPW(newPassword, currentUser.getUserID());
            // clear first account login status
            if (currentUser.isAccountFirstLogin() == true)
            {
                currentUser.setIsNewAccount(false);
                db.setAccIsPasswordTemp(currentUser.getUserID(), false);
                qDebug()<< "account is no longer temp";
            }

            printLogMessage(CONFIRM_MESSAGE_TEXT, "Password changed");
            rv = true;
        }
    }
    else{
        printLogMessage(ERROR_MESSAGE_TEXT, "incorrect password entered");
        rv = false;
    }

    return rv;
}


/* Create new account in database using information from GUI */
bool StreamlinedGradingSystem::createNewAccount()
{
    bool rv = true;

    SysAdmin systemAdmin;
    bool result;
    /* verify user input:
   * required info is filled
   * username is <= 8 chars, employeeID is 12 chars,
   * at least 1 role is selected
   */
    // check for empy input
    if (  (ui->FirstNameCreate->text().size() ==0) ||
          (ui->LastNameCreate->text().size() == 0) ||
          (ui->EmployeeIDCreate->text().size() == 0) ||
          (ui->UserIDCreate->text().size() == 0) ||
          (ui->UserIDCreate->text().size() == 0) ||
          (ui->PasswordCreate->text().size() == 0)
          )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Required information is empty");
        return false;
    }
    //check input type
    QRegExp re("\\d*");
    if ( !re.exactMatch(ui->EmployeeIDCreate->text()))
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Employee ID must be number");
        return false;
    }

    //check input length
    if (ui->UserIDCreate->text().size() > 8)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "User ID must be within 8 letters");
        return false;
    }
    if (ui->EmployeeIDCreate->text().size() != 12)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Employee ID must be 12 digits");
        return false;
    }
    // check if at least 1 role is selected
    if (!((ui->Instructor_checkBox->isChecked() == true) ||
          (ui->TA_checkBox->isChecked() == true) ||
          (ui->SA_checkBox->isChecked() == true) ||
          (ui->AdministrativeAdmin_checkBox->isChecked() == true) ||
          (ui->Admin_checkBox->isChecked() == true)
          ))
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "User must have at least one role");
        return false;
    }
    // check if employeeID already exist
    if (true == systemAdmin.employeeIDExists(ui->EmployeeIDCreate->text()))
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "EmployeeID already exists");
        return false;
    }

    if ( false == systemAdmin.userNameExists(ui->UserIDCreate->text()))
    {
        result = systemAdmin.createNewAccount(
                    ui->FirstNameCreate->text(),
                    ui->MiddleNameCreate->text(),
                    ui->LastNameCreate->text(),
                    ui->UserIDCreate->text(),
                    ui->EmployeeIDCreate->text(),
                    ui->PasswordCreate->text(),
                    ui->TA_checkBox->isChecked(),
                    ui->Instructor_checkBox->isChecked(),
                    ui->Admin_checkBox->isChecked(),
                    ui->AdministrativeAdmin_checkBox->isChecked(),
                    ui->SA_checkBox->isChecked());
        if (true == result)
        {
            rv = true;
        }
        else
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "Failed to create account. Database error");
            rv = false;
        }
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Username already exists");
        rv = false;
    }

    return rv;
}

//serachInstructorName through Adminassist.cpp
bool StreamlinedGradingSystem::searchInstructorName()
{
    bool rv = true;
    AdminAssist A;
    QString employeeID = ui->InstructorEmployeeID_CreateCourse->text();
    if(A.checkemployeeIDexist(employeeID))
    {
        if(A.IsInstructor(employeeID))
        {
            clearLogMessage();
            QString Name = A.searchName(employeeID);
            ui->InstructorName_CreateCourse->setText(Name);
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Instructor is added successfully");
            rv = true;
        }
        else
        {
            printLogMessage(ERROR_MESSAGE_TEXT,"Employee does not have instructor role");
            rv = false;
        }
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Employee ID does not exist");
        rv = false;
    }

    return rv;

}

//search the TA list from the employeeID to the list
bool StreamlinedGradingSystem::searchTAName()
{
    bool rv = true;
    AdminAssist A;
    QString employeeID = ui->TAEmployeeID_CreateCourse->text();
    bool notexist = true;
    if(A.checkemployeeIDexist(employeeID))
    {
        QString Name = A.searchName(employeeID);
        if(A.IsTA(employeeID))
        {
            for (int i=0; i < ui->TAList_CreateCourse->rowCount(); i++)
            {
                if(employeeID == ui->TAList_CreateCourse->item(i,0)->text())
                {
                    notexist = false;
                }
            }
            if(notexist)
            {
                ui->TAList_CreateCourse->insertRow(ui->TAList_CreateCourse->rowCount());
                ui->TAList_CreateCourse->setItem(ui->TAList_CreateCourse->rowCount()-1,0, new QTableWidgetItem(employeeID));
                ui->TAList_CreateCourse->setItem(ui->TAList_CreateCourse->rowCount()-1,1, new QTableWidgetItem(Name));
                ui->TAEmployeeID_CreateCourse->setText("");
                printLogMessage(CONFIRM_MESSAGE_TEXT,"TA is added!");
            }
            else
            {
                printLogMessage(ERROR_MESSAGE_TEXT,"Employee already on the List");
            }
            rv = true;
        }
        else
        {
            printLogMessage(ERROR_MESSAGE_TEXT,"Employee does not have TA role");
            rv = false;
        }
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Employee ID does not exist");
        rv = false;
    }
    return rv;

}

//remove the TA from the list in side course page
bool StreamlinedGradingSystem::removefromthelist(){
    bool rv = true;
    bool notexist = true;
    QString employeeID = ui->TAEmployeeID_CreateCourse->text();
    for (int i=0; i < ui->TAList_CreateCourse->rowCount(); i++)
    {
        if(employeeID == ui->TAList_CreateCourse->item(i,0)->text())
        {
            ui->TAList_CreateCourse->removeRow(i);
            ui->TAEmployeeID_CreateCourse->setText("");
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Successfully removed from TA List");
            rv = true;
            notexist = false;
            break;
        }
    }
    if(notexist){
        printLogMessage(ERROR_MESSAGE_TEXT,"Employee ID does not on the list");
        rv = false;
    }
    return rv;
}


/* User confirm course creation (or modification) */
bool StreamlinedGradingSystem::createNewCourse()
{

    AdminAssist A;
    Account Acc;
    bool rv = true;
    QString CourseName =  ui->CourseNameCreate_CreateCourse->text();
    QString CourseNum = ui->CourseNumberCreate_CreateCourse->text();
    QString startDate = ui->StartDateCreate_CreateCourse->date().toString("yyyy/MM/dd");
    QString endDate  = ui->EndDateCreate_CreateCourse->date().toString("yyyy/MM/dd");;
    //QString InstructorName = ui->InstructorName_CreateCourse->text();
    QString InstructorEmployeeID = ui->InstructorEmployeeID_CreateCourse->text();

    //check for unfilled stuff and put info to database
    if((CourseName == "Enter Course Name")||(CourseNum =="Enter Course Number")||(CourseName.size() ==0) ||
            (CourseNum.size() == 0) ||
            (startDate.size() == 0) ||
            (endDate.size() == 0))
    {
        rv = false;
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill out all required information");
    }
    else if (ui->EndDateCreate_CreateCourse->date() <= ui->StartDateCreate_CreateCourse->date())
    {
        rv = false;
        printLogMessage(ERROR_MESSAGE_TEXT,"End date must be after start date");
    }
    else
    {
        if(A.createCourse(CourseName,
                          CourseNum,
                          startDate,
                          endDate,
                          InstructorEmployeeID))
        {

            int courseID = Acc.retrieveCourseID(CourseName,CourseNum,startDate,endDate);

            //insert TAs
            for (int i=0; i < ui->TAList_CreateCourse->rowCount(); i++)
            {
                QString employeeID = ui->TAList_CreateCourse->item(i,0)->text();
                bool successAssignTA=A.assignOneTATMToCourse(employeeID,courseID);
                if(successAssignTA==false)
                {
                    printLogMessage(ERROR_MESSAGE_TEXT,"Fail to assign one TA. Database error.");
                    return false;
                }
            }

            //insert stds
            for (int i=0; i < ui->StudentList_CreateCourse->rowCount(); i++)
            {
                //retrieve info from GUI
                QString stdID = ui->StudentList_CreateCourse->item(i,0)->text();
                QString firstName = ui->StudentList_CreateCourse->item(i,1)->text();
                QString midName = ui->StudentList_CreateCourse->item(i,2)->text();
                QString lastName = ui->StudentList_CreateCourse->item(i,3)->text();

                //concatenate stdName
                QString stdName = firstName + ", " + midName + ", " + lastName;

                //              qDebug()<<ui->StudentList_CreateCourse->rowCount();
                //              qDebug()<<stdID;
                //              qDebug()<<stdName;

                //insert a student
                bool successAssignStd=A.assignOneStdToCourse(courseID,stdName,stdID);
                if(successAssignStd==false)
                {
                    printLogMessage(ERROR_MESSAGE_TEXT,"Fail to assign one student. Database error.");
                    return false;
                }
            }

            rv = true;
        }
        else
        {
            rv = false;
        }
    }
    return rv;
}

//Create Activity Function
bool StreamlinedGradingSystem::searchOwnCourse(){
    bool rv = true;
    for(int i = 0;i<currentUser.getcourseNumCount();i++)
    {
        QString CourseName = currentUser.searchmyCourse(i);
        ui->CourseList_ActivityType->addItem(CourseName);
        rv = true;
    }
    return rv;
}

/* Display the list of course the user is assigned to on ui->CourseList_xx */
bool StreamlinedGradingSystem::viewOwnCourse(){
    bool rv = true;
    QString CourseDisplay;

    //get the list of course from db
    dbManager db;
    if (ui->SelectRoleBox->currentText() == INSTRUCTOR_ROLE_TEXT)
    {
        qDebug() << "get course list for instructor";
        sqlModel = db.getAssignedCourses(currentUser.getEmployeeID());
    }
    else if (ui->SelectRoleBox->currentText() == TA_ROLE_TEXT)
    {
        qDebug() << "get course list for TA";
        sqlModel = db.getAssignedCoursesTATM(currentUser.getEmployeeID());
    }
    else if (ui->SelectRoleBox->currentText() == ADMINISTRATOR_ROLE_TEXT)
    {
        qDebug() << "get all course list for Admin";
        sqlModel = db.getListOfAllCourses();
    }
    if (NULL == sqlModel){
        qDebug() << "could not get course list";
        printLogMessage(ERROR_MESSAGE_TEXT, "Error connecting to DB");
        return false;
    }
    //clear current courselists (if exist)
    ui->CourseList_SelectModifyPage->clear();
    ui->CourseList_ActivityType->clear();
    ui->CourseList_CopyActivityPage->clear();
    ui->CourseList_Marker->clear();

    tempCourseIDList.clear();
    //display on table
    int courseCount = sqlModel->rowCount();
    qDebug() << "course count" << courseCount;
    if (courseCount == 0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "You are not assigned to any course");
    }
    for (int rowIndex = 0; rowIndex < courseCount; rowIndex++)
    {
        CourseDisplay = /*sqlModel->record(rowIndex).value("courseName").toString() + " " +*/
                sqlModel->record(rowIndex).value("courseNumber").toString() +
                "\tFrom " + sqlModel->record(rowIndex).value("startDate").toString() +
                " to "+ sqlModel->record(rowIndex).value("endDate").toString();
        // Add to each Course list table
        ui->CourseList_SelectModifyPage->addItem(CourseDisplay);
        ui->CourseList_Marker->addItem(CourseDisplay);
        ui->CourseList_ActivityType->addItem(CourseDisplay);
        ui->CourseList_CopyActivityPage->addItem(CourseDisplay);
        qDebug() << "Add course ID " << sqlModel->record(rowIndex).value("courseID").toString() << "to temp list";
        tempCourseIDList.append(sqlModel->record(rowIndex).value("courseID").toString());
    }

    delete sqlModel;
    sqlModel = NULL;
    return rv;
}

/* Display activity list of selected course on ui->ActivityList_xx */
bool StreamlinedGradingSystem::viewOwnCourseActivity(){
    bool rv = true;
    dbManager db;
    int rowNumber;

    if (ui->stackedWidget->currentIndex() == SELECT_MODIFY_ACTIVITY_INDEX)
    {
        rowNumber = ui->CourseList_SelectModifyPage->currentRow();
    }
    else if (ui->stackedWidget->currentIndex() == MARK_MAIN_PAGE_INDEX)
    {
        rowNumber = ui->CourseList_Marker->currentRow();
    }

    QString selectedCourseID = tempCourseIDList[rowNumber];

    qDebug() << "get activity list for course ID" << selectedCourseID;
    //get Activitylist of the courseID from db
    sqlModel = db.getListActivity(selectedCourseID);
    if (NULL == sqlModel)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Error retrieving activity list from database");
        return false;
    }
    if (sqlModel->rowCount() == 0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "This course does not have an activity");
    }

    //clear list (if exist)
    ui->ActivityList_SelectModifyPage->clear();
    while (ui->ActivityList_SelectModifyPage->rowCount() > 0)
    {
        ui->ActivityList_SelectModifyPage->removeRow(0);
    }
    ui->ActivityList_Marker->clear();
    while (ui->ActivityList_Marker->rowCount() > 0)
    {
        ui->ActivityList_Marker->removeRow(0);
    }
    //reset the header title
    QStringList Tableheader;
    Tableheader << "Activity Name" << "Activity Type";
    ui->ActivityList_SelectModifyPage->setHorizontalHeaderLabels(Tableheader);
    ui->ActivityList_Marker->setHorizontalHeaderLabels(Tableheader);
    qDebug() << "clear temp activity list";
    tempActivityIDList.clear();
    //display activity list on table
    qDebug() << "activity count " << sqlModel->rowCount();
    for (int index = 0; index < sqlModel->rowCount(); index ++)
    {
        //activity list on select activity to modify page
        ui->ActivityList_SelectModifyPage->insertRow(ui->ActivityList_SelectModifyPage->rowCount());
        ui->ActivityList_SelectModifyPage->setItem(index, 0, new QTableWidgetItem(sqlModel->record(index).value("activityName").toString()));
        ui->ActivityList_SelectModifyPage->setItem(index, 1, new QTableWidgetItem(sqlModel->record(index).value("activityType").toString()));
        //activity list on main marking page
        ui->ActivityList_Marker->insertRow(ui->ActivityList_Marker->rowCount());
        ui->ActivityList_Marker->setItem(index, 0, new QTableWidgetItem(sqlModel->record(index).value("activityName").toString()));
        ui->ActivityList_Marker->setItem(index, 1, new QTableWidgetItem(sqlModel->record(index).value("activityType").toString()));
        qDebug() << "add to temp activity list";
        tempActivityIDList.append(sqlModel->record(index).value("activityID").toString());
    }

    delete sqlModel;
    sqlModel = NULL;
    return rv;
}

/* display the rubric for the currently selected activity to ui*/
void StreamlinedGradingSystem::loadRubric(Activity currentActivity)
{
    QVector<rubricItem_t> rubricItemList;
    int rubricItemCount = 0;
    if (currentActivity.getRuricItemList(rubricItemList))
    {
        /* add items to rubric table */
        rubricItemCount = rubricItemList.size();
        qDebug() << "rubric size" << rubricItemCount;
        for (int i=0; i<rubricItemCount; i++)
        {
            //add a new row to the table
            ui->rubricTable->insertRow(i);
            /* allocate memory to each new cell */
            ui->rubricTable->setItem(i,RUBRIC_ITEM_NUMBER, new QTableWidgetItem);
            ui->rubricTable->setItem(i,RUBRIC_ITEM_DESC, new QTableWidgetItem);
            ui->rubricTable->setItem(i,RUBRIC_ITEM_GRADE, new QTableWidgetItem);

            qDebug() <<"Item num "<< rubricItemList[i].itemNum;
            ui->rubricTable->item(i,RUBRIC_ITEM_NUMBER)->setText(QString::number(rubricItemList[i].itemNum));
            qDebug() <<"Item desc " << rubricItemList[i].itemDescription;
            ui->rubricTable->item(i,RUBRIC_ITEM_GRADE)->setText(QString::number(rubricItemList[i].itemGrade));
            qDebug() <<"Item desc " << rubricItemList[i].itemDescription;
            ui->rubricTable->item(i,RUBRIC_ITEM_DESC)->setText(rubricItemList[i].itemDescription);
        }

    }

}


/* Essay Activity Part */
/* Save the new Essay essay activity info with infromation from ui into database */
bool StreamlinedGradingSystem::createEssayActivity(){
    Instructor myinstructor;
    bool rv = true;
    QString activityName = ui->ActivityName_Essay->text();
    QString dueDate = ui->DueDate_Essay->date().toString("yyyy/MM/dd");
    QString dueTime = ui->DueTime_Essay->dateTime().toString("HH:mm");
    QString dueDateTime = dueDate +" " + dueTime;
    QString pathToSolution = ui->SolutionPath_Essay -> text();
    QString pathToSubmissions = ui->StudentSubmissionPath_Essay-> text();
    QString language = ui->LanguageComboBox_Essay->currentText();

    //create Activity through instructor.cpp
    int newActivityID = 0;

    //check if activity already exist
    dbManager db;
    int result = db.activityExists(currentCourse.getcourseID().toInt(),activityName);
    //if activity already exist, the returned activity ID will be different from the current activityID
    if (result != -1)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Activity name already exists in this course, please change other name");
        rv = false;
    }
    else if(((activityName == "Enter Activity Name")||
             (activityName.size() == 0)||
             (pathToSolution == "//") ||
             (pathToSolution == "") ||
             (language == "Select A Language")))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill all the required information");
        rv = false;
    }
    //Checking if the path added for the console exists
    else if ((pathToSubmissions != "//")&&!QDir(pathToSubmissions).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Submission do not exist");
      rv = false;
    }
    else if (!QFile(pathToSolution).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Solution do not exist");
      rv = false;
    }
    else
    {
        newActivityID = myinstructor.createActivity(activityName, ACTIVITY_TYPE_ESSAY_PROBLEM_SET,dueDateTime,language,pathToSubmissions,0,pathToSolution,currentCourse.getcourseID().toInt());
        qDebug() <<"activity ID" << newActivityID <<" created";
        currentActivity.setActivityID(newActivityID);
        currentActivity.setActType(ACTIVITY_TYPE_ESSAY_PROBLEM_SET);
        /* Add rubric to database, item by item */
        if (false == currentActivity.isRubricEmpty())
        {
            currentActivity.saveRubricToDb();
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Create Essay Activity Successfully");
            qDebug() << "saved rubric";
        }
        else
        {
            printLogMessage(CONFIRM_MESSAGE_TEXT, "Activity created. Warning: No rubric has been created for this activity");
            qDebug() << "no rubric";
        }
        rv = true;
    }

    return rv;
}
//view Essay activity using rowNumber ActivityNumber (only used by copy all activity)
bool StreamlinedGradingSystem::viewEssayActivity(int courseRowNumber,int ActivityrowNumber){
        qDebug()<<"Crash location 4";
    bool rv = true;
    ui->CreateRubic_EssayButton->setVisible(false);
    ui->ModifyRubic_EssayButton->setVisible(true);
    QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    QString dueDateTime ="";
    QString pathToSolution  ="";
    QString pathToSubmissions = "";
    QString language = "";
    activityID ="";
    courseID ="";
        qDebug()<<"Crash location 1";
    currentUser.getownActivityInfo_Data(courseRowNumber,activityName,dueDateTime,pathToSolution,pathToSubmissions,language,activityID,courseID);
    QString Date = dueDateTime.left(10);
    QString Time = dueDateTime.right(8);
    ui->DueDate_Essay->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->DueTime_Essay->setTime(QTime::fromString(Time,"HH:mm:ss"));
    ui->ActivityName_Essay->setText(activityName);
    ui->SolutionPath_Essay->setText(pathToSolution);
    ui->StudentSubmissionPath_Essay->setText(pathToSubmissions);
    qDebug()<<"Crash location 2";
    for (int i=1; i<=4;i++)
    {
        if(ui->LanguageComboBox_Essay->itemText(i)==language)
        {
            ui->LanguageComboBox_Essay->setCurrentIndex(i);
                qDebug()<<"Crash location 3";
        }

    }

    return rv;
}
/* View Essay Activity */
bool StreamlinedGradingSystem::viewEssayActivity(){
        qDebug()<<"Crash location 4";
    bool rv = true;
    ui->CreateRubic_EssayButton->setVisible(false);  //
    ui->ModifyRubic_EssayButton->setVisible(true);  //
    int rowNumber = ui->CourseList_SelectModifyPage->currentRow();
    int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
    QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    QString dueDateTime ="";
    QString pathToSolution  ="";
    QString pathToSubmissions = "";
    QString language = "";
    activityID ="";
    courseID ="";
        qDebug()<<"Crash location 1";
    currentUser.getownActivityInfo_Data(rowNumber,activityName,dueDateTime,pathToSolution,pathToSubmissions,language,activityID,courseID);
    QString Date = dueDateTime.left(10);
    QString Time = dueDateTime.right(8);
    ui->DueDate_Essay->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->DueTime_Essay->setTime(QTime::fromString(Time,"HH:mm:ss"));
    ui->ActivityName_Essay->setText(activityName);
    ui->SolutionPath_Essay->setText(pathToSolution);
    ui->StudentSubmissionPath_Essay->setText(pathToSubmissions);
    qDebug()<<"Crash location 2";
    for (int i=1; i<=4;i++)
    {
        if(ui->LanguageComboBox_Essay->itemText(i)==language)
        {
            ui->LanguageComboBox_Essay->setCurrentIndex(i);
                qDebug()<<"Crash location 3";
        }

    }

    return rv;
}
/* Save Modified Essay Activity */
bool StreamlinedGradingSystem::modifyEssayActivityChange(){
    Instructor myinstructor;
    bool rv = true;

    QString activityName = ui->ActivityName_Essay->text();
    QString dueDate = ui->DueDate_Essay->date().toString("yyyy/MM/dd");
    QString dueTime = ui->DueTime_Essay->dateTime().toString("HH:MM");
    QString dueDateTime = dueDate +" " + dueTime;
    QString pathToSolution = ui->SolutionPath_Essay -> text();
    QString pathToSubmissions = ui->StudentSubmissionPath_Essay-> text();
    QString language = ui->LanguageComboBox_Essay->currentText();
    int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
    QString currentActivityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    dbManager db;
    int result = db.activityExists(currentCourse.getcourseID().toInt(),activityName);
    //create Activity through instructor.cpp
    if (result != -1 && activityName != currentActivityName)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Activity name already Exisits in this course, please change other name");
        rv = false;
    }
    else if(((activityName == "Enter Activity Name")||
        (activityName.size() == 0)||
        (pathToSolution == "//") ||
        (language == "Select A Language")))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill all the required information");
        rv = false;
    }
    //Checking if the path added for the console exists
    else if ((pathToSubmissions != "//")&&!QDir(pathToSubmissions).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Submission do not exist");
      rv = false;
    }
    else if (!QFile(pathToSolution).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Solution do not exist");
      rv = false;
    }
    else
    {
        myinstructor.modifyActivity(activityID,courseID,activityName,ACTIVITY_TYPE_ESSAY_PROBLEM_SET,dueDateTime,language,pathToSubmissions,"0",pathToSolution);
        /* Add rubric to database, item by item */
        currentActivity.saveRubricToDb();
        rv = true;
    }

    return rv;
}

/* MC Activity Part */
/* Save the new MC activity info with information from ui into database */
bool StreamlinedGradingSystem::createMCActivity(){
    Instructor myinstructor;
    bool rv = true;
    int newActivityID = 0;
    QString activityName = ui->ActivityName_MC->text();
    QString dueDate = ui->DueDate_MC->date().toString("yyyy/MM/dd");
    QString dueTime = ui->DueTime_MC->dateTime().toString("HH:MM");
    QString dueDateTime = dueDate +" " + dueTime;
//    QString pathToSolution = ui->SolutionPath_MC -> text();
    QString pathToSubmissions = ui->StudentSubmissionPath_MC-> text();
    QString language = ui->LanguageComboBox_MC->currentText();

    //create Activity through instructor.cpp
    dbManager db;
    int result = db.activityExists(currentCourse.getcourseID().toInt(),activityName);
    //if activity already exist, the returned activity ID will be different from the current activityID
    if (result != -1)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Activity name already exists in this course, please change other name");
        rv = false;
    }
    else if(((activityName == "Enter Activity Name")||
             (activityName.size() == 0)||
             (language == "Select A Language")))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill all the required information");
        rv = false;
    }
    //Checking if the path added for the console exists
    else if ((pathToSubmissions != "//")&&!QDir(pathToSubmissions).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Submission do not exist");
      rv = false;
    }
    else
    {
        newActivityID = myinstructor.createActivity(activityName,ACTIVITY_TYPE_MC,dueDateTime,language,pathToSubmissions,0,"",currentCourse.getcourseID().toInt());
        currentActivity.setActType(ACTIVITY_TYPE_MC);
        currentActivity.setActivityID(newActivityID);
        //Add answer key to database
        if (false == currentActivity.isRubricEmpty())
        {
            currentActivity.saveRubricToDb();
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Create MC Activity Successfully");
        }
        else
        {
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Create MC Activity Successfully but Answer Key has not been entered for the activity");
        }
        rv = true;
    }

    return rv;
}
//View MC Activity
bool StreamlinedGradingSystem::viewMCActivity(){
    bool rv = true;
    int rowNumber = ui->CourseList_SelectModifyPage->currentRow();
    int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
    QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    QString dueDateTime ="";
    QString pathToSolution  ="";
    QString pathToSubmissions = "";
    QString language = "";
    activityID ="";
    courseID ="";
    currentUser.getownActivityInfo_Data(rowNumber,activityName,dueDateTime,pathToSolution,pathToSubmissions,language,activityID,courseID);
    qDebug()<<"Course ID from the copy course";
    QString Date = dueDateTime.left(10);
    QString Time = dueDateTime.right(8);
    ui->DueDate_MC->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->DueTime_MC->setTime(QTime::fromString(Time,"HH:mm:ss"));
    ui->ActivityName_MC->setText(activityName);
//    ui->SolutionPath_MC->setText(pathToSolution);
    ui->StudentSubmissionPath_MC->setText(pathToSubmissions);
    for (int i=1; i<=4;i++)
    {
        if(ui->LanguageComboBox_MC->itemText(i)==language)
        {
            ui->LanguageComboBox_MC->setCurrentIndex(i);
        }

    }
    return rv;
}

//view MC activity using rowNumber ActivityNumber (only used by copy all activity)
bool StreamlinedGradingSystem::viewMCActivity(int rowNumber,int ActivityrowNumber){
    bool rv = true;
    QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    QString dueDateTime ="";
    QString pathToSolution  ="";
    QString pathToSubmissions = "";
    QString language = "";
    activityID ="";
    courseID ="";
    currentUser.getownActivityInfo_Data(rowNumber,activityName,dueDateTime,pathToSolution,pathToSubmissions,language,activityID,courseID);
    qDebug()<<"Course ID from the copy course";
    QString Date = dueDateTime.left(10);
    QString Time = dueDateTime.right(8);
    ui->DueDate_MC->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->DueTime_MC->setTime(QTime::fromString(Time,"HH:mm:ss"));
    ui->ActivityName_MC->setText(activityName);
//    ui->SolutionPath_MC->setText(pathToSolution);
    ui->StudentSubmissionPath_MC->setText(pathToSubmissions);
    for (int i=1; i<=4;i++)
    {
        if(ui->LanguageComboBox_MC->itemText(i)==language)
        {
            ui->LanguageComboBox_MC->setCurrentIndex(i);
        }

    }
    return rv;
}
/* Save Modified MC Activity */
bool StreamlinedGradingSystem::modifyMCActivityChange()
{
    Instructor myinstructor;
    bool rv = true;

    QString activityName = ui->ActivityName_MC->text();
    QString dueDate = ui->DueDate_MC->date().toString("yyyy/MM/dd");
    QString dueTime = ui->DueTime_MC->dateTime().toString("HH:MM");
    QString dueDateTime = dueDate +" " + dueTime;
//    QString pathToSolution = ui->SolutionPath_MC -> text();
    QString pathToSubmissions = ui->StudentSubmissionPath_MC-> text();
    QString language = ui->LanguageComboBox_MC->currentText();
    int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
    QString currentActivityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    dbManager db;
    int result = db.activityExists(currentCourse.getcourseID().toInt(),activityName);
    //create Activity through instructor.cpp
    if (result != -1 && activityName != currentActivityName)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Activity name already Exisits in this course, please change other name");
        rv = false;
    }
    else if(((activityName == "Enter Activity Name")||
        (activityName.size() == 0)||
        (language == "Select A Language")))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill all the required information");
        rv = false;
    }
    //Checking if the path added for the console exists
    else if ((pathToSubmissions != "//")&&!QDir(pathToSubmissions).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Submission do not exist");
      rv = false;
    }
    else
    {
        myinstructor.modifyActivity(activityID,courseID,activityName,"MC",dueDateTime,language,pathToSubmissions,"0","");
        rv = true;
    }

    return rv;
}
//create Programming Activity
bool StreamlinedGradingSystem::createProgrammingActivity(){
    Instructor myinstructor;
    bool rv = true;
    QString activityName = ui->ActivityName_Programming->text();
    QString dueDate = ui->DueDate_Programming->date().toString("yyyy/MM/dd");
    QString dueTime = ui->DueTime_Programming->dateTime().toString("HH:MM");
    QString dueDateTime = dueDate +" " + dueTime;
    QString pathToSolution = ui->SolutionPath_Programming -> text();
    QString pathToSubmissions = ui->StudentSubmissionPath_Programming-> text();
    QString language = ui->LanguageComboBox_Programming->currentText();
    QString environment = ui->EnviromentComboBox_Programming->currentText();
    //moved this code to here
    int NumberofTestCase = ui->TestCasecomboBox_Programming->count();
    bool consolepathempty = false;
    bool consoleinputexist = true;
    bool consoleoutputexist = true;
    int currentIndex = ui->TestCasecomboBox_Programming->currentIndex();
    QString checktestcaseexist = ui->TestCasecomboBox_Programming->itemText(currentIndex) ;
    if (checktestcaseexist != "Select Test Case Here" && ui->TestCasecomboBox_Programming->count() != 0)
    {
        for(int i =0;i<NumberofTestCase;i++){
            //Checking if the Innput Console paths are valid
            if (consoleInputPath_TestCase[i] != "//" && !QFile(consoleInputPath_TestCase[i]).exists())
            {
                printLogMessage(ERROR_MESSAGE_TEXT,"Console Input does not exist for test case " + QString::number(i+1));
                consoleinputexist = false;
            }
            //Checking if the Output Console paths are valid
            else if (consoleOutputPath_TestCase[i] != "//" && !QFile(consoleOutputPath_TestCase[i]).exists())
            {
              printLogMessage(ERROR_MESSAGE_TEXT,"Console Output does not exist does not exist for test case" + QString::number(i+1));
              consoleoutputexist = false;
            }
            else if(consoleInputPath_TestCase[i]=="" ||consoleInputPath_TestCase[i]=="//"||consoleOutputPath_TestCase[i]==""||consoleInputPath_TestCase[i]=="//")
            {
                consolepathempty = true;
            }
        }
    }
    int newActivityID = 0;
    dbManager db;
    int result = db.activityExists(currentCourse.getcourseID().toInt(),activityName);
    //if activity already exist, the returned activity ID will be different from the current activityID
    if (result != -1)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Activity name already exists in this course, please change other name");
        rv = false;
    }
    else if(((activityName == "Enter Activity Name")||
             (activityName.size() == 0)||
             (pathToSolution == "//") ||
             (pathToSolution == "") ||
             (language == "Select A Language")))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill all the required information");
        rv = false;
    }
    //Checking if the path added for the console exists
    else if (!consoleinputexist)
    {
      //printLogMessage(ERROR_MESSAGE_TEXT,"Console Input does not exist");
      rv = false;
    }
    else if (!consoleoutputexist)
    {
      //printLogMessage(ERROR_MESSAGE_TEXT,"Console Output does not exist");
      qDebug()<<"Path Doesn't Exists";
      rv = false;
    }
    else if(consolepathempty)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please enter the console input and output path for each test case");
        rv = false;
    }
    else
    {
      newActivityID = myinstructor.createActivity(activityName, ACTIVITY_TYPE_PROGRAMMING,dueDateTime,language,pathToSubmissions,0,pathToSolution,currentCourse.getcourseID().toInt());
      qDebug() <<"activity ID" << newActivityID <<" created";
      currentActivity.setActivityID(newActivityID);
      /* Add rubric to database, item by item */
      if (false == currentActivity.isRubricEmpty())
      {
        currentActivity.saveRubricToDb();
        printLogMessage(CONFIRM_MESSAGE_TEXT,"Create Essay Activity Successfully");
        qDebug() << "saved rubric";
      }
      else
      {
        printLogMessage(CONFIRM_MESSAGE_TEXT, "Activity created. Warning: No rubric has been created for this activity");
        qDebug() << "no rubric";
      }
      /* Add test case to database, item by item */
      int currentIndex = ui->TestCasecomboBox_Programming->currentIndex();
      QString checktestcaseexist = ui->TestCasecomboBox_Programming->itemText(currentIndex) ;


      if (checktestcaseexist != "Select Test Case Here" && ui->TestCasecomboBox_Programming->count() != 0)
      {
        int NumberofTestCase = ui->TestCasecomboBox_Programming->count();
        for(int i =0;i<NumberofTestCase;i++){
          int numofInputPath = testCaseList[i].getnumberOfInputPath();
          int numofOutputPath = testCaseList[i].getnumberOfOutputPath();
          //        qDebug() <<"NumberofInputPath" << numofInputPath;
          //        qDebug()<<"consoleInpath"<<consoleInputPath_TestCase[i];
          //        qDebug()<<"consoleOutpath"<<consoleOutputPath_TestCase[i];
          //        store in database through instructor.cpp
          myinstructor.storeTestCasetoDb(newActivityID, i+1,numofInputPath,numofOutputPath,environment,consoleInputPath_TestCase[i],consoleOutputPath_TestCase[i]);
          qDebug()<<"Stored TestCase" << i+1<<"to DB";
          QVector <testCaseItem_t> testcaseitemlist;
          testCaseList[i].getTestCaseItemList(testcaseitemlist);
          if (testcaseitemlist.isEmpty() == false)
          {
            qDebug()<<"Test Case Item has" << testcaseitemlist.size() <<" items";
            //store in database through testcase.cpp
            testCaseList[i].storeFilePathtoDb(newActivityID,i+1);
            testCaseList[i].clearTestCase();
          }
          else
          {
            qDebug()<<"Test Case Item is empty";
          }


        }
        testCaseList.clear();
      }

    }
    return rv;
}

//View Programming Activity
bool StreamlinedGradingSystem::viewProgrammingActivity(){
    ui->CreateRubic_ProgrammingButton->setVisible(false);
    ui->ModifyRubic_ProgrammingButton->setVisible(true);
    ui->Inputpathlist_Programming->clear();
    ui->Outputpathlist_Programming->clear();
    bool rv = true;
    int rowNumber = ui->CourseList_SelectModifyPage->currentRow();
    int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
    QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    QString dueDateTime ="";
    QString pathToSolution  ="";
    QString pathToSubmissions = "";
    QString language = "";
    activityID ="";
    courseID ="";
    currentUser.getownActivityInfo_Data(rowNumber,activityName,dueDateTime,pathToSolution,pathToSubmissions,language,activityID,courseID);
    QString Date = dueDateTime.left(10);
    QString Time = dueDateTime.right(8);
    ui->DueDate_Programming->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->DueTime_Programming->setTime(QTime::fromString(Time,"HH:mm:ss"));
    ui->ActivityName_Programming->setText(activityName);
    ui->SolutionPath_Programming->setText(pathToSolution);
    ui->StudentSubmissionPath_Programming->setText(pathToSubmissions);
    for (int i=1; i<=4;i++)
    {
        if(ui->LanguageComboBox_Programming->itemText(i)==language)
        {
            ui->LanguageComboBox_Programming->setCurrentIndex(i);
        }

    }



    //Loading Test Cases
    //QVector<TestCase> testCaseList ;
    int testCaseCount = 0;
    qDebug()<<"Running through View";

    currentActivity.getTestCasesProgrammingActivityfromdB();
    currentActivity.getTestCaseList(testCaseList);
    qDebug()<<"Finished running get test cases programming activity from db";
    if (currentActivity.getTestCaseList(testCaseList_global))
    {
      /* add items to rubric table */
      testCaseCount = testCaseList.size();
      qDebug() << "Test Case size" << testCaseCount;
      ui->NumberofTest_Programming->setText(QString::number(testCaseCount));
      //Clear Combo Box
      ui->TestCasecomboBox_Programming->clear();
      //Creating Test Case Combo Box to fill
      for (int i=1; i<testCaseCount+1; i++)
      {
        qDebug()<<"Creating Combo Box for Test Cases";
        ui->TestCasecomboBox_Programming->addItem("Test Case " + QString::number(i));
      }

      //Filling up Test Case Combo Box with test cases
      consoleInputPath_TestCase.clear();
      consoleOutputPath_TestCase.clear();
      testCaseList.clear();



      //Filling out UI for the user to view first case
      int NumberofTestCase = ui->TestCasecomboBox_Programming->count();

      //Displaying on the UI the initial values for the Test Cases

      //Appending the Vector
      for(int i =0;i<NumberofTestCase;i++){
        qDebug()<<"Test Case"<< i+1;
        qDebug()<<"Current Index"<<i;

        //Saving the remainder to the approriate global variable
        consoleInputPath_TestCase.append(testCaseList_global[i].getPrimaryInput());
        consoleOutputPath_TestCase.append(testCaseList_global[i].getPrimaryOutput());

      }

      //Initializing a local vector containing all the information needed to be displayed
      QVector <testCaseItem_t> local_item;
      local_item = testCaseList_global[0].getOptionalItems();

      //Saving all the test cases from the current activity to the modify page
      currentActivity.getTestCaseList(testCaseList);

      //For Input Test Case
      for(int a =0; a<testCaseList_global[0].getnumberOfInputPath();a++)
      {
        qDebug()<<"AddingTestCaseItem[Input]";
        ui->Inputpathlist_Programming->addItem(local_item[a].Path);

      }

      //For Output Test Case
      for(int b =0; b<testCaseList_global[0].getnumberOfOutputPath();b++)
      {
        qDebug()<<"AddingTestCaseItem[Output]";
        ui->Outputpathlist_Programming->addItem(local_item[b].Path);
      }

      //Displaying Intial Values for the Input/Output Console
      ui->ConsoleInputTestFilePath_Programming->setText(testCaseList_global[0].getPrimaryInput());
      ui->ConsoleOutputTestFilePath_Programming->setText(testCaseList_global[0].getPrimaryOutput());
    }
  return rv;
}

//view programming activity using rowNumber ActivityNumber (only used by copy all activity)
bool StreamlinedGradingSystem::viewProgrammingActivity(int rowNumber,int ActivityrowNumber){
    ui->CreateRubic_ProgrammingButton->setVisible(false);
    ui->ModifyRubic_ProgrammingButton->setVisible(true);
    ui->Inputpathlist_Programming->clear();
    ui->Outputpathlist_Programming->clear();
    bool rv = true;
    QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    QString dueDateTime ="";
    QString pathToSolution  ="";
    QString pathToSubmissions = "";
    QString language = "";
    activityID ="";
    courseID ="";
    currentUser.getownActivityInfo_Data(rowNumber,activityName,dueDateTime,pathToSolution,pathToSubmissions,language,activityID,courseID);
    QString Date = dueDateTime.left(10);
    QString Time = dueDateTime.right(8);
    ui->DueDate_Programming->setDate(QDate::fromString(Date,"yyyy-MM-dd"));
    ui->DueTime_Programming->setTime(QTime::fromString(Time,"HH:mm:ss"));
    ui->ActivityName_Programming->setText(activityName);
    ui->SolutionPath_Programming->setText(pathToSolution);
    ui->StudentSubmissionPath_Programming->setText(pathToSubmissions);
    for (int i=1; i<=4;i++)
    {
        if(ui->LanguageComboBox_Programming->itemText(i)==language)
        {
            ui->LanguageComboBox_Programming->setCurrentIndex(i);
        }

    }



    //Loading Test Cases
    //QVector<TestCase> testCaseList ;
    int testCaseCount = 0;
    qDebug()<<"Running through View";

    currentActivity.getTestCasesProgrammingActivityfromdB();
    currentActivity.getTestCaseList(testCaseList);
    qDebug()<<"Finished running get test cases programming activity from db";
    if (currentActivity.getTestCaseList(testCaseList_global))
    {
      /* add items to rubric table */
      testCaseCount = testCaseList.size();
      qDebug() << "Test Case size" << testCaseCount;
      ui->NumberofTest_Programming->setText(QString::number(testCaseCount));
      //Clear Combo Box
      ui->TestCasecomboBox_Programming->clear();
      //Creating Test Case Combo Box to fill
      for (int i=1; i<testCaseCount+1; i++)
      {
        qDebug()<<"Creating Combo Box for Test Cases";
        ui->TestCasecomboBox_Programming->addItem("Test Case " + QString::number(i));
      }

      //Filling up Test Case Combo Box with test cases
      consoleInputPath_TestCase.clear();
      consoleOutputPath_TestCase.clear();
      testCaseList.clear();



      //Filling out UI for the user to view first case
      int NumberofTestCase = ui->TestCasecomboBox_Programming->count();

      //Displaying on the UI the initial values for the Test Cases

      //Appending the Vector
      for(int i =0;i<NumberofTestCase;i++){
        qDebug()<<"Test Case"<< i+1;
        qDebug()<<"Current Index"<<i;

        //Saving the remainder to the approriate global variable
        consoleInputPath_TestCase.append(testCaseList_global[i].getPrimaryInput());
        consoleOutputPath_TestCase.append(testCaseList_global[i].getPrimaryOutput());

      }

      //Initializing a local vector containing all the information needed to be displayed
      QVector <testCaseItem_t> local_item;
      local_item = testCaseList_global[0].getOptionalItems();

      //Saving all the test cases from the current activity to the modify page
      currentActivity.getTestCaseList(testCaseList);

      //For Input Test Case
      for(int a =0; a<testCaseList_global[0].getnumberOfInputPath();a++)
      {
        qDebug()<<"AddingTestCaseItem[Input]";
        ui->Inputpathlist_Programming->addItem(local_item[a].Path);

      }

      //For Output Test Case
      for(int b =0; b<testCaseList_global[0].getnumberOfOutputPath();b++)
      {
        qDebug()<<"AddingTestCaseItem[Output]";
        ui->Outputpathlist_Programming->addItem(local_item[b].Path);
      }

      //Displaying Intial Values for the Input/Output Console
      ui->ConsoleInputTestFilePath_Programming->setText(testCaseList_global[0].getPrimaryInput());
      ui->ConsoleOutputTestFilePath_Programming->setText(testCaseList_global[0].getPrimaryOutput());
    }
  return rv;
}

//Save Modifed Programming Activity
bool StreamlinedGradingSystem::modifyProgrammingActivityChange(){
    Instructor myinstructor;
    bool rv = true;
    QString activityName = ui->ActivityName_Programming->text();
    QString dueDate = ui->DueDate_Programming->date().toString("yyyy/MM/dd");
    QString dueTime = ui->DueTime_Programming->dateTime().toString("HH:MM");
    QString dueDateTime = dueDate +" " + dueTime;
    QString pathToSolution = ui->SolutionPath_Programming -> text();
    QString pathToSubmissions = ui->StudentSubmissionPath_Programming-> text();
    QString language = ui->LanguageComboBox_Programming->currentText();
    QString number_of_test = "0";
    QString environment = ui->EnviromentComboBox_Programming->currentText();
    int NumberofTestCase = ui->TestCasecomboBox_Programming->count();
    bool consolepathempty = false;
    bool consoleinputexist = true;
    bool consoleoutputexist = true;
    int currentIndex = ui->TestCasecomboBox_Programming->currentIndex();
    QString checktestcaseexist = ui->TestCasecomboBox_Programming->itemText(currentIndex) ;
    int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
    QString currentActivityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
    //check test case
    if (checktestcaseexist != "Select Test Case Here" && ui->TestCasecomboBox_Programming->count() != 0)
    {
        for(int i =0;i<NumberofTestCase;i++){
            //Checking if the Innput Console paths are valid
            if (consoleInputPath_TestCase[i] != "//" && !QFile(consoleInputPath_TestCase[i]).exists())
            {
                consoleinputexist = false;
            }
            //Checking if the Output Console paths are valid
            else if (consoleOutputPath_TestCase[i] != "//" && !QFile(consoleOutputPath_TestCase[i]).exists())
            {
              consoleoutputexist = false;
            }
            else if(consoleInputPath_TestCase[i]=="" ||consoleInputPath_TestCase[i]=="//"||consoleOutputPath_TestCase[i]==""||consoleInputPath_TestCase[i]=="//")
            {
                consolepathempty = true;
            }
        }
    }
    dbManager db;
    int result = db.activityExists(currentCourse.getcourseID().toInt(),activityName);




    //if activity already exist, the returned activity ID will be different from the current activityID
    if (result != -1 && activityName != currentActivityName)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Activity name already Exisits in this course, please change other name");
        rv = false;
    }
    //create Activity through instructor.cpp
    else if(((activityName == "Enter Activity Name")||
        (activityName.size() == 0)||
        (pathToSolution == "//") ||
        (language == "Select A Language")))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please fill all the required information");
        rv = false;
    }
    else if (!consoleinputexist && !consoleoutputexist)
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"One of the Output and Input Console Path does not exists");
      rv = false;
    }
    else if (!consoleinputexist)
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"One of the Input Console Path does not exists");
      rv = false;
    }
    else if (!consoleoutputexist)
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"One of the Output Console Path does not exists");
      rv = false;
    }
    else if(consolepathempty)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please enter the console input and output path for each test case");
        rv = false;
    }
    //Checking if the path added for the console exists
    else if ((pathToSubmissions != "//")&&!QDir(pathToSubmissions).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Submission do not exist");
      rv = false;
    }
    else if (!QFile(pathToSolution).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"Path to Solution do not exist");
      rv = false;
    }
    else
    {
        qDebug()<<"Updating Modifying Activity";
        myinstructor.modifyActivity(activityID, courseID,activityName,ACTIVITY_TYPE_PROGRAMMING,dueDateTime,language,pathToSubmissions, number_of_test,pathToSolution);
        currentActivity.saveRubricToDb();

        //Reinserting test cases to the database item by item again

    /* Add test case to database, item by item */
    if (checktestcaseexist != "Select Test Case Here" && ui->TestCasecomboBox_Programming->count() != 0)
    {

        //If there are programming test removing all test related to items
        if (ui->NumberofTest_Programming->text() != "")
        {
          number_of_test = ui->NumberofTest_Programming->text();
          qDebug()<<"Creating Number of Test"<<number_of_test;
          //Removing all Test Cases related to the current Activity
          currentActivity.deleteTestCasesFromDb(currentActivity.getActivityID());
        }

            int NumberofTestCase = ui->TestCasecomboBox_Programming->count();
            qDebug()<<"Number of Test Cases Modified to Save"<<NumberofTestCase;
            for(int i =0;i<NumberofTestCase;i++){

                int numofInputPath = testCaseList[i].getnumberOfInputPath();
                int numofOutputPath = testCaseList[i].getnumberOfOutputPath();
                myinstructor.storeTestCasetoDb(currentActivity.getActivityID(), i+1,numofInputPath,numofOutputPath,environment,consoleInputPath_TestCase[i],consoleOutputPath_TestCase[i]);
                qDebug()<<"Stored Test Case" << i+1<<"to DB";
                QVector <testCaseItem_t> testcaseitemlist;
                testCaseList[i].getTestCaseItemList(testcaseitemlist);
                if (testcaseitemlist.isEmpty() == false)
                {
                    qDebug()<<"Test Case Item has" << testcaseitemlist.size() <<" items";
                    //store in database through testcase.cpp
                    testCaseList[i].storeFilePathtoDb(currentActivity.getActivityID(),i+1);
                    testCaseList[i].clearTestCase();
                }
                else
                {
                    qDebug()<<"Test Case Item is empty";
                }
            }
            testCaseList.clear();
        }
     }
    return rv;
}

/* save rubric for the selected activity with information from the rubric page */
bool StreamlinedGradingSystem::saveActivityRubric()
{
    bool rv = false;
    int rowCount = ui->rubricTable->rowCount();
    int rowRubricItemNum;
    QString rowRubricItemDesc;
    int rowRubricItemGrade;
    if (rowCount > 0)
    {
        rv = true;
        /* check inputs.
         *    Rubric item number must be positive integer
         *    Rubric item grade must be a positive integer
         */
        for (int i = 0; i < rowCount; i++)
        {
            //check empty input field
            if ((ui->rubricTable->item(i,RUBRIC_ITEM_NUMBER) == NULL)||
                    (ui->rubricTable->item(i,RUBRIC_ITEM_DESC) == NULL)||
                    (ui->rubricTable->item(i,RUBRIC_ITEM_GRADE) == NULL))
            {
                printLogMessage(ERROR_MESSAGE_TEXT, "One of the input field is empty. Click \"remove\" to remove the row if not used");
                return false;
            }
            //check input type
            QRegExp re("\\d*");
            if ( (!re.exactMatch(ui->rubricTable->item(i,RUBRIC_ITEM_NUMBER)->text())) ||
                 (!re.exactMatch(ui->rubricTable->item(i,RUBRIC_ITEM_GRADE)->text())) )
            {
                printLogMessage(ERROR_MESSAGE_TEXT, "Incorrect input. Rubric Item number and rubric Item grade must be numbers");
                return false;
            }

            rowRubricItemNum = ui->rubricTable->item(i,RUBRIC_ITEM_NUMBER)->text().toInt();
            if (rowRubricItemNum < 0)
            {
                printLogMessage(ERROR_MESSAGE_TEXT, "Incorrect input, one or more of the rubric item number is negative");
                return false;
            }
            rowRubricItemGrade = ui->rubricTable->item(i,RUBRIC_ITEM_GRADE)->text().toInt();
            if (rowRubricItemGrade < 0)
            {
                printLogMessage(ERROR_MESSAGE_TEXT, "Incorrect input, one or more of the rubric item has negative grade");
                return false;
            }
        }

        /* Save rubric for selected activity */
        //clear current rubric
        currentActivity.clearRubric();
        for (int i = 0; i < rowCount; i++)
        {
            rowRubricItemNum = ui->rubricTable->item(i,RUBRIC_ITEM_NUMBER)->text().toInt();
            rowRubricItemDesc = ui->rubricTable->item(i,RUBRIC_ITEM_DESC)->text();
            rowRubricItemGrade = ui->rubricTable->item(i,RUBRIC_ITEM_GRADE)->text().toInt();

            currentActivity.addRubricItem(rowRubricItemNum, rowRubricItemDesc, rowRubricItemGrade);
        }
        currentActivity.setRubricChanged(true);
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Rubric is empty");
        rv = false;
    }
    return rv;
}

//Display list of accounts in account selection page
void StreamlinedGradingSystem::displayAccountList()
{
    dbManager db;
    QSqlQueryModel* accModel=db.getListAccount(); //Returns (employeeID | firstName | lastName | userName | islocked)
    int numOfAcc=db.countAccountList();
    ui->ListOfAccounts->setRowCount(numOfAcc);
    for(int i=0;i<numOfAcc;i++)
    {
        ui->ListOfAccounts->setItem(i,0,new QTableWidgetItem(accModel->record(i).value("employeeID").toString()) );
        //        QTableWidgetItem* itm=ui->ListOfAccounts->item(i,0);
        //        itm->setFlags(Qt::ItemIsEnabled);

        ui->ListOfAccounts->setItem(i,1,new QTableWidgetItem(accModel->record(i).value("userName").toString()) );
        //        itm=ui->ListOfAccounts->item(i,1);
        //        itm->setFlags(Qt::NoItemFlags);

        ui->ListOfAccounts->setItem(i,2,new QTableWidgetItem(accModel->record(i).value("firstName").toString()) );
        //        itm=ui->ListOfAccounts->item(i,2);
        //        itm->setFlags(Qt::NoItemFlags);

        ui->ListOfAccounts->setItem(i,3,new QTableWidgetItem(accModel->record(i).value("lastName").toString()) );
        //        itm=ui->ListOfAccounts->item(i,3);
        //        itm->setFlags(Qt::NoItemFlags);

        if( accModel->record(i).value("islocked").toString() == "1")
            ui->ListOfAccounts->setItem(i,4,new QTableWidgetItem("YES") );
        if( accModel->record(i).value("islocked").toString() == "0")
            ui->ListOfAccounts->setItem(i,4,new QTableWidgetItem("NO") );

        //        itm=ui->ListOfAccounts->item(i,4);
        //        itm->setFlags(Qt::NoItemFlags);
    }

    //Resize width to fit table length. Credit to: http://stackoverflow.com/questions/17535563/how-to-get-a-qtableview-to-fill-100-of-the-width
    for(int i=0 ; i < ui->ListOfAccounts->horizontalHeader()->count() ; i++)
    {
        ui->ListOfAccounts->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    }

    delete accModel;
}

//Display account information in acount modification page
void StreamlinedGradingSystem::displayInfoInAccModPg(QString _selectedEID)
{
    accSelected=Account(_selectedEID,1);

    //account info
    ui->FirstNameCreate->setText(accSelected.getFirstName());
    ui->LastNameCreate->setText(accSelected.getLastName());
    ui->MiddleNameCreate->setText(accSelected.getMidName());
    ui->UserIDCreate->setText(accSelected.getUserID());
    ui->EmployeeIDCreate->setText(accSelected.getEmployeeID());
    ui->PasswordCreate->setText(accSelected.getPassword());

    //roles
    ui->Instructor_checkBox->setChecked(accSelected.isAccountInstructor());
    ui->SA_checkBox->setChecked(accSelected.isAccountSysAdmin());
    ui->TA_checkBox->setChecked(accSelected.isAccountTA());
    ui->AdministrativeAdmin_checkBox->setChecked(accSelected.isAccountAdminAssist());
    ui->Admin_checkBox->setChecked(accSelected.isAccountAdmin());

}

//Update account info on database when user click save changes
bool StreamlinedGradingSystem::updateAccInfoChanges()
{

    // check for empty input
    if (  (ui->FirstNameCreate->text().size() ==0) ||
          (ui->LastNameCreate->text().size() == 0) ||
          (ui->EmployeeIDCreate->text().size() == 0) ||
          (ui->UserIDCreate->text().size() == 0) ||
          (ui->UserIDCreate->text().size() == 0) ||
          (ui->PasswordCreate->text().size() == 0)
          )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Required information is empty");
        return false;
    }

    //check input length
    if (ui->UserIDCreate->text().size() > 8)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "User ID must be within 8 letters");
        return false;
    }

    // check if at least 1 role is selected
    if (!((ui->Instructor_checkBox->isChecked() == true) ||
          (ui->TA_checkBox->isChecked() == true) ||
          (ui->SA_checkBox->isChecked() == true) ||
          (ui->AdministrativeAdmin_checkBox->isChecked() == true) ||
          (ui->Admin_checkBox->isChecked() == true)
          ))
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "User must have at least one role");
        return false;
    }


    SysAdmin sysAdmin;

    //check for existing username
    if ( ( true == sysAdmin.userNameExists(ui->UserIDCreate->text() ) ) && ( ui->UserIDCreate->text() != accSelected.getUserID() ) )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Username already exists");
        return false;
    }

    // check if employeeID already exist
    if ( ( true == sysAdmin.employeeIDExists(ui->EmployeeIDCreate->text() ) ) && ( ui->EmployeeIDCreate->text() != accSelected.getEmployeeID() ) )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "EmployeeID already exists");
        return false;
    }

    //actually do update account
    if( false == sysAdmin.modifyAccount(accSelected.getAccountID(),
                                        ui->FirstNameCreate->text(),
                                        ui->MiddleNameCreate->text(),
                                        ui->LastNameCreate->text(),
                                        ui->UserIDCreate->text(),
                                        ui->EmployeeIDCreate->text(),
                                        ui->PasswordCreate->text(),
                                        ui->TA_checkBox->isChecked(),
                                        ui->Instructor_checkBox->isChecked(),
                                        ui->Admin_checkBox->isChecked(),
                                        ui->AdministrativeAdmin_checkBox->isChecked(),
                                        ui->SA_checkBox->isChecked()))
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Failed to modify account. Database error");
        return false;
    }

    //update object
    accSelected.setFirstName( ui->FirstNameCreate->text() );
    accSelected.setMidName( ui->MiddleNameCreate->text() );
    accSelected.setLastName( ui->LastNameCreate->text() );
    accSelected.setUserID( ui->UserIDCreate->text() );
    accSelected.setEmployeeID( ui->EmployeeIDCreate->text() );
    accSelected.setPassword( ui->PasswordCreate->text() );
    //    accSelected.setIsTA( ui->TA_checkBox->isChecked() );
    //    accSelected.setIsInstructor( ui->Instructor_checkBox->isChecked() );
    //    accSelected.setIsAdmin( ui->Admin_checkBox->isChecked() );
    //    accSelected.setIsAdminAssist( ui->AdministrativeAdmin_checkBox->isChecked() );
    //    accSelected.setIsSysAdmin( ui->SA_checkBox->isChecked() );

    //return true if no error
    printLogMessage(CONFIRM_MESSAGE_TEXT, "Successfully Modify Account");
    return true;

}

//BLOCK
bool StreamlinedGradingSystem::blockAcc(QString _selectedUsername)
{
    SysAdmin sysAdmin;
    int currRow=ui->ListOfAccounts->currentRow();

    //check if already blocked
    if(ui->ListOfAccounts->item(currRow,4)->text()=="YES")
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "User " + _selectedUsername + " is already blocked!");
        return false;
    }

    //actually block and check for db error
    if( false==sysAdmin.blockAccount(_selectedUsername) )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Failed to block account. Database error");
        return false;
    }

    //no error
    ui->ListOfAccounts->setItem(currRow,4,new QTableWidgetItem("YES"));
    printLogMessage(CONFIRM_MESSAGE_TEXT, "User " + _selectedUsername + " is now blocked!");
    return true;
}

//UNBLOCK
bool StreamlinedGradingSystem::unblockAcc(QString _selectedUsername)
{
    SysAdmin systAdmin;
    int currRow=ui->ListOfAccounts->currentRow();

    //check if already unblocked
    if(ui->ListOfAccounts->item(currRow,4)->text()=="NO")
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "User " + _selectedUsername + " is already unblocked!");
        return false;
    }

    //actually unblock and check for db error
    if( false==systAdmin.unblockAcount(_selectedUsername) )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Failed to unblock account. Database error");
        return false;
    }

    //no error
    ui->ListOfAccounts->setItem(currRow,4,new QTableWidgetItem("NO"));
    printLogMessage(CONFIRM_MESSAGE_TEXT, "User " + _selectedUsername + " is now unblocked!");
    return true;
}


/* Display student list when user select an activity on main marking page
 *  Student list is based on submission dir
 */
bool StreamlinedGradingSystem::viewStudentList_marking()
{
    QVector<QString> stdDirList;
    QString path;
    bool haveGradingRecord;
    int count;
    int selectedRow = ui->ActivityList_Marker->selectedRanges().first().topRow();
    currentActivity = Activity(tempActivityIDList[selectedRow].toInt());

    qDebug() << "get path";
    path = currentActivity.getPathToStdSubmission();
    qDebug() << "set path";
    QDir stdDir(path);
    if (!stdDir.exists())
    {
        ui->studentList_Marking->clear();
        ui->studentList_Marking->setRowCount(0);
        //set table header
        QStringList tableHeader;
        tableHeader << "Student #" << "Points" << "Submmited on" << "Marked by" << "Marker Role";
        ui->studentList_Marking->setHorizontalHeaderLabels(tableHeader);
        ui->studentList_Marking->verticalHeader()->hide();
        printLogMessage(ERROR_MESSAGE_TEXT,"Path to student submission does not exist");
        return false;
    }
    QFileInfoList files = stdDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug() << "check file";
    //get student numbers in submission directory
    foreach(QFileInfo file, files)
    {
      qDebug() << file.filePath();
        if (file.isDir())
        {
          qDebug() << file.fileName();
            stdDirList.append(file.fileName());
        }
    }

    //clear student list
    qDebug() << "clear list";
    while (ui->studentList_Marking->rowCount() > 0)
    {
      qDebug() << "remove row";
        ui->studentList_Marking->removeRow(0);
    }
    //Get grading record for this activity from DB
    dbManager db;
    sqlModel = db.getGradingRecord(currentActivity.getActivityID());
    if (NULL == sqlModel)
    {
        qDebug() << "failed to get activity grading record from database";
        printLogMessage(ERROR_MESSAGE_TEXT,"Failed to get activity grading record from database");
        haveGradingRecord = false;
    }
    //Update the student list
    int rowCount = stdDirList.size();
    if (rowCount == 0)
    {
        printLogMessage(CONFIRM_MESSAGE_TEXT, "There is no submission for this activity");
        return true;
    }
    for (int i = 0; i < rowCount; i++)
    {
        // Add student number to the list
        ui->studentList_Marking->insertRow(i);
        ui->studentList_Marking->setItem(i,0, new QTableWidgetItem(stdDirList[i]));
        // Add grading record(if exist) for marker to the list
        if (true == haveGradingRecord)
        {
            count = sqlModel->rowCount();
            for (int k = 0; k < count; k++)
            {
                //Find the student number in the record
                if (stdDirList[i] == sqlModel->record(k).value("studentID").toString())
                {
                    ui->studentList_Marking->setItem(i, STUDENT_LIST_MARKING_COL_MARKER, new QTableWidgetItem(sqlModel->record(k).value("userName").toString()));
                    ui->studentList_Marking->setItem(i, STUDENT_LIST_MARKING_COL_MARKEDAS, new QTableWidgetItem(sqlModel->record(k).value("lastGradedRole").toString()));
                }
                else
                {
                    qDebug() << "student " << stdDirList[i] << " has not been graded";
                }
            }
        }
        /* TODO: add other information (point graded, submit date, marked by) on the list */
    }

    if (sqlModel!=NULL)
    {
        delete sqlModel;
        sqlModel = NULL;
    }
    return true;
}

//display list of course in course selection page
void StreamlinedGradingSystem::displayCourseList()
{
    dbManager db;
    QSqlQueryModel* courseModel = db.getListOfAllCourses(); // Returns (course ID | courseName | courseNumber | startDate | endDate)
    ui->ListOfCourses->setRowCount(db.countCourseList());
    int countCourses = db.countCourseList();
    for(int i=0; i < countCourses; i++)
    {
        ui->ListOfCourses->setItem( i,0,new QTableWidgetItem( courseModel->record(i).value("courseNumber").toString() ) );

        ui->ListOfCourses->setItem( i,1,new QTableWidgetItem( courseModel->record(i).value("courseName").toString() ) );

        ui->ListOfCourses->setItem( i,2,new QTableWidgetItem( courseModel->record(i).value("startDate").toString() ) );

        ui->ListOfCourses->setItem( i,3,new QTableWidgetItem( courseModel->record(i).value("endDate").toString() ) );
    }

    //Resize width to fit table length. Credit to: http://stackoverflow.com/questions/17535563/how-to-get-a-qtableview-to-fill-100-of-the-width
    for(int i=0 ; i < ui->ListOfCourses->horizontalHeader()->count() ; i++)
    {
        ui->ListOfCourses->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    }

    delete courseModel;
}

//display course info
void StreamlinedGradingSystem::displayCourseInfo(int currRow)
{
    AdminAssist AA;
    QString cID=AA.getCourseID(ui->ListOfCourses->item(currRow,0)->text(),
                               ui->ListOfCourses->item(currRow,1)->text(),
                               ui->ListOfCourses->item(currRow,2)->text(),
                               ui->ListOfCourses->item(currRow,3)->text());
    currentCourse=Course(cID);
    ui->CourseNameCreate_CreateCourse->setText(currentCourse.getcourseName());
    ui->CourseNumberCreate_CreateCourse->setText(currentCourse.getcourseNumber());
    ui->StartDateCreate_CreateCourse->setDate(currentCourse.getstartDate());
    ui->EndDateCreate_CreateCourse->setDate(currentCourse.getendDate());
    ui->InstructorEmployeeID_CreateCourse->setText(currentCourse.getinstructorEmployeeID());
    ui->InstructorName_CreateCourse->setText(currentCourse.getinstructorName());

    //clear tables if not empty
    if(ui->TAList_CreateCourse->rowCount()!=0)
    {
        ui->TAList_CreateCourse->clearContents();
        ui->TAList_CreateCourse->setRowCount(0);
    }
    if(ui->StudentList_CreateCourse->rowCount()!=0)
    {
        ui->StudentList_CreateCourse->clearContents();
        ui->StudentList_CreateCourse->setRowCount(0);
    }

    //get TA list to UI
    QSqlQueryModel* TAmodel=currentCourse.getListTA(cID);
    int rowNumTA = TAmodel->rowCount();
    for(int i=0 ; i < rowNumTA ; i++)
    {
        ui->TAList_CreateCourse->insertRow(i);
        ui->TAList_CreateCourse->setItem(i,0,new QTableWidgetItem( TAmodel->record(i).value("employeeID").toString() ) );
        ui->TAList_CreateCourse->setItem(i,1,new QTableWidgetItem(TAmodel->record(i).value("firstName").toString() + " " + TAmodel->record(i).value("lastName").toString()));
    }

    //get Std list to UI
    QSqlQueryModel* stdModel=currentCourse.getListStd(cID);
    int rowNumStd = stdModel->rowCount();
    //    ui->StudentList_CreateCourse->setRowCount(rowNumStd);
    for(int i=0 ; i < rowNumStd ; i++)
    {
        ui->StudentList_CreateCourse->insertRow(i);
        ui->StudentList_CreateCourse->setItem(i,0,new QTableWidgetItem( stdModel->record(i).value("studentID").toString() ) );

        //seperate std name
        QString stdName = stdModel->record(i).value("studentName").toString();

        //        qDebug() << "StdName" << stdName;

        QStringList List;
        List.clear();
        List = stdName.split(", ");
        QString firstName = List.at(0);
        QString midName = List.at(1);
        QString lastName = List.at(2);

        //        qDebug() << "firstName" << firstName;
        //        qDebug() << "midName" << midName;
        //        qDebug() << "lastName" << lastName;

        ui->StudentList_CreateCourse->setItem(i,1,new QTableWidgetItem(firstName) );
        ui->StudentList_CreateCourse->setItem(i,2,new QTableWidgetItem(midName) );
        ui->StudentList_CreateCourse->setItem(i,3,new QTableWidgetItem(lastName) );
    }
}

//update any changes to course info and student/TA list
bool StreamlinedGradingSystem::saveChangesCourseInfo()
{
    QString selCourseID = currentCourse.getcourseID();
    AdminAssist AA;
    QString courseName = ui->CourseNameCreate_CreateCourse->text();
    QString courseNum = ui->CourseNumberCreate_CreateCourse->text();
    QString startDate = ui->StartDateCreate_CreateCourse->date().toString("yyyy/MM/dd");
    QString endDate = ui->EndDateCreate_CreateCourse->date().toString("yyyy/MM/dd");
    QString instID = ui->InstructorEmployeeID_CreateCourse->text();

    if(  courseName.size() == 0 ||
         courseNum.size() == 0  ||
         startDate.size() == 0 ||
         endDate.size() == 0 )
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Required information is empty");
        return false;
    }

    if(startDate>endDate)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Start Date should not be after End Date");
        return false;
    }

    if( instID.size() != 12 && instID.size() != 0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Intructor Employee ID must be 12 digits");
        return false;
    }

    if( instID.size() == 0 )
        ui->InstructorName_CreateCourse->setText("");

    if( AA.modifyCourse(selCourseID, courseName, courseNum, startDate, endDate, instID)==false )
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Fail to update course info. Database error.");
        return false;
    }

    if(AA.clearListTA(selCourseID)==false) //clear all TAs
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Fail to clear all TAs associated to the course. Database error.");
        return false;
    }

    if(AA.clearListStd(selCourseID)==false) //clear all students
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Fail to clear all students associated to the course. Database error.");
        return false;
    }

    //insert TAs
    for (int i=0; i < ui->TAList_CreateCourse->rowCount(); i++)
    {
        QString employeeID = ui->TAList_CreateCourse->item(i,0)->text();
        bool successAssignTA=AA.assignOneTATMToCourse(employeeID,selCourseID.toInt());
        if(successAssignTA==false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT,"Fail to assign one TA. Database error.");
            return false;
        }
    }

    //insert stds
    for (int i=0; i < ui->StudentList_CreateCourse->rowCount(); i++)
    {

        //retrieve info from GUI
        QString stdID = ui->StudentList_CreateCourse->item(i,0)->text();
        QString firstName = ui->StudentList_CreateCourse->item(i,1)->text();
        QString midName = ui->StudentList_CreateCourse->item(i,2)->text();
        QString lastName = ui->StudentList_CreateCourse->item(i,3)->text();

        //concatenate stdName
        QString stdName = firstName + ", " + midName + ", " + lastName;

        //insert a student
        bool successAssignStd=AA.assignOneStdToCourse(selCourseID.toInt(),stdName,stdID);
        if(successAssignStd==false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT,"Fail to assign one student. Database error.");
            return false;
        }

    }

    printLogMessage(CONFIRM_MESSAGE_TEXT,"Successfully Modify Course");
    return true;
}


/* Display student marking page when grading essay/problem set */
bool StreamlinedGradingSystem::viewSelectedSubmission_Essay_ProblemSet()
{
    dbManager db;
    /* display rubric list */
    //clear current table
    qDebug() << "clear grade table";
    ui->StudentGradeTable->clear();
    while (ui->StudentGradeTable->rowCount() != 0)
    {
        qDebug() << "remove student grade table row";
        ui->StudentGradeTable->removeRow(0);
    }
    //add rubric item to table
    QVector <rubricItem_t> rubricList;
    qDebug() <<"get activity rubric";
    if(!currentActivity.getRuricItemList(rubricList))
    {
        printLogMessage(CONFIRM_MESSAGE_TEXT, "This activity does not have a rubric");
        return false;
    }
    int selectedRow = ui->studentList_Marking->selectedRanges().first().topRow();
    QString studentNum = ui->studentList_Marking->item(selectedRow,0)->text();
    qDebug() << "student " << studentNum;
    sqlModel = db.getStudentActivityGrade(currentActivity.getActivityID(), studentNum);
    bool graded = true;
    if ((sqlModel == NULL)||(sqlModel->rowCount() == 0))
    {
        qDebug() << "No grade";
        graded = false;
    }
    for (int i = 0; i < rubricList.size(); i++)
    {
        ui->StudentGradeTable->insertRow(i);
        qDebug() << rubricList[i].itemNum;
        ui->StudentGradeTable->setItem(i,GRADE_TABLE_COL_RUBRIC_NUM, new QTableWidgetItem(QString::number(rubricList[i].itemNum)));
        ui->StudentGradeTable->item(i,GRADE_TABLE_COL_RUBRIC_NUM)->setFlags(Qt::NoItemFlags);
        ui->StudentGradeTable->setItem(i,GRADE_TABLE_COL_RUBRIC_DESC, new QTableWidgetItem(rubricList[i].itemDescription));
        ui->StudentGradeTable->item(i,GRADE_TABLE_COL_RUBRIC_DESC)->setFlags(Qt::NoItemFlags);
        ui->StudentGradeTable->setItem(i,GRADE_TABLE_COL_RUBRIC_MAX_GRADE, new QTableWidgetItem(QString::number(rubricList[i].itemGrade)));
        ui->StudentGradeTable->item(i,GRADE_TABLE_COL_RUBRIC_MAX_GRADE)->setFlags(Qt::NoItemFlags);
        // load student grade from db if exist;
        if (graded == false)
        {
            ui->StudentGradeTable->setItem(i,GRADE_TABLE_COL_STUDENT_GRADE, new QTableWidgetItem("N/A "));
        }
        else
        {
            // gotta match the rubric item with grade from model. Assume the model return the grade in the same order?
            qDebug() << sqlModel->record(i).value("earnedGrade").toString();
            ui->StudentGradeTable->setItem(i,GRADE_TABLE_COL_STUDENT_GRADE, new QTableWidgetItem(sqlModel->record(i).value("earnedGrade").toString()));
        }
    }

    return true;
}

/* Display student marking page when grading programming activity
 *  Author: ftran
 */
bool StreamlinedGradingSystem::viewSelectedSubmission_Programming()
{
    dbManager db;
    /* display rubric list */
    //clear current table
    qDebug() << "clear grade table";
    ui->StudentGradeTable_Programming->clear();
    while (ui->StudentGradeTable_Programming->rowCount() != 0)
    {
        qDebug() << "remove student grade table row";
        ui->StudentGradeTable_Programming->removeRow(0);
    }
    //add rubric item to table
    QVector <rubricItem_t> rubricList;
    qDebug() <<"get activity rubric";
    if(!currentActivity.getRuricItemList(rubricList))
    {
        printLogMessage(CONFIRM_MESSAGE_TEXT, "This activity does not have a rubric");
        return false;
    }
    int selectedRow = ui->studentList_Marking->selectedRanges().first().topRow();
    QString studentNum = ui->studentList_Marking->item(selectedRow,STUDENT_LIST_MARKING_COL_STD_NUM)->text();
    qDebug() << "student " << studentNum;
    sqlModel = db.getStudentActivityGrade(currentActivity.getActivityID(), studentNum);
    bool graded = true;
    if ((sqlModel == NULL)||(sqlModel->rowCount() == 0))
    {
        qDebug() << "No grade";
        graded = false;
    }
    for (int i = 0; i < rubricList.size(); i++)
    {
        ui->StudentGradeTable_Programming->insertRow(i);
        qDebug() << rubricList[i].itemNum;
        ui->StudentGradeTable_Programming->setItem(i,GRADE_TABLE_COL_RUBRIC_NUM, new QTableWidgetItem(QString::number(rubricList[i].itemNum)));
        ui->StudentGradeTable_Programming->item(i,GRADE_TABLE_COL_RUBRIC_NUM)->setFlags(Qt::NoItemFlags);
        ui->StudentGradeTable_Programming->setItem(i,GRADE_TABLE_COL_RUBRIC_DESC, new QTableWidgetItem(rubricList[i].itemDescription));
        ui->StudentGradeTable_Programming->item(i,GRADE_TABLE_COL_RUBRIC_DESC)->setFlags(Qt::NoItemFlags);
        ui->StudentGradeTable_Programming->setItem(i,GRADE_TABLE_COL_RUBRIC_MAX_GRADE, new QTableWidgetItem(QString::number(rubricList[i].itemGrade)));
        ui->StudentGradeTable_Programming->item(i,GRADE_TABLE_COL_RUBRIC_MAX_GRADE)->setFlags(Qt::NoItemFlags);
        // load student grade from db if exist;
        if (graded == false)
        {
            ui->StudentGradeTable_Programming->setItem(i,GRADE_TABLE_COL_STUDENT_GRADE, new QTableWidgetItem("N/A "));
        }
        else
        {
            // gotta match the rubric item with grade from model. Assume the model return the grade in the same order?
            qDebug() << sqlModel->record(i).value("earnedGrade").toString();
            ui->StudentGradeTable_Programming->setItem(i,GRADE_TABLE_COL_STUDENT_GRADE, new QTableWidgetItem(sqlModel->record(i).value("earnedGrade").toString()));
        }
    }
    /* Load file list for student submission and solution */
    QDir studentDir;
    QDir solutionDir;
    QString path;
    QFileInfoList filelist;

    //clear file list
    ui->StudentFilePrograming_comboBox->clear();
    ui->SolutionFileProgramming_ComboBox->clear();
    ui->StudentFileBrowser_Programming->clear();
    ui->SolutionFileBrowser_Programming->clear();
    ui->CommentProgramming->clear();
    //add student files
    //load file list for student
    path = currentActivity.getPathToStdSubmission();
    path = path + "/" + studentNum;
    qDebug() << "dir path: " << path;
    studentDir = QDir(path);

    filelist = studentDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    if (filelist.isEmpty())
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Student directory is empty");
        return false;
    }
    foreach (QFileInfo file, filelist)
    {
        ui->StudentFilePrograming_comboBox->addItem(file.fileName());
    }
    //add solution files
    filelist.clear();
    path = currentActivity.getPathToSolution();
    solutionDir = QDir(path);
    if (false == solutionDir.exists())
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Solution Directory path does not exist");
        return false;
    }
    filelist = solutionDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    if (filelist.isEmpty())
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Solution directory is empty");
        return false;
    }
    foreach (QFileInfo file, filelist)
    {
        ui->SolutionFileProgramming_ComboBox->addItem(file.fileName());
    }
    //load comment file if exist
    path = currentActivity.getPathToStdSubmission() + "/" + studentNum + "/" + studentNum + COMMENT_FILE_NAME_TAIL;
    if (!QFile(path).exists())
    {
        qDebug() << "There is no comment file for student "<< studentNum;
    }
    else
    {

        QFile selectedFile(path);
        if (selectedFile.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            QTextStream textstream(&selectedFile);
            ui->CommentProgramming->setPlainText(textstream.readAll());
        }
        else
        {
            qDebug() << "Could not open comment file " << path;
        }
    }
    //set tables header
    QStringList tableHeader;
    tableHeader << "Item Number" << "Item Description" << "Max Grade" << "Student Grade";
    ui->StudentGradeTable_Programming->setHorizontalHeaderLabels(tableHeader);
    ui->StudentGradeTable_Programming->verticalHeader()->hide();

    return true;
}

/* Display the student work for the selected student */
bool StreamlinedGradingSystem::viewStudentFile()
{
    QDir stdDir;
    QString path;
    QString stdNum;
    //get the student file from selected student
    int selectedRow = ui->studentList_Marking->selectedRanges().first().topRow();
    stdNum = ui->studentList_Marking->item(selectedRow,0)->text();
    qDebug() << "display grade page for student " << stdNum;
    //get student file path
    path = currentActivity.getPathToStdSubmission() + "/" + stdNum;
    qDebug() << "student file dir: " << path;

    //find the file in dir
    stdDir = QDir(path);
    QFileInfoList files = stdDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (QFileInfo file, files)
    {
        path = path + "/" + file.fileName();
    }

    qDebug() << path;
    QDesktopServices::openUrl(QUrl("file:" + path + ""));

    return true;
}

/* Save student grade on essay marking page to database */
bool StreamlinedGradingSystem::saveStudentGrade_Essay()
{
    dbManager db;

    QVector<rubricItem_t> rubricItemList;
    currentActivity.getRuricItemList(rubricItemList);
    /* this rubric list should correspond with the list shown on the grade table */
    //get student ID
    int selected_std_row = ui->studentList_Marking->selectedRanges().first().topRow();
    int selectedActivityRow = ui->ActivityList_Marker->selectedRanges().first().topRow();
    QString std_grade;
    QString studentID = ui->studentList_Marking->item(selected_std_row,0)->text();
    QString activityID = tempActivityIDList[selectedActivityRow];

    /* Check arguments */
    for (int i=0; i < ui->StudentGradeTable->rowCount(); i++)
    {
        //insert grade per rubric item to db
        std_grade = ui->StudentGradeTable->item(i,3)->text();
        db.assignStudentGrade(studentID, rubricItemList[i].itemID, std_grade);
    }
    //set name of marker
    if (db.insertGradingRecord(activityID.toInt(), studentID, currentUserRole, currentUser.getUserID()))
    {
        //update student list table
        if (NULL == ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKEDAS))
        {
            ui->studentList_Marking->setItem(selected_std_row, STUDENT_LIST_MARKING_COL_MARKEDAS, new QTableWidgetItem(currentUserRole));
        }
        else
        {
            ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKEDAS)->setText(currentUserRole);
        }
        if (NULL == ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKER))
        {
            ui->studentList_Marking->setItem(selected_std_row, STUDENT_LIST_MARKING_COL_MARKER, new QTableWidgetItem(currentUser.getUserID()));
        }
        else
        {
            ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKER)->setText(currentUser.getUserID());
        }
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Failed to store grading record to database");
    }

    printLogMessage(CONFIRM_MESSAGE_TEXT, "Saved grade for student "+studentID);

    return true;
}

/* Save student grade on mark programming page */
bool StreamlinedGradingSystem::saveStudentGrade_Programming()
{
    dbManager db;

    QVector<rubricItem_t> rubricItemList;
    currentActivity.getRuricItemList(rubricItemList);
    /* this rubric list should correspond with the list shown on the grade table */
    //get student ID
    int selected_std_row = ui->studentList_Marking->selectedRanges().first().topRow();
    QString std_grade;
    QString studentID = ui->studentList_Marking->item(selected_std_row,0)->text();
    int activityID = currentActivity.getActivityID();

    /* Check arguments */
    for (int i=0; i < ui->StudentGradeTable_Programming->rowCount(); i++)
    {
        //insert grade per rubric item to db
        std_grade = ui->StudentGradeTable_Programming->item(i,GRADE_TABLE_COL_STUDENT_GRADE)->text();
        db.assignStudentGrade(studentID, rubricItemList[i].itemID, std_grade);
    }
    //set name of marker
    if (db.insertGradingRecord(activityID, studentID, currentUserRole, currentUser.getUserID()))
    {
        //update student list table
        if (NULL == ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKEDAS))
        {
            ui->studentList_Marking->setItem(selected_std_row, STUDENT_LIST_MARKING_COL_MARKEDAS, new QTableWidgetItem(currentUserRole));
        }
        else
        {
            ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKEDAS)->setText(currentUserRole);
        }
        if (NULL == ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKER))
        {
            ui->studentList_Marking->setItem(selected_std_row, STUDENT_LIST_MARKING_COL_MARKER, new QTableWidgetItem(currentUser.getUserID()));
        }
        else
        {
            ui->studentList_Marking->item(selected_std_row, STUDENT_LIST_MARKING_COL_MARKER)->setText(currentUser.getUserID());
        }
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Failed to store grading record to database");
    }

    printLogMessage(CONFIRM_MESSAGE_TEXT, "Saved grade for student "+studentID);

    return true;
}

//delete selected account (sys admin)
bool StreamlinedGradingSystem::delAcc(int currRow)
{
    SysAdmin SA;
    if(SA.deleteAccount(ui->ListOfAccounts->item(currRow,0)->text()) == false)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Fail to delete account, database error!");
        return false;
    }

    ui->ListOfAccounts->removeRow(currRow);

    printLogMessage(CONFIRM_MESSAGE_TEXT, "Account successfully deleted!");
    return true;
}

//delete selected course (AA)
bool StreamlinedGradingSystem::delCourse(int currRow)
{
    AdminAssist AA;
    QString cID = AA.getCourseID(ui->ListOfCourses->item(currRow,0)->text(),
                                 ui->ListOfCourses->item(currRow,1)->text(),
                                 ui->ListOfCourses->item(currRow,2)->text(),
                                 ui->ListOfCourses->item(currRow,3)->text());
    QString cName = ui->ListOfCourses->item(currRow,0)->text();

    if( true == AA.courseContainAct( cID.toInt() ) )
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Course contains activity! Cannnot delete!");
        return false;
    }

    //display message box
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deleting a course!", "Are you sure you want to delete " + cName + " ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == reply)
    {
        if( false == AA.deleteCourse(cID.toInt()) )
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "Fail to delete course, database error!");
            return false;
        }
        ui->ListOfCourses->removeRow(currRow);
        printLogMessage(CONFIRM_MESSAGE_TEXT, cName + " is successfully deleted!");
        return true;
    }

    printLogMessage(ERROR_MESSAGE_TEXT, "Deletion canceled!");
    return true;
}

//delete activity
bool StreamlinedGradingSystem::delAct(QString _selActID)
{
    Instructor inst;
    Activity selAct(_selActID.toInt());
    int selActID = selAct.getActivityID();
    QString selActType = selAct.getActivityType();

    if( false == inst.deleteActivity(selActID, selActType) ) //check whether deletion success or fail
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Fail to delete activity. Database error!");
        return false;
    }

    //successful deletion
    viewOwnCourseActivity();
    printLogMessage(CONFIRM_MESSAGE_TEXT, "Successfully Delete Activity");
    return true;
}


//store the current test case locally
void StreamlinedGradingSystem::storecurrentTestCase(){

    int currentIndex = ui->TestCasecomboBox_Programming->currentIndex();
        qDebug()<<"current ComboBox Index" << currentIndex;

    QString checktestcaseexist = ui->TestCasecomboBox_Programming->itemText(currentIndex) ;
    if (checktestcaseexist != "Select Test Case Here" && ui->TestCasecomboBox_Programming->count()!=0)
    {
        consoleInputPath_TestCase[currentIndex]= ui->ConsoleInputTestFilePath_Programming->text();
        consoleOutputPath_TestCase[currentIndex]=ui->ConsoleOutputTestFilePath_Programming->text();
        qDebug()<<"console input path:"<<consoleInputPath_TestCase[currentIndex];
        qDebug()<<"console output path:"<<consoleOutputPath_TestCase[currentIndex];
        int numofGuiInputPath = ui->Inputpathlist_Programming->count();
        int numofGuiOutputPath = ui->Outputpathlist_Programming->count();
        testCaseList[currentIndex].clearTestCase();
        //add test case item to the item list
        for(int i =0; i<numofGuiInputPath;i++)
        {
            qDebug()<<"AddingTestCaseItem[Input]";
            QString Path = ui->Inputpathlist_Programming->item(i)->text();
            testCaseList[currentIndex].addTestCaseItem(currentIndex+1, Path, FILE_INPUT_TYPE);
        }
        for(int i =0; i<numofGuiOutputPath;i++)
        {
            qDebug()<<"AddingTestCaseItem[Output]";
            QString Path = ui->Outputpathlist_Programming->item(i)->text();
            testCaseList[currentIndex].addTestCaseItem(currentIndex+1, Path,  FILE_OUTPUT_TYPE);
        }
    }
}

//copy all activity
void StreamlinedGradingSystem::copyAllActivity(){
    //int courseID = currentCourse.getcourseID().toInt();
    int numberofActivity = ui->ActivityList_SelectModifyPage->rowCount();
        bool rv = true;
    for (int i =0;i<numberofActivity;i++){


            int rowNumber = ui->CourseList_SelectModifyPage->currentRow();
                        if (ui->ActivityList_SelectModifyPage->item(i,1)->text() == "Essay/Problem Set"){
//                            qDebug()<<"I am here Essay";
//                            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
//                            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
//                            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);

                            viewEssayActivity(rowNumber,i);
//                            qDebug()<<"view Essay done";
                            createEssayActivity();
//                            qDebug()<<"create Essay done";
                        }
                        else if (ui->ActivityList_SelectModifyPage->item(i,1)->text() == "MC"){
//                            qDebug()<<"I am here MC";
                            viewMCActivity(rowNumber,i);
//                            qDebug()<<"view MC done";
                            createMCActivity();
//                            qDebug()<<"create MC done";
                        }
                        else if (ui->ActivityList_SelectModifyPage->item(i,1)->text() == "Programming"){
//                            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
//                            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
//                            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);
                            //qDebug() << "currentCourse" << currentCourse;
//                            qDebug()<<"I am here Programming";
                            viewProgrammingActivity(rowNumber,i);
//                            qDebug()<<"view Programming done";
                            createProgrammingActivity();
//                            qDebug()<<"create Programming done";
                        }

                }

}

