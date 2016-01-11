/************************************
 *
 * File Name: streamlinedgradingsystem.cpp
 * Purpose  : This file mainly handle GUI display
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
 ***********************************/

//#include <unistd.h>
#include "streamlinedgradingsystem.h"
#include "ui_streamlinedgradingsystem.h"
#include "Control/DatabaseAPI/dbmanager.h"
#include "Model/CLASSES/testcase.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QtGui/QDesktopServices>

StreamlinedGradingSystem::StreamlinedGradingSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StreamlinedGradingSystem)
{
    ui->setupUi(this);
    if ( false == initializeWindow())
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Failed to initialize the system");
    }
}

StreamlinedGradingSystem::~StreamlinedGradingSystem()
{
    delete ui;
}

/* Initialize the window display at start up */
bool StreamlinedGradingSystem::initializeWindow()
{
    displayPage(LOGIN_PAGE_INDEX, false);
    pageStack.clear();
    return true;
}

/* helper function use to display log message on the UI
 * TEXT_TYPE can be:
 *    CONFIRM_MESSAGE_TEXT
 *    ERROR_MESSAGE_TEXT
 */
void StreamlinedGradingSystem::printLogMessage(int TEXT_TYPE, QString text)
{
    switch (TEXT_TYPE)
    {
    case ERROR_MESSAGE_TEXT:
        ui->LogMessage->setText("<font color=red>" + text + "</font>");
        break;
    case CONFIRM_MESSAGE_TEXT:
        ui->LogMessage->setText("<font color=blue>" + text + "</font>");
        break;
    default:
        break;
    }
}

/* Clear the text on log message */
void StreamlinedGradingSystem::clearLogMessage()
{
    ui->LogMessage->setText("");
}

/* helper function to set the role list for the current user
 *   Current user must have been validated at login
 */
bool StreamlinedGradingSystem::setRoleSelection()
{
    bool rv = false;
    // clear selection list (if exist)
    ui->SelectRoleBox->clear();
    // add option depending on user priviledges
    if (true == currentUser.isAccountSysAdmin())
    {
        ui->SelectRoleBox->addItem(SYS_ADMIN_ROLE_TEXT);
    }
    if (true == currentUser.isAccountAdminAssist())
    {
        ui->SelectRoleBox->addItem(ADMIN_ASSIST_ROLE_TEXT);
    }
    if (true == currentUser.isAccountInstructor())
    {
        ui->SelectRoleBox->addItem(INSTRUCTOR_ROLE_TEXT);
    }
    if (true == currentUser.isAccountTA())
    {
        ui->SelectRoleBox->addItem(TA_ROLE_TEXT);
    }
    if (true == currentUser.isAccountAdmin())
    {
        ui->SelectRoleBox->addItem(ADMINISTRATOR_ROLE_TEXT);
    }
    rv = true; //May have to add error check here? not sure.
    return rv;
}

/* Function to display a specified page:
 *
 * PARAMS:
 *    Index             -  page to displayed
 *    saveCurrentIndex  -  indicate whether to save the current page in page history
 *    ClearLogMessage   -  optional argument to indicate whether to clear the log message displayed on UI.
 *                         Explicitly specify false to not clear log, default is true
 */
bool StreamlinedGradingSystem::displayPage(enum SGS_page_index index, bool saveCurrentIndex, bool ClearLogMessage)
{
    qDebug() << "page index " << index;
    bool rv = false;
    enum SGS_page_index pageIndex = index;
    QStringList tableHeader;  //to set table header in page
    switch (pageIndex)
    {
    case LOGIN_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Login Page</b></font>");
        ui->LogOutBt->setVisible((false));
        ui->previousPageBt->setVisible((true));
        //clear user input
        ui->userID->setText("");
        ui->password->setText("");
        //clear role selection of previous user (if existed)
        ui->SelectRoleBox->clear();
        rv = true;
        break;
    case SELECT_ROLE_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Select Role Page</b></font>");
        ui->LogOutBt->setVisible((true));
        //display roles that user have
        if (false == setRoleSelection())
        {
            rv = false;
        }
        else
        {
            rv = true;
        }
        break;
    case CHANGE_PASSWORD_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Change Password Page</b></font>");
        rv = true;
        break;
    case SYSTEM_ADMIN_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>System Admin Page</b></font>");
        rv = true;
        break;
    case ADMIN_ASSIST_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Admin Assist Page</b></font>");
        rv = true;
        break;
    case INSTRUCTOR_MAIN_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Instructor Page</b></font>");
        rv = true;
        break;
    case TA_MAIN_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>TA Page</b></font>");
        rv = true;
        break;
    case CREATE_ACCOUNT_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Account Page</b></font>");
        rv = true;
        break;
    case SELECT_AN_ACCOUNT_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Select An Account Page</b></font>");
        rv = true;
        break;
    case CREATE_COURSE_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Course Page</b></font>");
        rv = true;
        ui->CourseNameCreate_CreateCourse->setReadOnly(false);
        ui->CourseNumberCreate_CreateCourse->setReadOnly(false);
        ui->StartDateCreate_CreateCourse->setReadOnly(false);
        ui->EndDateCreate_CreateCourse->setReadOnly(false);
        ui->InstructorEmployeeID_CreateCourse->setReadOnly(false);
        ui->CreateCourseButton_CreateCourse->setDisabled(false);
        ui->TAEmployeeID_CreateCourse->setDisabled(false);
        ui->ImportFromCSVButton_CreateCourse->setDisabled(false);
        ui->AddInstructorButton_CreateCourse->setDisabled(false);
        ui->AddTAButton_CreateCourse->setDisabled(false);
        ui->RemoveTAButton_CreateCourse->setDisabled(false);

        ui->CopyPerviouCourseButton_CreateCourse->setVisible(true);
        ui->CopyCourseBt->setVisible(false);
        ui->CourseNameCreate_CreateCourse->clear();
        ui->CourseNumberCreate_CreateCourse->clear();
        ui->StartDateCreate_CreateCourse->setReadOnly(false);

        ui->InstructorName_CreateCourse->clear();
        ui->InstructorEmployeeID_CreateCourse->clear();
        ui->TAList_CreateCourse->clearContents();
        ui->TAList_CreateCourse->setRowCount(0);
        ui->StudentList_CreateCourse->clearContents();
        ui->StudentList_CreateCourse->setRowCount(0);
        ui->StartDateCreate_CreateCourse->setDate(QDate::currentDate());
        ui->EndDateCreate_CreateCourse->setDate(QDate::currentDate());

        break;
    case COPY_COURSE_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Copy Course Page</b></font>");
        rv = true;
        break;
    case CREATE_ACTIVITY_STEP_1_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Activity Page</b></font>");
        rv = true;
        break;
    case CREATE_PROGRAMMING_ACTIVITY_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Activity Page</b></font>");
        rv = true;
        break;
    case CREATE_MC_ACTIVITY_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Activity Page</b></font>");
        rv = true;
        break;
    case CREATE_ESSAY_ACTIVITY_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Activity Page</b></font>");
        rv = true;
        break;
    case SELECT_MODIFY_ACTIVITY_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Modify Activity Page</b></font>");
        rv = true;
        break;
    case CREATE_RUBRIC_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Create/Modify Rubric Page</b></font>");
        ui->rubricTable->setSelectionMode(QAbstractItemView::SingleSelection);
        // set default table header
        tableHeader << "Item Number" << "Item Description" << "Max Grade";
        ui->rubricTable->setHorizontalHeaderLabels(tableHeader);
        ui->rubricTable->verticalHeader()->hide();
        rv = true;
        break;
    case MARK_MAIN_PAGE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Main Grading Page</b></font>");
        rv = true;
        break;
    case MARK_ESSAY_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Grade Essay/Problem Set Page</b></font>");
        rv = true;
        break;
    case MARK_PROGRAMMING_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Grade Programming Activity Page</b></font>");
        rv = true;
        break;
    case MODIFY_VIEW_COURSE_INDEX:
        ui->PageTitle->setText("<font size=16 color=gray><b>Course Modification Page</b></font>");
        rv = true;
        break;
#if 0
      case BONUS_PENALTY_PAGE:
        ui->PageTitle->setText("<font size=16 color=gray><b>Bonus & Penalty Page</b></font>");
        rv = true;
        if(pageStack.top() == CREATE_COURSE_PAGE_INDEX)
        {
            ui->PageTitle->setText("<font size=16 color=gray><b>Copying Course Page</b></font>");
        }

        //if(pageStack.top() == ADMIN_ASSIST_PAGE_INDEX)
        //{
          //  ui->PageTitle->setText("<font size=16 color=gray><b>Course Modification Page</b></font>");
        //}
        break;
#endif
    default:
        rv = false;
        break;
    }


    if (true == rv)
    {
        if (true == saveCurrentIndex)
        {
            //record the current page index
            pageStack.push((enum SGS_page_index)ui->stackedWidget->currentIndex());
        }
        ui->stackedWidget->setCurrentIndex(pageIndex);
        if (true == ClearLogMessage)
        {
            clearLogMessage();
        }
    }
    return rv;
}


/* ================================================================================
 *                   Function group: General page control
 * ================================================================================*/

/* User select log out */
void StreamlinedGradingSystem::on_LogOutBt_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Logging out", "Are you sure you want to log out?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (QMessageBox::Yes == reply)
    {
        // May have to do some additional clean up here (unsure yet)
        displayPage(LOGIN_PAGE_INDEX, false);
        pageStack.clear(); //clear out the page history
    }
}

/* User choose to go back to a previous page */
void StreamlinedGradingSystem::on_previousPageBt_clicked()
{
    if (pageStack.isEmpty() == false)
    {
        displayPage((pageStack.pop()), false);
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "page history is empty");
    }
}



/* ============================================================================
 *                 Function group: Login Page Control
 * ============================================================================*/

/********************
 * This function is executed when user click on the login button
 ********************/
void StreamlinedGradingSystem::on_LoginBt_clicked()
{
    bool isFirstLogIn = true;

    if (true == validatePassword(ui->userID->text(), ui->password->text(), isFirstLogIn))
    {
        if (isFirstLogIn == false)
        {
            displayPage(SELECT_ROLE_PAGE_INDEX, true);
            currentUserRole == NO_ROLE_TEXT;
        }
        else
        {
            displayPage(CHANGE_PASSWORD_PAGE_INDEX, true);
            printLogMessage(CONFIRM_MESSAGE_TEXT,"First Login, please change your password");
        }
    }
    // else do something (maybe)
}


/* ======================================================================
 *                  Function group: Select Role Page
 * ======================================================================*/

/* This function is executed when user click the Change Password button */
void StreamlinedGradingSystem::on_ChangePasswordBt_clicked()
{
    displayPage(CHANGE_PASSWORD_PAGE_INDEX, true);
}

