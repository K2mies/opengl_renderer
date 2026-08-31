#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//---------------------------------------------------------- Forward declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

//------------------------------------------------------------------- Vertex data

//Normalized Device Coordinates (NDC) for a triangle with RGB values
float triangle_vertices[] = {
      -.5f,  -.5f,  0.0f,  1,  0,  0,   // Bottom left
      0.5f,  -.5f,  0.0f,  0, 1, 0,   // Bottom right
     0.0f, 0.5f, 0.0f, 0, 0, 1    // Top
};

////Unique vertex positions (so they do not duplicate over each other)
//float rectangle_vertices[] = {
//    0.5f,  0.5f,   0.0f,  // top right
//    0.5f,  -.5f,   0.0f,  // bottom right
//    -.5f,  -.5f,   0.0f,  // bottom left
//    -.5f,  0.5f,  0.0f   // top left 
//};

//Unique vertex positions (so they do not duplicate over each other)
float rectangle_vertices[] = {
     0.5f,  0.5f,   0.0f,  0,  0,  1,  // top right
     0.5f,  -.5f,   0.0f,  0, 1, 0,  // bottom right
    -.5f, -.5f,  0.0f, 0, 0, 1,  // bottom left
    -.5f,  0.5f, 0.0f, 1, 0, 0   // top left 
};

//indices for index drawing
unsigned int rectangle_indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

//vertix positions for two triangles next to each other
float two_triangle_vertices[] = {
     // first triangle
     -.9f,  -.5f, 0.0f, 0,  0,  1,  // left 
     -.0f,  -.5f, 0.0f, 0, 1, 0,  // right
    -.45f,0.5f,0.0f,1, 0, 0,  // top 
     // second triangle
    0.0f, -.5f,0.0f,1, 0, 0,  // left
    0.9f, -.5f,0.0f,0, 1, 0,  // right
    .45f, 0.5f,0.0f,0, 0, 1   // top 
 };
//First Tirangle
float firstTriangle[] = {
  -.9f, -.5f,  0.0f, 0,  0, 1,  // left 
  -.0f, -.5f,  0.0f, 0, 1,0,  // right
 -.45f,0.5f,0.0f,1, 0,0   // top 
};

//Second Triangle
float secondTriangle[] = {
  0.0f,  -.5f, 0.0f, 1,  0,  0,  // left
  0.9f,  -.5f, 0.0f, 0, 1, 0,  // right
 0.45f, .5f,0.0f,0, 0, 1   // top 
};

//----------------------------------------------------------------------- Shaders

//Vertex Shader written in GLSL and stored as a c string litteral...
//TRIANGLE WITH COLORS
const char *vertexShaderSource = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec3 aColor;\n"

  "out vec3 ourVertexColor;\n"

  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "   ourVertexColor = aColor;\n"
  "}\0";

////Fragment Shader written in GLSL and stored as a c string litteral...
//const char *fragmentShaderSource =
//  "#version 330 core\n"
//  "in  vec3 ourColor;\n"
//  "out vec4 FragColor;\n"
//  "void main()\n"
//  "{\n"
//  " FragColor = vec4(ourColor, 1.0);\n"
//  "}\0";

//Fragment Shader that uses a uniform input
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in  vec3 ourVertexColor;\n"
"uniform vec4 ourColor;\n"

"void main()\n"
"{\n"
"    FragColor = ourColor + vec4(ourVertexColor, 1.0) / 2.0;\n"
"}\0";

