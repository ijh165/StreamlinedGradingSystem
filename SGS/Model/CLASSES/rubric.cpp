/************************************
 *
 * File Name: rubric.cpp
 * Purpose  : This class handles rubric items for activities
 *
 * Authors:
 *      ftran
 *
 * Revision: 1.0
 *
 ***********************************/

#include "rubric.h"
#include <QDebug>

Rubric::Rubric()
{

}

Rubric::~Rubric()
{

}

bool Rubric::isRubricEmpty()
{
  return rubricItemList.isEmpty();
}

int Rubric::getRubricLength()
{
  return rubricItemList.size();
}

void Rubric::clearRubric()
{
  rubricItemList.clear();;
}

void Rubric::addRubricItem(int ItemNum, QString ItemDescription, int ItemGrade)
{
  rubricItem_t item;
  item.itemNum = ItemNum;
  item.itemDescription = ItemDescription;
  item.itemGrade = ItemGrade;
  rubricItemList.append(item);
}

void Rubric::addRubricItem_withID(int ItemNum, QString ItemDescription, int ItemGrade, int ItemID)
{
  rubricItem_t item;
  item.itemNum = ItemNum;
  item.itemDescription = ItemDescription;
  item.itemGrade = ItemGrade;
  item.itemID = ItemID;
  rubricItemList.append(item);
}

void Rubric::modifyRubricItem(int index, QString ItemDescription, int ItemGrade)
{
  rubricItemList[index].itemDescription = ItemDescription;
  rubricItemList[index].itemGrade = ItemGrade;
}

void Rubric::removeRubricItem(int index)
{
  rubricItemList.remove(index);
}

bool Rubric::getRubricItemList(QVector <rubricItem_t> &rubricList)
{
  bool rv = false;
  if (!isRubricEmpty())
  {
    rubricList = rubricItemList;
    rv = true;
  }

  return rv;
}

bool Rubric::storeRubricToDb(int activityID)
{
  dbManager db;
  // wipe current rubric items for this activityID
  db.deleteAllRubricItems(activityID);
  /* Store one rubric item at a time */
  int rubricItemCount = rubricItemList.size();
  qDebug() << "rubric list size " << rubricItemCount;
  for (int i = 0; i< rubricItemCount; i++)
  {
    qDebug() << "insert rubric item " << rubricItemList[i].itemNum;
    db.insertRubricItem(activityID, rubricItemList[i].itemNum, rubricItemList[i].itemDescription, rubricItemList[i].itemGrade);
  }
  return true;
}

/* this function will store to answer key table instead of rubric table in database */
bool Rubric::storeRubricToDbAsAnswerkey(int activityID)
{
  dbManager db;
  // wipe current MCQ items (answer key) for this activityID
  db.deleteAllMCQItems(activityID);
  /* Store one rubric item at a time */
  int rubricItemCount = rubricItemList.size();
  qDebug() << "rubric list size " << rubricItemCount;
  for (int i = 0; i< rubricItemCount; i++)
  {
    qDebug() << "insert answer key item " << rubricItemList[i].itemNum;
    db.insertMCQItem(activityID, rubricItemList[i].itemNum, rubricItemList[i].itemDescription, rubricItemList[i].itemGrade);
  }
  return true;
}
