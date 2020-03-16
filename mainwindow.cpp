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

    file_loader_ = new(FileLoader);
    file_loader_->LoadConfig(program_path_); // return error if config read fails
    PopulateDeviceList();
    ResizeMainWindow();
    ConnectSignals();
    sender_ = new(Sender);
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
    table_model_ = new QStandardItemModel(this);
    ui->tableView->setModel(table_model_);
    QList<QStandardItem *> row;

    table_model_->setHorizontalHeaderItem(0, new QStandardItem("Device")); // text lookup
    table_model_->setHorizontalHeaderItem(1, new QStandardItem("OS"));

    unsigned int device_id = 0;

    while(file_loader_->GetDeviceName(device_id) != nullptr)
    {
        row.clear();
        row << new QStandardItem(QString::fromStdString(*file_loader_->GetDeviceName(device_id)));
        row << new QStandardItem(QString("%1").arg(QString::fromStdString(*file_loader_->GetDeviceOSType(device_id))));
        table_model_->appendRow(row);
        device_id++;
    }
}


void
MainWindow::ResizeMainWindow()
{
    // offset x, offset y, width, height
    std::vector<int> add_heights;
    std::vector<int> add_widths;
    int add_height, add_width;

    add_heights.push_back(60);
    add_heights.push_back(table_model_->rowCount() * 60);

    add_widths.push_back(60);
    add_widths.push_back(ui->tableView->horizontalHeader()->length());

    add_height = std::accumulate(add_heights.begin(), add_heights.end(), 0);
    add_width = std::accumulate(add_widths.begin(), add_widths.end(), 0);

    this->setGeometry(screen_size_.rwidth() - screen_size_.rwidth() * 0.5 - add_width * 0.5,
                      screen_size_.rheight() - screen_size_.rheight() * 0.5 - add_height * 0.5,
                      add_width,
                      add_height);
}


void
MainWindow::ConnectSignals()
{
    QObject::connect(this->ui->pushButton_ok, &QPushButton::clicked,
                     this, &MainWindow::OnPushButtonOkClick);
    QObject::connect(this->ui->pushButton_cancel, &QPushButton::clicked,
                     this, &MainWindow::OnPushButtonCancelClick);
}


void
MainWindow::OnPushButtonOkClick()
{
    if(ui->tableView->selectionModel()->hasSelection())
    {
        if(this->ui->verticalSlider->sliderPosition())
        {
            std::vector<unsigned int> row_indexes = GetSelectedRows();
            std::vector<const std::string *> macs;

            for(auto row : row_indexes)
                macs.push_back(file_loader_->GetDeviceMAC(row));
            sender_->SendWolSignals(macs);
        }
        else
        {
            // send shutdown signals
        }
    }
}


void
MainWindow::OnPushButtonCancelClick()
{
    qApp->exit();
}


std::vector<unsigned int>
MainWindow::GetSelectedRows()
{
    QModelIndexList selection_rows = ui->tableView->selectionModel()->selectedRows();
    std::vector<unsigned int> row_indexes;

    for(auto row : selection_rows)
        row_indexes.push_back(static_cast<unsigned int>(row.row()));

    return row_indexes;
}


void
MainWindow::DisplayErrorMsgBox()
{
    QMessageBox msgBox;
    msgBox.setText("Unspecified error occured.");
    msgBox.exec();
}
