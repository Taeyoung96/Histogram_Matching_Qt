#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_GrayScaleButton_clicked()
{   //when you check grayscale button, change the flag
    GrayColorFlag = false;
}

void Dialog::on_ColorButton_clicked()
{   //when you check color button, change the flag
    GrayColorFlag = true;
}

void Dialog::on_Width_editingFinished()
{   //enter the width
    width = ui->Width->text().toInt();
}

void Dialog::on_Height_editingFinished()
{   //enter the height
    height = ui->Height->text().toInt();
}

void Dialog::on_buttonBox_accepted()
{
    //pass Width to mainwindow
    connect(this,SIGNAL(sendData(int,int)),parent(),SLOT(Recv_Value(int,int)));
    emit sendData(width,height);

    connect(this,SIGNAL(sendFlag(bool)),parent(),SLOT(Recv_Flag(bool)));
    emit sendFlag(GrayColorFlag);

}

void Dialog::on_buttonBox_rejected()
{   //if you click 'cancel' button, just closed
    close();
}