/* User click proceed after selecting a role */
void StreamlinedGradingSystem::on_ProceedBt_clicked()
{
    if(ui->SelectRoleBox->currentText() == SYS_ADMIN_ROLE_TEXT)
    {
        if (currentUser.isAccountSysAdmin() == false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "You do not have System Administrator role");
        }
        else
        {
            currentUserRole = SYS_ADMIN_ROLE_TEXT;
            displayPage(SYSTEM_ADMIN_PAGE_INDEX, true);
            clearLogMessage();
        }
    }
    else if(ui->SelectRoleBox->currentText() == ADMIN_ASSIST_ROLE_TEXT)
    {
        if (currentUser.isAccountAdminAssist() == false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "You do not have Administrative Assistant role");
        }
        else
        {
            currentUserRole = ADMIN_ASSIST_ROLE_TEXT;
            displayPage(ADMIN_ASSIST_PAGE_INDEX, true);
            clearLogMessage();
        }
    }
    else if(ui->SelectRoleBox->currentText() == ADMINISTRATOR_ROLE_TEXT)
    {
        if (currentUser.isAccountAdmin() == false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "You do not have Administrator role");
        }
        else
        {
            //Admin use same main page as TA (admin obly grade)
            displayPage(TA_MAIN_PAGE_INDEX,true);
            ui->PageTitle->setText("<font size=16 color=gray><b>Administrator Page</b></font>");
            currentUserRole = ADMINISTRATOR_ROLE_TEXT;
        }

    }
    else if(ui->SelectRoleBox->currentText() == INSTRUCTOR_ROLE_TEXT)
    {
        if (currentUser.isAccountInstructor() == false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "You do not have Instructor role");
        }
        else
        {
            currentUserRole = INSTRUCTOR_ROLE_TEXT;
            displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, true);
            clearLogMessage();
        }
    }
    else if(ui->SelectRoleBox->currentText() == TA_ROLE_TEXT)
    {
        if (currentUser.isAccountTA() == false)
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "You do not have TA role");
        }
        else
        {
            currentUserRole = TA_ROLE_TEXT;
            displayPage(TA_MAIN_PAGE_INDEX, true);
            clearLogMessage();
        }
    }
}

/* ===================================================================
 *             Function group: Change Password Page
 * ===================================================================*/

/* user press cancel on change password scren */
void StreamlinedGradingSystem::on_CancelBt_clicked()
{
    displayPage(pageStack.pop(),false);
}

/* User  */
void StreamlinedGradingSystem::on_SavePasswordChangeBt_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Change Password?",
              "Permanently change password?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if (true == changePassword(ui->CurrentPassword->text(), ui->NewPassword->text(), ui->ConfirmPassword->text()))
        {
            pageStack.pop();
            displayPage(SELECT_ROLE_PAGE_INDEX, false, false);
        }
        // else do something (not sure yet). Feedback message should be displayed in control func
    }
}


/* ===================================================================
 *              Function Group: Sys Admin main page
 * ===================================================================*/

/* Sys admin click create an account */
void StreamlinedGradingSystem::on_SACreateAccountBt_clicked()
{
    displayPage(CREATE_ACCOUNT_PAGE_INDEX, true);

    //below stuff is to fix problem when sys admin wants to create account after he modified an account

    //initialize fields, labels and check boxes
    ui->FirstNameCreate->setText("");
    ui->MiddleNameCreate->setText("");
    ui->LastNameCreate->setText("");
    ui->UserIDCreate->setText("");
    ui->EmployeeIDCreate->setText("");
//    ui->EmployeeIDCreate->setDisabled(false);
    ui->PasswordCreate->setText("");
    ui->Instructor_checkBox->setChecked(false);
    ui->SA_checkBox->setChecked(false);
    ui->TA_checkBox->setChecked(false);
    ui->Admin_checkBox->setChecked(false);
    ui->AdministrativeAdmin_checkBox->setChecked(false);

    //set which one is visible
    ui->AccountSaveChangeButton->setVisible(false);
    ui->CreateAccountButton->setVisible(true);
}

/* Sys admin click view/modify account */
void StreamlinedGradingSystem::on_SAModifyAccountBt_clicked()
{
    displayPage(SELECT_AN_ACCOUNT_PAGE_INDEX,true);
    displayAccountList();
}

//Sys admin want to block/unblock account
//void StreamlinedGradingSystem::on_blockAccountBt_clicked()
//{
//    // seem to use the same page as view/modify account (do we even need this button?)
//    displayPage(SELECT_AN_ACCOUNT_PAGE_INDEX, true);
//}

/* ===================================================================
 *           Function Group: create (or modify) account page
 * ===================================================================*/

/* User confirm 'create' account on create account page */
void StreamlinedGradingSystem::on_CreateAccountButton_clicked()
{

    if (true == createNewAccount())
    {
        pageStack.pop();
        //clear the text field
        ui->FirstNameCreate->setText("");
        ui->MiddleNameCreate->setText("");
        ui->LastNameCreate->setText("");
        ui->UserIDCreate->setText("");
        ui->EmployeeIDCreate->setText("");
        ui->PasswordCreate->setText("");
        ui->SA_checkBox->setChecked(false);
        ui->TA_checkBox->setChecked(false);
        ui->Admin_checkBox->setChecked(false);
        ui->AdministrativeAdmin_checkBox->setChecked(false);
        ui->Instructor_checkBox->setChecked(false);
        displayPage(SYSTEM_ADMIN_PAGE_INDEX, false);
        printLogMessage(CONFIRM_MESSAGE_TEXT, "Account created");
    }
}

//User Click "Save Changes" Button
void StreamlinedGradingSystem::on_AccountSaveChangeButton_clicked()
{
    clearLogMessage();
    if (QMessageBox::Yes == QMessageBox::question(this, "Save changes to Activity?",
              "Permanently save changes to the selected activity?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if( updateAccInfoChanges() )
            displayAccountList();
    }
}

/* ===================================================================
 *           Function Group: select account page
 * ===================================================================*/

/* User click "View/Modify" button on account selection page */
void StreamlinedGradingSystem::on_ModifyAccountButton_clicked()
{
    int currRow=ui->ListOfAccounts->currentRow();
    if(currRow<0) //checks if any account is selected
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select an account!");
        return;
    }
    QString selectedEID=ui->ListOfAccounts->item(currRow,0)->text();
    displayPage(CREATE_ACCOUNT_PAGE_INDEX,true);
    displayInfoInAccModPg(selectedEID);
    ui->PageTitle->setText("<font size=16 color=gray><b>Modify " + ui->ListOfAccounts->item(currRow,2)->text() + " " + ui->ListOfAccounts->item(currRow,3)->text() + "</b></font>");
//    ui->EmployeeIDCreate->setDisabled(true);
    ui->CreateAccountButton->setVisible(false);
    ui->AccountSaveChangeButton->setVisible(true);
}

//sys admin click block
void StreamlinedGradingSystem::on_BlockButton_clicked()
{
    int currRow=ui->ListOfAccounts->currentRow();
    if(currRow<0) //checks if any account is selected
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select an account!");
        return;
    }
    QString selectedUsername=ui->ListOfAccounts->item(currRow,1)->text();
    bool success=blockAcc(selectedUsername);
    if(success)
        qDebug()<<"BLOCK ACCOUNT SUCCESSFUL!";
    //displayAccountList();
}

//sys admin click unblock
void StreamlinedGradingSystem::on_UnblockButton_clicked()
{
    int currRow=ui->ListOfAccounts->currentRow();
    if(currRow<0) //checks if any account is selected
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select an account!");
        return;
    }
    QString selectedUsername=ui->ListOfAccounts->item(currRow,1)->text();
    bool success=unblockAcc(selectedUsername);
    if(success)
        qDebug()<<"UNBLOCK ACCOUNT SUCCESSFUL!";
    //displayAccountList();
}

//sys admin click delete
void StreamlinedGradingSystem::on_DeleteAccountButton_clicked()
{
    int currRow=ui->ListOfAccounts->currentRow();
    if(currRow<0) //checks if any account is selected
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select an account!");
        return;
    }
    //display message box
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deleting an account!", "Are you sure you want to delete " + ui->ListOfAccounts->item(currRow,1)->text() + " ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == reply)
        qDebug()<<delAcc(currRow);
}

/* ===================================================================
 *              Function Group: Admin Assist main page
 * ===================================================================*/
void StreamlinedGradingSystem::on_ViewModifyCourseBt_clicked()
{
    displayPage(MODIFY_VIEW_COURSE_INDEX,true);
    ui->ViewCopyCourseBt->setVisible(false);
    ui->PageTitle->setText("<font size=16 color=gray><b>Course Modification Page</b></font>");
    ui->ViewModifyCourseSelectionBt->setVisible(true);
    ui->DeleteCourseBt->setVisible(true);
    ui->label_43->setText("Please select a course which you would like to view / modify.");
    displayCourseList();
}

/* ===================================================================
 *              Function Group: Course Selection page
 * ===================================================================*/

//admin assist clicks view/modify
void StreamlinedGradingSystem::on_ViewModifyCourseSelectionBt_clicked()
{
    int currRow=ui->ListOfCourses->currentRow();
    if(currRow<0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select a course!");
        return;
    }
    displayPage(CREATE_COURSE_PAGE_INDEX,true);
    displayCourseInfo(currRow);
    ui->PageTitle->setText("<font size=16 color=gray><b>Modify " + ui->ListOfCourses->item(currRow,0)->text() + "</b></font>");
    ui->CopyCourseBt->setVisible(false);
    ui->CreateCourseButton_CreateCourse->setVisible(false);
    ui->CopyPerviouCourseButton_CreateCourse->setVisible(false);
    ui->SaveChangeCourseButton_CreateCourse->setVisible(true);
//    ui->DeleteCourseButton_CreateCourse->setVisible(true);
    ui->CreateCourseButton_CreateCourse->setVisible(false);

}

//admin assist clicks delete
void StreamlinedGradingSystem::on_DeleteCourseBt_clicked()
{
    int currRow=ui->ListOfCourses->currentRow();
    if(currRow<0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select a course!");
        return;
    }

    qDebug()<<delCourse(currRow);
}




/* ===================================================================
 *              Function Group: Copying Course Page
 * ===================================================================*/

void StreamlinedGradingSystem::on_CopyPerviouCourseButton_CreateCourse_clicked()
{

    displayPage(MODIFY_VIEW_COURSE_INDEX, true);
    ui->PageTitle->setText("<font size=16 color=gray><b>Copying Course Page</b></font>");
    ui->label_43->setText("Please select a course which you woud like to copy");
    ui->ViewModifyCourseSelectionBt->setVisible(false);
    ui->DeleteCourseBt->setVisible(false);
    ui->ViewCopyCourseBt->setVisible(true);
    displayCourseList();
}



void StreamlinedGradingSystem::on_ViewCopyCourseBt_clicked()
{
    int currRow=ui->ListOfCourses->currentRow();
    if(currRow<0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select a course!");
        return;
    }
    displayPage(CREATE_COURSE_PAGE_INDEX,true);
    displayCourseInfo(currRow);
    ui->PageTitle->setText("<font size=16 color=gray><b>View/Copy " + ui->ListOfCourses->item(currRow,0)->text() + "</b></font>");
    ui->CopyCourseBt->setVisible(true);
    ui->CopyCourseBt->setEnabled(true);
    ui->CopyPerviouCourseButton_CreateCourse->setVisible(false);
    ui->CourseNameCreate_CreateCourse->setReadOnly(true);
    ui->CourseNumberCreate_CreateCourse->setReadOnly(true);
    ui->StartDateCreate_CreateCourse->setReadOnly(true);
    ui->EndDateCreate_CreateCourse->setReadOnly(true);
    ui->InstructorEmployeeID_CreateCourse->setReadOnly(true);
    ui->CreateCourseButton_CreateCourse->setDisabled(true);
    ui->TAEmployeeID_CreateCourse->setDisabled(true);
    ui->ImportFromCSVButton_CreateCourse->setDisabled(true);
    ui->AddInstructorButton_CreateCourse->setDisabled(true);
    ui->AddTAButton_CreateCourse->setDisabled(true);
    ui->RemoveTAButton_CreateCourse->setDisabled(true);


}

