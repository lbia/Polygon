#include "Renderer.h"
#include "GL/glew.h"
#include <iostream>


Renderer::Renderer() : shader(getVertexShader(), getFragmentShader()) {
    colors = {
        255,   0,   0,      //red
          0, 255,   0,      //lime
          0,   0, 255,      //blue
        255, 255,   0,      //yellow
          0, 255, 255,      //cyan
        255, 250, 205,      //lemon
        255,   0, 255,      //magenta
        139,   0,   0,      //dark red
          0, 128,   0,      //green
        138,  43, 226,      //blue violet
        255,  20, 147       //deep pink
    };
}

void Renderer::addShape(const Shape* shape){
    shapes.push_back(shape);
}

void Renderer::replaceShape(unsigned int position, const Shape *shape){
    if (position < shapes.size()){
        delete shapes[position];
        shapes[position] = shape;
    } else{
        addShape(shape);
    }
}

void Renderer::removeLastShape(){
    if (shapes.size() > 0){
        delete shapes[shapes.size() - 1];
        shapes.pop_back();
    }
}

void Renderer::drawShapes() const{
    shader.bind();
    for (unsigned int i = 0; i < shapes.size(); i++){
        const Shape* shape = shapes[i];
        unsigned int colorsSize = colors.size();
        setPolygonColorRGB(colors[(3 * i) % colorsSize], colors[(3 * i + 1) % colorsSize], colors[(3 * i + 2) % colorsSize]);
        shape->draw();
    }
}

void Renderer::clear() const{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setPolygonColorFloat(float r, float g, float b) const{
    shader.bind();
    shader.setUniform4f("u_Color", r, g, b, 1.0f);
}

void Renderer::setPolygonColorRGB(unsigned int r, unsigned int g, unsigned int b) const{
    shader.bind();
    shader.setUniform4f("u_Color", r / 255.0, g / 255.0, b / 255.0, 1.0f);
}

void Renderer::initGLEW(){
    if (glewInit() != GLEW_OK){
        std::cerr << "ERROR initializing Glew\n";
    }
    std::cout << "VERSION: " << glGetString(GL_VERSION) << "\n";

    // Returns the vendor
    auto vendor = glGetString(GL_VENDOR);
    // Returns a hint to the model
    auto model = glGetString(GL_RENDERER);

    std::cout << "VENDOR: " << vendor << "\n";
    std::cout << "MODEL: " << model << std::endl;

    // enable setting gl_PointSize in Vertex Shader
    glEnable(GL_PROGRAM_POINT_SIZE);

    //set the default line width to 3
    glLineWidth(3);
}

void Renderer::setLineWidth(unsigned int width){
    glLineWidth(width);
}

const char *Renderer::getVertexShader() const{
    return  "#version 330 core\n"
            "layout(location = 0) in vec2 position;\n"
            "void main(){\n"
            "   gl_PointSize = 5.0;\n"
            "   gl_Position = vec4(position, 0, 1);\n"
            "};";
}
const char *Renderer::getFragmentShader() const{
    return  "#version 330 core\n"
            "layout(location = 0) out vec4 color;\n"
            "uniform vec4 u_Color;\n"
            "void main(){\n"
            "   color = u_Color;\n"
            "};";
}