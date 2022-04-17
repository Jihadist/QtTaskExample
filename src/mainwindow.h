#pragma once

#include <QMainWindow>
#include <QModelIndex>

class ItemModel;
class QSortFilterProxyModel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

    void on_sortAcsButton_clicked();
    void on_sortDcsButton_clicked();

    void on_filterLineEdit_textChanged(const QString& text);
    void on_resetButton_clicked();

    void on_buttonRemoveOrigin_clicked();
    void on_buttonRemoveSorted_clicked();
    void on_buttonRemoveFiltered_clicked();

    void on_listView_clicked(const QModelIndex& index);
    void on_listViewSorted_clicked(const QModelIndex& index);
    void on_listViewFiltered_clicked(const QModelIndex& index);

private:
    Ui::MainWindow* ui;
    ItemModel* m_model;
    QSortFilterProxyModel *m_sortModel, *m_filterModel;

    QModelIndex m_currentIndex;
};
