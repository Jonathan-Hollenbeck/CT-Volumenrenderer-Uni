#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <vec4.hpp>
#include <numeric>

#include "SliceViewer.h"
#include "SliceViewerCanvas.h"


void SliceViewerCanvas::drawGL() {

    using namespace nanogui;

    shader.bind();
    glEnable(GL_DEPTH_TEST);

    Matrix4f view;
    view.setIdentity();
    view.topLeftCorner<3, 3>() = Eigen::Matrix3f(Eigen::AngleAxisf(rotation[0], Vector3f::UnitX()) * // mimic euler angles
                                                 Eigen::AngleAxisf(rotation[1], Vector3f::UnitY()) *
                                                 Eigen::AngleAxisf(rotation[2], Vector3f::UnitZ())) * zoom;

    Matrix4f projection;
    projection.setIdentity();

    Matrix4f vp = projection * view;

    shader.setUniform("viewProj", vp);

    drawBoundingBox();
    if(showSliceXY)
        drawSlice(SLICE_XY);
    if(showSliceYZ)
        drawSlice(SLICE_YZ);
    if(showSliceXZ)
        drawSlice(SLICE_XZ);
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

void SliceViewerCanvas::drawBoundingBox() {
    using namespace nanogui;
    MatrixXf  positions(3, 8);
    positions.col(0) << -1, -1, -1;
    positions.col(1) << 1, -1, -1;
    positions.col(2) << 1, -1, 1;
    positions.col(3) << -1, -1, 1;

    positions.col(4) << -1, 1, -1;
    positions.col(5) << 1, 1, -1;
    positions.col(6) << 1, 1, 1;
    positions.col(7) << -1, 1, 1;

    MatrixXu indices(2, 12);
    indices.col(0) << 0, 1;
    indices.col(1) << 1, 2;
    indices.col(2) << 2, 3;
    indices.col(3) << 3, 0;
    indices.col(4) << 4, 5;
    indices.col(5) << 5, 6;
    indices.col(6) << 6, 7;
    indices.col(7) << 7, 4;
    indices.col(8) << 0, 4;
    indices.col(9) << 1, 5;
    indices.col(10) << 2, 6;
    indices.col(11) << 3, 7;

    Matrix4f model;
    model.setIdentity();
    shader.setUniform("model", model);

    shader.uploadIndices(indices);
    shader.uploadAttrib("position", positions);

    shader.drawIndexed(GL_LINES, 0, 12);
}

void SliceViewerCanvas::drawSlice(SliceViewerCanvas::Slice slice) {
    using namespace nanogui;

    Matrix4f model;
    model.setIdentity();

    MatrixXu indices(3, 2);
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 0;

    MatrixXf positions(3, 4);
    switch (slice) {
        case SLICE_XY:
            positions.col(0) << -1, -1, 0;
            positions.col(1) << 1, -1, 0;
            positions.col(2) << 1, 1, 0;
            positions.col(3) << -1, 1, 0;
            model(2, 3) = positionXY;
            break;
        case SLICE_YZ:
            positions.col(0) << 0, -1, -1;
            positions.col(1) << 0, -1, 1;
            positions.col(2) << 0, 1, 1;
            positions.col(3) << 0, 1, -1;
            model(0, 3) = positionYZ;
            break;
        case SLICE_XZ:
            positions.col(0) << -1, 0, -1;
            positions.col(1) << 1, 0, -1;
            positions.col(2) << 1, 0, 1;
            positions.col(3) << -1, 0, 1;
            model(1, 3) = positionXZ;
            break;
    }

    Texture texture(10, 10, getExampleTexture(10, 10));
    MatrixXf texCoords(2, 4);
    texCoords.col(0) << 0, 0;
    texCoords.col(1) << 1, 0;
    texCoords.col(2) << 1, 1;
    texCoords.col(3) << 0, 1;

    texture.bind();

    shader.setUniform("model", model);
    shader.uploadIndices(indices);
    shader.uploadAttrib("position", positions);
    shader.uploadAttrib("uv", texCoords);
    shader.drawIndexed(GL_TRIANGLES, 0, 2);
}

void SliceViewerCanvas::setSlicePosition(SliceViewerCanvas::Slice slice, float position) {
    switch (slice) {
        case SLICE_XY:
            positionXY = position;
            break;
        case SLICE_YZ:
            positionYZ = position;
            break;
        case SLICE_XZ:
            positionXZ = position;
            break;
    }
}

void SliceViewerCanvas::setSliceVisibility(SliceViewerCanvas::Slice slice, bool isVisible) {
    switch (slice) {
        case SLICE_XY:
            showSliceXY = isVisible;
            break;
        case SLICE_YZ:
            showSliceYZ = isVisible;
            break;
        case SLICE_XZ:
            showSliceXZ = isVisible;
            break;
    }
}
