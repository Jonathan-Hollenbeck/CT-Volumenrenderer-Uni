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
                "uniform mat4 modelViewProj;\n"
                "in vec3 position;\n"
                "in vec3 color;\n"
                "out vec4 frag_color;\n"
                "void main() {\n"
                "    frag_color = 3.0 * modelViewProj * vec4(color, 1.0);\n"
                "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
                "}",

                /* Fragment shader */
                "#version 330\n"
                "out vec4 color;\n"
                "in vec4 frag_color;\n"
                "void main() {\n"
                "    color = frag_color;\n"
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


private:
    nanogui::GLShader shader;
    Eigen::Vector3f rotation;
    float zoom;
};


#endif //NANOGUI_SLICEVIEWERCANVAS_H
