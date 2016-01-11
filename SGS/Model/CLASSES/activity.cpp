/************************************
 *
 * File Name: activity.cpp
 * Purpose  : This class handles activity model
 *
 * Authors:
 *      cyrusc
 *      jhoi
 *      ftran
 *
 * Revision: 1.0
 *
 ***********************************/

#include "activity.h"
#include "Control/DatabaseAPI/dbmanager.h"

Activity::Activity()
{

}

Activity::Activity(int _activityID)
{
  dbManager db;
  QSqlQueryModel* model = db.getActivityInfo_AID(_activityID);
  if (NULL != model)
  {
    activityID = _activityID;
    actName = model->record(0).value("activityName").toString();
    actType = model->record(0).value("activityType").toString();
    dueDate = model->record(0).value("dueDateTime").toDate();
    dueTime = model->record(0).value("dueDateTime").toTime();
    pathToStdSubm = model->record(0).value("pathToSubmissions").toString();
    pathToSolnFile = model->record(0).value("pathToSolution").toString();
    actLanguage = model->record(0).value("language").toString();
//    bonusDays = model->record(0).value("dueDateTime").toString();
//    penaltyDays = model->record(0).value("dueDateTime").toString();
//    bonusPercentPerDay = model->record(0).value("dueDateTime").toString();
//    penaltyPercentPerDay = model->record(0).value("dueDateTime").toString();
    rubricChanged = false;

    delete model;
    model = db.getRubric(QString::number(activityID));
    if (model != NULL)
    {
      rubricItem_t rubricItem;
      rubric.clearRubric();
      int rowcount = model->rowCount();
      for (int i=0; i<rowcount; i++)
      {
        rubricItem.itemDescription = model->record(i).value("rubricItemText").toString();
        rubricItem.itemNum = model->record(i).value("rubricItemNumber").toInt();
        rubricItem.itemGrade = model->record(i).value("rubricItemValue").toInt();
        rubricItem.itemID = model->record(i).value("rubricItemID").toInt();
        rubric.addRubricItem_withID(rubricItem.itemNum,rubricItem.itemDescription, rubricItem.itemGrade, rubricItem.itemID);
      }
      delete model;
    }
  }
}

bool Activity::getTestCasesProgrammingActivityfromdB()
{
  dbManager db;
  int rowCount;
  QSqlQueryModel* model;
  TestCase TestCase_local;
  model = db.getListOfProgrammingTestInfo(activityID);
  if (NULL != model)
  {
    testCaseItem_t testCaseItem;
    QString consoleInputPath_local;
    QString consoleOutputPath_local;
    model = db.getListOfProgrammingTestInfo(activityID);
    //assume the test cases return is in numerical order;
    if (model != NULL)
    {

//      int numberOfInputFile =0;
//      int numberOfOutputFile =0;
//      int testNumber =0;
//      QString environment;

      rowCount= model->rowCount();
      if (rowCount == 0)
      {
        return false;

      }
      for (int i=0; i<rowCount; i++)
      {


        consoleInputPath_local = model->record(i).value("consoleInputPath").toString();
        consoleOutputPath_local = model->record(i).value("consoleOutputPath").toString();
        TestCase_local.setPrimaryInput(consoleInputPath_local);
        TestCase_local.setPrimaryOutput(consoleOutputPath_local);
        TestCaseList.append(TestCase_local);
      }


      delete model;

      model= db.getAllFilePaths(activityID);
      rowCount = model->rowCount();
      for (int i=0; i<rowCount; i++)
      {
        testCaseItem.TestCaseNumber = model->record(i).value("testNumber").toInt();
        testCaseItem.Path = model->record(i).value("pathToFile").toString();
        testCaseItem.FileType = model->record(i).value("fileTypeID").toInt();
        testCaseItem.TestCaseID = model->record(i).value("testID").toInt();
        //void addTestCaseItem(int TestCaseNumber,QString Path, int FileType);
        //void addTestCaseItem_withID(QString Path, int FileType,int TestCaseID)
        //qDebug() << TestCaseList.size() << " " << testCaseItem.TestCaseNumber;
        TestCaseList[testCaseItem.TestCaseNumber-1].addTestCaseItem_with_item (testCaseItem);
        //TestCaseList.addTestCaseItem_withID(testCaseItem.Path,testCaseItem.TestCaseNumber, testCaseItem.FileType);
      }
      delete model;

      }
    }
}

bool Activity::getTestCaseList(QVector <TestCase> &_testCaseList)
{
  bool rv = false;
  qDebug()<<"Test Case List is being checked if it's empty";
  if (!TestCaseList.isEmpty())
  {
    _testCaseList = TestCaseList;
    rv = true;
    qDebug()<<"Test Case List is not empty";
  }

  return rv;
}

