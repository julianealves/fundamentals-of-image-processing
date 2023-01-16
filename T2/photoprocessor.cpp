#include "photoprocessor.h"


PhotoProcessor::PhotoProcessor() {

}

int PhotoProcessor::open_image(int argc, char *argv[]) {
    QApplication app(argc, argv);

    this->window = new QWidget;
    QPushButton *button1 = new QPushButton("Load Image");
    QPushButton *button2 = new QPushButton("Save Image");
    QPushButton *button3 = new QPushButton("Mirror Image Horizontally");
    QPushButton *button4 = new QPushButton("Mirror Image Vertically");
    QPushButton *button5 = new QPushButton("Convert to Gray Scale");
    QPushButton *button6 = new QPushButton("Quantize Image");
    QPushButton *button7 = new QPushButton("Show image histogram");
    QPushButton *button8 = new QPushButton("Adjust Brightness");
    QPushButton *button9 = new QPushButton("Adjust Contrast");
    QPushButton *button10 = new QPushButton("Negative Image");
    QPushButton *button11 = new QPushButton("Equalize Image");
    QPushButton *button12 = new QPushButton("Histogram Matching");
    QPushButton *button13 = new QPushButton("Zoom out");
    QPushButton *button14 = new QPushButton("Zoom in");
    QPushButton *button15 = new QPushButton("Rotate 90 right");
    QPushButton *button16 = new QPushButton("Rotate 90 left");
    QLineEdit *lineEdit1 = new QLineEdit();
    QLineEdit *lineEdit2 = new QLineEdit();
    QLineEdit *lineEdit3 = new QLineEdit();

    QPushButton *button17 = new QPushButton("Low-pass");
    QPushButton *button18 = new QPushButton("High-pass");
    QPushButton *button19 = new QPushButton("Edges more sensitive");
    QPushButton *button20 = new QPushButton("Prewitt Hx");
    QPushButton *button21 = new QPushButton("Prewitt Hy");
    QPushButton *button22 = new QPushButton("Sobel Hx");
    QPushButton *button23 = new QPushButton("Sobel Hy");

    QLineEdit *lineEdit4 = new QLineEdit();
    QLineEdit *lineEdit5 = new QLineEdit();
    QLineEdit *lineEdit6 = new QLineEdit();
    QLineEdit *lineEdit7 = new QLineEdit();
    QLineEdit *lineEdit8 = new QLineEdit();
    QLineEdit *lineEdit9 = new QLineEdit();
    QLineEdit *lineEdit10 = new QLineEdit();

    QObject::connect(button1, &QPushButton::clicked, [=]() {
        this->open_file_explorer();
   });

    QObject::connect(button2, &QPushButton::clicked, [=]() {
        this->save_file();
   });

    QObject::connect(button3, &QPushButton::clicked, [=]() {
        this->mirror_image_horizontally();
   });

    QObject::connect(button4, &QPushButton::clicked, [=]() {
        this->mirror_image_vertically();
   });

    QObject::connect(button5, &QPushButton::clicked, [=]() {
        this->convert_to_gray_scale();
   });

    QObject::connect(button6, &QPushButton::clicked, [=]() {
        this->quantize_image(lineEdit1->text().toInt());
   });

    QObject::connect(button7, &QPushButton::clicked, [=]() {
        this->plot_image_histogram();
   });

    QObject::connect(button8, &QPushButton::clicked, [=]() {
        this->adjust_brightness(lineEdit2->text().toFloat());
   });

    QObject::connect(button9, &QPushButton::clicked, [=]() {
        this->adjust_contrast(lineEdit3->text().toFloat());
   });

    QObject::connect(button10, &QPushButton::clicked, [=]() {
        this->calculate_negative();
   });

    QObject::connect(button11, &QPushButton::clicked, [=]() {
        this->image_equalization();
   });

    QObject::connect(button12, &QPushButton::clicked, [=]() {
        this->histogram_matching();
   });

    QObject::connect(button13, &QPushButton::clicked, [=]() {
        this->zoom_out();
   });

    QObject::connect(button14, &QPushButton::clicked, [=]() {
        this->zoom_in();
   });

    QObject::connect(button15, &QPushButton::clicked, [=]() {
        this->rotate_image_90_clockwise();
   });

    QObject::connect(button16, &QPushButton::clicked, [=]() {
        this->rotate_image_90_anticlockwise();
   });

    QObject::connect(button17, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit4->text(), 1);
   });

    QObject::connect(button18, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit5->text(), 2);
   });

    QObject::connect(button19, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit6->text(), 3);
   });

    QObject::connect(button20, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit7->text(), 4);
   });

    QObject::connect(button21, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit8->text(), 5);
   });

    QObject::connect(button22, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit9->text(), 6);
   });

    QObject::connect(button23, &QPushButton::clicked, [=]() {
        this->treat_kernel(lineEdit10->text(), 7);
   });

    QGroupBox *horizontalGroupBox = new QGroupBox("Menu");
    QHBoxLayout *layout_buttons = new QHBoxLayout;

    this->layout = new QFormLayout(this->window);

    layout_buttons->addWidget(button1);
    layout_buttons->addWidget(button2);
    layout_buttons->addWidget(button3);
    layout_buttons->addWidget(button4);
    layout_buttons->addWidget(button5);
    layout_buttons->addWidget(button7);
    layout_buttons->addWidget(button10);
    layout_buttons->addWidget(button11);
    layout_buttons->addWidget(button12);
    layout_buttons->addWidget(button13);
    layout_buttons->addWidget(button14);
    layout_buttons->addWidget(button15);
    layout_buttons->addWidget(button16);
    horizontalGroupBox->setLayout(layout_buttons);

    this->layout->addWidget(horizontalGroupBox);
    this->layout->addRow(button6, lineEdit1);
    this->layout->addRow(button8, lineEdit2);
    this->layout->addRow(button9, lineEdit3);
    this->layout->addRow(button17, lineEdit4);
    this->layout->addRow(button18, lineEdit5);
    this->layout->addRow(button19, lineEdit6);
    this->layout->addRow(button20, lineEdit7);
    this->layout->addRow(button21, lineEdit8);
    this->layout->addRow(button22, lineEdit9);
    this->layout->addRow(button23, lineEdit10);
    this->window->resize(500, 500);
    this->window->show();

    return app.exec();
}

