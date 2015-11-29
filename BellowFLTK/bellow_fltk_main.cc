#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/gl.h>

using std::unique_ptr;

class MapWindow : public Fl_Gl_Window {
public:
  MapWindow(int x, int y, int w, int h, const char *L) :
    Fl_Gl_Window(x, y, w, h, L) {
  }

protected:
  void LoadTextures();
  void SetupShapes();
  void SetupShaders();

private:
  void draw() override;
  int handle(int);

  typedef struct {
    GLsizei count;
    GLuint vboId;
  } ShapeInfo;

  typedef struct {
    GLfloat x, y, texU, texV;
  } SpriteVertexInfo;

  static const int POS_ATTRIB_LOCATION = 0;
  static const int TEX_ATTRIB_LOCATION = 1;

  GLuint m_spriteMatrixUniform, m_spriteSamplerUniform;
  GLuint m_starTex;
  ShapeInfo m_starShape;
};

void MapWindow::LoadTextures() {
  // TODO: Texture atlas
  fl_register_images();
  Fl_PNG_Image star("Resources/star_yellow.png");
  Fl_PNG_Image dumm("unkown");
  Fl_Shared_Image *pImg = Fl_Shared_Image::get("Resources/star_yellow.png");
  if (pImg) {
    printf("%d shared bytes\n", pImg->count());
  }
  printf("%d / %d bytes\n", star.count(), dumm.count());
  printf("%d x %d\n", star.w(), star.h());
  glGenTextures(1, &m_starTex);
  glBindTexture(GL_TEXTURE_2D, m_starTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, star.w(), star.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, star.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glGenerateMipmap(GL_TEXTURE_2D);
}


void MapWindow::SetupShaders()
{
  GLchar infoLog[4096];
  GLsizei length;

  const GLchar *vertShaderSprite[] = {
    "#version 330 core\n"
    "layout(location = 0) uniform mat4 ModelViewProject;\n"
    "layout(location = 0) in vec4 vPosition;\n"
    "layout(location = 1) in vec2 vTexture;\n"
    "out vec2 tex_coord;\n"
    "void main() {\n"
    "    gl_Position = ModelViewProject * vPosition;\n"
    "    tex_coord = vTexture;\n"
    "}\n"
  };
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, vertShaderSprite, NULL);

  const GLchar *fragShaderSprite[] = {
    "#version 330 core\n"
    "uniform sampler2D tex;\n"
    "in vec2 tex_coord;\n"
    "out vec4 fColor;\n"
    "void \n"
    "main() {\n"
    "    fColor = texture(tex, tex_coord);\n"
    "}\n"
  };
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, fragShaderSprite, NULL);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glCompileShader(vertShader);
  glGetShaderInfoLog(vertShader, 4096, &length, infoLog);

  glAttachShader(program, fragShader);
  glCompileShader(fragShader);
  glGetShaderInfoLog(fragShader, 4096, &length, infoLog);

  glLinkProgram(program);
  glUseProgram(program);
  m_spriteMatrixUniform = glGetUniformLocation(program, "ModelViewProject");
  m_spriteSamplerUniform = glGetUniformLocation(program, "tex");
}


void MapWindow::SetupShapes() {
  static const SpriteVertexInfo starVerts[] = {
    { 0., 0., 0., 0. },
    { 1., 0., 1., 0. },
    { 0., 1., 0., 1. },
    { 1., 1., 1., 1. },
  };

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(starVerts), starVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(vpos, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexInfo), &starVerts[0].x);
  glEnableVertexAttribArray(vpos);
  glVertexAttribPointer(tpos, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexInfo), &starVerts[0].texU);

  m_starShape.count = 4;
  m_starShape.vboId = vbo;
}

void MapWindow::draw() {
  if (!valid()) {
    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glMatrixMode(GL_PROJECTION);
    GLdouble ratio = 640.0f / 480.0f;
    glOrtho(-ratio, ratio, -1, 1, -1, 1);
    LoadTextures();
    SetupShapes();
  }
  glClear(GL_COLOR_BUFFER_BIT);
}

int MapWindow::handle(int event) {
  switch (event) {
  case FL_PUSH:
    return 1;
  }

  return Fl_Gl_Window::handle(event);
}

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(650, 490);
  MapWindow *map = new MapWindow(5, 5, 640, 480, "whassis?");
  window->end();
  window->show(argc, argv);
  return Fl::run();
}