void StreamlinedGradingSystem::on_CopyCourseBt_clicked()
{
    ui->CopyCourseBt->setEnabled(false);

    ui->CourseNameCreate_CreateCourse->setReadOnly(false);
    ui->CourseNumberCreate_CreateCourse->setReadOnly(false);
    ui->StartDateCreate_CreateCourse->setReadOnly(false);
    ui->EndDateCreate_CreateCourse->setReadOnly(false);
    ui->InstructorEmployeeID_CreateCourse->setReadOnly(false);
    ui->CreateCourseButton_CreateCourse->setDisabled(false);
    ui->TAEmployeeID_CreateCourse->setDisabled(false);
    ui->ImportFromCSVButton_CreateCourse->setDisabled(false);
    ui->AddInstructorButton_CreateCourse->setDisabled(false);
    ui->AddTAButton_CreateCourse->setDisabled(false);
    ui->RemoveTAButton_CreateCourse->setDisabled(false);

    ui->StartDateCreate_CreateCourse->setReadOnly(false);
    ui->StartDateCreate_CreateCourse->setDate(QDate::currentDate());
    ui->EndDateCreate_CreateCourse->setDate(QDate::currentDate());

    ui->StudentList_CreateCourse->clearContents();
    ui->StudentList_CreateCourse->setRowCount(0);

    printLogMessage(CONFIRM_MESSAGE_TEXT,"Please set start and end dates of the copied course and click 'Create Course' button to successfully create course");
}

/* ===================================================================
 *        Function Group: View/Modify a selected course page
 * ===================================================================*/

void StreamlinedGradingSystem::on_ImportFromCSVButton_CreateCourse_clicked()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open CSV file"), "", tr( "All files (*.*);;CSV files (*.csv)") );
    QFile csvFile(FileName);

    QString data;
    QStringList rowOfdata;
    QStringList rowData;
    data.clear();
    rowOfdata.clear();
    rowData.clear();


    //Import data from CSV file. Credit to: http://stackoverflow.com/questions/9165183/how-to-import-a-csv-file-to-a-qtablewidget


    if (csvFile.open(QFile::ReadOnly))     //opening CSV file
    {
        ui->StudentList_CreateCourse->clear();

        //set headers
        ui->StudentList_CreateCourse->setHorizontalHeaderItem(0,new QTableWidgetItem("Student ID"));
        ui->StudentList_CreateCourse->setHorizontalHeaderItem(1,new QTableWidgetItem("First Name"));
        ui->StudentList_CreateCourse->setHorizontalHeaderItem(2,new QTableWidgetItem("Middle Name"));
        ui->StudentList_CreateCourse->setHorizontalHeaderItem(3,new QTableWidgetItem("Last Name"));

        ui->StudentList_CreateCourse->setRowCount(0);
        data = csvFile.readAll();
        rowOfdata = data.split("\n");
        csvFile.close();
    }


    rowOfdata.removeAll("");

    qDebug() << "data size " << rowOfdata.size();
    for (int x = 0; x < rowOfdata.size(); x++)
    {
        qDebug() << "row " << x;
        rowData = rowOfdata.at(x).split(",");
        QString firstName = rowData.at(0);
        QString middleName = rowData.at(1);
        QString lastName = rowData.at(2);
        //QString studentName;
        QString studentID = rowData.at(3);


        qDebug() << "x" << x;
        qDebug() << "studentID" << studentID;
        ui->StudentList_CreateCourse->insertRow(x);

        ui->StudentList_CreateCourse->setItem(x,0, new QTableWidgetItem(studentID) );
        ui->StudentList_CreateCourse->setItem(x,1, new QTableWidgetItem(firstName) );
        ui->StudentList_CreateCourse->setItem(x,2, new QTableWidgetItem(middleName) );
        ui->StudentList_CreateCourse->setItem(x,3, new QTableWidgetItem(lastName) );
    }

}

void StreamlinedGradingSystem::on_SaveChangeCourseButton_CreateCourse_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Save changes to course?",
              "Permanently save changes to the selected course?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if (saveChangesCourseInfo())
        {
            displayCourseList();
        }
    }
}


/* ===================================================================
 *              Function Group: Instructor main page
 * ===================================================================*/

/* user select create activity on instructor main page */
void StreamlinedGradingSystem::on_CreateActivityButton_clicked()
{
    displayPage(CREATE_ACTIVITY_STEP_1_INDEX,true);
    ui->CourseList_ActivityType->clear();
    (void)viewOwnCourse();
}

/* Instructor click grade/regrade on Instructor main page - go to grading page (same as TA and Admin) */
void StreamlinedGradingSystem::on_GradeActivityButton_clicked()
{
    // clear lists
    ui->CourseList_Marker->clear();
    while (ui->ActivityList_Marker->rowCount() > 0)
    {
        ui->ActivityList_Marker->removeRow(0);
    }
    while (ui->studentList_Marking->rowCount() > 0)
    {
      ui->studentList_Marking->removeRow(0);
    }
    displayPage(MARK_MAIN_PAGE_INDEX, true);
    viewOwnCourse();
    QStringList TableHeader;
    TableHeader << "Student #" << "Points" << "Submmited on" << "Marked by" << "Marker Role";
    ui->studentList_Marking->setHorizontalHeaderLabels(TableHeader);
}
/* ====================================================================
 *     Function group: Creating a Activity (1: select activity type)
 * ====================================================================*/

/* user click 'next' after selecting activity type */
void StreamlinedGradingSystem::on_ActivityNextButton_clicked()
{
    /* handle course selection on list */
    ui->CreateActivityButton_MC->setEnabled(true);
    ui->CreateActivityButton_Essay->setEnabled(true);
    ui->CreateActivityButton_Programming->setEnabled(true);

    if (!(ui->CourseList_ActivityType->selectedItems().isEmpty()))
    {
        clearLogMessage();
        currentCourse = Course(tempCourseIDList[ui->CourseList_ActivityType->currentRow()]);

        ui->CopyProgrammingActivityBt->setEnabled(false);
        ui->CreateActivityButton_Programming->setEnabled(true);
        ui->ActivityName_Programming->setReadOnly(false);
        ui->DueDate_Programming->setReadOnly(false);
        ui->DueTime_Programming->setReadOnly(false);
        ui->SolutionPath_Programming->setReadOnly(false);
        ui->SolutionPath_ProgrammingButton->setEnabled(true);
        ui->StudentSubmissionPath_Programming->setReadOnly(false);
        ui->StudentSubmissionPath_ProgrammingButton->setEnabled(true);
        ui->LanguageComboBox_Programming->setEnabled(true);
        ui->Inputpathlist_Programming->setEnabled(true);
        ui->InputTestFilePath_Programming->setReadOnly(false);
        ui->InputTestFilePath_ProgrammingButton->setEnabled(true);
        ui->addInputPathButton_Programming->setEnabled(true);
        ui->Outputpathlist_Programming->setEnabled(true);
        ui->OutputTestFilePath_Programming->setReadOnly(false);
        ui->OutputTestFilePath_ProgrammingButton->setEnabled(true);
        ui->addOutputPathButton_Programming->setEnabled(true);
        ui->EnviromentComboBox_Programming->setEnabled(true);
        ui->ConsoleInputTestFilePath_Programming->setReadOnly(false);
        ui->ConsoleInputTestFilePath_ProgrammingButton->setEnabled(true);
        ui->ConsoleOutputTestFilePath_Programming->setReadOnly(false);
        ui->ConsoleOutputTestFilePath_ProgrammingButton->setEnabled(true);
        ui->NumberofTest_Programming->setReadOnly(false);
        ui->confirmButton_Programming->setEnabled(true);
        ui->RemoveInputPathButton_Programming->setEnabled(true);
        ui->RemoveOutputPathButton_Programming->setEnabled(true);

        ui->CopyMCBt->setEnabled(false);
        ui->CreateActivityButton_MC->setEnabled(true);
        ui->ActivityName_MC->setReadOnly(false);
        ui->DueDate_MC->setReadOnly(false);
        ui->DueTime_MC->setReadOnly(false);
        ui->StudentSubmissionPath_MC->setReadOnly(false);
        ui->StudentSubmissionPath_MCButton->setEnabled(true);
        ui->LanguageComboBox_MC->setEnabled(true);

        ui->CopyEssayBt->setEnabled(false);
        ui->CreateActivityButton_Essay->setEnabled(true);
        ui->ActivityName_Essay->setReadOnly(false);
        ui->DueDate_Essay->setReadOnly(false);
        ui->DueTime_Essay->setReadOnly(false);
        ui->SolutionPath_Essay->setReadOnly(false);
        ui->SolutionPath_EssayButton->setEnabled(true);
        ui->StudentSubmissionPath_Essay->setReadOnly(false);
        ui->StudentSubmissionPath_EssayButton->setEnabled(true);
        ui->LanguageComboBox_Essay->setEnabled(true);

        //When Essay Activity Type is Selected, a custom page is shown
        if (ui->EssayRadioButton->isChecked())
        {
            ui->CopyEssayBt->setVisible(false);
            ui->ModifyRubic_EssayButton->setVisible(false);
            ui->CopyEssayBt->setVisible(false);
            ui->CreateRubic_EssayButton->setVisible(true);
            ui->SaveChangesButton_Essay->setVisible(false);
            ui->CreateActivityButton_Essay->setVisible(true);
            ui->ActivityName_Essay->clear();
            ui->SolutionPath_Essay->clear();
            ui->DueDate_Essay->setDate(QDate::currentDate());
            ui->DueTime_Essay->setTime(QTime::currentTime());
            ui->StudentSubmissionPath_Essay->clear();
            ui->LanguageComboBox_Essay->setCurrentIndex(0);
            displayPage(CREATE_ESSAY_ACTIVITY_INDEX,true);
            //re-instantiate currentActivity
            currentActivity = Activity();
        }
        //When Multiple Choice Activity Type is Selected a custom page is shown
        else if (ui->MultipleChoiceRadioButton->isChecked())
        {
            ui->CopyMCBt->setVisible(false);
            ui->SaveChangesButton_MC->setVisible(false);
            ui->CopyMCBt->setVisible(false);
            ui->CreateActivityButton_MC->setVisible(true);
            ui->ActivityName_MC->clear();
//            ui->SolutionPath_MC-> setText("//");
            ui->DueDate_MC->setDate(QDate::currentDate());
            ui->DueTime_MC->setTime(QTime::currentTime());
            ui->StudentSubmissionPath_MC->clear();
            ui->LanguageComboBox_MC->setCurrentIndex(0);
            displayPage(CREATE_MC_ACTIVITY_INDEX,true);
            //re-instantiate currentActivity
            currentActivity = Activity();
        }
        //When Programming Activity Type is Selected a custom page is shown
        else if (ui->ProgrammingRadioButton->isChecked())
        {
            ui->CopyProgrammingActivityBt->setVisible(false);
            ui->ModifyRubic_ProgrammingButton->setVisible(false);
            ui->CopyProgrammingActivityBt->setVisible(false);
            ui->CreateRubic_ProgrammingButton->setVisible(true);
            ui->SaveChangesButton_Programming->setVisible(false);
            ui->CreateActivityButton_Programming->setVisible(true);
            ui->ActivityName_Programming->clear();
            ui->SolutionPath_Programming->clear();
            ui->DueDate_Programming->setDate(QDate::currentDate());
            ui->DueTime_Programming->setTime(QTime::currentTime());
            ui->StudentSubmissionPath_Programming->clear();
            ui->LanguageComboBox_Programming->setCurrentIndex(0);
            ui->Inputpathlist_Programming->clear();
            ui->Outputpathlist_Programming->clear();
            ui->NumberofTest_Programming->setText("");
            ui->TestCasecomboBox_Programming->clear();
            ui->TestCasecomboBox_Programming->addItem("Select Test Case Here");
            consoleInputPath_TestCase.clear();
            consoleOutputPath_TestCase.clear();
            testCaseList.clear();
            ui->ConsoleInputTestFilePath_Programming->setText("");
            ui->ConsoleOutputTestFilePath_Programming->setText("");
            ui->InputTestFilePath_Programming->clear();
            ui->OutputTestFilePath_Programming->clear();
            displayPage(CREATE_PROGRAMMING_ACTIVITY_INDEX,true);
            //re-instantiate currentActivity
            currentActivity = Activity();
        }
        else
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "Please Select a type of activity");
        }
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "No course is selected");
    }

}

