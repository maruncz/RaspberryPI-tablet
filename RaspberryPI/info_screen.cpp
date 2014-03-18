#include "info_screen.h"
#include "ui_info_screen.h"

info_screen::info_screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info_screen)
{
    ui->setupUi(this);
    QString s;
    hw.set_up();
    ui->tableWidget->setRowCount((ui->tableWidget->rowCount())+1);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("Teplota"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(QString::number(hw.get_gpu_temp())));
    ui->tableWidget->setRowCount((ui->tableWidget->rowCount())+1);
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("Paměť CPU"));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(hw.get_cpu_mem())));
    ui->tableWidget->setRowCount((ui->tableWidget->rowCount())+1);
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("Paměť GPU"));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(QString::number(hw.get_gpu_mem())));
}

info_screen::~info_screen()
{
    delete ui;
}

void info_screen::on_close_button_clicked()
{
    info_screen::close();
}
