#ifndef QTGLSCENE_HPP__
#define QTGLSCENE_HPP__

#include <chrono>
#include "QOpenGLWindow"
#include "QTime"

class QTGLScene: public QOpenGLWindow
{
public:
  QTGLScene();
  ~QTGLScene();
  void initializeGL();
  void paintGL();
private:
  void resizeGL(QResizeEvent *_event);
  void resizeGL(int _w, int _h);
  GLuint loadShaderSrc(std::string _fileName, GLenum _shaderType);
  void timerEvent(QTimerEvent *_event);
  void keyPressEvent(QKeyEvent *_event);
  void mouseMoveEvent (QMouseEvent * _event );
  void mousePressEvent ( QMouseEvent *_event);
  void mouseReleaseEvent ( QMouseEvent *_event );
  void wheelEvent( QWheelEvent *_event);
  int m_width;
  int m_height;
  GLuint m_vbo;
  GLuint m_ebo;
  GLuint m_tex;
  GLuint m_shaderProg;
  GLuint m_vao;
  GLint m_timeUniform;
  QTime m_time;
};


#endif // end of QTGLSCENE_HPP__
