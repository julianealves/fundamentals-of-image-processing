#ifndef PHOTOPROCESSOR_H
#define PHOTOPROCESSOR_H
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <QPixmap>
#include <QImage>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtWidgets>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QObject>
#include <QMainWindow>
using namespace std;

class PhotoProcessor
{
private:
    QString file_path;
    QWidget *window;
    QFormLayout *layout;
    QImage image;


    float calculate_luminance(QRgb pixelData);
    void open_file_explorer();
    void display_result(QImage image);
    void save_file();

public:
    PhotoProcessor();
    int open_image(int argc, char *argv[]);
    void mirror_image_horizontally();
    void mirror_image_vertically();
    void convert_to_gray_scale();
    void quantize_image(int n_tones);

};

#endif // PHOTOPROCESSOR_H
