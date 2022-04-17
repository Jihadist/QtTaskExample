#pragma once

#include <QObject>
#include <QtTest>

class TestItemModel : public QObject {
    Q_OBJECT
private slots:
    void appendRow();
    void removeRow();
    void rowCount();
    void checkData();
};
