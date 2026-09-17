// Imported libraries

// Glad/GLFW/stb_image/assimp 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "glm/ext/matrix_clip_space.hpp"
#include "stb_image.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include <assimp/version.h>

// Objects / Classes
#include "Objects/Model.h"
#include "Objects/Mesh.h"
#include "Objects/Shader.h"
#include "Objects/Texture.h"

// Shader Typedefs
#include "Typedefs/ShaderTypes.h"

// Scene Objects
#include "Scene/Camera.h"

// Custom Types
#include "Typedefs/Vec4.h"
#include "Typedefs/Vec3.h"
#include "Typedefs/Vec2.h"
#include "Typedefs/Mat4.h"
#include "Typedefs/Mat3.h"
#include "Typedefs/Mat2.h"

// custom libraries
#include "Utils/MathUtils.h"

// Imports
#include <iostream>

//------------------------------------------------------------------------ Macros
#define   NUM_OF_POINT_LIGHTS 4
#define   NUM_OF_CUBES        10

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


struct Orbit
{
    float yaw        = 90.0f;
    float pitch      = 0.0f;
    float radius     = 3.0f;

    float sensitivity = 0.15f;
    float zoom_speed  = 0.25f;

    vec3 target = vec3(
        0.0f,
        0.29f,
        0.0f
    );
};

Matrix      matrix;

Time        timer;

Euler       euler;

Color       color;

Material    material;

Light       light;

SunLight    sunlight;

SpotLight   spotlight;

PointLight  pointlights[4];

Orbit orbit;

Projection  projection;

//-------------------------------------------------------------- Global Variables

float point_world_size;

int   window_dimensions[2];

float location[4];
float rotation[3];
float scale[3];

float fov;

float last[2];
bool  firstMouse = true;

vec3  position       = vec3(0.0f, 0.0f, 3.0f);
vec3  up             = vec3(0.0f, 1.0f, 0.0f);

vec3  light_position = vec3(1.2f, 1.0f, 2.0f);

float cutoff;
float outer_cutoff;

float orthographic_size = 1.0f;

ProjectionType  projection_type   = perspective;
//---------------------------------------------------------------- Global Objects
Camera camera(position, up, YAW, PITCH);

//---------------------------------------------------------- Forward declarations

