#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QPushButton>
#include <QString>
#include <QObject>
#include <QMessageBox>

#include <file_loader.h>
#include <sender.h>

#include <string>
#include <numeric>

#include <iostream>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, char **argv = nullptr);
    ~MainWindow();

signals:

private slots:
    void OnPushButtonOkClick();
    void OnPushButtonCancelClick();
    void DisplayErrorMsgBox();


private:
    std::string program_path_;
    QSize screen_size_;
    int min_heigth_;
    int min_width_;

    Ui::MainWindow *ui;
    FileLoader *file_loader_;
    Sender *sender_;
    QStandardItemModel *table_model_;

    void init(char **argv);
    std::string GetProgramPath(char **argv = nullptr);
    void PopulateDeviceList();
    void ResizeMainWindow();
    void ConnectSignals();
    std::vector<unsigned int> GetSelectedRows();

};
#endif // MAINWINDOW_H
