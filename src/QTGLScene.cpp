#include <iostream>
#include <fstream>
#include <vector>
#include "QGuiApplication"
#include "QMouseEvent"
#include "ngl/NGLInit.h"
#include "ngl/Util.h"
#include "ngl/Image.h"
#include "ngl/Texture.h"
#include "QTGLScene.hpp"
#include "Texture.hpp"

QTGLScene::QTGLScene()
{
  setTitle("QT GL");
}

QTGLScene::~QTGLScene()
{
  std::cout << "Shutting down QT GL" << std::endl;
}

void QTGLScene::initializeGL()
{
  // setting up timer
  m_time = QTime::currentTime();

  // seting up gl
  ngl::NGLInit::instance();
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  //setting up vao
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  //setting up my scene
  GLfloat data[] = {
                    -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//tl
                     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,//tr
                    -0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//bl
                     0.5f,-0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f //br
                    };

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  GLuint elements[] = {0, 1, 2, 2, 3, 1};
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  //shaders
  GLuint vertShader = loadShaderSrc("shaders/basicVert.glsl", GL_VERTEX_SHADER);
  GLuint fragShader = loadShaderSrc("shaders/basicFrag.glsl", GL_FRAGMENT_SHADER);
  m_shaderProg = glCreateProgram();
  glAttachShader(m_shaderProg, vertShader);
  glAttachShader(m_shaderProg, fragShader);
  glLinkProgram(m_shaderProg);
  glUseProgram(m_shaderProg);

  //linking shader and vert data
  GLint posAttrib = glGetAttribLocation(m_shaderProg, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0);
  glEnableVertexAttribArray(posAttrib);

  GLint colAttrib = glGetAttribLocation(m_shaderProg, "colour");
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
  glEnableVertexAttribArray(colAttrib);

  GLint uvAttrib = glGetAttribLocation(m_shaderProg, "uv");
  glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
  glEnableVertexAttribArray(uvAttrib);

  //texture
  ngl::Image cat("textures/cat_01.jpg");
  glGenTextures(1, &m_tex);
  glBindTexture(GL_TEXTURE_2D, m_tex);

  //loading texture data
  std::cout << cat.getColour((GLuint)1200, (GLuint)2400).m_r << ", " <<
               cat.getColour((GLuint)1200, (GLuint)2400).m_g << ", " <<
               cat.getColour((GLuint)1200, (GLuint)2400).m_b << std::endl;
  glTexImage2D(GL_TEXTURE, 0, GL_RGBA, cat.width(), cat.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, cat.getPixels());

  //texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//  float borderCol[] = {1.0f, 1.0f, 0.0f, 1.0f};
//  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderCol);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  m_timeUniform = glGetUniformLocation(m_shaderProg, "time");

  startTimer(10);
}

void QTGLScene::timerEvent(QTimerEvent *_event)
{
  update();
}

void QTGLScene::paintGL()
{

  float elapsed = m_time.elapsed()/1000.0;
  glUniform1f(m_timeUniform, elapsed);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_width, m_height);
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLuint QTGLScene::loadShaderSrc(std::string _fileName, GLenum _shaderType)
{
    std::ifstream shaderFile;
    shaderFile.open(_fileName);

    std::string shaderSrc = std::string(std::istreambuf_iterator<char>(shaderFile),
                                        std::istreambuf_iterator<char>()) + "\0";
    const char * shaderSrcC = shaderSrc.c_str();

    // compiling shaders
    GLuint shader = glCreateShader(_shaderType);

    glShaderSource(shader, 1, &shaderSrcC, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
      char buffer[512];
      glGetShaderInfoLog(shader, 512, NULL, buffer);
      std::cout << "ERROR: " << _fileName << std::endl;
      std::cout << buffer << std::endl;
    }
    shaderFile.close();
    return shader;
}







void QTGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
}

void QTGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}


void QTGLScene::mouseMoveEvent (QMouseEvent * _event)
{

}


void QTGLScene::mousePressEvent ( QMouseEvent * _event)
{

}

void QTGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{

}

void QTGLScene::wheelEvent(QWheelEvent *_event)
{

}

void QTGLScene::keyPressEvent(QKeyEvent *_event)
{
  switch (_event->key())
  {
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_1: glClearColor(0.5f, 0.5f, 0.5f, 1.0f); break;
  case Qt::Key_2: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break;
  default : break;
  }
    update();
}
