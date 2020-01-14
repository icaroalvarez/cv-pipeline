#include <QtWidgets/QVBoxLayout>
#include "FrameSourceNavigationWidget.h"
#include "easylogging++.h"

FrameSourceNavigationWidget::FrameSourceNavigationWidget(PipelineController *controller)
        :slider(Qt::Horizontal), controller(controller)
{
    // add navigation slider
    slider.setMaximum(1);

    // connect signals
    QObject::connect(&slider, &QSlider::valueChanged,
            this, &FrameSourceNavigationWidget::sliderValueChanged);
    QObject::connect(&slider, &QSlider::sliderReleased,
            this, &FrameSourceNavigationWidget::sliderReleased);

    // create layout and add widgets
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(&originalImageLabel);
    this->layout()->addWidget(&slider);
    this->layout()->addWidget(&indexLabel);

    // load first image
    originalImageLabel.setImage(controller->getCurrentLoadedImage());
    indexLabel.setText(QString::number(slider.tickInterval()));
}

void FrameSourceNavigationWidget::sliderValueChanged(int value) {
    // check if user is pressed down the slider
    if(!slider.isSliderDown()) {
        fireNewImage(value);

    }
}

void FrameSourceNavigationWidget::fireNewImage(int value) {
    LOG(INFO) << "Slider value changed: " << value;
    // show original image
    try{
        originalImageLabel.setImage(controller->getCurrentLoadedImage());
        indexLabel.setText(QString::number(value));

        // fire image processing
        controller->processCurrentImage();
    }catch(const std::exception& e)
    {
        LOG(WARNING) << "Not possible to process image at index: " << value <<
                     ". Exception: " << e.what();
    }
}

void FrameSourceNavigationWidget::sliderReleased() {
    fireNewImage(slider.value());
}