//user clicks copy activity button on activity creation
void StreamlinedGradingSystem::on_CopyActivityButton_clicked()
{
    int currRow = ui->CourseList_ActivityType->currentRow();
    if(currRow<0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please select a course you want to copy to.");
        return;
    }

    currentCourse = Course(tempCourseIDList[currRow]);

    ui->CourseList_SelectModifyPage->clear();
    while (ui->ActivityList_SelectModifyPage->rowCount() > 0)
    {
        ui->ActivityList_SelectModifyPage->removeRow(0);
    }
    QStringList Tableheader;
    Tableheader << "Activity Name" << "Activity Type";
    ui->ActivityList_SelectModifyPage->setHorizontalHeaderLabels(Tableheader);
    if (viewOwnCourse())
    {
        displayPage(SELECT_MODIFY_ACTIVITY_INDEX,true);
    }
    ui->PageTitle->setText("<font size=16 color=gray><b>Select Activity</b></font>");
    //set which button is visible
    ui->ModifyButton_SelectModifyPage->setVisible(false);
    ui->DeleteButton_SelectModifyPage->setVisible(false);
    ui->CopyActivityBt_SelModPage->setVisible(true);

    //Resize width to fit table length. Credit to: http://stackoverflow.com/questions/17535563/how-to-get-a-qtableview-to-fill-100-of-the-width
    for(int i=0 ; i < ui->ActivityList_SelectModifyPage->horizontalHeader()->count() ; i++)
    {
        ui->ActivityList_SelectModifyPage->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    }

    //the purpose of the rest part of this function is to hide the course that is more than 5 years from the list when admin asssist tries to copy the activity.
#if 0
//    QDate currTime = QDate::currentDate();
//    int year = currTime.year();
//    int month = currTime.month();
//    int date = currTime.day();
//    QDate restrictTime = QDate(year-5,month,date);

//    for (int i = 0; i < ui->CourseList_SelectModifyPage->rowCount(); i++)
//    {
//        QString endDateItem = ui->CourseList_SelectModifyPage->item(i,3)->text();
//        QDate endDateInTime = QDate::fromString( endDateItem, "yyyy-MM-dd");

//        if(endDateInTime < restrictTime)
//        {
//            ui->CourseList_SelectModifyPage->removeRow(i);
//        }
//    }
#endif
}

/* ====================================================================
 *     Function group: Creating a Activity (2: Filling out Details)
 * ====================================================================*/

/* user select create rubric on create programming activity page */
void StreamlinedGradingSystem::on_CreateRubic_ProgrammingButton_clicked()
{
    while (0 < ui->rubricTable->rowCount())
    {
        qDebug() << "row count = " << ui->rubricTable->rowCount();
        ui->rubricTable->removeRow(0);
    }
    displayPage(CREATE_RUBRIC_PAGE_INDEX, true);
    loadRubric(currentActivity);
}

/* user select create rubric on create essay/problem set activity page */
void StreamlinedGradingSystem::on_CreateRubic_EssayButton_clicked()
{
    displayPage(CREATE_RUBRIC_PAGE_INDEX, true);
    // clear rubricTable and load rubric info from current activity;
    while (0 < ui->rubricTable->rowCount())
    {
        qDebug() << "row count = " << ui->rubricTable->rowCount();
        ui->rubricTable->removeRow(0);
    }
    loadRubric(currentActivity);
}


/* =====================================================================
 *                 Function group: Creating Rubric Page
 * =====================================================================*/

/* user add a new item to rubric. This function adds an empty row to the rubric table */
void StreamlinedGradingSystem::on_AddCriteriaButton_clicked()
{
    //add a new row to the table
    int rowCount = ui->rubricTable->rowCount();
    ui->rubricTable->insertRow(rowCount);
    //auto number the new row
    ui->rubricTable->setItem(rowCount,RUBRIC_ITEM_NUMBER, new QTableWidgetItem(QString::number(rowCount+1)));
}

/* user remove a rubric item */
void StreamlinedGradingSystem::on_RemoveCriteriaButton_clicked()
{
    int currentRow;
    if (false == ui->rubricTable->selectedRanges().isEmpty())
    {
        currentRow = ui->rubricTable->selectedRanges().first().topRow();
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"No item selected. Cannot remove");
        return;
    }
    ui->rubricTable->removeRow(currentRow);
    printLogMessage(CONFIRM_MESSAGE_TEXT, "Rubric item removed");
}

/* user click save rubric */
void StreamlinedGradingSystem::on_SaveRubricButton_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Save Rubric?",
        "Rubric will be save permanently when clicking Create/Save on activity page.\n\
Note that all grades for the current activity related to the previous rubric will be deleted",
              QMessageBox::Yes|QMessageBox::No))
    {
        // may need to do some clean up here later
        if (true == saveActivityRubric())
        {
            displayPage(pageStack.pop(),false);
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Rubric Created Successfully");
        }
    }
    else
    {

    }

}

/* ===================================================================
 *               Function Group: TA main page
 * ===================================================================*/

/* TA click grade/regrade on TA main page */
void StreamlinedGradingSystem::on_TAGradeButton_clicked()
{
    // clear lists
    ui->CourseList_Marker->clear();
    while (ui->ActivityList_Marker->rowCount() > 0)
    {
        ui->ActivityList_Marker->removeRow(0);
    }
    ui->studentList_Marking->clear();
    while (ui->studentList_Marking->rowCount() > 0)
    {
      ui->studentList_Marking->removeRow(0);
    }
    displayPage(MARK_MAIN_PAGE_INDEX, true);
    viewOwnCourse();
    QStringList TableHeader;
    TableHeader << "Student #" << "Points" << "Submmited on" << "Marked by" << "Marker Role";
    ui->studentList_Marking->setHorizontalHeaderLabels(TableHeader);
}


/* ====================================================================
 *               Function group: Copying a Course
 * ====================================================================*/
void StreamlinedGradingSystem::on_CopyPerviouCourseButton_clicked()
{
    displayPage(COPY_COURSE_PAGE_INDEX,true);
}



/* =====================================================================
 *               Function group: View/Modify Activity
 * =====================================================================*/
void StreamlinedGradingSystem::on_ModifyActivityButton_clicked()
{

    ui->CourseList_SelectModifyPage->clear();
    while (ui->ActivityList_SelectModifyPage->rowCount() > 0)
    {
        ui->ActivityList_SelectModifyPage->removeRow(0);
    }
    QStringList Tableheader;
    Tableheader << "Activity Name" << "Activity Type";
    ui->ActivityList_SelectModifyPage->setHorizontalHeaderLabels(Tableheader);
    if (viewOwnCourse())
    {
        displayPage(SELECT_MODIFY_ACTIVITY_INDEX,true);
    }

    //set title
    ui->PageTitle->setText("<font size=16 color=gray><b>Select Activity</b></font>");

    //set which buttons is visible
    ui->ModifyButton_SelectModifyPage->setVisible(true);
    ui->DeleteButton_SelectModifyPage->setVisible(true);
    ui->CopyActivityBt_SelModPage->setVisible(false);

    //Resize width to fit table length. Credit to: http://stackoverflow.com/questions/17535563/how-to-get-a-qtableview-to-fill-100-of-the-width
    for(int i=0 ; i < ui->ActivityList_SelectModifyPage->horizontalHeader()->count() ; i++)
    {
        ui->ActivityList_SelectModifyPage->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    }
}

/* =====================================================================
 *              Function group: Creating Course Page
 * =====================================================================*/


/* user click "create course" on create course page */
void StreamlinedGradingSystem::on_CreateCourseButton_clicked()
{

    ui->CourseNameCreate_CreateCourse->setReadOnly(false);
    ui->CourseNumberCreate_CreateCourse->setReadOnly(false);
    ui->StartDateCreate_CreateCourse->setReadOnly(false);
    ui->EndDateCreate_CreateCourse->setReadOnly(false);
    ui->InstructorEmployeeID_CreateCourse->setReadOnly(false);
    ui->CreateCourseButton_CreateCourse->setDisabled(false);
    ui->TAEmployeeID_CreateCourse->setDisabled(false);
    ui->ImportFromCSVButton_CreateCourse->setDisabled(false);
    ui->AddInstructorButton_CreateCourse->setDisabled(false);
    ui->AddTAButton_CreateCourse->setDisabled(false);
    ui->RemoveTAButton_CreateCourse->setDisabled(false);

    ui->CourseNameCreate_CreateCourse->clear();
    ui->CourseNumberCreate_CreateCourse->clear();
    ui->StartDateCreate_CreateCourse->setReadOnly(false);
    ui->StartDateCreate_CreateCourse->clear();
    ui->EndDateCreate_CreateCourse->clear();

    ui->StudentList_CreateCourse->clearContents();
    ui->StudentList_CreateCourse->setRowCount(0);



    //Reset the Create Course Page
    displayPage(CREATE_COURSE_PAGE_INDEX,true);
    ui->SaveChangeCourseButton_CreateCourse->setVisible(false);
    ui->CopyCourseBt->setVisible(false);
//    ui->DeleteCourseButton_CreateCourse->setVisible(false);
    ui->CreateCourseButton_CreateCourse->setVisible(true);
    ui->CopyPerviouCourseButton_CreateCourse->setVisible(true);
    ui->InstructorName_CreateCourse->clear();
    //ui->CourseNameCreate_CreateCourse->setText("Enter Course Name");
    //ui->CourseNumberCreate_CreateCourse->setText("Enter Course Number");
    ui->StartDateCreate_CreateCourse->setDate(QDate::currentDate());
    ui->EndDateCreate_CreateCourse->setDate(QDate::currentDate());
    //ui->InstructorName_CreateCourse->setText("Enter Instructor Employee ID");
    ui->InstructorEmployeeID_CreateCourse->setText("");
    int k = ui->TAList_CreateCourse->rowCount();
    for (int i=0; i <= k; i++)
    {
        ui->TAList_CreateCourse->removeRow(0);
    }
    k = ui->StudentList_CreateCourse->rowCount();
    for (int i=0; i <= k;i++)
    {
        ui->StudentList_CreateCourse->removeRow(0);
    }
    //    ui->TAList_CreateCourse->setHorizontalHeaderItem(0,new QTableWidgetItem("TA Employee ID"));
    //    ui->TAList_CreateCourse->setHorizontalHeaderItem(1,new QTableWidgetItem("TA Name"));
    //    ui->StudentList_CreateCourse->setHorizontalHeaderItem(0,new QTableWidgetItem("Student ID"));
    //    ui->StudentList_CreateCourse->setHorizontalHeaderItem(1,new QTableWidgetItem("Student Name"));
}

