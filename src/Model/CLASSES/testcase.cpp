/************************************
 *
 * File Name: testcase.cpp
 * Purpose  : This class is for storing test case items for programming activity
 *
 * Authors:
 *      phoi
 *      cyrusc
 *
 * Revision: 1.0
 *
 ***********************************/

#include "testcase.h"
#include <QDebug>

TestCase::TestCase()
{

}

TestCase::~TestCase()
{

}
//check isTestCaseEmpty
bool TestCase::isTestCaseEmpty()
{
    return testCaseItemList.isEmpty();
}
//getTestCaseLength
int TestCase::getTestCaseLength()
{
    return testCaseItemList.size();
}

void TestCase::clearTestCase()
{
    testCaseItemList.clear();
}

//addTestCaseItem
void TestCase::addTestCaseItem(int TestCaseNumber,QString Path, int FileType)
{
    //qDebug()<<"===============addTestCaseItem=============";
    testCaseItem_t item;
    item.TestCaseNumber = TestCaseNumber;
    //qDebug()<<"TestCaseNumber" <<TestCaseNumber;
    item.Path = Path;
    //qDebug()<<"Path" <<Path;
    item.FileType = FileType;
    //qDebug()<<"FileType" <<FileType;
    testCaseItemList.append(item);
    //qDebug()<<"Number of Item in the test case list" <<testCaseItemList.size();
    //qDebug()<<"=================================";
}

void TestCase::addTestCaseItem_withID(QString Path, int FileType,int TestCaseID)
{
  testCaseItem_t item;
  item.Path = Path;
  item.FileType = FileType;
  item.TestCaseID = TestCaseID;
  testCaseItemList.append(item);
}

void TestCase::addTestCaseItem_with_item (testCaseItem_t item)
{
  testCaseItemList.append(item);
}

void TestCase::modifyTestCaseItem(int index, QString Path, int FileType)
{
    testCaseItemList[index].Path = Path;
    testCaseItemList[index].FileType = FileType;
}

void TestCase::removeTestCaseItem(int index)
{
    testCaseItemList.remove(index);
}
//getTestCaseItemList
QString TestCase::getPrimaryInput()
{
  return primaryinput;
}

QString TestCase::getPrimaryOutput()
{
  return primaryoutput;
}

void TestCase::setPrimaryInput(QString path)
{
  primaryinput=path;
}

void TestCase::setPrimaryOutput(QString path)
{
  primaryoutput=path;
}

//bool TestCase::getTestCaseList(QVector <TestCase> &_testCaseList)
//{
//  bool rv = false;
//  if (!isTestCaseEmpty())
//  {
//    _testCaseList = TestCaseList;
//    rv = true;
//  }

//  return rv;
//}

QVector<testCaseItem_t> TestCase::getOptionalItems()
{
  return testCaseItemList;
}

bool TestCase::getTestCaseItemList(QVector <testCaseItem_t> &testCaseList)
{
    bool rv = false;
    if (!isTestCaseEmpty())
    {
        testCaseList = testCaseItemList;
        rv = true;
    }

    return rv;
}
//getnumberOfInputPath
int TestCase::getnumberOfInputPath()
{
    int counter=0;
    for (int i=0; i<testCaseItemList.size(); i++)
    {
        if (testCaseItemList[i].FileType == FILE_INPUT_TYPE)
        {
            qDebug() << "Number of Input Files"<<counter;
            counter++;
        }
    }
    return counter;

}
//getnumberOfOutputPath
int TestCase::getnumberOfOutputPath()
{
    int counter=0;
    for (int i=0; i<testCaseItemList.size(); i++)
    {
        if (testCaseItemList[i].FileType == FILE_OUTPUT_TYPE)
        {
            qDebug() << "Number of Output Files"<<counter;
            counter++;
        }
    }
    return counter;

}
//store in database
bool TestCase::storeFilePathtoDb(int activityID,int TestCaseNumber)
{
    dbManager db;
    // wipe current TestCase items for this activityID
    for (int i=0; i<testCaseItemList.size(); i++)
    {
        qDebug() << "File Type"<<testCaseItemList[i].FileType;
        qDebug() << "Path" << testCaseItemList[i].Path;
        db.insertFilePath(activityID, testCaseItemList[i].FileType,TestCaseNumber, testCaseItemList[i].Path);
    }
    //store primary test case files


    return true;
}


