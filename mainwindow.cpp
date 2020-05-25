#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//when you click the Open Image button
void MainWindow::on_actionOpen_Image_triggered()
{
    strFileName = FileDialogPath();         //excute FileDialogPath function

    dlg = new Dialog(this);
    dlg->setModal(true);       //show dialog
    dlg->exec();

    SourceImage = RawDataToQImage(strFileName,data);  //excute RawDataToQImage function
    source_width = width;
    source_height = height;

    pix = QPixmap::fromImage(SourceImage,Qt::AutoColor); //initialize raw image to qpixmap
    ui->SourceLabel->setPixmap(pix);               //show image through QLabel
    ui->SourceLabel->setScaledContents(true);      //resize image through QLabel
}

//when you click the 45 button
void MainWindow::on_actionRotate45_triggered()
{
    if(rotateFlag){ //when you click histogram button, initialize pixmap
        pix = resultpix;
        rotateFlag = false;
    }

    transform.rotate(45);   //rotate image 45 degrees
    ui->SourceLabel->setPixmap(pix.transformed(transform));
}

//when you click the 90 button
void MainWindow::on_actionRotate90_triggered()
{
    if(rotateFlag){  //when you click histogram button, initialize pixmap
        pix = resultpix;
        rotateFlag = false;
    }

    transform.rotate(90);
    ui->SourceLabel->setPixmap(pix.transformed(transform));
}

//when you click the 180 button
void MainWindow::on_actionRotate180_triggered()
{
    if(rotateFlag){  //when you click histogram button, initialize pixmap
        pix = resultpix;
        rotateFlag = false;
    }

    transform.rotate(180);
    ui->SourceLabel->setPixmap(pix.transformed(transform));
}

//when you want histogramEqualizaion
void MainWindow::on_actionHistogramEqualization_triggered()
{
   //excute histogram_equalization function
   Histogram_Equalization();

   //show image with changed data in QLabel
   ResultImage = QImage(data,source_width,source_height,QImage::Format_Grayscale8);
   resultpix = QPixmap::fromImage(ResultImage,Qt::AutoColor);
   ui->SourceLabel->setPixmap(resultpix);
   ui->SourceLabel->setScaledContents(true);

   rotateFlag = true;
}

//when you want match with image
void MainWindow::on_actionMatch_with_Image_triggered()
{
    strFileName = FileDialogPath();     //excute FileDialogPath function
    SourceRefFlag = false;
    dlg = new Dialog(this);
    dlg->setModal(true);                //show dialog
    dlg->exec();

    ReferenceImage = RawDataToQImage(strFileName,reference_data);  //excute RawDataToQImage function

    pix = QPixmap::fromImage(ReferenceImage,Qt::AutoColor); //initialize raw image to qpixmap
    ui->ReferenceLabel->setPixmap(pix);               //show image through SourceLabel
    ui->ReferenceLabel->setScaledContents(true);

    refer_width = width;
    refer_height = height;

    //make Dynamic allocation array
    unsigned int *refer_histogram = new unsigned int [256];
    unsigned int *source_histogram = new unsigned int [256];
    unsigned int *sum_refer_histogram = new unsigned int [256];
    unsigned int *sum_source_histogram = new unsigned int [256];
    unsigned int *final_histogram = new unsigned int [256];

    //


    int sum_ref = 0;
    int sum_sor = 0;
    float ref_scale_factor=255.0f/(float)(refer_height*refer_width);
    float sor_scale_factor=255.0f/(float)(source_height*source_width);

    //initialize histogram array
    for(int i=0; i<256; i++) refer_histogram[i]=0;
    for(int i=0; i<256; i++) source_histogram[i]=0;

    for(int i=0; i<refer_width *refer_height; i++) refer_histogram[reference_data[i]]++;
    for(int i =0; i<source_width * source_height; i++) source_histogram[data[i]]++;

    // normalize accumulate historgram
    for(int i=0; i<256; i++){
        sum_ref+= refer_histogram[i];
        sum_refer_histogram[i] = (int)((sum_ref*ref_scale_factor)+0.5);
    }

    for(int i=0; i<256; i++){
        sum_sor+= source_histogram[i];
        sum_source_histogram[i] = (int)((sum_sor*sor_scale_factor)+0.5);
    }

    //calculate final histogram
    for(int i =0;i<256;i++){
        int key = sum_source_histogram[i]+1;
        int result = closestValue(sum_refer_histogram,key);
        final_histogram[i] = result;
    }

    // delete Dynamic allocation array
    delete []refer_histogram; delete []source_histogram;
    delete []sum_refer_histogram; delete [] sum_source_histogram;
    delete []final_histogram;

    // update data from final accumulate histogram
    for (int i = 0; i < source_width*source_height; i++){
       data[i] = static_cast<uchar>(final_histogram[data[i]]);
    }

    //show image with changed data in QLabel
    ResultImage = QImage(data,source_width,source_height,QImage::Format_Grayscale8);
    resultpix = QPixmap::fromImage(ResultImage,Qt::AutoColor);
    ui->SourceLabel->setPixmap(resultpix);
    ui->SourceLabel->setScaledContents(true);

    rotateFlag = true;
}

