#include "info_screen.h"
#include "ui_info_screen.h"

info_screen::info_screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info_screen)
{
    ui->setupUi(this);
    QString s;
    hw.set_up();
    addrow("CPU memmory",QString::number(hw.get_cpu_mem()));
    addrow("GPU memmory",QString::number(hw.get_gpu_mem()));
}

info_screen::~info_screen()
{
    delete ui;
}

void info_screen::on_close_button_clicked()
{
    info_screen::close();
}

void info_screen::addrow(QString label, QString value)
{
    rows=ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rows+1);
    ui->tableWidget->setItem(rows,0,new QTableWidgetItem(label));
    ui->tableWidget->setItem(rows,1,new QTableWidgetItem(value));
}