/* User add instructor to course */
void StreamlinedGradingSystem::on_AddInstructorButton_CreateCourse_clicked()
{
    if(ui->InstructorEmployeeID_CreateCourse->text()!=""){
        (void) searchInstructorName();
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Please enter the instructor employee ID");
    }
}
//search Name function for TA
void StreamlinedGradingSystem::on_AddTAButton_CreateCourse_clicked()
{
    if(ui->TAEmployeeID_CreateCourse->text()!="")
        (void) searchTAName();
    else
        printLogMessage(ERROR_MESSAGE_TEXT,"Please enter the TA employee ID!");
}

void StreamlinedGradingSystem::on_RemoveTAButton_CreateCourse_clicked()
{
    if( ui->TAEmployeeID_CreateCourse->text() != "" )
        (void) removefromthelist();
    else if( ui->TAList_CreateCourse->currentRow() >= 0 )
    {
        ui->TAList_CreateCourse->removeRow( ui->TAList_CreateCourse->currentRow() );
        printLogMessage(CONFIRM_MESSAGE_TEXT,"TA removed!");
    }
    else
        printLogMessage(ERROR_MESSAGE_TEXT, "Please enter the TA employee ID or select from the TA/TM List!");
}

void StreamlinedGradingSystem::on_CreateCourseButton_CreateCourse_clicked()
{
    if(createNewCourse()){
        pageStack.pop();
        displayPage(ADMIN_ASSIST_PAGE_INDEX, false);
        printLogMessage(CONFIRM_MESSAGE_TEXT,"Created course successfully");
    }
}

/*------------------ Functions for browsing file path ---------------*
 * Author: saad
 */
void StreamlinedGradingSystem::on_StudentSubmissionPath_ProgrammingButton_clicked()
{
    QString filename=QFileDialog::getExistingDirectory(this, tr("Open File"), "C://"

                                                       );
    ui->StudentSubmissionPath_Programming->setText(filename);
}

void StreamlinedGradingSystem::on_SolutionPath_ProgrammingButton_clicked()
{
    QString filename=QFileDialog::getExistingDirectory(this, tr("Open File"), "C://"

                                                  );
    ui->SolutionPath_Programming->setText(filename);
}

void StreamlinedGradingSystem::on_ConsoleInputTestFilePath_ProgrammingButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://","All files (*.*) "

                                                  );
    ui->ConsoleInputTestFilePath_Programming->setText(filename);
}

void StreamlinedGradingSystem::on_ConsoleOutputTestFilePath_ProgrammingButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://","All files (*.*) "

                                                  );
    ui->ConsoleOutputTestFilePath_Programming->setText(filename);
}

void StreamlinedGradingSystem::on_InputTestFilePath_ProgrammingButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://","All files (*.*) "

                                                  );
    ui->InputTestFilePath_Programming->setText(filename);
}

void StreamlinedGradingSystem::on_OutputTestFilePath_ProgrammingButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://","All files (*.*) "

                                                  );
    ui->OutputTestFilePath_Programming->setText(filename);
}

void StreamlinedGradingSystem::on_StudentSubmissionPath_MCButton_clicked()
{
    QString filename=QFileDialog::getExistingDirectory(this, tr("Open File"), "C://"
                                                       );
    ui->StudentSubmissionPath_MC->setText(filename);
}

//void StreamlinedGradingSystem::on_SolutionPath_MCButton_clicked()
//{
//    QString filename=QFileDialog::getOpenFileName(this, tr("Open CSV file"), "", tr( "CSV files (*.csv)")

//                                                  );
//    ui->SolutionPath_MC->setText(filename);
//}
\
void StreamlinedGradingSystem::on_StudentSubmissionPath_EssayButton_clicked()
{
    QString filename=QFileDialog::getExistingDirectory(this, tr("Open File"), "C://"

                                                       );
    ui->StudentSubmissionPath_Essay->setText(filename);
}

void StreamlinedGradingSystem::on_SolutionPath_EssayButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "C://","All files (*.*) "

                                                  );
    ui->SolutionPath_Essay->setText(filename);
}
/*---------- /functions for browsing file path/ -------------*/

/* User click "create" on create programming activity page */
void StreamlinedGradingSystem::on_CreateActivityButton_Programming_clicked()
{
    clearLogMessage();
    storecurrentTestCase();
    if(createProgrammingActivity()){
        pageStack.pop();
        pageStack.pop();
        displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, false);
        printLogMessage(CONFIRM_MESSAGE_TEXT,"Create Activity successfully");
    }
}

/* User click "create" on create essay activity page */
void StreamlinedGradingSystem::on_CreateActivityButton_Essay_clicked()
{
    if(createEssayActivity()){
        pageStack.pop();
        pageStack.pop();
        displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, false, false);
    }
}
void StreamlinedGradingSystem::on_CreateActivityButton_MC_clicked()
{

    if(createMCActivity()){
        pageStack.pop();
        pageStack.pop();
        displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, false, false);
    }
}

/* =====================================================================
 *              Function group: View/Modify Activity Page
 * =====================================================================*/


/* user click "create course" on create course page */
void StreamlinedGradingSystem::on_CourseList_SelectModifyPage_itemClicked(QListWidgetItem *item)
{
    clearLogMessage();

    //refresh the activity list every time when the use choose another course
    while (ui->ActivityList_SelectModifyPage->rowCount() > 0)
    {
        ui->ActivityList_SelectModifyPage->removeRow(0);
    }
    QStringList Tableheader;
    Tableheader << "Activity Name" << "Activity Type";
    ui->ActivityList_SelectModifyPage->setHorizontalHeaderLabels(Tableheader);
    (void) viewOwnCourseActivity();
    ui->ActivityList_SelectModifyPage->setSelectionMode(QAbstractItemView::SingleSelection);
}

/* user click "View/Modify" on view modify Activity page */
void StreamlinedGradingSystem::on_ModifyButton_SelectModifyPage_clicked()
{

    ui->rubricTable->setEnabled(true);
    ui->AddCriteriaButton->setEnabled(true);
    ui->RemoveCriteriaButton->setEnabled(true);

    ui->CopyProgrammingActivityBt->setEnabled(false);
    ui->CreateActivityButton_Programming->setEnabled(true);
    ui->ActivityName_Programming->setReadOnly(false);
    ui->DueDate_Programming->setReadOnly(false);
    ui->DueTime_Programming->setReadOnly(false);
    ui->SolutionPath_Programming->setReadOnly(false);
    ui->SolutionPath_ProgrammingButton->setEnabled(true);
    ui->StudentSubmissionPath_Programming->setReadOnly(false);
    ui->StudentSubmissionPath_ProgrammingButton->setEnabled(true);
    ui->LanguageComboBox_Programming->setEnabled(true);
    ui->Inputpathlist_Programming->setEnabled(true);
    ui->InputTestFilePath_Programming->setReadOnly(false);
    ui->InputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->addInputPathButton_Programming->setEnabled(true);
    ui->Outputpathlist_Programming->setEnabled(true);
    ui->OutputTestFilePath_Programming->setReadOnly(false);
    ui->OutputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->addOutputPathButton_Programming->setEnabled(true);
    ui->EnviromentComboBox_Programming->setEnabled(true);
    ui->ConsoleInputTestFilePath_Programming->setReadOnly(false);
    ui->ConsoleInputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->ConsoleOutputTestFilePath_Programming->setReadOnly(false);
    ui->ConsoleOutputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->NumberofTest_Programming->setReadOnly(false);
    ui->confirmButton_Programming->setEnabled(true);
    ui->RemoveInputPathButton_Programming->setEnabled(true);
    ui->RemoveOutputPathButton_Programming->setEnabled(true);

    ui->CopyMCBt->setEnabled(false);
    ui->CreateActivityButton_MC->setEnabled(true);
    ui->ActivityName_MC->setReadOnly(false);
    ui->DueDate_MC->setReadOnly(false);
    ui->DueTime_MC->setReadOnly(false);
    ui->StudentSubmissionPath_MC->setReadOnly(false);
    ui->StudentSubmissionPath_MCButton->setEnabled(true);
    ui->LanguageComboBox_MC->setEnabled(true);

    ui->CopyEssayBt->setEnabled(false);
    ui->CreateActivityButton_Essay->setEnabled(true);
    ui->ActivityName_Essay->setReadOnly(false);
    ui->DueDate_Essay->setReadOnly(false);
    ui->DueTime_Essay->setReadOnly(false);
    ui->SolutionPath_Essay->setReadOnly(false);
    ui->SolutionPath_EssayButton->setEnabled(true);
    ui->StudentSubmissionPath_Essay->setReadOnly(false);
    ui->StudentSubmissionPath_EssayButton->setEnabled(true);
    ui->LanguageComboBox_Essay->setEnabled(true);

    int currentCourse = ui->CourseList_SelectModifyPage->currentRow();
    int currentList = ui->ActivityList_SelectModifyPage->currentRow();
    if (currentCourse != -1 && currentList != -1)
    {
        int currentRow = ui->ActivityList_SelectModifyPage->currentRow();
        if ((ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == ACTIVITY_TYPE_ESSAY_PROBLEM_SET) ||
            (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == ACTIVITY_TYPE_ESSAY) ||
            (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == ACTIVITY_TYPE_PROBLEM_SET))

        {
            ui->SaveChangesButton_Essay->setVisible(true);
            ui->CreateActivityButton_Essay->setVisible(false);
            ui->CopyEssayBt->setVisible(false);
            displayPage(CREATE_ESSAY_ACTIVITY_INDEX, true);
            //Calling Constructor for Activity
            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);
            (void)viewEssayActivity();
        }
        else if (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == "MC"){
            ui->SaveChangesButton_MC->setVisible(true);
            ui->CreateActivityButton_MC->setVisible(false);
            ui->CopyMCBt->setVisible(false);
            displayPage(CREATE_MC_ACTIVITY_INDEX, true);
            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);
            (void)viewMCActivity();
        }
        else if (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text()  == "Programming"){
            ui->SaveChangesButton_Programming->setVisible(true);
            ui->CreateActivityButton_Programming->setVisible(false);
            ui->CopyProgrammingActivityBt->setVisible(false);
            displayPage(CREATE_PROGRAMMING_ACTIVITY_INDEX, true);
            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);
            (void)viewProgrammingActivity();
        }
        ui->PageTitle->setText("<font size=16 color=gray><b>View/Modify Activity</b></font>");
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please select an activity!");
    }
}