void PhotoProcessor::mirror_image_horizontally() {
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    for (int y = 0; y < height/2; y++) {
        for (int x = 0; x < width; x++) {
            QRgb lastPixelData = this->image.pixel(x, height-y-1);
            QRgb firstPixelData = this->image.pixel(x,y);
            this->image.setPixel(x, y, lastPixelData);
            this->image.setPixel(x, height-y-1, firstPixelData);
        }
    }

    this->display_result(this->image);
}

void PhotoProcessor::mirror_image_vertically() {
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width/2; x++) {
            QRgb lastPixelData = this->image.pixel(width-x-1, y);
            QRgb firstPixelData = this->image.pixel(x,y);
            this->image.setPixel(x, y, lastPixelData);
            this->image.setPixel(width-x-1, y, firstPixelData);
        }
    }

    this->display_result(this->image);
}

void PhotoProcessor::apply_luminance(){
    // The method apply the luminance in the image, but does not display it

    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            float luminance = this->calculate_luminance(pixelData);

            this->image.setPixel(x, y, qRgb(luminance,luminance,luminance));
        }
    }

}

void PhotoProcessor::convert_to_gray_scale() {
    // The method display the image in Gray Scale
    this->apply_luminance();
    this->display_result(this->image);
}

void PhotoProcessor::quantize_image(int n_tones) {
    int height;
    int width;
    float lowest;
    float biggest;

    if (n_tones == 0){
        n_tones = 256;
    }

    height = this->image.height();
    width = this->image.width();

    // Convert to gray scale and find lowest and biggest luminance
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            float luminance = this->calculate_luminance(pixelData);

            this->image.setPixel(x, y, qRgb(luminance,luminance,luminance));

            if (x == 0 && y == 0) {
                lowest = luminance;
                biggest = luminance;
            }
            else {
                if (luminance < lowest){
                    lowest = luminance;
                }
                if (luminance > biggest) {
                    biggest = luminance;
                }
            }
        }
    }

    int size_int = (int)(biggest - lowest + 1);

    if (n_tones < size_int) {
        int size_bin = size_int/n_tones;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                QRgb pixelData = this->image.pixel(x, y);

                for (int i=0; i < n_tones; i++){
                    float lowestInterval = lowest - 0.5 + (i * size_bin);
                    float biggestInterval = lowest - 0.5 + ((i + 1) * size_bin);
                    int center = (int)((lowestInterval + biggestInterval)/2);

                    // Pixel belongs to the interval
                    if (qRed(pixelData) >= lowestInterval && qRed(pixelData) < biggestInterval) {
                        this->image.setPixel(x, y, qRgb(center,center,center));
                        break;
                    }
                }

            }
        }
    }

    this->display_result(this->image);
}