// closestValue - to search close value in array
// Input : SearchArray , target number
// Output : index of simmilar number to target number
int MainWindow::closestValue(unsigned int *array, int key)
{
    int value = ceil(abs(key - array[0]));
    int num = array[0];
    int output = 0;

    for(int i=0; i<256; i++){
        if(value > ceil(abs(key - array[i])))
        {
            value = ceil(abs(key - array[i]));
            num = array[i];
            output = i;
        }
    }

    return output;
}

//RawDataToQImage function - to translate RawData to QImage
//Input : String = ImagePath
//Output : Qimage
QImage MainWindow::RawDataToQImage(QString string, uchar* receive_data)
{
    QImage image;
    file->setFileName(string);  //load the file
    file->open(QFile::ReadOnly);  //just read the file

    byteArray = file->readAll(); // transfer file to byteArray
    receive_data = (uchar*) byteArray.constData(); //transfer byteArray to uchar

    if(SourceRefFlag){
        data = receive_data;
        // to save the unsigned char data because we use this data in histogram equalization
    }
    else{
        reference_data = receive_data;
        SourceRefFlag = true;
    }


    file->close(); //if you get data, close the file

    if(GrayColorFlag){     // if you choose color button
        QImage *temp = new QImage(receive_data,width,height,QImage::Format_RGB888);
        image = *temp;  //initialize image
    }

    else{                  // if you choose gray button
        QImage *temp = new QImage(receive_data,width,height,QImage::Format_Grayscale8);
        image = *temp;  //initialize image
    }

    return image;
}

//FileDialogPath function - to show QFileDialog and
//Input : None
//Output : String = ImagePath
QString MainWindow::FileDialogPath()
{
    QString result;
    QString strFilter = "raw file(*.raw);;jpg file(*.jpg);;png file(*.png);;bmp file(*.bmp)";
    result = QFileDialog::getOpenFileName(this,"Open a file","C:/Users/TaeYoung/Desktop/영상처리시스템설계_과제2",strFilter);

    return result;
}

//Histogram Equalization
//Input - None
//Output - None (update_data)
void MainWindow::Histogram_Equalization()
{
    //make Dynamic allocation array
    unsigned int *histogram = new unsigned int [256];
    unsigned int *sum_hist = new unsigned int [256];

    //initialize histogram array
    for(int i=0; i<256; i++) histogram[i]=0;

    for(int i=0; i<source_width *source_height; i++){
        histogram[data[i]]++;
    }

    int sum= 0;
    float scale_factor=255.0f/(float)(source_height*source_width);

    // normalize accumulate historgram
    for(int i=0; i<256; i++)
     {
      sum += histogram[i];
      sum_hist[i] =(int)((sum*scale_factor) + 0.5);
     }

    // delete Dynamic allocation array
    delete []histogram; delete []sum_hist;

    // update data from normalize accumulate histogram
    for (int i = 0; i < source_width*source_height; i++){
       data[i] = static_cast<uchar>(sum_hist[data[i]]);
    }
}




