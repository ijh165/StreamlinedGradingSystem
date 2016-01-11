/************************************
 *
 * File Name: streamlinedgradingsystem.h
 * Purpose  : This the main GUI class
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

#ifndef STREAMLINEDGRADINGSYSTEM_H
#define STREAMLINEDGRADINGSYSTEM_H

#include <QMainWindow>
#include <QtWidgets>
#include <Qstack>
#include <QVector>
#include <QtGui>
#include "Model/CLASSES/account.h"
#include "Model/CLASSES/course.h"
#include "Model/CLASSES/activity.h"
#include "Model/CLASSES/testcase.h"
#include "Control/DatabaseAPI/dbmanager.h"
#include "Model/CLASSES/testcase.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

/*======= general defines and constant definitions ============*/
/* User roles */
#define SYS_ADMIN_ROLE_TEXT       "System Administrator"
#define ADMIN_ASSIST_ROLE_TEXT    "Administrative Assistant"
#define ADMINISTRATOR_ROLE_TEXT   "Administrator"
#define INSTRUCTOR_ROLE_TEXT      "Instructor"
#define TA_ROLE_TEXT              "TA"
#define NO_ROLE_TEXT              ""

/* SGS_page_index: the index in this list should match with the UI stacked widget */
enum SGS_page_index {
  LOGIN_PAGE_INDEX =                0,
  SELECT_ROLE_PAGE_INDEX =          1,
  CHANGE_PASSWORD_PAGE_INDEX =      2,
  SYSTEM_ADMIN_PAGE_INDEX =         3,
  CREATE_ACCOUNT_PAGE_INDEX =       4,
  SELECT_AN_ACCOUNT_PAGE_INDEX=     5,
  ADMIN_ASSIST_PAGE_INDEX =         6,
  CREATE_COURSE_PAGE_INDEX =        7,
  COPY_COURSE_PAGE_INDEX =          8,
  INSTRUCTOR_MAIN_PAGE_INDEX =      9,
  CREATE_ACTIVITY_STEP_1_INDEX =    10,
  CREATE_PROGRAMMING_ACTIVITY_INDEX = 11,
  CREATE_MC_ACTIVITY_INDEX =        12,
  CREATE_ESSAY_ACTIVITY_INDEX =     13,
  SELECT_MODIFY_ACTIVITY_INDEX =    14,
  COPY_ACTIVITY_PAGE_INDEX =        15,
  CREATE_RUBRIC_PAGE_INDEX =        16,
  TA_MAIN_PAGE_INDEX =              17,
  MARK_MAIN_PAGE_INDEX =            18,
  MARK_ESSAY_INDEX =                19,
  MARK_PROGRAMMING_INDEX =          20,
  MODIFY_VIEW_COURSE_INDEX =        21,
  BONUS_PENALTY_PAGE =              22,
};

/*Log message type*/
#define CONFIRM_MESSAGE_TEXT 0
#define ERROR_MESSAGE_TEXT   1

/* rubric table columns.
 * The defined value should match the column number in the rubric table
 */
#define RUBRIC_ITEM_NUMBER  0
#define RUBRIC_ITEM_DESC    1
#define RUBRIC_ITEM_GRADE   2

/* defines for studentlist_marking table */
#define STUDENT_NOT_GRADED  ""
#define STUDENT_LIST_MARKING_COL_STD_NUM  0
#define STUDENT_LIST_MARKING_COL_POINT    1
#define STUDENT_LIST_MARKING_COL_DATE     2
#define STUDENT_LIST_MARKING_COL_MARKEDAS 4   //column for marker role
#define STUDENT_LIST_MARKING_COL_MARKER   3   //column for marker name

/* defines for student grade table on marking page */
#define GRADE_TABLE_COL_RUBRIC_NUM        0
#define GRADE_TABLE_COL_RUBRIC_DESC       1
#define GRADE_TABLE_COL_RUBRIC_MAX_GRADE  2
#define GRADE_TABLE_COL_STUDENT_GRADE     3
#define GRADE_TABLE_NOT_GRADED            "N/A"


/* Other defines */
#define COMMENT_FILE_NAME_TAIL            "_comment.txt"

/* main ui class */
namespace Ui {
  class StreamlinedGradingSystem;
}

