#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent):QOpenGLWidget{parent}
{
}

OpenGLWidget::~OpenGLWidget()
{
    destroyVBOs();
    destroyShaders();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1,1,1,1);

    qDebug("OpenGL Version: %s",glGetString(GL_VERSION));
    qDebug("GLSL Version: %s",glGetString(GL_SHADING_LANGUAGE_VERSION));

    createShaders();
    createVBOs();
}

void OpenGLWidget::resizeGL(int w, int h)
{

}

void OpenGLWidget::drawSquare()
{
    makeCurrent();
    glUseProgram(shaderProgram);

    auto locColor{glGetUniformLocation(shaderProgram, "vColor")};
    glUniform4f(locColor, 0, 0, 1, 1);

    glBindVertexArray(vaoSquare);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void OpenGLWidget::drawTriangle()
{
    makeCurrent();
    glUseProgram(shaderProgram);

    auto locColor{glGetUniformLocation(shaderProgram, "vColor")};
    glUniform4f(locColor, 0, 1, 0, 1);

    glBindVertexArray(vaoTriangle);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void OpenGLWidget::drawCircle()
{
    makeCurrent();
    glUseProgram(shaderProgram);

    auto locColor{glGetUniformLocation(shaderProgram, "vColor")};
    glUniform4f(locColor, 1, 1, 0, 1);

    glBindVertexArray(vaoCircle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, steps);
}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSquare();
    drawTriangle();
    drawCircle();
}

void OpenGLWidget::toggleDarkMode(bool changeToDarkMode)
{
    makeCurrent();
    if(changeToDarkMode){
        glClearColor(0,0,0,1);
    } else {
        glClearColor(1,1,1,1);
    }
    update();
}

void OpenGLWidget::createShaders()
{
    makeCurrent();
    destroyShaders();

    QFile vs(":/shaders/vshader1.glsl");
    QFile fs(":/shaders/fshader1.glsl");

    if(!vs.open(QFile::ReadOnly | QFile::Text)) return;
    if(!fs.open(QFile::ReadOnly | QFile::Text)) return;

    auto byteArrayVs{vs.readAll()};
    auto byteArrayFs{fs.readAll()};
    const char *c_strVs{byteArrayVs};
    const char *c_strFs{byteArrayFs};

    vs.close();
    fs.close();

    GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader, 1, &c_strVs, 0);
    glCompileShader(vertexShader);
    GLint isCompiled{0};

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength{0};
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);
        glDeleteShader(vertexShader);
        return;
    }

    GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &c_strFs, 0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength{0};
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint isLinked{0};
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength{0};
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLWidget::destroyShaders()
{
    makeCurrent();
    glDeleteProgram(shaderProgram);
}

void OpenGLWidget::createVBOs()
{
    makeCurrent();
    destroyVBOs();

    std::vector<QVector4D> verticesSquare;
    std::vector<GLuint> indicesSquare;

    verticesSquare.resize(4);
    indicesSquare.resize(6);

    float posx{-0.5f};
    float posy{-0.5f};
    float offset{0.2f};

    verticesSquare[0] = QVector4D(posx - offset, posy - offset, 0, 1);
    verticesSquare[1] = QVector4D(posx + offset, posy - offset, 0, 1);
    verticesSquare[2] = QVector4D(posx + offset, posy + offset, 0, 1);
    verticesSquare[3] = QVector4D(posx - offset, posy + offset, 0, 1);

    indicesSquare[0] = 0;
    indicesSquare[1] = 1;
    indicesSquare[2] = 2;
    indicesSquare[3] = 2;
    indicesSquare[4] = 3;
    indicesSquare[5] = 0;

    glGenVertexArrays(1, &vaoSquare);
    glBindVertexArray(vaoSquare);

    glGenBuffers(1, &vboVerticesSquare);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesSquare);
    glBufferData(GL_ARRAY_BUFFER, verticesSquare.size()*sizeof(QVector4D), verticesSquare.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &eboIndicesSquare);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndicesSquare);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSquare.size()*sizeof(GLuint), indicesSquare.data(), GL_STATIC_DRAW);




    std::vector<QVector4D> verticesTriangle;
    std::vector<GLuint> indicesTriangle;

    verticesTriangle.resize(3);
    indicesTriangle.resize(3);

    float posxTriangle{-0.5f};
    float posyTriangle{-0.25f};
    float offsetTriangle{0.25f};

    verticesTriangle[0] = QVector4D(posxTriangle - offsetTriangle, posyTriangle, 0, 1);
    verticesTriangle[1] = QVector4D(posxTriangle + offsetTriangle, posyTriangle, 0, 1);
    verticesTriangle[2] = QVector4D(posxTriangle, posyTriangle + offsetTriangle, 0, 1);

    indicesTriangle[0] = 0;
    indicesTriangle[1] = 1;
    indicesTriangle[2] = 2;

    glGenVertexArrays(1, &vaoTriangle);
    glBindVertexArray(vaoTriangle);

    glGenBuffers(1, &vboVerticesTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesTriangle);
    glBufferData(GL_ARRAY_BUFFER, verticesTriangle.size()*sizeof(QVector4D), verticesTriangle.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &eboIndicesTriangle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndicesTriangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesTriangle.size()*sizeof(GLuint), indicesTriangle.data(), GL_STATIC_DRAW);




    glGenVertexArrays(1, &vaoCircle);
    glBindVertexArray(vaoCircle);

    std::vector<QVector4D> verticesCircle;
    verticesCircle.resize(steps);
    for(unsigned int i{0}; i < steps; i++)
    {
        float os[2] = {0.5, 0.5};
        float r[2] = {0.1, 0.12};
        verticesCircle[i] = QVector4D(os[0] + r[0]*sin(qDegreesToRadians(i)), os[1] + r[1]*cos(qDegreesToRadians(i)), 0, 1);
    }

    glGenBuffers(1, &vboVerticesCircle);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesCircle);
    glBufferData(GL_ARRAY_BUFFER, verticesCircle.size()*sizeof(QVector4D), verticesCircle.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
}

void OpenGLWidget::destroyVBOs()
{
    makeCurrent ();

    glDeleteBuffers(1, &vboVerticesSquare);
    glDeleteBuffers (1, &eboIndicesSquare);
    glDeleteVertexArrays (1, &vaoSquare);
    vboVerticesSquare=0;
    eboIndicesSquare=0;
    vaoSquare=0;

    glDeleteBuffers(1, &vboVerticesTriangle);
    glDeleteBuffers (1, &eboIndicesTriangle);
    glDeleteVertexArrays (1, &vaoTriangle);
    vboVerticesTriangle=0;
    eboIndicesTriangle=0;
    vaoTriangle=0;

    glDeleteBuffers(1, &vboVerticesCircle);
    glDeleteVertexArrays (1, &vaoCircle);
    vboVerticesCircle=0;
    vaoCircle=0;
}