void framebuffer_size_callback  (GLFWwindow *window, int /*width*/, int /*height*/);
void scroll_callback            (GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback             (GLFWwindow* window, double xpos, double ypos);
void processInput               (GLFWwindow *window);
void implamentation_info        ();
void updateOrbitCamera          ();

//-------------------------------------------------------------------------- Main

int main (){ 

  //----------------------------------------------------------------------- TESTS
  std::cout
    << "Assimp: "
    << aiGetVersionMajor() << '.'
    << aiGetVersionMinor() << '.'
    << aiGetVersionPatch()
    << '\n';

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

  point_world_size  = 0.014f;

  fov               = math::radians(45.0f);

  cutoff            = 12.5f;
  outer_cutoff      = 17.5f; 

  //weight.blend      = 0.5f;

  //weight.texture    = 0.5f;
  //weight.vertex     = 0.25f;
  //weight.location   = 0.25f;

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

  //color.object      = vec3(1.0f, 0.5f, 0.31f);
  //color.light       = vec3(1.0f, 1.0f, 1.0f);

  light.position    = vec4(1.2f, 1.0f, 2.0f);

  //---------------------------------------------------------- 1. Initialize GLFW

  if (!glfwInit()){
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  // Initialize GLWF window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

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
  
  // Mouse movement rotates around the target.
  glfwSetCursorPosCallback(window, mouse_callback);

  // Scroll changes the orbit radius.
  glfwSetScrollCallback(window, scroll_callback);
  
  //---------------------------------------------------------- 4. Initialize GLAD 
  
  //Load the openGL function pointers with GLAD..
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cerr << "Failed to initialize GLAD\n";
      glfwTerminate();
      return -1;
  }
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_FRAMEBUFFER_SRGB);
 //---------------------------------------------------- 5. Create Shader Program
    
  //Shader lightShader (
  //  "../assets/Shaders/light.vert",
  //  "../assets/Shaders/light.frag"
  //);

  //Shader model_shader (
  //  "../assets/Shaders/directional_light.vert",
  //  "../assets/Shaders/directional_light.frag"
  //);
  
  //Shader model_shader (
  //  "../assets/Shaders/point_light.vert",
  //  "../assets/Shaders/point_light.frag"
  //);

  //Shader model_shader (
  //  "../assets/Shaders/flash_light.vert",
  //  "../assets/Shaders/flash_light.frag"
  //);
  
  //Shader model_shader (
  //  "../assets/Shaders/multiple_lights.vert",
  //  "../assets/Shaders/multiple_lights.frag"
  //);

  //Shader model_shader (
  //  "../assets/Shaders/gouraud_flash_light.vert",
  //  "../assets/Shaders/gouraud_flash_light.frag"
  //);
  
  //Shader model_shader  (
  //  "../assets/Shaders/temp.vert",
  //  "../assets/Shaders/temp.frag"
  //);


  // GOURAUD SHADING
  //Shader model_shader (
  //  "../assets/Shaders/gouraud.vert",
  //  "../assets/Shaders/gouraud.frag"
  //);

  //Shader model_shader (
  //  "../assets/Shaders/model.vert",
  //  "../assets/Shaders/model.frag"
  //);
  Shader model_shader (
    "../assets/Shaders/point_cloud.vert",
    "../assets/Shaders/point_cloud.frag"
  );
  //------------------------------------------------------------- 5. Model Loader
  //OBJ
  //Model  loaded_model("../assets/Models/backpack/backpack.obj");
  //Model loaded_model("../assets/Models/Cube.obj");
  
  //GLTF
  //Model loaded_model("../assets/Models/testbox/Box.glb");
  //Model loaded_model("../assets/Models/birb/birb_rgba.glb");
  Model loaded_model("../assets/Models/birb/birb_rigged_test_001.glb");
  
  //PLY
  //Model loaded_model("../assets/Models/birb/birb_001.ply");

  // configure global opengl state
  // -----------------------------
  //  glEnable(GL_DEPTH_TEST);

  //--------------------------------------------------------------- 6 Vxertex data

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  //float vertices[] = {
  //  // positions          // normals           // texture coords
  //  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
  //   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
  //   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
  //   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
  //  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
  //  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

  //  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
  //   0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
  //   0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
  //   0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
  //  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
  //  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

  //  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  //  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
  //  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  //  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  //  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
  //  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

  //   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  //   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
  //   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  //   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  //   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
  //   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

  //  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
  //   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
  //   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
  //   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
  //  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
  //  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

  //  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
  //   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
  //   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
  //   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
  //  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
  //  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  //};

  //unsigned int indices[] =
  //{
  //    0, 1, 2, 2, 3, 0,
  //    6, 7, 8, 8, 9, 6,
  //    12,13,14,14,15,12,
  //    18,19,20,20,21,18,
  //    24,25,26,26,27,24,
  //    30,31,32,32,33,30
  //};

  //vec3 cubePositions[] = {
  //  vec3( 0.0f,  0.0f,  0.0f),
  //  vec3( 2.0f,  5.0f, -15.0f),
  //  vec3(-1.5f, -2.2f, -2.5f),
  //  vec3(-3.8f, -2.0f, -12.3f),
  //  vec3( 2.4f, -0.4f, -3.5f),
  //  vec3(-1.7f,  3.0f, -7.5f),
  //  vec3( 1.3f, -2.0f, -2.5f),
  //  vec3( 1.5f,  2.0f, -2.5f),
  //  vec3( 1.5f,  0.2f, -1.5f),
  //  vec3(-1.3f,  1.0f, -1.5f)
  //};

  //vec3 pointLightPositions[] = {
	//  vec3( 0.7f,  0.2f,  2.0f),
	//  vec3( 2.3f, -3.3f, -4.0f),
	//  vec3(-4.0f,  2.0f, -12.0f),
	//  vec3( 0.0f,  0.0f, -3.0f)
  //};  
  //---------------------------------------------------- 7. Create OpenGL Objects 

  // CUBE OBJECT---------------------------------------------
  //unsigned int cube_objs[3];

  //glGenVertexArrays (1, &cube_objs[VAO]);
  //glGenBuffers      (1, &cube_objs[VBO]);
  //glGenBuffers      (1, &cube_objs[EBO]);

  //// Setup
  //// --------------------
  //glBindVertexArray (cube_objs[VAO]);

  //glBindBuffer      (GL_ARRAY_BUFFER,   cube_objs[VBO]);
  //glBufferData      (GL_ARRAY_BUFFER,
  //                                      sizeof(vertices), 
  //                                      vertices, 
  //                                      GL_STATIC_DRAW);


  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_objs[EBO]);
  //
  //glBufferData(
  //    GL_ELEMENT_ARRAY_BUFFER,
  //    sizeof(indices),
  //    indices,
  //    GL_STATIC_DRAW
  //);

  //// Attribute #0 (LOCATION) :
  //glVertexAttribPointer (
  //  0, 
  //  3,
  //  GL_FLOAT,
  //  GL_FALSE,
  //  8 * sizeof(float),
  //  (void*)0
  //);

  //glEnableVertexAttribArray (0);

  // // Attribute #1 (NORMAL) :
  //glVertexAttribPointer (
  //  1, 
  //  3,
  //  GL_FLOAT,
  //  GL_FALSE,
  //  8 * sizeof(float),
  //  (void*)(3 * sizeof(float))
  //);

  //glEnableVertexAttribArray (1);

  //// Attribute #2 (UV) :
  //glVertexAttribPointer (
  //  2,
  //  2,
  //  GL_FLOAT,
  //  GL_FALSE,
  //  8 * sizeof(float),
  //  (void*)(6 * sizeof(float))
  //);

  //glEnableVertexAttribArray (2);


  //// LIGHT OBJECT--------------------------------------------
  //unsigned int light_objs[3];

  //glGenVertexArrays (1, &light_objs[VAO]);
  //glGenBuffers      (1, &light_objs[VBO]);
  //glGenBuffers      (1, &light_objs[EBO]);

  //glBindVertexArray (    light_objs[VAO]);

  //glBindBuffer      (GL_ARRAY_BUFFER,   light_objs[VBO]);
  //glBufferData      (GL_ARRAY_BUFFER,
  //                                      sizeof(vertices), 
  //                                      vertices, 
  //                                      GL_STATIC_DRAW);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_objs[EBO]);
  //
  //glBufferData(
  //    GL_ELEMENT_ARRAY_BUFFER,
  //    sizeof(indices),
  //    indices,
  //    GL_STATIC_DRAW
  //);
  //
  //// Attribute #0 (LOCATION) :
  //glVertexAttribPointer (
  //  0, 
  //  3,
  //  GL_FLOAT,
  //  GL_FALSE,
  //  8 * sizeof(float),
  //  (void*)0
  //);

  //glEnableVertexAttribArray(0);

 

  ////// Unbind
  //glBindBuffer(GL_ARRAY_BUFFER, 0);
  //glBindVertexArray            (0);

 
  //----------------------------------------------------------------- 8. Textures 
  
  //Texture diffuseMap ("../assets/Textures/container2.png");
  //Material material;
  //material.diffuse.load ("../assets/Textures/diffuse.png");
  //material.specular.load("../assets/Textures/specular.png");
  //material.emission.load("../assets/Textures/emission.jpg");
  ////material.specular.load("../assets/Textures/color_specular.png");
  //material.shininess = 32.0f;

  //--------------------------------------------- 9. Build/Compile Shader Program


  //-------------------------------------------------------- 10. Set the viewport 
  
  int                      frameBuffer[2];
  glfwGetFramebufferSize   (window, &frameBuffer[width], &frameBuffer[height]);
  glViewport               (0, 0,    frameBuffer[width],  frameBuffer[height]);


  //------------------------------------------------------- 11. Rendering Settings

  // Render points as 10x10 pixels
  //glPointSize (pointSize);

  //----------------------------------------------------- 12. Implamentation info
 
  //print useful information about the openGL implamentation
  implamentation_info();

  //------------------------------------------------------- 13. Projection matrix
  
  float aspect;
        aspect = static_cast<float>(window_dimensions[width])
               / static_cast<float>(window_dimensions[height]);

  float half[2];
        half[height] = orthographic_size;
        half[width]  = orthographic_size * aspect;

  frustum.left   = -half[width];
  frustum.right  =  half[width];
  frustum.bottom = -half[height];
  frustum.top    =  half[height];

  mat4 projection_mode[2];
  projection_mode[orthographic] = mat4::ortho        (frustum.left,
                                                      frustum.right,
                                                      frustum.bottom,
                                                      frustum.top,
                                                      frustum.near,
                                                      frustum.far
                                                     );

  projection_mode[perspective]  = mat4::perspective  (fov, 
                                                     (float)window_dimensions[width] 
                                                   / (float)window_dimensions[height],
                                                      frustum.near,
                                                      frustum.far
                                                     );
  
  // select which perspective projection matrix to use:
  matrix.projection = projection_mode[projection_type];

 
  // activate the shader 
  model_shader.use();

  // Pass the coordinate matricies to the shader.
  model_shader.setMatrix      ("matrix", matrix);

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
      //glClearColor        (0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Update the point size
      //glPointSize         (pointSize);

      // Use our Lightshader program
      model_shader.use();

      // Bind Textures to GL_TEXTURE0 + unit.
      //material.diffuse.bind   (0);
      //material.specular.bind  (1);
      //material.emission.bind  (2);

      //model_shader.setInt    ("material.diffuse",  0);
      //model_shader.setInt    ("material.specular", 1);
      //model_shader.setInt    ("material.emission", 2);
      //model_shader.setFloat  ("material.shininess", material.shininess);

      //// Set Camera as viewPosition
      //model_shader.setVec3   ("viewPosition", camera.position);


      // LIGHT'S -------------------------------------------------------
      
      //float time;
      //      time = glfwGetTime();
      //      //time = math::radians(time);
      //      time = sin(time);

      //float axis[3];
      //      axis[x] = 1.0f;
      //      axis[y] = 0.0f;
      //      axis[z] = 0.0f;
      //
      //// Sunlight
      //sunlight.direction = vec4::direction(-0.2f, 1.0f, -0.3f);
      //sunlight.direction = mat4::rotate(time, axis) * sunlight.direction;
      //sunlight.ambient   = vec3(0.0f, 0.0f, 0.2f);
      //sunlight.diffuse   = vec3(0.0f, 0.0f, 1.0f);
      //sunlight.specular  = vec3(0.0f, 0.0f, 1.0f);

      //sunlight.ambient   = vec3(0.05f, 0.05f, 0.05f);
      //sunlight.diffuse   = vec3(0.4f , 0.4f , 0.4f);
      //sunlight.specular  = vec3(0.5f, 0.5f, 0.5f);

      //model_shader.setSunLight("sunlight", sunlight);

      // PointLight;
      
      //spotlight.position  = vec4::position(camera.position);
      //spotlight.direction = vec4::direction(camera.front);
      //
      //spotlight.ambient   = vec3(0.0f, 0.0f, 0.0f);
      //spotlight.diffuse   = vec3(1.0f, 1.0f, 1.0f);
      //spotlight.specular  = vec3(1.0f, 1.0f, 1.0f);

      //spotlight.constant  = 1.0f;
      //spotlight.linear    = 0.09f;
      //spotlight.quadratic = 0.032f;

      //spotlight.cutoff       = cutoff;
      //spotlight.cutoff       = math::radians(spotlight.cutoff);
      //spotlight.cutoff       = math::cos(spotlight.cutoff);

      //spotlight.outer_cutoff = outer_cutoff;
      //spotlight.outer_cutoff = math::radians(spotlight.outer_cutoff);
      //spotlight.outer_cutoff = math::cos(spotlight.outer_cutoff);
      //
      //model_shader.setSpotLight("spotlight", spotlight);

      //float   multiplier = 2.5f;
      
      // Point Lights
      
      //for (int i = 0; i < NUM_OF_POINT_LIGHTS; i++)
      //{
      //  float time;
      //        time = glfwGetTime();
      //        time = time * (i + 1);

      //        position[x] = 0.0f;
      //        position[y] = sin(time) * multiplier;
      //        position[z] = 0.0f;

      //  pointlights[i].position  = vec4::position(pointLightPositions[i] + position);

      //  pointlights[i].ambient   = vec3(0.05f, 0.05f, 0.05f);
      //  //pointlights[i].diffuse   = vec3(0.8f, 0.8f, 0.8f);
      //  pointlights[i].diffuse   = vec3(0.0f, 0.0f, 0.0f);
      //  pointlights[i].specular  = vec3(1.0f, 1.0f, 1.0f);

      //  if (i == 1 || i == 3)
      //  {
      //    pointlights[i].diffuse  = vec3(1.0f, 0.0f, 0.0f);
      //    pointlights[i].specular = vec3(1.0f, 0.0f, 0.0f);
      //    pointlights[i].ambient  = vec3(0.2f, 0.0f, 0.0f);
      //  }

      //  if (i == 0)
      //  {
      //    pointlights[i].diffuse  = vec3(0.0f, 1.0f, 0.0f);
      //    pointlights[i].specular = vec3(0.0f, 1.0f, 0.0f);
      //    pointlights[i].ambient  = vec3(0.0f, 0.2f, 0.0f);

      //  }
      //  if (i == 2)
      //  {
      //    pointlights[i].diffuse  = vec3(0.0f, 0.0f, 1.0f);
      //    pointlights[i].specular = vec3(0.0f, 0.0f, 1.0f);
      //    pointlights[i].ambient  = vec3(0.0f, 0.0f, 0.2f);

      //  }

      //  pointlights[i].constant  = 1.0f;
      //  pointlights[i].linear    = 0.09f;
      //  pointlights[i].quadratic = 0.032f;
      //
      //  model_shader.setPointLight("pointlights[" + std::to_string(i) + "]", pointlights[i]);
      //}
    
      float aspect;
            aspect   = static_cast<float>(window_dimensions[width])
                     / static_cast<float>(window_dimensions[height]);

      half[height]   = orthographic_size;
      half[width]    = orthographic_size * aspect;

      frustum.left   = -half[width];
      frustum.right  =  half[width];
      frustum.bottom = -half[height];
      frustum.top    =  half[height];

      projection_mode[orthographic] = mat4::ortho        (frustum.left,
                                                          frustum.right,
                                                          frustum.bottom,
                                                          frustum.top,
                                                          frustum.near,
                                                          frustum.far
                                                         );

      projection_mode[perspective]  = mat4::perspective  (fov, 
                                                         (float)window_dimensions[width] 
                                                       / (float)window_dimensions[height],
                                                          frustum.near,
                                                          frustum.far
                                                         );
      
      // select which perspective projection matrix to use:
      matrix.projection = projection_mode[projection_type];

      // update the orbit camera
        updateOrbitCamera();

      // update view matrix with LookAt every frame
      matrix.view   = camera.getViewMatrix();

      // Ok lets draw the object
      //------------------------------------------------ draw model
      model_shader.use();
      
      model_shader.setVec3("material.color", vec3(1.0f));
      
      matrix.model = mat4(1.0f);

      matrix.model = matrix.model * mat4::translate(vec3(0.0f));

      matrix.model = matrix.model * mat4::scale(vec3(1.0f));

      matrix.normal = mat4::normalMatrix(matrix.model);

      model_shader.setMatrix("matrix", matrix);


      // load projection properties
      //model_shader.setBool  ("orthographic_projection", projection_type == orthographic);
      //model_shader.setFloat ("orthographic_size",       orthographic_size);
      //model_shader.setFloat ("viewport_height",         static_cast<float>(window_dimensions[height]));
      //model_shader.setFloat ("point_world_size",        point_world_size);
      //model_shader.setFloat ("fov", fov);

      projection.type       = projection_type;
      projection.size       = orthographic_size;
      projection.height     = static_cast<float>(window_dimensions[height]);
      projection.point_size = point_world_size;
      projection.fov        = fov;

      model_shader.setProjection("projection", projection);
      
      // Draw the loaded model
      loaded_model.draw(model_shader);

     
      //matrix.normal = mat4::normalMatrix(matrix.model);


      //for (unsigned int i = 0; i < NUM_OF_CUBES; i++)
      //{
      //  matrix.model = mat4(1.0f);
      //  matrix.model = matrix.model
      //               * mat4::translate(cubePositions[i]);

      //  float angle  = 20.0f * i;
      //        angle  = math::radians(angle);

      //  float axis[3];
      //        axis[x] = 1.0f;
      //        axis[y] = 0.3f;
      //        axis[z] = 0.5f;

      //  matrix.model = matrix.model 
      //               * mat4::rotate(angle, vec3(axis));
      //  
      //  matrix.normal = mat4::normalMatrix(matrix.model);


      //  model_shader.setMatrix("matrix", matrix);
      //  
      //  glBindVertexArray(cube_objs[VAO]);
      //  //glDrawElements(
      //  //  GL_TRIANGLES,
      //  //  36,
      //  //  GL_UNSIGNED_INT,
      //  //  0
      //  //);
      //  glDrawArrays(GL_TRIANGLES, 0, 36);
      //  glDrawArrays(GL_POINTS, 0, 24);
      //  
      //}

      // setup the lgiht object
      //lightShader.use();

      //glBindVertexArray(light_objs[VAO]);
      //for (int i = 0; i < NUM_OF_POINT_LIGHTS; i++ )
      //{

      //  matrix.model = mat4(1.0f);
      //  matrix.model = matrix.model * mat4::translate(pointLightPositions[i]);
      //  matrix.model = matrix.model * mat4::scale(vec3(0.2f));

      //  float time;
      //        time = glfwGetTime();
      //        time = time * (i + 1);

      //        position[x] = 0.0f;
      //        position[y] = sin(time) * multiplier;
      //        position[z] = 0.0f;
      //  
      //  matrix.model = matrix.model * mat4::translate(position);

      //  lightShader.setMatrix("matrix", matrix);
      //  lightShader.setPointLight("light", pointlights[i]);

      //  glDrawArrays(GL_TRIANGLES, 0, 36);
      //  glDrawArrays(GL_POINTS, 0, 24);

      //}
      
 
      // Swap the buffers / present the finished frame.
      glfwSwapBuffers               (window);

      // Clear input poll events (needed for inputs)
      glfwPollEvents();
  }
  
  //----------------------------------------------------------------- 15. Cleanup

  // Destroy vertex buffer, Array , Element buffer object and program
  //glDeleteVertexArrays        (1, &cube_objs[VAO]);
  //glDeleteBuffers             (1, &cube_objs[VBO]);
  //glDeleteBuffers             (1, &cube_objs[EBO]);

  //glDeleteVertexArrays        (1, &light_objs[VAO]);
  //glDeleteBuffers             (1, &light_objs[VBO]);
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