/* user click "Delete" on view modify Activity page */
void StreamlinedGradingSystem::on_DeleteButton_SelectModifyPage_clicked()
{
    int currRowCourse = ui->CourseList_SelectModifyPage->currentRow();
    int currRowAct = ui->ActivityList_SelectModifyPage->currentRow();
    if (currRowCourse < 0 || currRowAct < 0)
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please select an activity!");
        return;
    }

    //display message box
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deleting an activity!", "Are you sure you want to delete " + ui->ActivityList_SelectModifyPage->item(currRowAct,0)->text() + " ?\n" + "All associated grades (and rubric items) will be deleted!",
                                  QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == reply)
        qDebug() << delAct( tempActivityIDList[currRowAct] );

}

/* user click "Copy" on view modify Activity page */
void StreamlinedGradingSystem::on_CopyActivityBt_SelModPage_clicked()
{
    /*====================================================STILL NEED SOME WORK================================================*/
    int currentCourse = ui->CourseList_SelectModifyPage->currentRow();
    int currentList = ui->ActivityList_SelectModifyPage->currentRow();
    ui->rubricTable->setEnabled(false);
    ui->AddCriteriaButton->setEnabled(false);
    ui->RemoveCriteriaButton->setEnabled(false);
    if (currentCourse != -1 && currentList != -1)
    {
        int currentRow = ui->ActivityList_SelectModifyPage->currentRow();
        if ((ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == ACTIVITY_TYPE_ESSAY_PROBLEM_SET) ||
            (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == ACTIVITY_TYPE_ESSAY) ||
            (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == ACTIVITY_TYPE_PROBLEM_SET))

        {
            ui->SaveChangesButton_Essay->setVisible(false);
            ui->CreateActivityButton_Essay->setVisible(false);
            //ui->CreateActForCopy_Essay->setVisible(true);
            ui->CopyEssayBt->setVisible(true);
            ui->CopyEssayBt->setEnabled(true);
            ui->CreateActivityButton_Essay->setVisible(true);
            ui->CreateActivityButton_Essay->setEnabled(false);
            //ui->CreateActForCopy_Essay->setEnabled(false);
            ui->ActivityName_Essay->setReadOnly(true);
            ui->DueDate_Essay->setReadOnly(true);
            ui->DueTime_Essay->setReadOnly(true);
            ui->SolutionPath_Essay->setReadOnly(true);
            ui->SolutionPath_EssayButton->setEnabled(false);
            ui->StudentSubmissionPath_Essay->setReadOnly(true);
            ui->StudentSubmissionPath_EssayButton->setEnabled(false);
            ui->LanguageComboBox_Essay->setEnabled(false);

            displayPage(CREATE_ESSAY_ACTIVITY_INDEX, true);
            //Calling Constructor for Activity
            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);
            (void)viewEssayActivity();
        }
        else if (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text() == "MC"){
            ui->SaveChangesButton_MC->setVisible(false);
            ui->CreateActivityButton_MC->setVisible(true);
            ui->CopyMCBt->setVisible(true);
            ui->CopyMCBt->setEnabled(true);
            ui->CreateActivityButton_MC->setEnabled(false);
            ui->ActivityName_MC->setReadOnly(true);
            ui->DueDate_MC->setReadOnly(true);
            ui->DueTime_MC->setReadOnly(true);
            //ui->SolutionPath_MC->setReadOnly(true);
            ui->StudentSubmissionPath_MC->setReadOnly(true);
            ui->StudentSubmissionPath_MCButton->setEnabled(false);
            ui->LanguageComboBox_MC->setEnabled(false);

            displayPage(CREATE_MC_ACTIVITY_INDEX, true);
            (void)viewMCActivity();
        }
        else if (ui->ActivityList_SelectModifyPage->item(currentRow,1)->text()  == "Programming"){
            ui->SaveChangesButton_Programming->setVisible(false);
            ui->CreateActivityButton_Programming->setVisible(true);
            ui->CopyProgrammingActivityBt->setVisible(true);
            ui->CopyProgrammingActivityBt->setEnabled(true);
            ui->CreateActivityButton_Programming->setEnabled(false);
            ui->ActivityName_Programming->setReadOnly(true);
            ui->DueDate_Programming->setReadOnly(true);
            ui->DueTime_Programming->setReadOnly(true);
            ui->SolutionPath_Programming->setReadOnly(true);
            ui->SolutionPath_ProgrammingButton->setEnabled(false);
            ui->StudentSubmissionPath_Programming->setReadOnly(true);
            ui->StudentSubmissionPath_ProgrammingButton->setEnabled(false);
            ui->LanguageComboBox_Programming->setEnabled(false);
            ui->Inputpathlist_Programming->setEnabled(false);
            ui->InputTestFilePath_Programming->setReadOnly(true);
            ui->InputTestFilePath_ProgrammingButton->setEnabled(false);
            ui->addInputPathButton_Programming->setEnabled(false);
            ui->Outputpathlist_Programming->setEnabled(false);
            ui->OutputTestFilePath_Programming->setReadOnly(true);
            ui->OutputTestFilePath_ProgrammingButton->setEnabled(false);
            ui->addOutputPathButton_Programming->setEnabled(false);
            ui->EnviromentComboBox_Programming->setEnabled(false);
            ui->ConsoleInputTestFilePath_Programming->setReadOnly(true);
            ui->ConsoleInputTestFilePath_ProgrammingButton->setEnabled(false);
            ui->ConsoleOutputTestFilePath_Programming->setReadOnly(true);
            ui->ConsoleOutputTestFilePath_ProgrammingButton->setEnabled(false);
            ui->NumberofTest_Programming->setReadOnly(true);
            ui->confirmButton_Programming->setEnabled(false);
            ui->RemoveInputPathButton_Programming->setEnabled(false);
            ui->RemoveOutputPathButton_Programming->setEnabled(false);

            displayPage(CREATE_PROGRAMMING_ACTIVITY_INDEX, true);
            int ActivityrowNumber = ui->ActivityList_SelectModifyPage->currentRow();
            QString activityName = ui->ActivityList_SelectModifyPage->item(ActivityrowNumber,0)->text();
            currentActivity = Activity(tempCourseIDList[currentCourse], activityName);
            qDebug() << "currentCourse" << currentCourse;
            qDebug() << "activityName" << activityName;
            (void)viewProgrammingActivity();
        }
        ui->PageTitle->setText("<font size=16 color=gray><b>Create Activity By Copying</b></font>");
    }
    else
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please select an activity!");
    }





    /*====================================================STILL NEED SOME WORK================================================*/
}

/*================================================================================================================================================================================================================================================*/

/* user click "Save" on Essay Activity Page */
void StreamlinedGradingSystem::on_SaveChangesButton_Essay_clicked()
{
    clearLogMessage();
    if (QMessageBox::Yes == QMessageBox::question(this, "Save changes to Activity?",
              "Permanently save changes to the selected activity?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if(modifyEssayActivityChange()){
    //        pageStack.pop();
    //        pageStack.pop();
    //        displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, false);
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Successfully Modify Essay/Problem Set Activity");
        }

    }

}

/* user click "Save" on MC Activity Page */
void StreamlinedGradingSystem::on_SaveChangesButton_MC_clicked()
{
    clearLogMessage();
    if (QMessageBox::Yes == QMessageBox::question(this, "Save changes to Activity?",
              "Permanently save changes to the selected activity?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if(modifyMCActivityChange()){
    //        pageStack.pop();
    //        pageStack.pop();
    //        displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, false);
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Successfully Modify Multiple Choice Activity");
        }
    }
}

/* user click "Save" on Programming Activity Page */
void StreamlinedGradingSystem::on_SaveChangesButton_Programming_clicked()
{
    clearLogMessage();
    if (QMessageBox::Yes == QMessageBox::question(this, "Save changes to Activity?",
              "Permanently save changes to the selected activity?",
              QMessageBox::Yes|QMessageBox::No))
    {
        storecurrentTestCase();
        if(modifyProgrammingActivityChange()){
            pageStack.pop();
            pageStack.pop();
            displayPage(INSTRUCTOR_MAIN_PAGE_INDEX, false);
            printLogMessage(CONFIRM_MESSAGE_TEXT,"Successfully Modify Programming Activity");
    }
    }
}

/*
 *  User clicks on "Modify Rubric" on the Create Essay Page
 */
void StreamlinedGradingSystem::on_ModifyRubic_EssayButton_clicked()
{
    displayPage(CREATE_RUBRIC_PAGE_INDEX, true);
    // clear rubricTable and load rubric info from current activity;
    while (0 < ui->rubricTable->rowCount())
    {
        qDebug() << "row count = " << ui->rubricTable->rowCount();
        ui->rubricTable->removeRow(0);
    }
    loadRubric(currentActivity);
}

/*
 *  User clicks on "Modify Rubric" on the Create Programming Page
 */
void StreamlinedGradingSystem::on_ModifyRubic_ProgrammingButton_clicked()
{
    displayPage(CREATE_RUBRIC_PAGE_INDEX, true);
    // clear rubricTable and load rubric info from current activity;
    while (0 < ui->rubricTable->rowCount())
    {
        qDebug() << "row count = " << ui->rubricTable->rowCount();
        ui->rubricTable->removeRow(0);
    }
    loadRubric(currentActivity);
}

/* User click on "Answer Key" button on create/modify MC activity page */
void StreamlinedGradingSystem::on_MCAnswerKeyBt_clicked()
{
  displayPage(CREATE_RUBRIC_PAGE_INDEX, true);
  ui->PageTitle->setText("<font size=16 color=gray><b>Multiple Choice Answer Key Page</b></font>");
  /* clear rubricTable and load rubric info from current activity;
   * We will user the rubric as answer key */
  while (0 < ui->rubricTable->rowCount())
  {
      qDebug() << "row count = " << ui->rubricTable->rowCount();
      ui->rubricTable->removeRow(0);
  }
  loadRubric(currentActivity);
  // set answer key table header
  QStringList tableHeader;
  tableHeader << "Question number" << "Answer" << "Grade";
  ui->rubricTable->setHorizontalHeaderLabels(tableHeader);
}

/* =====================================================================
 *              Function group: Main Marking Page
 * =====================================================================*/

