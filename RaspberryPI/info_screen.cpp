#include "info_screen.h"
#include "ui_info_screen.h"

info_screen::info_screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info_screen)
{
    ui->setupUi(this);
    QString s;
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("Teplota"));
    s=hw.get_gpu_temp();
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(s));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("Paměť CPU"));
    s=hw.from_vcdencmd(hw.mem_arm);
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(s));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("Paměť GPU"));
    s=hw.from_vcdencmd(hw.mem_gpu);
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(s));
}

info_screen::~info_screen()
{
    delete ui;
}

void info_screen::on_close_button_clicked()
{
    info_screen::close();
}