void updateOrbitCamera()
{
  float yaw;
        yaw = math::radians(orbit.yaw);

  float pitch;
        pitch = math::radians(orbit.pitch);

  vec3 offset;
       offset.x = orbit.radius * math::cos(pitch) * math::cos(yaw);
       offset.y = orbit.radius * math::sin(pitch);
       offset.z = orbit.radius * math::cos(pitch) * math::sin(yaw);

  camera.position = orbit.target + offset;
  camera.front    = vec3::normalized(orbit.target - camera.position);
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

//void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
//
//  float offset[2];
//
//  offset[x] = static_cast<float>(xoffset);
//  offset[y] = static_cast<float>(yoffset);
//
//  camera.processMouseScroll(offset);
//}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  (void)window;
  (void)xoffset;

  //------------------------------------------------- change orbit distance
  orbit.radius -= static_cast<float>(yoffset) * orbit.zoom_speed;

  //----------------------------------------------- restrict orbit distance
  if (orbit.radius <= 0.1f)
    orbit.radius = 0.1f;
  if (orbit.radius >= 100.0f)
    orbit.radius = 100.0f;


}
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    // Camera mouse movement code goes here
//    float position[2];
//
//    position[x]  = static_cast<float>(xpos);
//    position[y]  = static_cast<float>(ypos);
//    
//    if (firstMouse){
//
//      last  [x]  = position[x];
//      last  [y]  = position[y];
//
//      firstMouse = false;
//
//    }
//
//    float offset[2];
//
//    offset  [x]  = position[x] - last    [x];
//    offset  [y]  = last    [y] - position[y];
//
//    last    [x]  = position[x];
//    last    [y]  = position[y];
//
//    camera.processMouseMovement(offset);
//}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  (void)window;

  //------------------------------------------------ current mouse position
  float position[2];

  position[x] = static_cast<float>(xpos);
  position[y] = static_cast<float>(ypos);

  //--------------------------------------------- initialize first position
  if (firstMouse)
  {
    last[x] = position[x];
    last[y] = position[y];

    firstMouse = false;

    return ;
  }

  //---------------------------------------------------- calculate movement
  float offset[2];

  offset[x] = position[x] - last[x];
  offset[y] = position[y] - last[y];

  //----------------------------------------------------- remember position
  last[x] = position[x];
  last[y] = position[y];

  //--------------------------------------------------- update orbit angles
  orbit.yaw   += offset[x] * orbit.sensitivity;
  orbit.pitch += offset[y] * orbit.sensitivity;

  //--------------------------------------------- stop vertical camera flip
  if (orbit.pitch >= 89.0f)
    orbit.pitch = 89.0f;
  if (orbit.pitch <= -89.0f)
    orbit.pitch = -89.0f;


}

