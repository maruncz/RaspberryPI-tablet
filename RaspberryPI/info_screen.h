#ifndef INFO_SCREEN_H
#define INFO_SCREEN_H

#include <QDialog>
#include "hwinfo.h"
#include "rpi.h"
#include <QString>

namespace Ui {
class info_screen;
}

class info_screen : public QDialog
{
    Q_OBJECT
    
public:
    explicit info_screen(QWidget *parent = 0);
    ~info_screen();
    
private slots:
    void on_close_button_clicked();

private:
    Ui::info_screen *ui;

    void addrow(QString label, QString value);

    hwinfo hw;
    int rows;
};

#endif // INFO_SCREEN_H
