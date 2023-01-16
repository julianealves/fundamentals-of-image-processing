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
#include <QtCharts>
using namespace std;
//using namespace QtCharts;

class PhotoProcessor
{
private:
    QString file_path;
    QWidget *window;
    QFormLayout *layout;
    QImage image;
    int histogram[256];
    float histogram_normalized[256];
    int cumulative_histogram[256];


    float calculate_luminance(QRgb pixelData);
    void open_file_explorer();
    void display_result(QImage image);
    void save_file();
    bool is_gray_scale();
    void apply_luminance();
    int fix_interval(int color);
    void get_image_histogram(int* image_histogram, QImage image_copy);
    void get_image_cumulative_histogram(int* image_histogram, QImage image_copy, float* image_cumulative_histogram);
    int find_target_shade_level_closest_to(float value, float* hist_target_cum);
    void treat_kernel(QString kernel_str, int type);
    void plot_image_histogram();

public:
    PhotoProcessor();
    int open_image(int argc, char *argv[]);
    void mirror_image_horizontally();
    void mirror_image_vertically();
    void convert_to_gray_scale();
    void quantize_image(int n_tones);

    void calculate_image_histogram();
    void adjust_brightness(float tone);
    void adjust_contrast(float scalar);
    void calculate_negative();
    void calculate_equalization();
    void image_equalization();
    void histogram_matching();
    void zoom_out();
    void zoom_in();
    void rotate_image_90_clockwise();
    void rotate_image_90_anticlockwise();
    void convolution(float kernel[][3], int filter_type);
};

#endif // PHOTOPROCESSOR_H
