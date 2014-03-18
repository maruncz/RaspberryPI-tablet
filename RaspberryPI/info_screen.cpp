#include "info_screen.h"
#include "ui_info_screen.h"

info_screen::info_screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info_screen)
{
    ui->setupUi(this);
    int i,j;
    for(i=0;i<8;i++)
    {
        ui->tableWidget->setItem(i,1,new QTableWidgetItem());
    }
}

info_screen::~info_screen()
{
    delete ui;
}

void info_screen::on_close_button_clicked()
{
    info_screen::close();
}
