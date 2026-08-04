#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

//------------------------------------------------------------------------- Enums
enum AxisType
{
    x = 0,
    y = 1,
    z = 2,
    w = 3
};

enum BufferType
{
    VBO = 0,
    VAO = 1,
    EBO = 2
};

//--------------------------------------------------------------------- Variables
float pointSize = 40.0f;

//---------------------------------------------------------- Forward declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void implamentation_info();

//-------------------------------------------------------------------------- Main

int main (){
  
  std::cout << "Starting OpenGL Renderer..." << std::endl;
  
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
   800,
   600,
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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //---------------------------------------------- 3. Make OpenGL Context Current
  
  //make the window context active on the current thread
  glfwMakeContextCurrent(window);
  
  //---------------------------------------------------------- 4. Initialize GLAD 
  
  //Load the openGL function pointers with GLAD..
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cerr << "Failed to initialize GLAD\n";
      glfwTerminate();
      return -1;
  }

 //----------------------------------------------------- 5. Create Vertex Shader 
  
  Shader shaderProgram (
      "../assets/shaders/basic.vert",
    "../assets/shaders/basic.frag"
  ); 

  //--------------------------------------------- 6. Build/Compile Shader Program
  
  shaderProgram.use();

  //--------------------------------------------------------------- 7 Vertex data
  
  //Single triangle with colored verticies
  float vertices[] = {
    0.5f, -.5f,  0.0f,  1.0f,  0.0f,  0.0f,  // bottom right
    -.5f, -.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
   0.0f,0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top 
   };
  //---------------------------------------------------- 8. Create OpenGL Objects 

  //initialise all VBO's and VAO's as openGL objects
  unsigned int objects[2];

  glGenVertexArrays (1, &objects[VAO]); // we can also generate multiple VAOs or buffers at the same time
  glGenBuffers      (1, &objects[VBO]);

  // Tiangle setup
  // --------------------
  glBindVertexArray(objects[VAO]);
  glBindBuffer(GL_ARRAY_BUFFER, objects[VBO]);
  glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices), 
                 vertices, 
                GL_STATIC_DRAW);

  glVertexAttribPointer(
    0, 
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    (void*)0
  );

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 
    3, 
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    (void*)(3 * sizeof(float))
  );

  glEnableVertexAttribArray(1);


  //--------------------------------------------------------- 9. Set the viewport 
  
  //glViewport(0, 0, 800, 600);
  int fbWidth, fbHeight;
  glfwGetFramebufferSize  (window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  //------------------------------------------------------- 10. Rendering Settings

  // Render points as 10x10 pixels
  glPointSize(pointSize);


  //----------------------------------------------------- 11. Implamentation info
 
  //print useful information about the openGL implamentation
  implamentation_info();
  //------------------------------------------------------------- 12. Render Loop

  while (!glfwWindowShouldClose(window))
  {
      // Input
      processInput(window);

      // Set Background Color 
      glClearColor  (0.2f, 0.3f, 0.3f, 1.0f);
      glClear       (GL_COLOR_BUFFER_BIT);

      // Use our shader program
      shaderProgram.use();
      
      glPointSize(pointSize);

      // Feed the uniform location
      float time = glfwGetTime();
      float location[4];

      location[x] = (sin(time) / 2.0f) + 0.5f;
      location[y] = 0.0f;
      location[z] = 0.0f;
      location[w] = 1.0f;

      shaderProgram.setVec4(
        "ourVertexLocation",
        location[x],
        location[y],
        location[z],
        location[w]
      );
  
      // Draw a triangle
      glBindVertexArray(objects[VAO]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_POINTS, 0, 3);

      // Swap the buffers / present the finished frame.
      glfwSwapBuffers (window);

      // Clear input poll events (needed for inputs)
      glfwPollEvents();
  }

  //----------------------------------------------------------------- 13. Cleanup

  // Destroy vertex buffer, Array , Element buffer object and program
  glDeleteVertexArrays  (1, &objects[VAO]);
  glDeleteBuffers       (1, &objects[VBO]);

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
void framebuffer_size_callback  (GLFWwindow* window, int width, int height)
{
  int fbWidth, fbHeight;
  glfwGetFramebufferSize  (window,    &fbWidth,&fbHeight);
  glViewport              (0, 0, fbWidth, fbHeight);
}

// Key Hooks
void processInput(GLFWwindow *window)
{
  if  (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  if  (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
    pointSize += 0.1f;
    if (pointSize < 20.0f){
      pointSize = 20.0f;
    }
  }
  if  (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
    pointSize -= 0.1f;
    if (pointSize < 20.0f){
      pointSize = 20.0f;
    }
  }
}
