#include "Shader.h"

enum ShaderType
{
  vertex    = 0,
  fragment  = 1
};

//------------------------- constructor reads and builds the shader
Shader::Shader(const char *vertexPath, const char *fragmentPath){

  // 1. retrieve the vertex/fragment source code from filePath:
  std::string       Code[2];
  std::ifstream     ShaderFile[2];

  // ensure ifstream objects can throw exceptions:
  ShaderFile[vertex].exceptions    (std::ifstream::failbit | std::ifstream::badbit);
  ShaderFile[fragment].exceptions  (std::ifstream::failbit | std::ifstream::badbit);

  try{

    // open files:
    ShaderFile[vertex].open   (vertexPath);
    ShaderFile[fragment].open (fragmentPath);
    std::stringstream ShaderStream[2];
    
    // read file's buffer contents into streams:
    ShaderStream[vertex]    << ShaderFile[vertex].rdbuf();
    ShaderStream[fragment]  << ShaderFile[fragment].rdbuf();

    // close file handlers:
    ShaderFile[vertex].close();
    ShaderFile[fragment].close();

    // convert stream into string:
    Code[vertex]    = ShaderStream[vertex].str();
    Code[fragment]  = ShaderStream[fragment].str();
  }

  catch (std::ifstream::failure e){
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  
  const char *ShaderCode[2];

  ShaderCode[vertex]    = Code[vertex].c_str();
  ShaderCode[fragment]  = Code[fragment].c_str();

  // 2. compile shaders
  unsigned int      shader[2];
  int               success;
  char              infoLog[512];

  // Vertex Shader:
  shader[vertex] = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource  (shader[vertex], 1, &ShaderCode[vertex], NULL);
  glCompileShader (shader[vertex]);
  
  checkCompileErrors(shader[vertex], "VERTEX");

  // Fragment Shader:
  shader[fragment] = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource  (shader[fragment], 1, &ShaderCode[fragment], NULL);
  glCompileShader (shader[fragment]);

  checkCompileErrors  (shader[fragment], "FRAGMENT");
  
  // Shader Program:
  ID = glCreateProgram();

  glAttachShader  (ID, shader[vertex]);
  glAttachShader  (ID, shader[fragment]);
  glLinkProgram   (ID);

  checkCompileErrors  (ID, "PROGRAM");

  // delete the shaders as they're linked into our program now 
  // and no longer necessary. Think of it like deleting .o files
  // after linking and creating a binary.
  glDeleteShader  (shader[vertex]);
  glDeleteShader  (shader[fragment]);


}
//----------------------------------------- use/activate the shader
void Shader::use(){
  glUseProgram(ID);
}
//--------------------------------------- utility uniform functions
void Shader::setBool  (const std::string &name, bool value)   const {
  glUniform1i (glGetUniformLocation  (ID, name.c_str()), (int)value);
}  
void Shader::setInt   (const std::string &name, int value)    const {
  glUniform1i (glGetUniformLocation  (ID, name.c_str()), value);
}
void Shader::setFloat (const std::string &name, float value)  const {
  glUniform1f (glGetUniformLocation  (ID, name.c_str()), value );
}
void Shader::setVec4(const std::string& name,
                     float x,
                     float y,
                     float z,
                     float w) const
{
    glUniform4f(
        glGetUniformLocation(ID, name.c_str()),
        x, y, z, w
    );
}

// utility function for checking shader compilation/linking errors.
// ----------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, std::string type){
  int   success;
  char  infoLog[1024];
  if (type != "PROGRAM"){
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout 
        << "ERROR::SHADER_COMPILATION_ERROR of type: " 
        << type << "\n" 
        << infoLog 
        << "\n -- --------------------------------------------------- -- " 
        << std::endl;
    }
    else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success){
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout 
          << "ERROR::PROGRAM_LINKING_ERROR of type: " 
          << type 
          << "\n" 
          << infoLog 
          << "\n -- --------------------------------------------------- -- " 
          << std::endl;
      }
    }
  }
}