////Vertex Shader written in GLSL and stored as a c string litteral...
//const char *vertexShaderSource = 
//  "#version 330 core\n"
//  "layout (location = 0) in vec3 aPos;\n"
//
//  "void main()\n"
//  "{\n"
//  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//  "}\n";
//
////Fragment Shader written in GLSL and stored as a c string litteral...
//const char *fragmentShaderSource =
//  "#version 330 core\n"
//  "out vec4 FragColor;\n"
//  "void main()\n"
//  "{\n"
//  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//  "}\n";

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

  //---------------------------------------------------- 5. Create OpenGL Objects 

  //initialise all VBO's and VAO's
  unsigned int VBOs[2], VAOs[2];
  glGenVertexArrays (2, VAOs); // we can also generate multiple VAOs or buffers at the same time
  glGenBuffers      (2, VBOs);

  // first triangle setup
  // --------------------
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER,
                 sizeof(firstTriangle), 
                 firstTriangle, 
                GL_STATIC_DRAW);
  glVertexAttribPointer(
    0, 
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    (void*)0
  );
  glVertexAttribPointer(
    1, 
    3, 
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    (void*)(3 * sizeof(float))
  );
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // second triangle setup
  // --------------------
  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(secondTriangle), 
                 secondTriangle, 
                GL_STATIC_DRAW);
  glVertexAttribPointer(
    0, 
    3, 
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    (void*)0
  );	
  glVertexAttribPointer(
    1, 
    3, 
    GL_FLOAT, 
    GL_FALSE, 
    6 * sizeof(float), 
    (void*)(3 * sizeof(float))
  );	
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);


  //----------------------------------------------------- 6. Create Vertex Shader 
  
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

  //--------------------------------------------------- 7. Create Fragment Shader

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

  //---------------------------------------------------- 8. Create Shader Program
  
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

  //---------------------------------------------------- 9. Delete Shader Objects
 
  //think of it like deleting .o files after linking is complete.
  glDeleteShader  (vertexShader);
  glDeleteShader  (fragmentShader);

  //------------------------------------------------------ 10. Use Shader Program
  
  glUseProgram    (shaderProgram);

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
  std::cout << "Maximum vertex attributes: "
            << nrAttributes << std::endl;
  //------------------------------------------------------------- 14. Render Loop
  
  /* Main "render loop":
   * The glfwWindowShouldClose function checks at the start of each loop 
   * iteration if GLFW has been instructed to close. If so, the function 
   * returns true and the render loop stops running, after which we can  
   * close the application.                                              */

  //The glfwPollEvents function checks if any events are triggered
  //(like keyboard input or mouse movement events)
  
  //The glfwSwapBuffers will swap the color buffer
  //(a large 2D buffer that contains color values for each pixel in GLFW's window)
  
  // Set draw mode to polygon wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
      // Input
      processInput(window);

      // Rendering commands here 
      glClearColor  (0.2f, 0.3f, 0.3f, 1.0f);
      glClear       (GL_COLOR_BUFFER_BIT);

      /* DRAW OUR FIRST TRIANGLE*/
      // Use our shader program
      glUseProgram  (shaderProgram);

      // Bind the VAO containing our vertex configuration
      //glBindVertexArray(VAO);

      // Draw points in place of vertices (see no.12)...
      //glDrawArrays(GL_POINTS, 0, 3);

      // Draw 3 triangle_vertices as a triangle
      //glDrawArrays  (GL_TRIANGLES, 0, 3);
      
      // Draw 2 triangles
      //glDrawArrays  (GL_TRIANGLES, 0, 6);

      // Draw a Rectangle
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      //glBindVertexArray(0);
      
      // Draw first triangle using the first VAO 1:
      glBindVertexArray(VAOs[0]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_POINTS, 0, 3);

      // Draw second triangle using the first VAO 1:
      glBindVertexArray(VAOs[1]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_POINTS, 0, 3);

      // Here we are effecting and passing the uniform to the shader
      float timeValue = glfwGetTime();
      float redValue = (sin(timeValue) / 2.0f) + 0.5f;
      int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
      glUseProgram(shaderProgram);
      glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

      // Swap the buffers / present the finished frame.
      glfwSwapBuffers (window);

      // Clear input poll events (needed for inputs)
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

  //----------------------------------------------------------------- 15. Cleanup

  // Destroy vertex buffer, Array , Element buffer object and program
  glDeleteVertexArrays  (2, VAOs);
  glDeleteBuffers       (2, VBOs);
  //glDeleteBuffers       (1, &EBO);
  glDeleteProgram       (shaderProgram);

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
