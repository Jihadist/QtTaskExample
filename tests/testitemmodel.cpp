#include "testitemmodel.h"
#include <QModelIndex>
#include <itemmodel.h>

void TestItemModel::appendRow()
{
    ItemModel model;
    model.addValue("test");
    QVERIFY(model.rowCount() == 1);
}

void TestItemModel::removeRow()
{
    ItemModel model;
    model.addValue("test");
    model.removeValue(model.index(0, 0));
    QVERIFY(model.rowCount() == 0);
}

void TestItemModel::rowCount()
{
    ItemModel model;
    QVERIFY(model.rowCount() == 0);
    model.addValue("test");
    model.addValue("test");
    model.addValue("test");
    QVERIFY(model.rowCount() == 3);

    model.removeValue(model.index(0, 0));
    QVERIFY(model.rowCount() == 2);
    model.removeValue(model.index(0, 0));
    QVERIFY(model.rowCount() == 1);
    model.removeValue(model.index(0, 0));
    QVERIFY(model.rowCount() == 0);
}

void TestItemModel::checkData()
{
    ItemModel model;
    auto data = model.data(model.index(0, 0));
    QVERIFY(data.isNull());

    model.addValue("test");
    data = model.data(model.index(0, 0));
    QVERIFY(!data.isNull());

    QCOMPARE(data.toString(), "test");
}

QTEST_MAIN(TestItemModel)
