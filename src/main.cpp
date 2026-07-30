#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main (){
  
  std::cout << "Starting OpenGL Renderer..." << std::endl;
  
  //if initialization fails then return -1
  if (!glfwInit()){
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  //Create a window
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  //create the actual window
  GLFWwindow* window = glfwCreateWindow(
   800,
   600,
   "OpenGL Renderer",
   nullptr,
   nullptr
  );

  const char* description;
  int error = glfwGetError(&description);
  
  if (error != GLFW_NO_ERROR)
  {
      std::cerr << "GLFW Error " << error << ": " << description << '\n';
  }

  //Check if window creation failed...
  if (window == nullptr)
  {
      std::cerr << "Failed to create GLFW window\n";
      glfwTerminate();
      return -1;
  }

  //make the window context active
  glfwMakeContextCurrent(window);

  //Load the openGL function pointers with GLAD..
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cerr << "Failed to initialize GLAD\n";
      glfwTerminate();
      return -1;
  }

  std::cout << "GLFW initialized successfully\n";

  //Close GLFW window
  while (!glfwWindowShouldClose(window))
  {
      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  //clear all allocated memory free, close... etc
  glfwTerminate(); 

  return 0;
}
