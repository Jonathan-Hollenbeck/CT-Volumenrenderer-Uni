//
// Created by karim on 27.10.20.
//

#include "SliceViewer.h"

void SliceViewer::addPlaneControls(nanogui::Window *controlsWindow,
                                   std::pair<int, int> sliderRange,
                                   std::string label,
                                   const std::function<void(bool)> checkboxCallback,
                                   std::function<void(float)> sliderCallback) {
    using namespace nanogui;

    CheckBox *planeCheckBox = new CheckBox(controlsWindow, label);
    planeCheckBox->setCallback(checkboxCallback);

    Widget *planePanel = new Widget(controlsWindow);
    planePanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));

    Slider *planeSlider = new Slider(planePanel);
    planeSlider->setRange(sliderRange);
    planeSlider->setValue(0);
    planeSlider->setFixedWidth(80);

    TextBox *planeTextBox = new TextBox(planePanel);
    planeTextBox->setEditable(false);
    planeTextBox->setFixedSize(Vector2i(60, 25));
    planeTextBox->setValue("0");
    planeTextBox->setFixedSize(Vector2i(60, 25));
    planeTextBox->setFontSize(20);
    planeTextBox->setAlignment(TextBox::Alignment::Right);

    planeSlider->setCallback([planeTextBox, sliderCallback](float value) {
        planeTextBox->setValue(std::to_string((int) (value)));
        sliderCallback(value);
    });
}

void SliceViewer::addViewControls(nanogui::Window *controlsWindow,
                                  std::pair<int, int> sliderRange,
                                  std::string label, int value = 0,
                                  std::function<void(float)> sliderCallback = nullptr) {
    using namespace nanogui;

    new Label(controlsWindow, label);

    Widget *viewControlPanel = new Widget(controlsWindow);
    viewControlPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));

    Slider *viewControlSlider = new Slider(viewControlPanel);
    viewControlSlider->setRange(sliderRange);
    viewControlSlider->setValue(value);
    viewControlSlider->setFixedWidth(80);

    TextBox *viewControlTextBox = new TextBox(viewControlPanel);
    viewControlTextBox->setEditable(true);
    viewControlTextBox->setFixedSize(Vector2i(60, 25));
    viewControlTextBox->setValue(std::to_string(value));
    viewControlTextBox->setFixedSize(Vector2i(60, 25));
    viewControlTextBox->setFontSize(20);
    viewControlTextBox->setAlignment(TextBox::Alignment::Right);

    viewControlTextBox->setCallback([viewControlSlider, viewControlTextBox, sliderCallback](const std::string &value) {
        int val = std::stoi(value);
        val = val % 360;
        viewControlSlider->setValue(val);
        viewControlTextBox->setValue(std::to_string(val));
        if (sliderCallback != nullptr)
            sliderCallback((float) val);
        return true;
    });

    viewControlSlider->setCallback([viewControlTextBox, sliderCallback](float value) {
        viewControlTextBox->setValue(std::to_string((int) (value)));
        if (sliderCallback != nullptr)
            sliderCallback(value);
    });
}
