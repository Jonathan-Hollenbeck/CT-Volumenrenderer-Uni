#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <vec4.hpp>
#include <numeric>

#include "SliceViewer.h"
#include "SliceViewerCanvas.h"


void SliceViewerCanvas::drawGL() {

    using namespace nanogui;

    // rendering code here

}

std::vector<float> SliceViewerCanvas::getExampleTexture(int width, int height) {
    int count = width * height;
    std::vector<float> values(count);
    std::iota(values.begin(), values.end(), 0.f);
    for (auto &v: values) { // normalize between 0 and 1
        v /= (float)count;
    }
    return values;
}


void SliceViewerCanvas::setRotation(nanogui::Vector3f deg) {
    deg *= 3.1415f / 180.f;
    rotation = deg;
}

void SliceViewerCanvas::setRotationX(float deg) {
    deg *= 3.1415f / 180.f;
    rotation.row(0) << deg;
}

void SliceViewerCanvas::setRotationY(float deg) {
    deg *= 3.1415f / 180.f;
    rotation.row(1) << deg;
}

void SliceViewerCanvas::setRotationZ(float deg) {
    deg *= 3.1415f / 180.f;
    rotation.row(2) << deg;
}

void SliceViewerCanvas::setZoom(float scale) {
    zoom = scale;
}