/* user select a course on course list */
void StreamlinedGradingSystem::on_CourseList_Marker_itemClicked(QListWidgetItem *item)
{
    clearLogMessage();
    //clear sudent list
    ui->studentList_Marking->clear();
    while (ui->studentList_Marking->rowCount() > 0)
    {
        ui->studentList_Marking->removeRow(0);
    }
    (void)viewOwnCourseActivity();
    //set table header
    QStringList tableHeader;
    tableHeader << "Student #" << "Points" << "Submmited on" << "Marked by" << "Marker Role";
    ui->studentList_Marking->setHorizontalHeaderLabels(tableHeader);
    ui->studentList_Marking->verticalHeader()->hide();
}

void StreamlinedGradingSystem::on_ActivityList_Marker_itemClicked(QTableWidgetItem *item)
{
    clearLogMessage();
    qDebug() << "open student list";
    viewStudentList_marking();
    //set table header
    QStringList tableHeader;
    tableHeader << "Student #" << "Points" << "Submmited on" << "Marked by" << "Marker Role";
    ui->studentList_Marking->setHorizontalHeaderLabels(tableHeader);
    ui->studentList_Marking->verticalHeader()->hide();
}

void StreamlinedGradingSystem::on_GradeButton_clicked()
{
    clearLogMessage();

    //check if any submission is selected
    if( (true == ui->CourseList_Marker->selectedItems().isEmpty()) ||
        (true == ui->ActivityList_Marker->selectedRanges().isEmpty()) ||
        (true == ui->studentList_Marking->selectedRanges().isEmpty()) )
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please select a student/group submission!");
        return;
    }

    bool canGrade = false;
    /* If current user has priviledge over the previous grader, then he can re-grade.
     * If current student has not been graded, then anyone can grade */
    int selectedRow = ui->studentList_Marking->selectedRanges().first().topRow();
    // not graded
    if ((ui->studentList_Marking->item(selectedRow,STUDENT_LIST_MARKING_COL_MARKEDAS) == NULL))
    {
      qDebug() << "Not graded yet. NULL";
      canGrade = true;
    }
    else if (ui->studentList_Marking->item(selectedRow,STUDENT_LIST_MARKING_COL_MARKEDAS)->text() == STUDENT_NOT_GRADED)
    {
      qDebug() << "Not graded yet";
      canGrade = true;
    }
    //already graded
    else
    {
      //check role of current user against previous grader
      if (currentUserRole == ADMINISTRATOR_ROLE_TEXT)
      {
        qDebug() << "user is admin, can override grade";
        canGrade = true;
      }
      else if (currentUserRole == INSTRUCTOR_ROLE_TEXT)
      {
        qDebug() << "user is instructor, can override grade";
        canGrade = true;
      }
      else if ((currentUserRole == TA_ROLE_TEXT) && (ui->studentList_Marking->item(selectedRow,STUDENT_LIST_MARKING_COL_MARKEDAS)->text() == TA_ROLE_TEXT) )
      {
        qDebug() << "user is TA, can override grade by TA";
        canGrade = true;
      }
      else
      {
        qDebug() << "user does not have priviledge to override grade";
        canGrade = false;
      }
    }

    if (false == canGrade)
    {
      printLogMessage(ERROR_MESSAGE_TEXT, "You cannot regrade for selected student.<br>Current role selected does not have the priviledge");
      return;
    }

    if ((currentActivity.getActivityType() == ACTIVITY_TYPE_ESSAY) ||
        (currentActivity.getActivityType() == ACTIVITY_TYPE_PROBLEM_SET) ||
        (currentActivity.getActivityType() == ACTIVITY_TYPE_ESSAY_PROBLEM_SET))
    {
        qDebug() << "Displaying grade table for selected student";
        (void)viewSelectedSubmission_Essay_ProblemSet();
        displayPage(MARK_ESSAY_INDEX, true);
        QStringList tableHeader;
        tableHeader << "Item Number" << "Item Description" << "Max Grade" << "Student Grade";
        ui->StudentGradeTable->setHorizontalHeaderLabels(tableHeader);
        ui->StudentGradeTable->verticalHeader()->hide();
    }
    else if (currentActivity.getActivityType() == ACTIVITY_TYPE_MC)
    {
        printLogMessage(CONFIRM_MESSAGE_TEXT,"Not Implemented for MC activity");
    }
    else if (currentActivity.getActivityType() == ACTIVITY_TYPE_PROGRAMMING)
    {        
        if (true == viewSelectedSubmission_Programming())
        {
            displayPage(MARK_PROGRAMMING_INDEX, true);
        }
    }
}

/* User click export grade on main marking page */
void StreamlinedGradingSystem::on_ExportGradeButton_pressed()
{
    clearLogMessage();
    exportGrades();
}

/* =====================================================================
 *              Function group: Grading per student page
 * =====================================================================*/

/* user click display student file */
void StreamlinedGradingSystem::on_DisplayStudentFile_essay_Bt_clicked()
{
    (void)viewStudentFile();
}

/* user click display solution file */
void StreamlinedGradingSystem::on_DisplaySolutionFile_essay_Bt_clicked()
{
    QString filePath = currentActivity.getPathToSolution();
    if ((NULL == filePath)||(""==filePath))
    {
        printLogMessage(ERROR_MESSAGE_TEXT, "Could not get path to solution file");
    }
    else
    {
        QDesktopServices::openUrl(QUrl("file:" + filePath + ""));
    }
}

/* user click "save grade" on essay marking page */
void StreamlinedGradingSystem::on_SaveGradeChangesButton_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Save Grade for this student?",
              "Permanently save grades for the selected student?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if (saveStudentGrade_Essay())
        {
          displayPage(MARK_MAIN_PAGE_INDEX, false, false);
        }
    }

}

/* marker click view student file button on programming marking page */
void StreamlinedGradingSystem::on_ViewStdFileProgramming_Bt_clicked()
{
    clearLogMessage();
    QString path;
    QString selectedFile;
    QString studentNum;
    path = currentActivity.getPathToStdSubmission();
    studentNum = ui->studentList_Marking->item(ui->studentList_Marking->currentRow(),STUDENT_LIST_MARKING_COL_STD_NUM)->text();
    path = path + "/" + studentNum;
    //get selected file path
    selectedFile = ui->StudentFilePrograming_comboBox->currentText();
    path = path + "/" + selectedFile;
    if (!QFile(path).exists())
    {
        qDebug() << path;
        printLogMessage(ERROR_MESSAGE_TEXT, "selected file path does not exist");
        return;
    }
    else
    {
        qDebug() << path;
        //open file... where?
        QFile file(path);

        if(!file.open(QIODevice::ReadOnly))
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "Could not open selected file");
        }
        QTextStream in(&file);

        ui->StudentFileBrowser_Programming->setText(in.readAll());
    }
}


void StreamlinedGradingSystem::on_viewSolFileProgramming_Bt_clicked()
{
    clearLogMessage();
    QString path;
    QString selectedFile;
    path = currentActivity.getPathToSolution();
    //get selected file path
    selectedFile = ui->SolutionFileProgramming_ComboBox->currentText();
    path = path + "/" + selectedFile;
    if (!QFile(path).exists())
    {
        qDebug() << path;
        printLogMessage(ERROR_MESSAGE_TEXT, "selected file path does not exist");
        return;
    }
    else
    {
        //open file... where?
        QFile file(path);

        if(!file.open(QIODevice::ReadOnly))
        {
            printLogMessage(ERROR_MESSAGE_TEXT, "Could not open selected file");
        }
        QTextStream in(&file);

        ui->SolutionFileBrowser_Programming->setText(in.readAll());
    }
}


void StreamlinedGradingSystem::on_SaveCommentProgramming_Bt_clicked()
{
    clearLogMessage();
    QString path;
    QString selectedFile;
    QString studentNum;
    path = currentActivity.getPathToStdSubmission();
    studentNum = ui->studentList_Marking->item(ui->studentList_Marking->currentRow(),STUDENT_LIST_MARKING_COL_STD_NUM)->text();
    path = path + "/" + studentNum;


    //REFERENCE: http://www.qtcentre.org/threads/49670-how-to-use-QtextEdit-to-open-amp-read-text-files

    QString nomeFile = path + "/" + studentNum + COMMENT_FILE_NAME_TAIL;
    if (nomeFile != "")
    {
        QFile file(nomeFile);


        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << ui->CommentProgramming->toPlainText();
            file.flush();
            file.close();
            printLogMessage(CONFIRM_MESSAGE_TEXT, "Comments saved");
        }
        else {
            printLogMessage(ERROR_MESSAGE_TEXT, "Could not save comments");
            return;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
}


void StreamlinedGradingSystem::on_SaveStudentGrade_Programming_Bt_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Save Grade for this student?",
              "Permanently save grades for the selected student?",
              QMessageBox::Yes|QMessageBox::No))
    {
        if (saveStudentGrade_Programming())
        {
            displayPage(pageStack.pop(),false, false);
        }
    }
}


/* =====================================================================
 *              Function group: Programming Activity Path List
 * =====================================================================*/
//Set Number of Test Case
void StreamlinedGradingSystem::on_confirmButton_Programming_clicked()
{
    ui->ConsoleInputTestFilePath_Programming->setText("");
    ui->ConsoleOutputTestFilePath_Programming->setText("");
    ui->InputTestFilePath_Programming->clear();
    ui->OutputTestFilePath_Programming->clear();
    ui->Inputpathlist_Programming->clear();
    ui->Outputpathlist_Programming->clear();
    QString number_of_test;
    int int_number_of_test;
    consoleInputPath_TestCase.clear();
    consoleOutputPath_TestCase.clear();
    testCaseList.clear();
    number_of_test=ui->NumberofTest_Programming->text();
    qDebug()<<"number_of_test"<<number_of_test;
    QRegExp re("\\d*");
    if( !re.exactMatch(ui->NumberofTest_Programming->text()))
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Number of test must be integer");
    }
    else if( ui->NumberofTest_Programming->text() =="")
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please Input the Number of Test Case and press 'Confirm'");
    }
    else
    {
        clearLogMessage();
        ui->TestCasecomboBox_Programming->clear();
        int_number_of_test = number_of_test.toInt();
//        consoleInputPath_TestCase = new QString[int_number_of_test];
//        consoleOutputPath_TestCase = new QString[int_number_of_test];
        //Input_Output_Table
        //comboBox_number_of_test
//        testCaseList = new TestCase[int_number_of_test];
        TestCase testcase;
        QString path;
        for (int i=1; i<int_number_of_test+1; i++)
        {
            ui->TestCasecomboBox_Programming->addItem("Test Case " + QString::number(i));
            testCaseList.append(testcase);
            //testCaseItemList.append(testCaseList[i]);
            consoleInputPath_TestCase.append(path);
            consoleOutputPath_TestCase.append(path);
        }
    }
}


//add input path to list
void StreamlinedGradingSystem::on_addInputPathButton_Programming_clicked()
{
    QString path;
    path = ui->InputTestFilePath_Programming->text();
    if ((path != "//")&&!QFile(path).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT,"The input file path does not exists");
    }
    else if(path !="//" && path !=""){
        ui->Inputpathlist_Programming->addItem(path);
        printLogMessage(CONFIRM_MESSAGE_TEXT,"input path added successfully");
    }
    else if(path =="//")
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please enter/browse the input file path");
    }
    else if(path =="")
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Input path cannot be empty");
    }
    else
    {      printLogMessage(ERROR_MESSAGE_TEXT,"Input path failed to add");
    }
    ui->InputTestFilePath_Programming->clear();
}

