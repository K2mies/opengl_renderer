// Imported libraries

// Glad/GLFW/stb_image 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

// GLM 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Objects / Classes
#include "Shader.h"
#include "Texture.h"

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

//-------------------------------------------------------------- Global Variables

float pointSize;
int   window_dimensions[2];
float location[4];
float rotation[3];
float scale[3];
float fov;

//---------------------------------------------------------- Forward declarations

void framebuffer_size_callback  (GLFWwindow *window, int /*width*/, int /*height*/);
void processInput               (GLFWwindow *window);
void implamentation_info        ();

//-------------------------------------------------------------------------- Main

int main (){
  
  std::cout << "Starting OpenGL Renderer..." << std::endl;

  //----------------------------------------------------- 0. Initialize variables

  window_dimensions[width]   = 800;
  window_dimensions[height]  = 600;

  location[x]     = 0.0f;
  location[y]     = 0.0f;
  location[z]     = 0.0f;
  location[w]     = 1.0f;

  rotation[x]     = 0.0f;
  rotation[y]     = 0.0f;
  rotation[z]     = 1.0f;

  scale[x]        = 0.5f;
  scale[y]        = 0.5f;
  scale[z]        = 0.5f;

  pointSize       = 40.f;

  fov             = glm::radians(45.0f);

  weight.blend    = 0.5f;

  weight.texture  = 0.5f;
  weight.vertex   = 0.25f;
  weight.location = 0.25f;

  //frustum.left    = 0.0f;
  //frustum.right   = 800.0f;
  //frustum.bottom  = 0.0f;
  //frustum.top     = 600.0f;
  //frustum.near    = 0.1f;
  //frustum.far     = 100.0f;

  frustum.left    = -1.0f;
  frustum.right   = 1.0f;
  frustum.bottom  = -1.0f;
  frustum.top     = 1.0f;
  frustum.near    = 0.1f;
  frustum.far     = 100.0f;
  //-------------------------------------------------------- 0. matrix operations
  //glm::mat4 trans = glm::mat4(1.0f); //create identity matrix.
  //trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  //trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));


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
    "../assets/Shaders/matrix.vert",
    "../assets/Shaders/matrix.frag"
  );   

  //--------------------------------------------------------------- 6 Vertex data
  
  float vertices[] = {
     // positions         // colors           // uv coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top    right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top    left 
  };

  unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3,  // first  triangle
    1, 2, 3   // second triangle
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
    8 * sizeof(float),
    (void*)0
  );

  glEnableVertexAttribArray (0);

  // Attribute #1 (COLOR) :
  glVertexAttribPointer (
    1, 
    3, 
    GL_FLOAT,
    GL_FALSE,
    8 * sizeof(float),
    (void*)(3 * sizeof(float))
  );

  glEnableVertexAttribArray (1);
  
  // Attribute #2 (UV) :
  glVertexAttribPointer(
    2,
    2,
    GL_FLOAT,
    GL_FALSE,
    8 * sizeof(float),
    (void*)(6 * sizeof(float))
  );

  glEnableVertexAttribArray    (2);

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

  //------------------------------------------------------------- 13. Render Loop
  
  while (!glfwWindowShouldClose(window))
  {
      // Input
      processInput        (window);

      // Set Background Color 
      glClearColor        (0.2f, 0.3f, 0.3f, 1.0f);
      glClear             (GL_COLOR_BUFFER_BIT);

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
    
      // local space matrix
      matrix.local = glm::mat4       (1.0f); // create identity matrix
      matrix.local = glm::translate   (matrix.local, 
                                       glm::vec3  (location[x], 
                                                   location[y], 
                                                   location[z])
                                      );

      // model space matrix
      matrix.model = glm::mat4       (1.0f); // create identity matrix
      matrix.model = glm::rotate     (matrix.model, 
                                      glm::radians(-55.0f), 
                                      glm::vec3(1.0f, 0.0f, 0.0f));

      // view space matrix
      matrix.view = glm::mat4        (1.0f); // create identity matrix
      matrix.view = glm::translate   (matrix.view, 
                                      glm::vec3(0.0f, 0.0f, -3.0f));

      // projection matricies
      glm::mat4 projection[2];

      projection[orthographic] = glm::ortho       (frustum.left,
                                                   frustum.right,
                                                   frustum.bottom,
                                                   frustum.top,
                                                   frustum.near,
                                                   frustum.far
                                                  );

      projection[perspective]  = glm::perspective (fov, 
                                                  (float)window_dimensions[width] 
                                                / (float)window_dimensions[height],
                                                   frustum.near,
                                                   frustum.far
                                                  );
  
      // select which perspective projection matrix to use:
      matrix.projection = projection[perspective];
      
      // Pass the coordinate matricies to the shader.
      shaderProgram.setMatrix      ("matrix", matrix);

      // set weights
      shaderProgram.setWeight      ("weight", weight); 

      // DRAW...

      // Drawn Rectangle
      glDrawElements(
          GL_TRIANGLES,
          6,
          GL_UNSIGNED_INT,
          nullptr
      );

      // Draw vertex points
      glDrawArrays            (GL_POINTS, 0, 4);

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

  window_dimensions[width]  = frameBuffer[width];
  window_dimensions[height] = frameBuffer[height];
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
        location[y] += 0.01f;
    if (location[y] >= 1.4f)
        location[y] =  1.4f;
  }

  if  (glfwGetKey (window, GLFW_KEY_DOWN)   == GLFW_PRESS){
        location[y] -= 0.01f;
    if (location[y] <= -1.4f)
        location[y] =  -1.4f;
  }

  if  (glfwGetKey (window, GLFW_KEY_RIGHT)  == GLFW_PRESS){
        location[x] += 0.01f;
    if (location[x] >= 1.4f)
        location[x] =  1.4f;
  }

  if  (glfwGetKey (window, GLFW_KEY_LEFT)   == GLFW_PRESS){
        location[x] -= 0.01f;
    if (location[x] <= -1.4f)
        location[x] = -1.4f;
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
        fov += glm::radians(1.0f);
    if (fov >= glm::radians(180.0f))
        fov =  glm::radians(180.0f);
  }

  if  (glfwGetKey (window, GLFW_KEY_Z)   == GLFW_PRESS){
        fov -= glm::radians(1.0f);
    if (fov <= glm::radians(1.0f))
        fov =  glm::radians(1.0f);
  }
}