Activity::Activity(QString courseID, QString activityName)
{
  dbManager db;
  QSqlQueryModel* model = db.getActivityInfo(courseID, activityName);
  if (model != NULL)
  {
    activityID = model->record(0).value("activityID").toInt();
  //actName = activityName;
  //actType = model->record(0).value("activityType").toString();
  //dueDate = model->record(0).value("dueDateTime").toString();
  //dueTime = model->record(0).value("dueDateTime").toString();
  //pathToStdSubm = model->record(0).value("dueDateTime").toString();
  //pathToSolnFile = model->record(0).value("dueDateTime").toString();
  //actLanguage = model->record(0).value("dueDateTime").toString();
  //bonusDays = model->record(0).value("dueDateTime").toString();
  //penaltyDays = model->record(0).value("dueDateTime").toString();
  //bonusPercentPerDay = model->record(0).value("dueDateTime").toString();
  //penaltyPercentPerDay = model->record(0).value("dueDateTime").toString();
  //rubric = model->record(0).value("dueDateTime").toString();
    rubricChanged = false;
    delete model;
  }
  model = db.getRubric(QString::number(activityID));
  if (model != NULL)
  {
    //int itemNum;
    //QString itemDescription;
    //int itemGrade;
    rubricItem_t rubricItem;
    rubric.clearRubric();
    int rowcount = model->rowCount();
    for (int i=0; i<rowcount; i++)
    {
      rubricItem.itemDescription = model->record(i).value("rubricItemText").toString();
      rubricItem.itemNum = model->record(i).value("rubricItemNumber").toInt();
      rubricItem.itemGrade = model->record(i).value("rubricItemValue").toInt();
      rubricItem.itemID = model->record(i).value("rubricItemID").toInt();
      rubric.addRubricItem_withID(rubricItem.itemNum,rubricItem.itemDescription, rubricItem.itemGrade, rubricItem.itemID);
    }

  }


}

Activity::~Activity()
{

}

void Activity::setActivityID(int ID)
{
  activityID = ID;
}

void Activity::setActType(QString activityType)
{
  actType = activityType;
}

void Activity::addRubricItem(int rubricItemNum, QString rubricItemDesc, int rubricItemGrade)
{
  rubric.addRubricItem(rubricItemNum, rubricItemDesc, rubricItemGrade);
}

bool Activity::saveRubricToDb()
{
  bool rv;
  //delete all grade related to previous rubic (if exist)
  dbManager db;
  if (true == rubricChanged)
  {
    qDebug() << "deleting grade";
    db.deleteAllGrades(activityID);
    if (actType != ACTIVITY_TYPE_MC)
    {
      qDebug() << "store rubric";
      rv = rubric.storeRubricToDb(activityID);
    }
    else
    {
      qDebug() << "store rubric as MC answer key";
      rv = rubric.storeRubricToDbAsAnswerkey(activityID);
    }
  }

  return rv;
}

bool Activity::isRubricEmpty()
{
  return rubric.isRubricEmpty();
}

void Activity::clearRubric()
{
  rubric.clearRubric();
}

void Activity::setRubricChanged(bool value)
{
    rubricChanged = value;
}

bool Activity::getRubricFromDb(int ActivityID)
{
  /* stub. May not be needed */
  return true;
}

bool Activity::getRuricItemList(QVector <rubricItem_t> &rubricList)
{
  return rubric.getRubricItemList(rubricList);
}

bool Activity::removeRubricfromdB()
{
  dbManager db;
  return db.deleteAllRubricItems(activityID);

}

/* Attribute getter */
QString Activity::getPathToStdSubmission()
{
  return pathToStdSubm;
}

QString Activity::getPathToSolution()
{
  return pathToSolnFile;
}

QString Activity::getActivityType()
{
  return actType;
}

QString Activity::getActivityName()
{
    return actName;
}

QDate Activity::getDueDate()
{
    return dueDate;
}

QTime Activity::getDueTime()
{
    return dueTime;
}

int Activity::getActivityID()
{
  return activityID;
}

/* Check Activity Exist */
int Activity::checkActivityNameexist(QString activityName,int rowNumber,  QSqlQueryModel *sqlModel,QVector<QString> &tempCourseIDList)
{
    int ActivityNameExisiting = 0;
    dbManager db;
    QString selectedCourseID = tempCourseIDList[rowNumber];
    sqlModel = db.getListActivity(selectedCourseID);
    if (NULL == sqlModel)
    {
        return 2;
    }
    else{
        for (int index = 0; index < sqlModel->rowCount(); index ++)
        {
            //activity list on select activity to modify page
            if(sqlModel->record(index).value("activityName").toString()== activityName)
            {
                ActivityNameExisiting = 1;
            }

        }
    }
    delete sqlModel;
    return ActivityNameExisiting;
}

bool Activity::deleteTestCasesFromDb(int activityID)
{
  bool result = true;
  dbManager db;
  // wipe current rubric items for this activityID
  result = db.deleteAllTestFiles(activityID);;

  return result;
}