float PhotoProcessor::calculate_luminance(QRgb pixelData) {
    return 0.299*qRed(pixelData) + 0.587*qGreen(pixelData) + 0.114*qBlue(pixelData);
}

void PhotoProcessor::open_file_explorer(){
    this->file_path = QFileDialog::getOpenFileName(this->window, ("Open Image"), ".", ("Image Files (*.png *.jpg *.bmp)"));
    qDebug().nospace() << "Path: " << qPrintable(this->file_path);

    this->image = QImage(this->file_path);
    QLabel *myLabel = new QLabel;

    myLabel->setPixmap(QPixmap::fromImage(this->image));
    myLabel->setAlignment(Qt::AlignCenter);
    this->layout->addWidget(myLabel);

}

void PhotoProcessor::save_file(){

    QString path = QFileDialog::getSaveFileName(this->window, ("Save Image"), ".", ("Image Files (*.jpg)"));
    this->image.save(path, "JPG");

}


void PhotoProcessor::display_result(QImage image) {
    QLabel *myLabel = new QLabel;

    myLabel->setPixmap(QPixmap::fromImage(image));
    myLabel->setAlignment(Qt::AlignCenter);
    this->layout->addWidget(myLabel);

}

bool PhotoProcessor::is_gray_scale(){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);

            if ((qRed(pixelData) != qGreen(pixelData)) || (qBlue(pixelData) != qGreen(pixelData)) || (qRed(pixelData) != qBlue(pixelData))){
                return false;
            }
        }
    }

    return true;
}

void PhotoProcessor::plot_image_histogram(){
    this->calculate_image_histogram();

    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    QBarSet *set0 = new QBarSet("Shade");
    QBarSeries *barseries = new QBarSeries();

    for (int i=0; i<256; i++){
        *set0 << this->histogram[i];
    }

    //    for (int i=0; i<256; i++){
    //        this->histogram_normalized[i] = (this->histogram[i] - min) / (max  - min);
    //    }

    barseries->append(set0);
    chart->addSeries(barseries);
    chart->setTitle("Image histogram");

    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    barseries->attachAxis(axisY);

    this->layout->addWidget(chartView);
}


void PhotoProcessor::calculate_image_histogram(){
    int height;
    int width;
    int max;
    int min;

    if (!this->is_gray_scale()){
        this->apply_luminance();
    }

    for (int i=0; i<256; i++){
        this->histogram[i] = 0;
        this->histogram_normalized[i] = 0.0;
    }

    height = this->image.height();
    width = this->image.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // All the channels in the pixel have the same value
            QRgb pixelData = this->image.pixel(x, y);
            this->histogram[qRed(pixelData)] += 1;
        }
    }

    min = this->histogram[0];
    max = this->histogram[0];

    for (int i=0; i<256; i++){
        if (this->histogram[i] > max){
            max = this->histogram[i];
        }
        else if (this->histogram[i] < min){
            min = this->histogram[i];
        }
    }
}

int PhotoProcessor::fix_interval(int color){
    if(color < 0){
        return 0;
    }
    else if (color > 255){
        return 255;
    }
    return color;
}