class StreamlinedGradingSystem : public QMainWindow
{
  Q_OBJECT

public:
  explicit StreamlinedGradingSystem(QWidget *parent = 0);
  ~StreamlinedGradingSystem();
  // ask user confirmation when closing application. Credit to: http://www.codeprogress.com/cpp/libraries/qt/HandlingQCloseEvent.php#.Vb1eb8BVhBc
  void closeEvent ( QCloseEvent * event )
  {
      event->ignore();
      if (QMessageBox::Yes == QMessageBox::question(this, "Closing Application?",
                "Are you sure you want to exit?",
                QMessageBox::Yes|QMessageBox::No))
      {
        event->accept();
      }
  }

  /* helpler functions to help with ui display */
  bool displayPage(enum SGS_page_index index, bool saveCurrentIndex, bool ClearLogMessage = true);
  bool initializeWindow();
  bool setRoleSelection();

  void printLogMessage(int TEXT_TYPE, QString text);
  void clearLogMessage();

  /* gui_interaction.cpp */
  bool validatePassword(QString username, QString password, bool &isFirstLogin);
  bool changePassword(QString oldPassword, QString newPassword, QString reEnterPassword);
  bool createNewAccount();
  bool createNewCourse();
  bool searchInstructorName();
  bool searchTAName();
  bool removefromthelist();
  bool searchOwnCourse();
  bool viewOwnCourse();
  bool createEssayActivity();
  bool viewEssayActivity();
  bool modifyEssayActivityChange();
  bool createMCActivity();
  bool viewMCActivity();
  bool createProgrammingActivity();
  bool viewProgrammingActivity();
  bool modifyProgrammingActivityChange();
  bool modifyMCActivityChange();
  bool viewOwnCourseActivity();
  bool setMarkerCourseList();

  bool saveActivityRubric();
  void loadRubric(Activity currentActivity);
  void displayAccountList();
  void displayInfoInAccModPg(QString _selectedEID);
  bool updateAccInfoChanges();
  bool blockAcc(QString _selectedUsername);
  bool unblockAcc(QString _selectedUsername);
  bool viewStudentList_marking();
  bool viewSelectedSubmission_Essay_ProblemSet();
  bool viewSelectedSubmission_Programming();
  bool viewStudentFile();
  bool saveStudentGrade_Essay();
  bool saveStudentGrade_Programming();
  void displayCourseList();
  void displayCourseInfo(int currRow);
  bool saveChangesCourseInfo();
  bool delAcc(int currRow);
  bool delCourse(int currRow);
  bool delAct(QString _selActID);
  void storecurrentTestCase();
  void exportGrades();
  void copyAllActivity();
  bool viewEssayActivity(int courseRowNumber,int ActivityrowNumber);
  bool viewProgrammingActivity(int rowNumber,int ActivityrowNumber);
  bool viewMCActivity(int rowNumber,int ActivityrowNumber);


private slots:
  /* function group: general */
  void on_LogOutBt_clicked();
  void on_previousPageBt_clicked();

  /* function group: Login */
  void on_LoginBt_clicked();

  /* function group: Select Role */
  void on_ChangePasswordBt_clicked();
  void on_ProceedBt_clicked();

  /* function group: Change Password */
  void on_CancelBt_clicked();
  void on_SavePasswordChangeBt_clicked();

  /* function group: sys admin main page */
  void on_SACreateAccountBt_clicked();
  void on_SAModifyAccountBt_clicked();

  //void on_blockAccountBt_clicked();

  /* view/modify account page */
  void on_ModifyAccountButton_clicked();
  void on_AccountSaveChangeButton_clicked();
  void on_BlockButton_clicked();
  void on_UnblockButton_clicked();

  void on_CreateCourseButton_clicked();

  void on_CopyPerviouCourseButton_clicked();

  /* instructor main page */
  void on_CreateActivityButton_clicked();
  void on_ActivityNextButton_clicked();

  void on_CreateRubic_ProgrammingButton_clicked();

  /* create rubric page */
  void on_AddCriteriaButton_clicked();
  void on_RemoveCriteriaButton_clicked();
  void on_SaveRubricButton_clicked();

  void on_ModifyActivityButton_clicked();

  void on_CreateAccountButton_clicked();

  void on_CreateCourseButton_CreateCourse_clicked();

  void on_AddInstructorButton_CreateCourse_clicked();

  void on_AddTAButton_CreateCourse_clicked();

  void on_RemoveTAButton_CreateCourse_clicked();

  void on_StudentSubmissionPath_ProgrammingButton_clicked();

  void on_SolutionPath_ProgrammingButton_clicked();

  void on_ConsoleInputTestFilePath_ProgrammingButton_clicked();