//add output path to list
void StreamlinedGradingSystem::on_addOutputPathButton_Programming_clicked()
{
    QString path;
    path = ui->OutputTestFilePath_Programming->text();
    if ((path != "//")&&!QFile(path).exists())
    {
      printLogMessage(ERROR_MESSAGE_TEXT, "The Output File Path does not exists");

    }
    else if(path !="//" && path !=""){
        ui->Outputpathlist_Programming->addItem(path);
        printLogMessage(CONFIRM_MESSAGE_TEXT,"Ouptput path added successfully");
    }
    else if(path =="//")
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Please enter/browse the output file path");
    }
    else if(path =="")
    {
        printLogMessage(ERROR_MESSAGE_TEXT,"Output path cannot be empty");
    }
    else
    {      printLogMessage(ERROR_MESSAGE_TEXT,"Output path failed to add");
    }
    ui->OutputTestFilePath_Programming->clear();
}

//remove input path from list
void StreamlinedGradingSystem::on_RemoveInputPathButton_Programming_clicked()
{
    int rowcount;
    rowcount = ui->Inputpathlist_Programming->currentRow();
    ui->Inputpathlist_Programming->takeItem(rowcount);
}

//remove output path from list
void StreamlinedGradingSystem::on_RemoveOutputPathButton_Programming_clicked()
{
    int rowcount;
    rowcount = ui->Outputpathlist_Programming->currentRow();
    qDebug() << rowcount << "output path list row count";
    ui->Outputpathlist_Programming->takeItem(rowcount);
}

//check user choose the item on the list and reload the content of the test case
void StreamlinedGradingSystem::on_TestCasecomboBox_Programming_activated(int index)
{

    int currentIndex = ui->TestCasecomboBox_Programming->currentIndex();
    //check test case exist
    if (ui->TestCasecomboBox_Programming->itemText(currentIndex) != "Select Test Case Here")
    {
        //reset the page
        ui->ConsoleInputTestFilePath_Programming->setText("");
        ui->ConsoleOutputTestFilePath_Programming->setText("");
        ui->InputTestFilePath_Programming->clear();
        ui->OutputTestFilePath_Programming->clear();
        ui->Inputpathlist_Programming->clear();
        ui->Outputpathlist_Programming->clear();
        QString currentTestConsoleInputPath = consoleInputPath_TestCase[currentIndex];
        QString currentTestConsoleOutputPath = consoleOutputPath_TestCase[currentIndex];
        ui->ConsoleInputTestFilePath_Programming->setText(currentTestConsoleInputPath);
        ui->ConsoleOutputTestFilePath_Programming->setText(currentTestConsoleOutputPath);
        QVector<testCaseItem_t> list;
        testCaseList[currentIndex].getTestCaseItemList(list);
        //refresh the input output list for the test case
        for (int i=0; i<list.size(); i++)
        {
            if (list[i].FileType == FILE_INPUT_TYPE)
            {

                ui->Inputpathlist_Programming->insertItem(i,list[i].Path);
            }
            if (list[i].FileType == FILE_OUTPUT_TYPE)
            {

                ui->Outputpathlist_Programming->insertItem(i,list[i].Path);
            }
        }
    }
}
//check user touch the test case combo box
void StreamlinedGradingSystem::on_TestCasecomboBox_Programming_highlighted(int index)
{
    (void)index;
    qDebug() << "highlighted";
    storecurrentTestCase();
    qDebug() << "highlighted Pass";
}



void StreamlinedGradingSystem::on_setBouns_EssayButton_clicked()
{
//    //reset the table info when rentering into bonus page
//    ui->BonusPenaltyTable->removeColumn(2);
//    ui->BonusPenaltyTable->removeColumn(1);
//    ui->BonusPenaltyTable->removeColumn(0);
//    ui->BonusPenaltyTable->removeRow(0);
//    ui->BonusPenaltyTable->clear();

//    displayPage(BONUS_PENALTY_PAGE,true);

//    QStringList tableHeader;
//    ui->BonusPenaltyTable->insertColumn(0);
//    ui->BonusPenaltyTable->insertColumn(1);
//    ui->BonusPenaltyTable->insertColumn(1);
//    ui->BonusPenaltyTable->setColumnWidth(1,150);
//    tableHeader << "Duration" << "Bonus/Penalty Per Day" << "Path to CSV";
//    ui->BonusPenaltyTable->setHorizontalHeaderLabels(tableHeader);
//    ui->BonusPenaltyTable->insertRow(0);
    printLogMessage(CONFIRM_MESSAGE_TEXT,"Not implemented");


}

void StreamlinedGradingSystem::on_setBouns_ProgrammingButton_clicked()
{
//    //reset the table info when rentering into bonus page
//    ui->BonusPenaltyTable->removeColumn(2);
//    ui->BonusPenaltyTable->removeColumn(1);
//    ui->BonusPenaltyTable->removeColumn(0);
//    ui->BonusPenaltyTable->removeRow(0);
//    ui->BonusPenaltyTable->clear();

//    displayPage(BONUS_PENALTY_PAGE,true);

//    QStringList tableHeader;
//    ui->BonusPenaltyTable->insertColumn(0);
//    ui->BonusPenaltyTable->insertColumn(1);
//    ui->BonusPenaltyTable->insertColumn(1);
//    ui->BonusPenaltyTable->setColumnWidth(1,150);
//    tableHeader << "Duration" << "Bonus/Penalty Per Day" << "Path to Boobies";
//    ui->BonusPenaltyTable->setHorizontalHeaderLabels(tableHeader);
//    ui->BonusPenaltyTable->insertRow(0);
    printLogMessage(CONFIRM_MESSAGE_TEXT,"Not implemented");

}




//bool StreamlinedGradingSystem::viewSelectedSubmission_Essay_ProblemSet()
void StreamlinedGradingSystem::on_Save_Bonus_Penalty_Bt_pressed()
{
//    QString Duration = ui->BonusPenaltyTable->item(0,0)->text();
//    QString PerDay = ui->BonusPenaltyTable->item(0,1)->text();
//    QString pathCSV = ui->BonusPenaltyTable->item(0,2)->text();

//    // insert into database
//    qDebug() << Duration;
//    qDebug() << PerDay;
//    qDebug() << pathCSV;
    printLogMessage(CONFIRM_MESSAGE_TEXT,"Not implemented");

}


void StreamlinedGradingSystem::on_CopyProgrammingActivityBt_clicked()
{
    ui->rubricTable->setEnabled(true);
    ui->AddCriteriaButton->setEnabled(true);
    ui->RemoveCriteriaButton->setEnabled(true);
    ui->CopyProgrammingActivityBt->setEnabled(false);
    ui->CreateActivityButton_Programming->setEnabled(true);
    ui->ActivityName_Programming->setReadOnly(false);
    ui->DueDate_Programming->setReadOnly(false);
    ui->DueTime_Programming->setReadOnly(false);
    ui->SolutionPath_Programming->setReadOnly(false);
    ui->SolutionPath_ProgrammingButton->setEnabled(true);
    ui->StudentSubmissionPath_Programming->setReadOnly(false);
    ui->StudentSubmissionPath_ProgrammingButton->setEnabled(true);
    ui->LanguageComboBox_Programming->setEnabled(true);
    ui->Inputpathlist_Programming->setEnabled(true);
    ui->InputTestFilePath_Programming->setReadOnly(false);
    ui->InputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->addInputPathButton_Programming->setEnabled(true);
    ui->Outputpathlist_Programming->setEnabled(true);
    ui->OutputTestFilePath_Programming->setReadOnly(false);
    ui->OutputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->addOutputPathButton_Programming->setEnabled(true);
    ui->EnviromentComboBox_Programming->setEnabled(true);
    ui->ConsoleInputTestFilePath_Programming->setReadOnly(false);
    ui->ConsoleInputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->ConsoleOutputTestFilePath_Programming->setReadOnly(false);
    ui->ConsoleOutputTestFilePath_ProgrammingButton->setEnabled(true);
    ui->NumberofTest_Programming->setReadOnly(false);
    ui->confirmButton_Programming->setEnabled(true);
    ui->RemoveInputPathButton_Programming->setEnabled(true);
    ui->RemoveOutputPathButton_Programming->setEnabled(true);
    ui->DueDate_Programming->setDate(QDate::currentDate());
    printLogMessage(CONFIRM_MESSAGE_TEXT,"Modify if you need and press 'create' button to succesfully copy the activity");
}

void StreamlinedGradingSystem::on_CopyMCBt_clicked()
{
    ui->rubricTable->setEnabled(true);
    ui->AddCriteriaButton->setEnabled(true);
    ui->RemoveCriteriaButton->setEnabled(true);
    ui->CopyMCBt->setEnabled(false);
    ui->CreateActivityButton_MC->setEnabled(true);
    ui->ActivityName_MC->setReadOnly(false);
    ui->DueDate_MC->setReadOnly(false);
    ui->DueTime_MC->setReadOnly(false);
    ui->StudentSubmissionPath_MC->setReadOnly(false);
    ui->StudentSubmissionPath_MC->clear();
    ui->StudentSubmissionPath_MCButton->setEnabled(true);
    ui->LanguageComboBox_MC->setEnabled(true);
    ui->DueDate_MC->setDate(QDate::currentDate());
    ui->DueTime_MC->setTime(QTime::currentTime());
    printLogMessage(CONFIRM_MESSAGE_TEXT,"Modify if you need and press 'create' button to succesfully copy the activity");
}

void StreamlinedGradingSystem::on_CopyEssayBt_clicked()
{
    ui->rubricTable->setEnabled(true);
    ui->AddCriteriaButton->setEnabled(true);
    ui->RemoveCriteriaButton->setEnabled(true);
    ui->CopyEssayBt->setEnabled(false);
    ui->CreateActivityButton_Essay->setEnabled(true);
    ui->ActivityName_Essay->setReadOnly(false);
    ui->DueDate_Essay->setReadOnly(false);
    ui->DueTime_Essay->setReadOnly(false);
    ui->SolutionPath_Essay->setReadOnly(false);
    ui->SolutionPath_EssayButton->setEnabled(true);
    ui->StudentSubmissionPath_Essay->clear();
    ui->StudentSubmissionPath_Essay->setReadOnly(false);
    ui->StudentSubmissionPath_EssayButton->setEnabled(true);
    ui->LanguageComboBox_Essay->setEnabled(true);
    ui->DueDate_Essay->setDate(QDate::currentDate());
    printLogMessage(CONFIRM_MESSAGE_TEXT,"Modify if you need and press 'create' button to succesfully copy the activity");
}


void StreamlinedGradingSystem::on_CopyAllActivitybt_SelectModifyPage_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Copy Activity", "Are you sure copy all the activity from this course to current course?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (QMessageBox::Yes == reply)
    {

        int currentCourse = ui->CourseList_SelectModifyPage->currentRow();
        if (currentCourse != -1)
        {
        copyAllActivity();
        }
        else {

                printLogMessage(ERROR_MESSAGE_TEXT,"Please select an course!");
        }
    }
}