void PhotoProcessor::adjust_brightness(float tone){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    if (tone > 255){
        tone = 255;
    }
    else if (tone < -255){
        tone = -255;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            float red, green, blue;

            red = qRed(pixelData) + tone;
            green = qGreen(pixelData) + tone;
            blue = qBlue(pixelData) + tone;

            red = this->fix_interval(red);
            green = this->fix_interval(green);
            blue = this->fix_interval(blue);

            this->image.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    this->display_result(this->image);
}

void PhotoProcessor::adjust_contrast(float scalar){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    if (scalar > 255){
        scalar = 255;
    }
    else if (scalar <= 0){
        scalar = 1;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            float red, green, blue;

            red = qRed(pixelData) * scalar;
            green = qGreen(pixelData) * scalar;
            blue = qBlue(pixelData) * scalar;

            red = this->fix_interval(red);
            green = this->fix_interval(green);
            blue = this->fix_interval(blue);

            this->image.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    this->display_result(this->image);
}


void PhotoProcessor::calculate_negative(){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            float red, green, blue;

            red = 255 - qRed(pixelData);
            green = 255 - qGreen(pixelData);
            blue = 255 - qBlue(pixelData);

            this->image.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    this->display_result(this->image);
}

void PhotoProcessor::calculate_equalization(){
    float alpha;
    int total_num_pixels;
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    total_num_pixels = height * width;

    alpha = 255.0 / total_num_pixels;

    this->calculate_image_histogram();

    this->cumulative_histogram[0] = alpha * this->histogram[0];

    for (int i=1; i<=255; i++){
        this->cumulative_histogram[i] = this->cumulative_histogram[i-1] + (alpha * this->histogram[i]);
    }
}

void PhotoProcessor::image_equalization(){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    this->calculate_equalization();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            int new_pixel_value;

            new_pixel_value = this->cumulative_histogram[qRed(pixelData)];

            this->image.setPixel(x, y, qRgb(new_pixel_value, new_pixel_value, new_pixel_value));
        }
    }

    this->display_result(this->image);

//    QChart *chart = new QChart();
//    QChartView *chartView = new QChartView(chart);
//    QBarSet *set0 = new QBarSet("Shade");
//    QBarSeries *barseries = new QBarSeries();

//    for (int i=0; i<256; i++){
//        *set0 << this->cumulative_histogram[i];
//    }

//    barseries->append(set0);
//    chart->addSeries(barseries);
//    chart->setTitle("Histogram Matching");

//    chartView->setRenderHint(QPainter::Antialiasing);

//    QValueAxis *axisY = new QValueAxis();
//    chart->addAxis(axisY, Qt::AlignLeft);
//    barseries->attachAxis(axisY);

//    this->layout->addWidget(chartView);
}

void PhotoProcessor::get_image_histogram(int* image_histogram, QImage image_copy){
    int height;
    int width;

    for (int i=0; i<256; i++){
        image_histogram[i] = 0;
    }

    height = image_copy.height();
    width = image_copy.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // All the channels in the pixel have the same value
            QRgb pixelData = image_copy.pixel(x, y);
            image_histogram[qRed(pixelData)] += 1;
        }
    }

}

void PhotoProcessor::get_image_cumulative_histogram(int* image_histogram, QImage image_copy, float* image_cumulative_histogram){
    float alpha;
    int total_num_pixels;
    int height;
    int width;
    int max;
    int min;

    height = image_copy.height();
    width = image_copy.width();

    total_num_pixels = height * width;

    alpha = 255.0 / total_num_pixels;

    image_cumulative_histogram[0] = alpha * image_histogram[0];

    for (int i=1; i<256; i++){
        image_cumulative_histogram[i] = image_cumulative_histogram[i-1] + (alpha * image_histogram[i]);
    }

    min = image_cumulative_histogram[0];
    max = image_cumulative_histogram[0];

    for (int i=0; i<256; i++){
        if (image_cumulative_histogram[i] > max){
            max = image_cumulative_histogram[i];
        }
        else if (image_cumulative_histogram[i] < min){
            min = image_cumulative_histogram[i];
        }
    }

    //Normalization
    for (int i=0; i<256; i++){
        image_cumulative_histogram[i] = floorf(((image_cumulative_histogram[i] - min) / (max - min)) * 100) / 100;
    }
}

int PhotoProcessor::find_target_shade_level_closest_to(float value, float* hist_target_cum){
    int shade_target = 0;
    float diff;

    diff = abs(value - hist_target_cum[0]);

    for (int i=0; i<256; i++){
        if (abs(value - hist_target_cum[i]) < diff){
            diff = abs(value - hist_target_cum[i]);
            shade_target = i;
        }
    }

    return shade_target;
}


