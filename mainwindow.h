#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openTextFile_triggered();
    void on_saveTextFile_triggered();
    void on_openBinaryFile_triggered();
    void on_saveBinaryFile_triggered();
    void on_exit_triggered();

    void on_addBook_triggered();
    void on_deleteSelected_triggered();
    void on_selectPhoto_triggered();

    void on_findByName_triggered();

    void on_sortByAuthorAsc_triggered();
    void on_sortByAuthorDesc_triggered();

    void on_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::MainWindow *ui;
    void loadPhotoForRow(int row);
    void updateTableAfterPurchase(int row);

    struct BookData {
        QString name;
        QString genre;
        QString author;
        QString publisher;
        int quantity;
        double price;
        QString photoPath;
    };

    BookData getBookDataFromRow(int row);
    int findBookByFields(const QString& name, const QString& genre, const QString& publisher, const QString& author);
};

#endif // MAINWINDOW_H
