#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <vec4.hpp>
#include <numeric>

#include "SliceViewer.h"
#include "SliceViewerCanvas.h"


void SliceViewerCanvas::drawGL() {

    using namespace nanogui;

    shader.bind();

    Matrix4f view;
    view.setIdentity();
    view.topLeftCorner<3, 3>() = Eigen::Matrix3f(Eigen::AngleAxisf(rotation[0], Vector3f::UnitX()) * // mimic euler angles
                                                 Eigen::AngleAxisf(rotation[1], Vector3f::UnitY()) *
                                                 Eigen::AngleAxisf(rotation[2], Vector3f::UnitZ())) * zoom;

    Matrix4f projection;
    projection.setIdentity();

    Matrix4f model;
    model.setIdentity();

    Matrix4f mvp = projection * view * model;

    MatrixXf vertices(3, 3);
    vertices.col(0) << -1, -1, -1;
    vertices.col(1) << 1, -1, -1;
    vertices.col(2) << -1, 1, 1;

    MatrixXf colors(3, 3);
    colors.col(0) << 1, 0, 0;
    colors.col(1) << 0, 1, 0;
    colors.col(2) << 0, 0, 1;

    MatrixXu indices(3, 1);
    indices.col(0) << 0, 1, 2;

    shader.setUniform("modelViewProj", mvp);
    shader.uploadIndices(indices);
    shader.uploadAttrib("position", vertices);
    shader.uploadAttrib("color", colors);

    shader.drawIndexed(GL_TRIANGLES, 0, 1); // GL_LINES
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