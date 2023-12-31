//
// Created by karim on 27.10.20.
//

#ifndef NANOGUI_SLICEVIEWERCANVAS_H
#define NANOGUI_SLICEVIEWERCANVAS_H


#include <nanogui/nanogui.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "Texture.h"

class SliceViewerCanvas : public nanogui::GLCanvas {
public:

    enum Slice {
        SLICE_XY = 0,
        SLICE_YZ = 1,
        SLICE_XZ = 2
    };

    SliceViewerCanvas(Widget *parent) : nanogui::GLCanvas(parent), rotation(nanogui::Vector3f(0.f, 0.f, 0.f)), zoom(1.f) {
        using namespace nanogui;

        shader.init(
                /* An identifying name */
                "a_simple_shader",

                /* Vertex shader */
                "#version 330\n"
                "uniform mat4 viewProj;\n"
                "uniform mat4 model;\n"
                "in vec3 position;\n"
                "in vec2 uv;\n"
                "out vec2 uv_;\n"
                "void main() {\n"
                "    gl_Position = viewProj * model * vec4(position, 1.0);\n"
                "    uv_ = uv;\n"
                "}",

                /* Fragment shader */
                "#version 330\n"
                "out vec4 color;\n"
                "in vec2 uv_;\n"
                "uniform sampler2D tex;\n"
                "void main() {\n"
                "    color = vec4(vec3(texture(tex, uv_).r), 1);\n"
                "}"
        );
    }

    ~SliceViewerCanvas() {
        shader.free();
    }

    void setRotation(nanogui::Vector3f deg);

    void setRotationX(float deg);

    void setRotationY(float deg);

    void setRotationZ(float deg);

    void setZoom(float scale);

    std::vector<float> getExampleTexture(int width, int height);

    virtual void drawGL() override;

    void drawBoundingBox();

    void drawSlice(Slice slice);

    void setSlicePosition(Slice slice, float position);

    void setSliceVisibility(Slice slice, bool isVisible);


private:
    nanogui::GLShader shader;
    Eigen::Vector3f rotation;
    float zoom;
    float positionXY = -1.f;
    float positionYZ = -1.f;
    float positionXZ = -1.f;
    bool showSliceXY = false;
    bool showSliceYZ = false;
    bool showSliceXZ = false;
};


#endif //NANOGUI_SLICEVIEWERCANVAS_H
