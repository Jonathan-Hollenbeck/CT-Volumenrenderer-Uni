//
// Created by karim on 27.10.20.
//

#ifndef NANOGUI_SLICEVIEWER_H
#define NANOGUI_SLICEVIEWER_H


#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/checkbox.h>
#include <nanogui/popupbutton.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/glcanvas.h>
#include <nanogui/widget.h>
#include "SliceViewerCanvas.h"
#include "../ext/pvm/pvmvolume.h"

class SliceViewer {
public:
    SliceViewer(nanogui::Screen *screen) {
        using namespace nanogui;
        /**
         * Create windows for gl canvas, slice viewer controls and view controls
         */
        Window *sliceViewerWindow = new Window(screen, "Slice viewer");
        sliceViewerWindow->setPosition(Vector2i(250, 15));
        sliceViewerWindow->setLayout(new GroupLayout());

        canvas = new SliceViewerCanvas(sliceViewerWindow);
        canvas->setBackgroundColor({100, 100, 100, 255});
        canvas->setSize({500, 500});

        Window *planeControlsWindow = new Window(screen, "SliceView Controls");
        planeControlsWindow->setPosition(Vector2i(15, 15));
        planeControlsWindow->setLayout(new GroupLayout());

        Window *viewControlWindow = new Window(screen, "View Controls");
        viewControlWindow->setPosition(Vector2i(15, 300));
        viewControlWindow->setLayout(new GroupLayout());

        /*
         * Add the slice viewer controls
         */
        addPlaneControls(planeControlsWindow, std::pair<int, int>(0, 20), "Show xy-plane",
                         [this](bool isChecked) {
                             std::cout << isChecked << std::endl;
                             return isChecked;
                         },
                         [this](int value) {
                             std::cout << value << std::endl;
                         }
        );
        addPlaneControls(planeControlsWindow, std::pair<int, int>(0, 30), "Show yz-plane",
                         [this](bool isChecked) {
                             std::cout << isChecked << std::endl;
                             return isChecked;
                         },
                         [this](int value) {
                             std::cout << value << std::endl;
                         }
        );
        addPlaneControls(planeControlsWindow, std::pair<int, int>(0, 4), "Show xz-plane",
                         [this](bool isChecked) {
                             std::cout << isChecked << std::endl;
                             return isChecked;
                         },
                         [this](int value) {
                             std::cout << value << std::endl;
                         }
        );

        /*
         * Add view controls rotation and zoom
         */
        addViewControls(viewControlWindow, std::pair<int, int>(-360, 360), "Rotate x-axis °", -10, [this](int value) {
            canvas->setRotationX(value);
        });

        addViewControls(viewControlWindow, std::pair<int, int>(-360, 360), "Rotate y-axis °", 20, [this](int value) {
            canvas->setRotationY(value);
        });

        addViewControls(viewControlWindow, std::pair<float, float>(-360, 360), "Rotate z-axis °", 0, [this](int value) {
            canvas->setRotationZ(value);
        });

        addViewControls(viewControlWindow, std::pair<float, float>(1, 100), "Zoom", 50, [this](int value) {
            canvas->setZoom(value / 200.f);
        });

        /*
         * initialize canvas so that controls match the initial view
         */
        canvas->setRotation(nanogui::Vector3f(-10.f, 20.f, 0.f));
        canvas->setZoom(50.f / 200.f);
    }

    void addPlaneControls(nanogui::Window *controlsWindow, std::pair<int, int> sliderRange, std::string label, const std::function<void(bool)> checkboxCallback, std::function<void(float)> sliderCallback);

    void addViewControls(nanogui::Window *controlsWindow, std::pair<int, int> sliderRange, std::string label, int value, std::function<void(float)> sliderCallback);

private:
    SliceViewerCanvas *canvas;
    PVMVolume *volume;
};


#endif //NANOGUI_SLICEVIEWER_H