void PhotoProcessor::histogram_matching(){
    // The method expects images in gray scale
    QString file_path_src, file_path_target;
    QImage image_src, image_target;
    int* hist_src = new int[256];
    int* hist_target = new int[256];
    float* hist_src_cum = new float[256];
    float* hist_target_cum = new float[256];
    int hm[256];
    int height;
    int width;

    file_path_src = QFileDialog::getOpenFileName(this->window, ("Open Image"), ".", ("Image Files (*.png *.jpg *.bmp)"));
    qDebug().nospace() << "Path: " << qPrintable(file_path_src);

    file_path_target = QFileDialog::getOpenFileName(this->window, ("Open Image"), ".", ("Image Files (*.png *.jpg *.bmp)"));
    qDebug().nospace() << "Path: " << qPrintable(file_path_target);

    image_src = QImage(file_path_src);
    image_target = QImage(file_path_target);

    this->get_image_histogram(hist_src, image_src);
    this->get_image_histogram(hist_target, image_target);

    this->get_image_cumulative_histogram(hist_src, image_src, hist_src_cum);
    this->get_image_cumulative_histogram(hist_target, image_target, hist_target_cum);

    for (int shade_level=0; shade_level<256; shade_level++){
        hm[shade_level] = this->find_target_shade_level_closest_to(hist_src_cum[shade_level], hist_target_cum);
    }

    height = image_src.height();
    width = image_src.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // All the channels in the pixel have the same value
            QRgb pixelData = image_src.pixel(x, y);
            int new_pixel_value;

            new_pixel_value = hm[qRed(pixelData)];

            image_src.setPixel(x, y, qRgb(new_pixel_value, new_pixel_value, new_pixel_value));
        }
    }

    this->display_result(image_src);
}

void PhotoProcessor::zoom_out(){
    int height;
    int width;
    int sx;
    int sy;
    int total;
    int sum_red, sum_blue, sum_green;
    int new_x, new_y;
    int new_width;
    int new_height;

    height = this->image.height();
    width = this->image.width();

    sx = 2;
    sy = 2;
    total = sx * sy;

    new_y = 0;

    new_width = ((width-sx)/sx) + 1;
    new_height = ((height-sy)/sy) + 1;

    QImage new_image(QSize(new_width, new_height),QImage::Format_RGB32);

    for (int y = 0; y < height; y=y+sy) {
        new_x = 0;
        for (int x = 0; x < width; x=x+sx) {
            sum_red = 0;
            sum_blue = 0;
            sum_green = 0;

            int stop_y = y + sy;
            if (stop_y > height){
                stop_y = height;
            }

            for (int i = y; i < stop_y; i++){

                int stop_x = x + sx;
                if (stop_x > width){
                    stop_x = width;
                }

                for (int j = x; j < stop_x; j++){
                    QRgb pixelData = this->image.pixel(j, i);
                    sum_red = sum_red + qRed(pixelData);
                    sum_green = sum_green + qGreen(pixelData);
                    sum_blue = sum_blue + qBlue(pixelData);
                }
            }

            int new_red, new_green, new_blue;
            new_red = (int) (sum_red / total);
            new_green = (int) (sum_green / total);
            new_blue = (int) (sum_blue / total);
            new_image.setPixel(new_x, new_y, qRgb(new_red, new_green, new_blue));
            new_x++;
        }
        new_y++;
    }

    this->display_result(new_image);
    this->image = new_image;
}


