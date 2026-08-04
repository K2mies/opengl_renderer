#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

//---------------------------------------------------------- Forward declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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

 //----------------------------------------------------- 6. Create Vertex Shader 
  
  Shader shader (
      "../assets/shaders/basic.vert",
    "../assets/shaders/basic.frag"
  ); 

  //--------------------------------------------- 6. Build/Compile Shader Program
  
  shader.use();

  //--------------------------------------------------------------- 7 Vertex data
  
  //Single triangle with colored verticies
  float vertices[] = {
    0.5f, -.5f,  0.0f,  1.0f,  0.0f,  0.0f,    // bottom right
    -.5f, -.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
   0.0f,0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top 
   };
  //---------------------------------------------------- 5. Create OpenGL Objects 

  //initialise all VBO's and VAO's
  unsigned int VBO, VAO;
  glGenVertexArrays (1, &VAO); // we can also generate multiple VAOs or buffers at the same time
  glGenBuffers      (1, &VBO);

  // Tiangle setup
  // --------------------
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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


  //-------------------------------------------------------- 11. Set the viewport 
  
  //glViewport(0, 0, 800, 600);
  int fbWidth, fbHeight;
  glfwGetFramebufferSize  (window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  //------------------------------------------------------- 12. Rendering Settings

  // Render points as 10x10 pixels
  glPointSize(40.0f);


  //----------------------------------------------------- 13. Implamentation info
 
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
  //------------------------------------------------------------- 14. Render Loop

  while (!glfwWindowShouldClose(window))
  {
      // Input
      processInput(window);

      // Set Background Color 
      glClearColor  (0.2f, 0.3f, 0.3f, 1.0f);
      glClear       (GL_COLOR_BUFFER_BIT);

      // Use our shader program
      shader.use();
  
      // Draw a triangle
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_POINTS, 0, 3);

      // Swap the buffers / present the finished frame.
      glfwSwapBuffers (window);

      // Clear input poll events (needed for inputs)
      glfwPollEvents();
  }

  //----------------------------------------------------------------- 15. Cleanup

  // Destroy vertex buffer, Array , Element buffer object and program
  glDeleteVertexArrays  (1, &VAO);
  glDeleteBuffers       (1, &VBO);

  // Clear/handle all allocated memory free, close... etc for GLFW
  glfwTerminate(); 
  return 0;
}

//---------------------------------------------------------- non-member functions

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
}
