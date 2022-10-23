#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
// #include <QTimer>
#include <QVector4D>
#include <vector>
#include <QFile>

class OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    void drawSquare();
    void drawTriangle();
    void drawCircle();

    GLuint vboVerticesSquare{0};
    GLuint eboIndicesSquare{0};
    GLuint vaoSquare{0};

    GLuint vboVerticesTriangle{0};
    GLuint eboIndicesTriangle{0};
    GLuint vaoTriangle{0};

    GLuint vaoCircle{0};
    GLuint vboVerticesCircle{0};
    const unsigned int steps{719};

    GLuint shaderProgram{0};

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void changeDiagonal();

private:

signals:

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void toggleDarkMode(bool);
};

#endif // OPENGLWIDGET_H
