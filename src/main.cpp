// Imported libraries

// Glad/GLFW/stb_image 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

// GLM 
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/string_cast.hpp>

// Objects / Classes
#include "Shader.h"
#include "Texture.h"

#include "Camera.h"

// Custom Types
#include "Vec4.h"
#include "Vec3.h"
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

//-------------------------------------------------------------- Global Variables

float pointSize;
int   window_dimensions[2];
float location[4];
float rotation[3];
float scale[3];
float fov;

float last[2];
bool  firstMouse = true;

//---------------------------------------------------------------- Global Objects
Camera camera(vec3(0.0f, 0.0f, 3.0f), 
              vec3(0.0f, 1.0f, 0.0f),
              YAW, PITCH);

//---------------------------------------------------------- Forward declarations

void framebuffer_size_callback  (GLFWwindow *window, int /*width*/, int /*height*/);
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

  pointSize         = 40.f;

  fov               = math::radians(45.0f);

  last[x]           = 400.0f;
  last[y]           = 300.0f;

  weight.blend      = 0.5f;

  weight.texture    = 0.5f;
  weight.vertex     = 0.25f;
  weight.location   = 0.25f;

  //frustum.left    = 0.0f;
  //frustum.right   = 800.0f;
  //frustum.bottom  = 0.0f;
  //frustum.top     = 600.0f;
  //frustum.near    = 0.1f;
  //frustum.far     = 100.0f;

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

  euler.pitch       = 0.0f;
  euler.yaw         = -90.0f;
  euler.roll        = 0.0f;
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
  
  Shader shaderProgram (
    "../assets/Shaders/coord.vert",
    "../assets/Shaders/coord.frag"
  );   
  // configure global opengl state
  // -----------------------------
    glEnable(GL_DEPTH_TEST);

  //--------------------------------------------------------------- 6 Vxertex data

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

    float vertices[] = {
  
      //======================================================== Back (-Z)
  
      -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   // 0
       0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   // 1
       0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   // 2
      -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   // 3
  
      //======================================================= Front (+Z)
  
      -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   // 4
       0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   // 5
       0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   // 6
      -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   // 7
  
      //======================================================== Left (-X)
  
      -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   // 8
      -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   // 9
      -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   //10
      -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   //11
  
      //======================================================= Right (+X)
  
       0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   //12
       0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   //13
       0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   //14
       0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   //15
  
      //======================================================= Bottom (-Y)
  
      -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   //16
       0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   //17
       0.5f, -0.5f,  0.5f,   1.0f, 1.0f,   //18
      -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,   //19
  
      //========================================================== Top (+Y)
  
      -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,   //20
       0.5f,  0.5f, -0.5f,   1.0f, 0.0f,   //21
       0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   //22
      -0.5f,  0.5f,  0.5f,   0.0f, 1.0f    //23
  };

    // EBO indicies
    unsigned int indices[] = {

        // back
         0,  1,  2,
         2,  3,  0,
    
        // front
         4,  5,  6,
         6,  7,  4,
    
        // left
         8,  9, 10,
        10, 11,  8,
    
        // right
        12, 13, 14,
        14, 15, 12,
    
        // bottom
        16, 17, 18,
        18, 19, 16,
    
        // top
        20, 21, 22,
        22, 23, 20

    };

    // world space positions of our cubes
    vec3 cubePositions[] = {

       vec3( 0.0f,  0.0f,  0.0f),
       vec3( 2.0f,  5.0f, -15.0f),
       vec3(-1.5f, -2.2f, -2.5f),
       vec3(-3.8f, -2.0f, -12.3f),
       vec3( 2.4f, -0.4f, -3.5f),
       vec3(-1.7f,  3.0f, -7.5f),
       vec3( 1.3f, -2.0f, -2.5f),
       vec3( 1.5f,  2.0f, -2.5f),
       vec3( 1.5f,  0.2f, -1.5f),
       vec3(-1.3f,  1.0f, -1.5f)

    };

  //---------------------------------------------------- 7. Create OpenGL Objects 

  //initialise all VBO's and VAO's as openGL objects
  unsigned int objects[3];

  glGenVertexArrays (1, &objects[VAO]);
  glGenBuffers      (1, &objects[VBO]);
  glGenBuffers      (1, &objects[EBO]);

  // Setup
  // --------------------
  glBindVertexArray (objects[VAO]);

  glBindBuffer      (GL_ARRAY_BUFFER,   objects[VBO]);
  glBufferData      (GL_ARRAY_BUFFER,
                                        sizeof(vertices), 
                                        vertices, 
                                        GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects[EBO]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                        sizeof(indices),
                                        indices,
                                        GL_STATIC_DRAW);

  // Attribute #0 (LOCATION) :
  glVertexAttribPointer (
    0, 
    3,
    GL_FLOAT,
    GL_FALSE,
    5 * sizeof(float),
    (void*)0
  );

  glEnableVertexAttribArray (0);

  
  // Attribute #2 (UV) :
  glVertexAttribPointer(
    1,
    2,
    GL_FLOAT,
    GL_FALSE,
    5 * sizeof(float),
    (void*)(3 * sizeof(float))
  );

  glEnableVertexAttribArray    (1);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray            (0);

  //----------------------------------------------------------------- 8. Textures 
  
  Texture containerTexture ("../assets/Textures/container.jpg");
  Texture faceTexture      ("../assets/Textures/awesomeface.png");

  //--------------------------------------------- 9. Build/Compile Shader Program

  shaderProgram.use();

  // Send textures to the shader program uniforms
  shaderProgram.setInt     ("ourTexture1", 0);
  shaderProgram.setInt     ("ourTexture2", 1);

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

  //------------------------------------------------------- 12. Projection matrix
  
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
  shaderProgram.setMatrix      ("matrix", matrix);

  //------------------------------------------------------------- 13. Render Loop
  
  while (!glfwWindowShouldClose(window))
  {
      // Input
      processInput        (window);

      // Set Background Color 
      glClearColor        (0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Use our shader program
      shaderProgram.use();
     
      // Update the point size
      glPointSize         (pointSize);
      
      // Bind Textures to GL_TEXTURE0 + unit.
      containerTexture.bind (0);
      faceTexture.bind      (1);
      
      // Bind the relvent VAO
      glBindVertexArray     (objects[VAO]); 

      // clip = projection * view * model * local....
      
      // set delta time; 
      timer.current_frame = glfwGetTime();
      timer.delta_time    = timer.current_frame 
                          - timer.last_frame;

      timer.last_frame    = timer.current_frame;

      // local space matrix
      matrix.local = mat4                               ( 1.0f  ); // create identity matrix
      //matrix.local = matrix.local *  mat4::rotate       ( math::radians ( time  ) * 10.f, 
      //                                            vec3  ( 1.0f, 
      //                                                    1.0f,
      //                                                    1.0f )
      //                                                   );
      float angles[3];

      float time = timer.current_frame;

      angles[x]  = math::radians(time) * 10.0f;
      angles[y]  = math::radians(time) * 20.0f;
      angles[z]  = math::radians(time) * 30.0f;

      matrix.local = matrix.local * mat4::rotateXYZ     ( angles );

      matrix.local = matrix.local * mat4::translate     ( vec3 ( sin(time) 
                                                               , sin(time)
                                                               , sin(time) 
                                                               ) );

      //matrix.local = matrix.local *  mat4::translate  ( vec3  ( location[x],
      //                                                          location[y], 
      //                                                          location[z] ) 
      //                                                        );
      

      // model space matrix
      matrix.model = mat4 (1.0f); // create identity matrix
      matrix.model = matrix.model * mat4::rotate  ( math::radians ( -55.0f  ), 
                                                             vec3 (  1.0f,
                                                                     0.0f, 
                                                                     0.0f  ) 
                                                  );

      //Camera / view space
    
      //camera.direction.x = cos(math::radians(euler.yaw)) * cos(math::radians(euler.pitch));
      //camera.direction.y = sin(math::radians(euler.pitch));
      //camera.direction.z = sin(math::radians(euler.yaw)) * cos(math::radians(euler.pitch));
    
      //camera.target = camera.position + camera.direction;

      //matrix.view   = mat4::lookAt(camera.position,
      //                             camera.target,
      //                             camera.up);


      //// view space matrix
      //matrix.view = mat4 (1.0f); // create identity matrix
      //matrix.view = matrix.view * mat4::translate ( vec3  ( 0.0f, 0.0f, -3.0f ) );
      //matrix.view = matrix.view * mat4::translate ( vec3  ( location[x], 
      //                                                      location[y], 
      //                                                      location[z] )
      //                                            );
      //const float radius    = 10.0f;
      //float camX = sin(time / 10.0f) * radius;
      //float camZ = cos(time / 10.0f) * radius;

      //matrix.view = matrix.view 
      //            * mat4::lookAt  ( vec3  ( camX, 0.0f, camZ  ),
      //                              vec3  ( 0.0,  0.0,  0.0   ),
      //                              vec3  ( 0.0,  1.0,  0.0   )
      //                            );

   
      projection[perspective]  = mat4::perspective  (fov, 
                                                    (float)window_dimensions[width] 
                                                  / (float)window_dimensions[height],
                                                     frustum.near,
                                                     frustum.far
                                                    );
      
      // select which perspective projection matrix to use:
      matrix.projection = projection[perspective];

      shaderProgram.setMatrix      ("matrix", matrix);

      mat4 view = camera.getViewMatrix();
      shaderProgram.setMat4("matrix.view", view);

      // set weights
      shaderProgram.setWeight      ("weight", weight); 

      // DRAW...

      for (unsigned int i = 0; i < 10; i++)  {

        // calculate the model matrix for each object and pass it to shader before drawing
        matrix.model = mat4(1.0f);
        matrix.model = matrix.model * mat4::translate(cubePositions[i]);

        float angle  = 20.0f * i;

        matrix.model = matrix.model * mat4::rotate( math::radians ( angle ),
                                                    vec3          ( 1.0f,
                                                                    0.0f,
                                                                    0.0f  ) 
                                                  );

        shaderProgram.setMat4("matrix.model", matrix.model);

        glDrawElements(
                        GL_TRIANGLES,
                        36,
                        GL_UNSIGNED_INT,
                        nullptr
                      );

        glDrawArrays(GL_POINTS, 0, 36);

      }

      // Swap the buffers / present the finished frame.
      glfwSwapBuffers               (window);

      // Clear input poll events (needed for inputs)
      glfwPollEvents();
  }
  
  //----------------------------------------------------------------- 14. Cleanup

  // Destroy vertex buffer, Array , Element buffer object and program
  glDeleteVertexArrays        (1, &objects[VAO]);
  glDeleteBuffers             (1, &objects[VBO]);
  glDeleteBuffers             (1, &objects[EBO]);
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
    float pos[2];

    pos[x] = static_cast<float>(xpos);
    pos[y] = static_cast<float>(ypos);
    
    if (firstMouse){
      last[x]    = pos[x];
      last[y]    = pos[y];

      firstMouse = false;
    }

    float offset[2];

    offset[x]   =  pos[x] - last[x];
    offset[y]   = last[y] -  pos[y];

    last[x]     = pos[x];
    last[y]     = pos[y];

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

  if  (glfwGetKey (window, GLFW_KEY_UP)     == GLFW_PRESS){
        location[y] += 0.1f;
  }

  if  (glfwGetKey (window, GLFW_KEY_DOWN)   == GLFW_PRESS){
        location[y] -= 0.1f;
  }

  if  (glfwGetKey (window, GLFW_KEY_RIGHT)  == GLFW_PRESS){
        location[x] += 0.1f;
  }

  if  (glfwGetKey (window, GLFW_KEY_LEFT)   == GLFW_PRESS){
        location[x] -= 0.1f;
  }

  if  (glfwGetKey (window, GLFW_KEY_3)   == GLFW_PRESS){
        weight.texture -= 0.01f;
    if (weight.texture <= 0.0f)
        weight.texture =  0.0f;
  }

  if  (glfwGetKey (window, GLFW_KEY_4)   == GLFW_PRESS){
        weight.texture += 0.01f;
    if (weight.texture >= 1.0f)
        weight.texture =  1.0f;
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
}
