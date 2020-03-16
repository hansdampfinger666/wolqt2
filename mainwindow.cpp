#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent, char **argv)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init(argv);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void
MainWindow::init(char **argv)
{
    screen_size_ = qApp->screens()[0]->size();
    program_path_ = GetProgramPath(argv);

    file_loader = new(FileLoader);
    file_loader->LoadConfig(program_path_); // return error if config read fails
    PopulateDeviceList();
    ResizeMainWindow();
}


std::string
MainWindow::GetProgramPath(char **argv)
{
    char *p_realpath;
    char actualpath[PATH_MAX];

    char *p_symlinkpath = argv[0];
    p_realpath = realpath(p_symlinkpath, actualpath);
    return program_path_ = program_path_.substr(0, program_path_.append(p_realpath).find_last_of("/") + 1);
}


void
MainWindow::PopulateDeviceList()
{
    table_model = new QStandardItemModel(this);
    ui->tableView->setModel(table_model);
    QList<QStandardItem *> row;

    table_model->setHorizontalHeaderItem(0, new QStandardItem("Device")); // text lookup
    table_model->setHorizontalHeaderItem(1, new QStandardItem("OS"));

    unsigned int device_id = 0;

    while(file_loader->GetDeviceName(device_id) != nullptr)
    {
        row.clear();
        row << new QStandardItem(QString::fromStdString(*file_loader->GetDeviceName(device_id)));
        row << new QStandardItem(QString("%1").arg(QString::fromStdString(*file_loader->GetDeviceOSType(device_id))));
        table_model->appendRow(row);
        device_id++;
    }
}


void
MainWindow::ResizeMainWindow()
{
    // offset x, offset y, width, hight

    int base_height = 0;
    int base_width = 0;

    std::vector<int> add_heights;
    int add_height = 0;
    std::vector<int> add_widths;
    int add_width = 0;

    add_heights.push_back(table_model->rowCount() * 60);
    add_widths.push_back(ui->tableView->horizontalHeader()->length());

    for(auto height : add_heights)
        add_height += height;

    for(auto width : add_widths)
        add_width += width;

    this->setGeometry(screen_size_.rwidth() - screen_size_.rwidth() * 0.5 - add_width * 0.5,
                      screen_size_.rheight() - screen_size_.rheight() * 0.5 - add_height * 0.5,
                      base_width + add_width,
                      base_height + add_height);

    QList<QObject*> children;
    children = this->findChildren<QObject *>();

    for(auto child : children)
    {

    }
}
