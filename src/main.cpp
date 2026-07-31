#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//------------------------------------------------------- Forward declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

//---------------------------------------------------------------- Vertex data

//Normalized Device Coordinates (NDC)
float vertices[] = {
     -.5f,  -.5f,  0.0f, // Bottom left
     0.5f,  -.5f,  0.0f, // Bottom right
     0.0f,  0.5f,  0.0f  // Top
};

//-------------------------------------------------------------------- Shaders

//Vertex Shader written in GLSL and stored as a c string litteral...
const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//Fragment Shader written in GLSL and stored as a c string litteral...
const char *fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n";

//----------------------------------------------------------------------- Main
int main (){
  
  std::cout << "Starting OpenGL Renderer..." << std::endl;
  
  //------------------------------------------------------- 1. Initialize GLFW
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
  
  //--------------------------------------------------------- 2. Create Window
  
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

  //------------------------------------------- 3. Make OpenGL Context Current
  
  //make the window context active on the current thread
  glfwMakeContextCurrent(window);
  
  //------------------------------------------------------- 4. Initialize GLAD 
  
  //Load the openGL function pointers with GLAD..
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cerr << "Failed to initialize GLAD\n";
      glfwTerminate();
      return -1;
  }

  //------------------------------------------------- 5. Create OpenGL Objects 
  
  //VAO "Vertex Array Object"
  unsigned int VAO;
  glGenVertexArrays(1, &VAO); 

  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  
  // VBO "Vertex Buffer Object"
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // Upload vertex data to the VBO object
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

  // Tell OpenGL how the vertex data is laid out
  glVertexAttribPointer(
        0,                    // attribute location
        3,                     // 3 values per vertex
        GL_FLOAT,              // each value is a float
        GL_FALSE,        // don't normalize
        3 * sizeof(float),   // size of one vertex
        (void*)0            // starts at beginning
  );
  
  // Enable attribute location 0
  glEnableVertexAttribArray(0);

  //-------------------------------------------------- 6. Create Vertex Shader 
  
  // Create an empty shader object with an id (vertexShader itself is the id ie: 15)
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Take shader #15 and give it this GLSL source code.
  glShaderSource  (vertexShader, 1, &vertexShaderSource, NULL);
  
  // Compile the GLSL source into GPU-executable code.
  glCompileShader (vertexShader);

  // Let's check for compile-time errors with the shader
  int   success;
  char  infoLog[512];
  glGetShaderiv   (vertexShader, GL_COMPILE_STATUS, &success);

  if  (!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //------------------------------------------------ 7. Create Fragment Shader 

  // Create an empty shader object with an id (FragmentShader itself is the id ie: 15)
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Take shader #15 and give it this GLSL source code.
  glShaderSource  (fragmentShader, 1, &fragmentShaderSource, NULL);

  // Compile the GLSL source into GPU-executable code.
  glCompileShader (fragmentShader);

  // Let's check for compile-time errors with the shader
  glGetShaderiv   (fragmentShader, GL_COMPILE_STATUS, &success);

  if  (!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //------------------------------------------------- 8. Create Shader Program
  
  //same again that the shaderProgram is an id so ie:15.
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  //attatch the shaders and then link them into the shader program, a bit like .o files in cpp
  glAttachShader  (shaderProgram, vertexShader);
  glAttachShader  (shaderProgram, fragmentShader);
  glLinkProgram   (shaderProgram);

  //again lets check for errors
  glGetProgramiv  (shaderProgram, GL_LINK_STATUS, &success);

  if  (!success) {
      glGetProgramInfoLog (shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //------------------------------------------------- 9. Delete Shader Objects
 
  //think of it like deleting .o files after linking is complete.
  glDeleteShader  (vertexShader);
  glDeleteShader  (fragmentShader);

  //--------------------------------------------------- 10. Use Shader Program
  
  glUseProgram    (shaderProgram);

  //----------------------------------------------------- 11. Set the viewport 
  
  //glViewport(0, 0, 800, 600);
  int fbWidth, fbHeight;
  glfwGetFramebufferSize  (window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);


  //-------------------------------------------------- 12. Implamentation info
 
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
  
  //---------------------------------------------------------- 13. Render Loop
  
  /* Main "render loop":
   * The glfwWindowShouldClose function checks at the start of each loop 
   * iteration if GLFW has been instructed to close. If so, the function 
   * returns true and the render loop stops running, after which we can  
   * close the application.                                              */

  //The glfwPollEvents function checks if any events are triggered
  //(like keyboard input or mouse movement events)
  
  //The glfwSwapBuffers will swap the color buffer
  //(a large 2D buffer that contains color values for each pixel in GLFW's window)
  
  while (!glfwWindowShouldClose(window))
  {
      // input
      processInput(window);

      // rendering commands here 
      glClearColor  (0.2f, 0.3f, 0.3f, 1.0f);
      glClear       (GL_COLOR_BUFFER_BIT);

      /*DRAW OUR FIRST TRIANGLE*/
      // Use our shader program
      glUseProgram  (shaderProgram);

      // Bind the VAO containing our vertex configuration
      glBindVertexArray(VAO);

      // Draw 3 vertices as a triangle
      glDrawArrays  (GL_TRIANGLES, 0, 3);

      // check and call events and swap the buffers / present the finished fram.
      glfwSwapBuffers (window);

      //clear input poll events (needed for inputs)
      glfwPollEvents();
  }

  /* Double buffer:
   * When an application draws in a single buffer the resulting image may display flickering
   * issues. This is because the resulting output image is not drawn in an instant,
   * but drawn pixel by pixel and usually from left to right and top to bottom.
   * Because this image is not displayed at an instant to the user while still being rendered 
   * to, the result may contain artifacts. To circumvent these issues, windowing applications
   * apply a double buffer for rendering. The front buffer contains the final output image
   * that is shown at the screen, while all the rendering commands draw to the back buffer.
   * As soon as all the rendering commands are finished we swap the back buffer to the 
   * front buffer so the image can be displayed without still being rendered to,
   * removing all the aforementioned artifacts.*/

  //-------------------------------------------------------------- 14. Cleanup
  
  //clear/handle all allocated memory free, close... etc
  glfwTerminate(); 

  return 0;
}

//------------------------------------------------------- non-member functions

//window resize update function
void framebuffer_size_callback  (GLFWwindow* window, int width, int height)
{
  int fbWidth, fbHeight;
  glfwGetFramebufferSize  (window,    &fbWidth,&fbHeight);
  glViewport              (0, 0, fbWidth, fbHeight);

  //glViewport(0, 0, width, height);
   // std::cout 
   //   << "viewport resize: " 
   //   << "width: "
   //   << width
   //   << ", "
   //   << "height: "
   //   << height
   //   << std::endl;
}

// Key Hooks
void processInput(GLFWwindow *window)
{
  if  (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}
