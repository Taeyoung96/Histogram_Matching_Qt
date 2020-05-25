#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    int width = 0;                  //Declare width
    int height = 0;                 //Declare height
    ~Dialog();

signals:
    void sendData(int width, int height);
    void sendFlag(bool flag);

public slots:
    void on_buttonBox_accepted();

private slots:
    void on_GrayScaleButton_clicked();

    void on_ColorButton_clicked();

    void on_buttonBox_rejected();

    void on_Width_editingFinished();

    void on_Height_editingFinished();

private:
    Ui::Dialog *ui;
    bool GrayColorFlag = true; // Declare GrayColorFlag
};

#endif // DIALOG_H