// Key Hooks
void processInput(GLFWwindow *window)
{

  if  (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);


  // PERSPECTIVE SWITCH
  static bool p_was_pressed = false;
  const  bool p_is_pressed  = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
  if (p_is_pressed && !p_was_pressed)
  {
    if (projection_type == perspective)
      projection_type = orthographic;
    else
      projection_type = perspective;

    std::cout
        << "Projection: "
        << (projection_type == perspective
                ? "perspective"
                : "orthographic")
        << '\n';
  }

  // POINT SIZE CONTROLS
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
  {
    point_world_size -= 0.001f;
    if (point_world_size <= 0.001f)
      point_world_size = 0.001f;

  }
  
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
  {
    point_world_size += 0.001f;
    if (point_world_size >= 10.0f)
      point_world_size = 10.0f;

  }

  p_was_pressed = p_is_pressed;
  
  // FOV CONTROLS
  if  (glfwGetKey (window, GLFW_KEY_X)   == GLFW_PRESS){
        fov += math::radians(1.0f);
        orthographic_size += 0.01f;
    if (fov >= math::radians(180.0f))
        fov =  math::radians(180.0f);
  }

  if  (glfwGetKey (window, GLFW_KEY_Z)   == GLFW_PRESS){
        fov -= math::radians(1.0f);
        orthographic_size -= 0.01f;
    if (fov <= math::radians(1.0f))
        fov =  math::radians(1.0f);
  }
  
  // CAMERA MOVEMENT
  const float zoom_amount = orbit.zoom_speed * timer.delta_time;

  const float pan_speed   = 0.25f;
  const float pan_amount  = pan_speed * timer.delta_time;

  //------------------------------------------------------ forward / zoom

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
      if (projection_type == perspective)
          orbit.radius -= zoom_amount * 10.0f;
      else
          orthographic_size -= zoom_amount;
  }
  
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
      if (projection_type == perspective)
          orbit.radius += zoom_amount * 10.0f;
      else
          orthographic_size += zoom_amount;
  }

  //----------------------------------------------------------- left / right
  
  const vec3 camera_right = vec3::normalized( vec3::cross(camera.front, camera.up));
  
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
      orbit.target = orbit.target - camera_right * pan_amount;
  }
  
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
      orbit.target = orbit.target + camera_right * pan_amount;
  }
  
  //------------------------------------------------------------- up / down
  
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
      orbit.target.y += pan_amount;
  }
  
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
      orbit.target.y -= pan_amount;
  }
  
  //-------------------------------------------------------------- limits
  
  if (orbit.radius < 0.1f)
      orbit.radius = 0.1f;
  
  if (orbit.radius > 100.0f)
      orbit.radius = 100.0f;
  
  if (orthographic_size < 0.01f)
      orthographic_size = 0.01f;
  
  if (orthographic_size > 100.0f)
      orthographic_size = 100.0f;
}
