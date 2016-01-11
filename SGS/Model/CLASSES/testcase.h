/************************************
 *
 * File Name: testcase.h
 * Purpose  : This class is for storing test case items for programming activity
 *
 * Authors:
 *      phoi
 *      cyrusc
 *
 * Revision: 1.0
 *
 ***********************************/

#ifndef TESTCASE_H
#define TESTCASE_H


#include <QVector>
#include <QString>

#define FILE_INPUT_TYPE  1
#define FILE_OUTPUT_TYPE  2
#define FILE_CONSOLE_INPUT_TYPE 3
#define FILE_CONSOLE_OUTPUT_TYPE 4

typedef struct
{
  int TestCaseNumber;
  QString Path ;
  int FileType ;
  int TestCaseID;
}testCaseItem_t;

class TestCase
{
public:
    TestCase();
    ~TestCase();
    bool isTestCaseEmpty();
    int getTestCaseLength();
    void clearTestCase();
    void addTestCaseItem(int TestCaseNumber,QString Path, int FileType);
    void addTestCaseItem_withID(QString Path, int FileType,int TestCaseID);
    bool getTestCaseItemList(QVector <testCaseItem_t> &testCaseList);
    QVector<testCaseItem_t> getOptionalItems();
    void addTestCaseItem_with_item(testCaseItem_t item);
    void modifyTestCaseItem(int index, QString Path, int FileType);
    void removeTestCaseItem(int index);

    bool storeFilePathtoDb(int activityID,int TestCaseNumber);
    int getnumberOfInputPath();
    int getnumberOfOutputPath();

    QString getPrimaryInput();
    QString getPrimaryOutput();
    void setPrimaryInput(QString path);
    void setPrimaryOutput(QString path);

private:
    QVector<testCaseItem_t> testCaseItemList ;
    QString primaryinput;
    QString primaryoutput;
};

#endif // TestCase_H
