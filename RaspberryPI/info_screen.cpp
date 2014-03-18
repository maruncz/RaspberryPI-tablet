#include "info_screen.h"
#include "ui_info_screen.h"

info_screen::info_screen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info_screen)
{
    ui->setupUi(this);
}

info_screen::~info_screen()
{
    delete ui;
}

void info_screen::on_pushButton_clicked()
{
    close();
}
