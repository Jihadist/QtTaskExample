#include "mainwindow.h"
#include <QSortFilterProxyModel>
#include <delegate.h>
#include <itemmodel.h>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new ItemModel(this);
    m_model->setObjectName("Classic model");

    ui->listView->setModel(m_model);
    auto delegate = new Delegate(this);

    delegate->setVerticalSpacing(4);
    delegate->setContentsMargins(4, 4, 4, 4);

    ui->listView->setItemDelegate(delegate);

    m_sortModel = new QSortFilterProxyModel(m_model);
    m_sortModel->setObjectName("Sort model");
    m_sortModel->setSourceModel(m_model);
    ui->listViewSorted->setModel(m_sortModel);

    m_filterModel = new QSortFilterProxyModel(m_sortModel);
    m_filterModel->setObjectName("Filter model");
    m_filterModel->setSourceModel(m_sortModel);
    ui->listViewFiltered->setModel(m_filterModel);

    ui->listViewFiltered->setItemDelegate(delegate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QString text = ui->dataLineEdit->text();
    m_model->addValue({ text });
    ui->dataLineEdit->clear();
}

// remove item

void MainWindow::on_buttonRemoveOrigin_clicked()
{
    auto index = ui->listView->currentIndex();
    m_model->removeValue(index);
}

void MainWindow::on_buttonRemoveSorted_clicked()
{
    auto index = ui->listViewSorted->currentIndex();
    m_model->removeValue(index);
}

void MainWindow::on_buttonRemoveFiltered_clicked()
{
    auto index = ui->listViewFiltered->currentIndex();
    m_model->removeValue(index);
}

// sort view

void MainWindow::on_sortAcsButton_clicked()
{
    m_sortModel->sort(0);
}

void MainWindow::on_sortDcsButton_clicked()
{
    m_sortModel->sort(0, Qt::SortOrder::DescendingOrder);
}

// filter view

void MainWindow::on_filterLineEdit_textChanged(const QString& text)
{
    m_filterModel->setFilterFixedString(text);

    ui->listViewFiltered->selectionModel()->clearSelection();
    auto newIndex = m_filterModel->mapFromSource(m_sortModel->mapFromSource(m_currentIndex));
    ui->listViewFiltered->selectionModel()->select(newIndex, QItemSelectionModel::Select);
}

void MainWindow::on_resetButton_clicked()
{
    ui->filterLineEdit->clear();
}

// sync views

void MainWindow::on_listView_clicked(const QModelIndex& index)
{
    if (ui->listView->model() != index.model())
        return;
    if (m_currentIndex != index) {

        auto sortIndex = m_sortModel->mapFromSource(index);
        ui->listViewSorted->selectionModel()->clearSelection();
        ui->listViewSorted->selectionModel()->select(sortIndex, QItemSelectionModel::Select);

        auto filterIndex = m_filterModel->mapFromSource(sortIndex);
        ui->listViewFiltered->selectionModel()->clearSelection();
        ui->listViewFiltered->selectionModel()->select(filterIndex, QItemSelectionModel::Select);
    }
    m_currentIndex = index;
}

void MainWindow::on_listViewSorted_clicked(const QModelIndex& index)
{
    auto newIndex = m_sortModel->mapToSource(index);
    assert(newIndex.model() == m_model);

    if (m_currentIndex != newIndex) {
        {
            auto proxyIndex = m_filterModel->mapFromSource(index);
            ui->listViewFiltered->selectionModel()->clearSelection();
            ui->listViewFiltered->selectionModel()->select(proxyIndex, QItemSelectionModel::Select);
        }
        {
            ui->listView->selectionModel()->clearSelection();
            ui->listView->selectionModel()->select(newIndex, QItemSelectionModel::Select);
        }
    }
    m_currentIndex = newIndex;
}

void MainWindow::on_listViewFiltered_clicked(const QModelIndex& index)
{
    auto newIndex = m_sortModel->mapToSource(m_filterModel->mapToSource(index));
    assert(newIndex.model() == m_model);

    if (m_currentIndex != newIndex) {
        {
            ui->listView->selectionModel()->clearSelection();
            ui->listView->selectionModel()->select(newIndex, QItemSelectionModel::Select);
        }
        {
            auto proxyIndex = m_filterModel->mapToSource(index);
            ui->listViewSorted->selectionModel()->clearSelection();
            ui->listViewSorted->selectionModel()->select(proxyIndex, QItemSelectionModel::Select);
        }
    }
    m_currentIndex = newIndex;
}
