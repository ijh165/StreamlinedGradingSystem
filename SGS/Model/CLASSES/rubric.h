/************************************
 *
 * File Name: rubric.h
 * Purpose  : This class handles rubric items for activities
 *
 * Authors:
 *      ftran
 *
 * Revision: 1.0
 *
 ***********************************/

#ifndef RUBRIC_H
#define RUBRIC_H

#include <QVector>
#include <QString>

typedef struct
{
  int itemNum;
  QString itemDescription;
  int itemGrade;
  int itemID;
}rubricItem_t;

class Rubric
{
public:
    Rubric();
    ~Rubric();
    bool isRubricEmpty();
    int getRubricLength();
    void clearRubric();
    void addRubricItem(int ItemNum, QString ItemDescription, int ItemGrade);
    void addRubricItem_withID(int ItemNum, QString ItemDescription, int ItemGrade, int ItemID);
    void modifyRubricItem(int index, QString ItemDescription, int ItemGrade);
    void removeRubricItem(int index);
    bool getRubricItemList(QVector <rubricItem_t> &rubricList);
    bool storeRubricToDb(int activityID);
    bool storeRubricToDbAsAnswerkey(int activityID);

private:
    QVector<rubricItem_t> rubricItemList;
};

#endif // RUBRIC_H
