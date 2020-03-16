#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>

#include <QStandardItemModel>
#include <QStringListModel>
#include <QString>

#include <file_loader.h>

#include <string>
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
    void exit();

private:
    typedef struct{
        int offset_x;
        int offset_y;
        int width;
        int height;
    } placement;

    Ui::MainWindow *ui;
    FileLoader *file_loader;
    QStandardItemModel *table_model;

    std::string program_path_;
    QSize screen_size_;
    int min_heigth_;
    int min_width_;

    void init(char **argv);
    std::string GetProgramPath(char **argv = nullptr);
    void PopulateDeviceList();
    void ResizeMainWindow();

};
#endif // MAINWINDOW_H
