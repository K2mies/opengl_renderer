// Imported libraries

// Glad/GLFW/stb_image 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"


// Objects / Classes
#include "Shader.h"
#include "Texture.h"

#include "Camera.h"

// Custom Types
#include "Vec4.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Mat4.h"
#include "Mat3.h"

// custom libraries
#include "MathUtils.h"

// Imports
#include <iostream>

//------------------------------------------------------------------------- Enums
enum AxisType {
    
    x = 0,
    y = 1,
    z = 2,
    w = 3

};

enum ObjectType {
    
    VBO = 0,
    VAO = 1,
    EBO = 2,
    TEX = 3

};

enum DimensionsType {
    
    width  = 0,
    height = 1

};

enum ProjectionType {

  orthographic = 0,
  perspective  = 1
};

//----------------------------------------------------------------------- Structs

Weight  weight  { 

    0.0,       // texture
    0.0,       // vertex
    0.0,       // location
    0.0        // blend

  };

Frustum frustum {

    0.0f,      // left
    0.0f,      // right
    0.0f,      // bottom
    0.0f,      // top
    0.0f,      // near
    0.0f       // far

};

Matrix  matrix;

Time    timer;

Euler   euler;

Color   color;

//-------------------------------------------------------------- Global Variables

float pointSize;
int   window_dimensions[2];
float location[4];
float rotation[3];
float scale[3];
float fov;

float last[2];
bool  firstMouse = true;

vec3  position   = vec3(0.0f, 0.0f, 3.0f);
vec3  up         = vec3(0.0f, 1.0f, 0.0f);

vec3  light_position = vec3(1.2f, 1.0f, 2.0f);


//---------------------------------------------------------------- Global Objects
Camera camera(position, up, YAW, PITCH);

//---------------------------------------------------------- Forward declarations