  void on_ConsoleOutputTestFilePath_ProgrammingButton_clicked();

  void on_InputTestFilePath_ProgrammingButton_clicked();

  void on_OutputTestFilePath_ProgrammingButton_clicked();

  void on_StudentSubmissionPath_MCButton_clicked();

//  void on_SolutionPath_MCButton_clicked();

  void on_StudentSubmissionPath_EssayButton_clicked();

  void on_SolutionPath_EssayButton_clicked();
  void on_CreateRubic_EssayButton_clicked();

  void on_CreateActivityButton_Programming_clicked();

  void on_CreateActivityButton_Essay_clicked();

  void on_CourseList_SelectModifyPage_itemClicked(QListWidgetItem *item);

  void on_ModifyButton_SelectModifyPage_clicked();

  void on_TAGradeButton_clicked();

  void on_SaveChangesButton_Essay_clicked();

  void on_CreateActivityButton_MC_clicked();

  void on_SaveChangesButton_MC_clicked();

  void on_ModifyRubic_EssayButton_clicked();

  void on_CourseList_Marker_itemClicked(QListWidgetItem *item);

  void on_ActivityList_Marker_itemClicked(QTableWidgetItem *item);


  void on_ViewModifyCourseBt_clicked();

  void on_ViewModifyCourseSelectionBt_clicked();

  void on_ImportFromCSVButton_CreateCourse_clicked();

  void on_SaveChangeCourseButton_CreateCourse_clicked();

  void on_GradeButton_clicked();

  void on_DisplayStudentFile_essay_Bt_clicked();

  void on_DisplaySolutionFile_essay_Bt_clicked();

  void on_SaveGradeChangesButton_clicked();

  void on_SaveChangesButton_Programming_clicked();

  void on_confirmButton_Programming_clicked();

  void on_addOutputPathButton_Programming_clicked();

  void on_addInputPathButton_Programming_clicked();

  void on_RemoveInputPathButton_Programming_clicked();

  void on_RemoveOutputPathButton_Programming_clicked();

  void on_ModifyRubic_ProgrammingButton_clicked();

  void on_GradeActivityButton_clicked();

  void on_DeleteAccountButton_clicked();

  void on_DeleteCourseBt_clicked();

  void on_DeleteButton_SelectModifyPage_clicked();

  void on_TestCasecomboBox_Programming_activated(int index);

  void on_TestCasecomboBox_Programming_highlighted(int index);

  void on_MCAnswerKeyBt_clicked();

  void on_CopyActivityButton_clicked();

  void on_CopyActivityBt_SelModPage_clicked();

  void on_CopyPerviouCourseButton_CreateCourse_clicked();

  void on_ViewCopyCourseBt_clicked();

  void on_CopyCourseBt_clicked();

  void on_ExportGradeButton_pressed();


  void on_setBouns_EssayButton_clicked();

  void on_Save_Bonus_Penalty_Bt_pressed();

  void on_setBouns_ProgrammingButton_clicked();

  void on_CopyProgrammingActivityBt_clicked();

  void on_CopyMCBt_clicked();

  void on_CopyEssayBt_clicked();

  void on_ViewStdFileProgramming_Bt_clicked();

  void on_viewSolFileProgramming_Bt_clicked();

  void on_SaveCommentProgramming_Bt_clicked();

  void on_SaveStudentGrade_Programming_Bt_clicked();

  void on_CopyAllActivitybt_SelectModifyPage_clicked();

private:
  Ui::StreamlinedGradingSystem *ui;
  QStack <enum SGS_page_index> pageStack; // stack to keep track of page history.
  Account currentUser;  /* currently logged in user */
  QString currentUserRole; /* current role selected by the user */
  Account accSelected; /* currently selected account (for system admin modify account purposes) */
  Course currentCourse; /* currently selected course */
  Activity currentActivity; /* currently selected activity */
  QSqlQueryModel * sqlModel;
  QString courseID;
  QString activityID;

  QVector<QString> tempVector; //temporary vector
  QVector<QString> tempCourseIDList;  //temp storage for courseID list (to be used with CourseList ui display)
  QVector<QString> tempActivityIDList; // temp storage for activityID list (to be use with ActivityList ui display)
  QVector<QString> consoleInputPath_TestCase;
  QVector<QString> consoleOutputPath_TestCase;
  QVector<TestCase> testCaseList;
  QVector<TestCase> testCaseList_global;
};

#endif // STREAMLINEDGRADINGSYSTEM_H
