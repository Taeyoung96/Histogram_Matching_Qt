#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "dialog.h"  //Add dialog header files
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QImage>
#include <QtGlobal>  //to use unsigned char
#include <QPixmap>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QImage RawDataToQImage(QString str, uchar *receive_data);
    QString FileDialogPath();
    void Histogram_Equalization();
    ~MainWindow();

public slots:
    void Recv_Value(int receive_width, int receive_height){    //to send the width and height
        width = receive_width;
        height = receive_height;
    }

    void Recv_Flag(bool receive_flag){       //to send the flag
        GrayColorFlag = receive_flag;
    }

private slots:
    void on_actionOpen_Image_triggered();

    void on_actionRotate45_triggered();

    void on_actionRotate90_triggered();

    void on_actionRotate180_triggered();

    void on_actionHistogramEqualization_triggered();

    void on_actionMatch_with_Image_triggered();

private:
    int closestValue(unsigned int *array ,int key);

    Ui::MainWindow *ui;
    QString strFileName = "";   //Declare strFileName
    Dialog *dlg;                //Declare Dialog
    QFile *file = new QFile;    //Declare File
    QByteArray byteArray;       //Declare byteArray
    QImage SourceImage;         //Declare QImage
    QImage ReferenceImage;     //Declare QImage
    QImage ResultImage;
    QPixmap pix;            //Declare QPixmap
    QPixmap resultpix;
    QTransform transform;  //Declare Transform

    int width = 0;              //Declare Width
    int height = 0;             //Declare Height
    int source_width = 0;
    int source_height =0;
    int refer_width = 0;
    int refer_height = 0;
    bool GrayColorFlag = true;  //Declare GrayColorFlag
    bool SourceRefFlag = true;
    bool rotateFlag = false;
    uchar* data;          //Declare Data
    uchar* reference_data;

};

#endif // MAINWINDOW_H
