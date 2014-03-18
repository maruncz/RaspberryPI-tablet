#include "info_screen.h"
#include "ui_info_screen.h"

info_screen::info_screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info_screen)
{
    ui->setupUi(this);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("Teplota"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(hw.from_vcdencmd(hw.vcgencmd_args.temp)));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("Paměť CPU"));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(hw.from_vcdencmd(hw.vcgencmd_args.mem_arm)));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("Paměť GPU"));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(hw.from_vcdencmd(hw.vcgencmd_args.mem_gpu)));
}

info_screen::~info_screen()
{
    delete ui;
}

void info_screen::on_close_button_clicked()
{
    info_screen::close();
}