void framebuffer_size_callback  (GLFWwindow *window, int /*width*/, int /*height*/);
void scroll_callback            (GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback             (GLFWwindow* window, double xpos, double ypos);
void processInput               (GLFWwindow *window);
void implamentation_info        ();

//-------------------------------------------------------------------------- Main

int main (){ 

  //----------------------------------------------------------------------- TESTS
  

  //----------------------------------------------------- 0. Initialize variables

  window_dimensions[width]   = 800;
  window_dimensions[height]  = 600;

  location[x]       = 0.0f;
  location[y]       = 0.0f;
  location[z]       = 0.0f;
  location[w]       = 1.0f;

  rotation[x]       = 0.0f;
  rotation[y]       = 0.0f;
  rotation[z]       = 1.0f;

  scale   [x]       = 0.5f;
  scale   [y]       = 0.5f;
  scale   [z]       = 0.5f;

  last    [x]       = 400.0f;
  last    [y]       = 300.0f;

  pointSize         = 40.f;

  fov               = math::radians(45.0f);

  weight.blend      = 0.5f;

  weight.texture    = 0.5f;
  weight.vertex     = 0.25f;
  weight.location   = 0.25f;

  frustum.left      =  -1.0f;
  frustum.right     =   1.0f;
  frustum.bottom    =  -1.0f;
  frustum.top       =   1.0f;
  frustum.near      =   0.1f;
  frustum.far       = 100.0f;

  matrix.local      = mat4(1.0f);
  matrix.model      = mat4(1.0f);
  matrix.view       = mat4(1.0f);
  matrix.projection = mat4(1.0f);
  matrix.clip       = mat4(1.0f);

  timer.delta_time  = 0.0f;
  timer.last_frame  = 0.0f;

  euler.pitch       =   0.0f;
  euler.yaw         = -90.0f;
  euler.roll        =   0.0f;

  color.object      = vec3(1.0f, 0.5f, 0.31f);
  color.light       = vec3(1.0f, 1.0f, 1.0f);

  //---------------------------------------------------------- 1. Initialize GLFW

  if (!glfwInit()){
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  // Initialize GLWF window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif
  
  //------------------------------------------------------------ 2. Create Window
  
  GLFWwindow* window = glfwCreateWindow(
    window_dimensions[width],
    window_dimensions[height],
    "OpenGL Renderer",
    nullptr,
    nullptr
  ); 

  //Check if window creation failed...
  if (window == nullptr)
  {
      std::cerr << "Failed to create GLFW window\n";
      glfwTerminate();
      return -1;
  }

  // Register resize callback
  glfwSetFramebufferSizeCallback  (window, framebuffer_size_callback);

  //---------------------------------------------- 3. Make OpenGL Context Current
  
  //make the window context active on the current thread
  glfwMakeContextCurrent          (window);

  // Capture and hide the mouse cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetCursorPosCallback(window, mouse_callback);  

  
  //---------------------------------------------------------- 4. Initialize GLAD 
  
  //Load the openGL function pointers with GLAD..
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cerr << "Failed to initialize GLAD\n";
      glfwTerminate();
      return -1;
  }

 //---------------------------------------------------- 5. Create Shader Program
    
  Shader lightShader (
    "../assets/Shaders/light.vert",
    "../assets/Shaders/light.frag"
  );

  Shader colorShader (
    "../assets/Shaders/colors.vert",
    "../assets/Shaders/colors.frag"
  );

  // configure global opengl state
  // -----------------------------
    glEnable(GL_DEPTH_TEST);

  //--------------------------------------------------------------- 6 Vxertex data

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
 
    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

  //---------------------------------------------------- 7. Create OpenGL Objects 

  // CUBE OBJECT---------------------------------------------
  unsigned int cube_objs[2];

  glGenVertexArrays (1, &cube_objs[VAO]);
  glGenBuffers      (1, &cube_objs[VBO]);

  // Setup
  // --------------------
  glBindVertexArray (cube_objs[VAO]);

  glBindBuffer      (GL_ARRAY_BUFFER,   cube_objs[VBO]);
  glBufferData      (GL_ARRAY_BUFFER,
                                        sizeof(vertices), 
                                        vertices, 
                                        GL_STATIC_DRAW);

  // Attribute #0 (LOCATION) :
  glVertexAttribPointer (
    0, 
    3,
    GL_FLOAT,
    GL_FALSE,
      6 * sizeof(float),
    (void*)0
  );

  glEnableVertexAttribArray (0);

   // Attribute #1 (NORMAL) :
  glVertexAttribPointer (
    1, 
    3,
    GL_FLOAT,
    GL_FALSE,
      6 * sizeof(float),
    (void*)(3 * sizeof(float))
  );

  glEnableVertexAttribArray (1);


  // LIGHT OBJECT--------------------------------------------
  unsigned int light_objs[2];

  glGenVertexArrays (1, &light_objs[VAO]);
  glGenBuffers      (1, &light_objs[VBO]);

  glBindVertexArray (    light_objs[VAO]);

  glBindBuffer      (GL_ARRAY_BUFFER,   light_objs[VBO]);
  glBufferData      (GL_ARRAY_BUFFER,
                                        sizeof(vertices), 
                                        vertices, 
                                        GL_STATIC_DRAW);
  
  // Attribute #0 (LOCATION) :
  glVertexAttribPointer (
    0, 
    3,
    GL_FLOAT,
    GL_FALSE,
      6 * sizeof(float),
    (void*)0
  );

  glEnableVertexAttribArray(0);

 

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray            (0);

 

  //--------------------------------------------- 9. Build/Compile Shader Program


  //-------------------------------------------------------- 10. Set the viewport 
  
  int                      frameBuffer[2];
  glfwGetFramebufferSize   (window, &frameBuffer[width], &frameBuffer[height]);
  glViewport               (0, 0,    frameBuffer[width],  frameBuffer[height]);


  //------------------------------------------------------- 11. Rendering Settings

  // Render points as 10x10 pixels
  glPointSize (pointSize);

  //----------------------------------------------------- 12. Implamentation info
 
  //print useful information about the openGL implamentation
  implamentation_info();

  //------------------------------------------------------- 13. Projection matrix
  
  mat4 projection[2];
  projection[orthographic] = mat4::ortho        (frustum.left,
                                                 frustum.right,
                                                 frustum.bottom,
                                                 frustum.top,
                                                 frustum.near,
                                                 frustum.far
                                                );

  projection[perspective]  = mat4::perspective  (fov, 
                                                (float)window_dimensions[width] 
                                              / (float)window_dimensions[height],
                                                 frustum.near,
                                                 frustum.far
                                                );
  
  // select which perspective projection matrix to use:
  matrix.projection = projection[perspective];
  
  // Pass the coordinate matricies to the shader.
  colorShader.setMatrix      ("matrix", matrix);
  lightShader.setMatrix      ("matrix", matrix);

  //------------------------------------------------------------- 14. Render Loop
  
  while (!glfwWindowShouldClose(window))
 {   
      // set delta time; 
      timer.current_frame = glfwGetTime();
      timer.delta_time    = timer.current_frame 
                          - timer.last_frame;

      timer.last_frame    = timer.current_frame;
      // Input
      processInput        (window);

      // Set Background Color 
      glClearColor        (0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update the point size
      glPointSize         (pointSize);

      // Use our Lightshader program
      colorShader.use();

      colorShader.setVec3("lightColor",       color.light);
      colorShader.setVec3("objectColor",     color.object);
      colorShader.setVec3("lightPosition", light_position);

      colorShader.setVec3("viewPosition", camera.position);

 
      projection[perspective]  = mat4::perspective  (fov, 
                                                    (float)window_dimensions[width] 
                                                  / (float)window_dimensions[height],
                                                     frustum.near,
                                                     frustum.far
                                                    );
      
      // select which perspective projection matrix to use:
      matrix.projection = projection[perspective];

      // update view matrix with LookAt every frame
      matrix.view   = camera.getViewMatrix();

      matrix.model  = mat4(1.0f);

      // create time variable to use for offsets
      float time; 
            time    = glfwGetTime    ();
            time    = math::radians  (time);
            time    = time * 30.0f;
      
      
      matrix.model  = matrix.model
                    * mat4::rotate(time, vec3(1.0f, 0.0f, 0.0f));
     
      matrix.normal = mat4::normalMatrix(matrix.model);

      colorShader.setMatrix      ("matrix",      matrix);
      

      // Draw the Cube
      glBindVertexArray(cube_objs[VAO]);
      //glDrawElements(
      //  GL_TRIANGLES,
      //  36,
      //  GL_UNSIGNED_INT,
      //  0
      //);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glDrawArrays(GL_POINTS, 0, 36);

      // setup the lgiht object
      lightShader.use();

      lightShader.setVec3("lightColor", color.light);

      
      // Move the light object upand down on the y
      light_position.y = sin(time);
      

      matrix.model  = mat4(1.0);
      matrix.model  = matrix.model 
                    * mat4::translate (light_position);
    
      matrix.model  = matrix.model 
                    * mat4::scale     (0.2f);

      matrix.normal = mat4::normalMatrix(matrix.model);

      lightShader.setMatrix     ("matrix",       matrix);

      glBindVertexArray(light_objs[VAO]);
      //glDrawElements(
      //  GL_TRIANGLES,
      //  36,
      //  GL_UNSIGNED_INT,
      //  0
      //);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glDrawArrays(GL_POINTS, 0, 36);
      
 
      // Swap the buffers / present the finished frame.
      glfwSwapBuffers               (window);

      // Clear input poll events (needed for inputs)
      glfwPollEvents();
  }
  
  //----------------------------------------------------------------- 15. Cleanup

  // Destroy vertex buffer, Array , Element buffer object and program
  glDeleteVertexArrays        (1, &cube_objs  [VAO]);
  glDeleteBuffers             (1, &cube_objs  [VBO]);

  glDeleteVertexArrays        (1, &light_objs [VAO]);
  glDeleteBuffers             (1, &light_objs [VBO]);
  // Clear/handle all allocated memory free, close... etc for GLFW
  glfwTerminate(); 
  return 0;
}

//---------------------------------------------------------- non-member functions

// Implamentation info
void implamentation_info() {

  //print useful information about the openGL implamentation
  std::cout << "Vendor:   "
            << glGetString(GL_VENDOR) << std::endl;

  std::cout << "Renderer: "
            << glGetString(GL_RENDERER) << std::endl;
  
  std::cout << "OpenGL:   "
            << glGetString(GL_VERSION) << std::endl;
  
  std::cout << "GLSL:     "
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "openGL initialized successfully\n";
  
  // Query the maximum number of vertex attributes
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout 
    << "Maximum vertex attributes: "
    << nrAttributes << std::endl;
}

//window resize update function
void framebuffer_size_callback  (GLFWwindow* window, int /*width*/, int /*height*/)
{
  int frameBuffer[2];
  glfwGetFramebufferSize  (window, &frameBuffer[width],&frameBuffer[height]);
  glViewport              (0, 0,    frameBuffer[width], frameBuffer[height]);

  window_dimensions[width]        = frameBuffer[width];
  window_dimensions[height]       = frameBuffer[height];
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){

  float offset[2];

  offset[x] = static_cast<float>(xoffset);
  offset[y] = static_cast<float>(yoffset);

  camera.processMouseScroll(offset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Camera mouse movement code goes here
    float position[2];

    position[x]  = static_cast<float>(xpos);
    position[y]  = static_cast<float>(ypos);
    
    if (firstMouse){

      last  [x]  = position[x];
      last  [y]  = position[y];

      firstMouse = false;

    }

    float offset[2];

    offset  [x]  = position[x] - last    [x];
    offset  [y]  = last    [y] - position[y];

    last    [x]  = position[x];
    last    [y]  = position[y];

    camera.processMouseMovement(offset);
}

// Key Hooks
void processInput(GLFWwindow *window)
{

  if  (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

  if  (glfwGetKey (window, GLFW_KEY_2)      == GLFW_PRESS){
        pointSize += 0.1f;
    if (pointSize < 20.0f){
        pointSize = 20.0f;
    }
  }
  
  if  (glfwGetKey (window, GLFW_KEY_1)      == GLFW_PRESS){
        pointSize -= 0.1f;
    if (pointSize < 20.0f){
        pointSize = 20.0f;
    }
  }

  if  (glfwGetKey (window, GLFW_KEY_3)   == GLFW_PRESS){
      color.light = color.light - vec3(0.01f, 0.0f, 0.0f);
  }

  if  (glfwGetKey (window, GLFW_KEY_4)   == GLFW_PRESS){
      color.light = color.light + vec3(0.01f, 0.0f, 0.0f);
  }

   if  (glfwGetKey (window, GLFW_KEY_5)   == GLFW_PRESS){
        weight.vertex -= 0.01f;
    if (weight.vertex <= 0.0f)
        weight.vertex =  0.0f;
  }

  if  (glfwGetKey (window, GLFW_KEY_6)   == GLFW_PRESS){
        weight.vertex += 0.01f;
    if (weight.vertex >= 1.0f)
        weight.vertex =  1.0f;
  }

 if  (glfwGetKey (window, GLFW_KEY_7)   == GLFW_PRESS){
        weight.location -= 0.01f;
    if (weight.location <= 0.0f)
        weight.location =  0.0f;
  }

  if  (glfwGetKey (window, GLFW_KEY_8)   == GLFW_PRESS){
        weight.location += 0.01f;
    if (weight.location >= 1.0f)
        weight.location =  1.0f;
  }

 if  (glfwGetKey (window, GLFW_KEY_9)   == GLFW_PRESS){
        weight.blend -= 0.01f;
    if (weight.blend <= 0.0f)
        weight.blend =  0.0f;
  }

  if  (glfwGetKey (window, GLFW_KEY_0)   == GLFW_PRESS){
        weight.blend += 0.01f;
    if (weight.blend >= 1.0f)
        weight.blend =  1.0f;
  }

  if  (glfwGetKey (window, GLFW_KEY_X)   == GLFW_PRESS){
        fov += math::radians(1.0f);
    if (fov >= math::radians(180.0f))
        fov =  math::radians(180.0f);
  }

  if  (glfwGetKey (window, GLFW_KEY_Z)   == GLFW_PRESS){
        fov -= math::radians(1.0f);
    if (fov <= math::radians(1.0f))
        fov =  math::radians(1.0f);
  }

  //    CAMERA MOVEMENT
  if (glfwGetKey(window, GLFW_KEY_W)    == GLFW_PRESS){
    camera.processKeyboard(FORWARD,  timer.delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_S)    == GLFW_PRESS){
    camera.processKeyboard(BACKWARD, timer.delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_A)    == GLFW_PRESS){
    camera.processKeyboard(LEFT,     timer.delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_D)    == GLFW_PRESS){
    camera.processKeyboard(RIGHT,    timer.delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_UP)   == GLFW_PRESS){
    camera.processKeyboard(UP,        timer.delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
    camera.processKeyboard(DOWN,      timer.delta_time);
  }
}
