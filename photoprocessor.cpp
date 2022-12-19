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
    QLineEdit *lineEdit1 = new QLineEdit();

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


    this->layout = new QFormLayout(this->window);
    this->layout->addWidget(button1);
    this->layout->addWidget(button2);
    this->layout->addWidget(button3);
    this->layout->addWidget(button4);
    this->layout->addWidget(button5);
    this->layout->addRow(button6, lineEdit1);

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

void PhotoProcessor::convert_to_gray_scale() {
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