void PhotoProcessor::zoom_in(){
    int height;
    int width;
    int new_width;
    int new_height;
    int avg_pixel_red, avg_pixel_green, avg_pixel_blue;

    height = this->image.height();
    width = this->image.width();

    new_width = width + (width - 1);
    new_height = height + (height - 1);

    QImage new_image(QSize(new_width, new_height),QImage::Format_RGB32);

    // Interpolation between columns
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == width-1){
                QRgb pixelData = this->image.pixel(x, y);
                int new_x = 2 * x;
                int new_y = 2 * y;
                new_image.setPixel(new_x, new_y, qRgb(pixelData, pixelData, pixelData));
            }
            else{
                QRgb pixelData1 = this->image.pixel(x, y);
                QRgb pixelData2 = this->image.pixel(x+1, y);

                avg_pixel_red = (int) ((qRed(pixelData2) + qRed(pixelData1))/2);
                avg_pixel_green = (int) ((qGreen(pixelData2) + qGreen(pixelData1))/2);
                avg_pixel_blue = (int) ((qBlue(pixelData2) + qBlue(pixelData1))/2);

                int new_x = 2 * x;
                int new_y = 2 * y;

                new_image.setPixel(new_x, new_y, qRgb(pixelData1, pixelData1, pixelData1));
                new_image.setPixel(new_x+1, new_y, qRgb(avg_pixel_red, avg_pixel_green, avg_pixel_blue));
            }
        }
    }

    // Interpolation between rows
    for (int y = 1; y < new_height; y=y+2) {
        for (int x = 0; x < new_width; x++) {
            QRgb pixelData1 = new_image.pixel(x, y-1);
            QRgb pixelData2 = new_image.pixel(x, y+1);

            avg_pixel_red = (int) ((qRed(pixelData2) + qRed(pixelData1))/2);
            avg_pixel_green = (int) ((qGreen(pixelData2) + qGreen(pixelData1))/2);
            avg_pixel_blue = (int) ((qBlue(pixelData2) + qBlue(pixelData1))/2);

            new_image.setPixel(x, y, qRgb(avg_pixel_red, avg_pixel_green, avg_pixel_blue));
        }
    }

    this->display_result(new_image);
}


void PhotoProcessor::rotate_image_90_clockwise(){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    QImage new_image(QSize(height, width),QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            new_image.setPixel(height-1-y,x, pixelData);
        }
    }

    this->image = new_image;
    this->display_result(new_image);
}

void PhotoProcessor::rotate_image_90_anticlockwise(){
    int height;
    int width;

    height = this->image.height();
    width = this->image.width();

    QImage new_image(QSize(height, width),QImage::Format_RGB32);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixelData = this->image.pixel(x, y);
            new_image.setPixel(y, width-1-x, pixelData);
        }
    }

    this->image = new_image;
    this->display_result(new_image);
}

void PhotoProcessor::treat_kernel(QString kernel_str, int type){
    float kernel[3][3];
    QStringList pieces = kernel_str.split( "," );
    int size = pieces.length() - 1;

    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            kernel[i][j] = pieces.value(size).toFloat();
            size--;
        }
    }

    this->convolution(kernel, type);
}

void PhotoProcessor::convolution(float kernel[][3], int filter_type){
    int height;
    int width;
    int n2 = (int) 3/2;
    int m2 = (int) 3/2;
    float sum_red, sum_green, sum_blue;
    int new_value;
    int new_value_red;
    int new_value_green;
    int new_value_blue;

    height = this->image.height();
    width = this->image.width();

    QImage new_image(QSize(width, height),QImage::Format_RGB32);

    if (filter_type != 1){
        this->apply_luminance();
    }

    for (int y = n2; y <= height-n2-1; y++){
        for (int x = m2; x <= width-m2-1; x++){
            sum_red = 0;
            sum_green = 0;
            sum_blue = 0;

            for (int k = -n2; k <= n2; k++){
                for (int j  =-m2; j <= m2; j++){
                    QRgb pixelData = this->image.pixel(x-j, y-k);
                    sum_red = sum_red + kernel[m2+j][n2+k]*qRed(pixelData);
                    sum_green = sum_green + kernel[m2+j][n2+k]*qGreen(pixelData);
                    sum_blue = sum_blue + kernel[m2+j][n2+k]*qBlue(pixelData);
                }
            }

            switch(filter_type){
                case 1:
                    // Low-pass
                    new_value_red = this->fix_interval((int) sum_red);
                    new_value_green = this->fix_interval((int) sum_green);
                    new_value_blue = this->fix_interval((int) sum_blue);
                    new_image.setPixel(x,y, qRgb(new_value_red, new_value_green, new_value_blue));
                    break;
                ;
                case 2:
                    // High-pass
                    new_value = this->fix_interval((int) sum_red);
                    new_image.setPixel(x,y, qRgb(new_value, new_value, new_value));
                    break;
                ;
                case 3:
                    // Generic high-pass
                    new_value = this->fix_interval((int) sum_red);
                    new_image.setPixel(x,y, qRgb(new_value, new_value, new_value));
                    break;
                ;
                default :
                    new_value = this->fix_interval((int) (sum_red + 127));
                    new_image.setPixel(x,y, qRgb(new_value, new_value, new_value));
            }
        }
    }

    this->image = new_image;
    this->display_result(new_image);
}
