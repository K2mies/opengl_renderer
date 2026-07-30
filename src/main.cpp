#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//------------------------------------------------------- Forward declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main (){
  
  std::cout << "Starting OpenGL Renderer..." << std::endl;
  
  //if initialization fails then return -1
  if (!glfwInit()){
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  //Create a window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif
  
  //create the actual window
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

  //make the window context active on the current thread
  glfwMakeContextCurrent(window);

  //Load the openGL function pointers with GLAD..
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cerr << "Failed to initialize GLAD\n";
      glfwTerminate();
      return -1;
  }

  //set the viewport
  glViewport(0, 0, 800, 600);


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
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // check and call events and swap the buffers
      glfwSwapBuffers(window);
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

  //clear/handle all allocated memory free, close... etc
  glfwTerminate(); 

  return 0;
}

//window resize update function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}
