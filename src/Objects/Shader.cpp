
#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Mat4.h"
#include "Mat3.h"
#include "Mat2.h"
#include "Vec4.h"
#include "Vec3.h"
#include "MathUtils.h"

enum ShaderType
{
  vert  = 0,
  frag  = 1
};

//------------------------- constructor reads and builds the shader
Shader::Shader(const char *vertexPath, const char *fragmentPath){

  // 1. retrieve the vertex/fragment source code from filePath:
  std::string     Code[2];
  std::ifstream   ShaderFile[2];

  // ensure ifstream objects can throw exceptions:
  ShaderFile      [vert].exceptions  (std::ifstream::failbit | std::ifstream::badbit);
  ShaderFile      [frag].exceptions  (std::ifstream::failbit | std::ifstream::badbit);

  try{

    // open files:
    ShaderFile    [vert].open (vertexPath);
    ShaderFile    [frag].open (fragmentPath);
    
    std::stringstream ShaderStream[2];
    
    // read file's buffer contents into streams:
    ShaderStream  [vert] << ShaderFile[vert].rdbuf();
    ShaderStream  [frag] << ShaderFile[frag].rdbuf();

    // close file handlers:
    ShaderFile    [vert].close();
    ShaderFile    [frag].close();

    // convert stream into string:
    Code          [vert] = ShaderStream[vert].str();
    Code          [frag] = ShaderStream[frag].str();
  }

  catch (std::ifstream::failure e){
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  
  const char *ShaderCode[2];

  ShaderCode      [vert] = Code[vert].c_str();
  ShaderCode      [frag] = Code[frag].c_str();

  // 2. compile shaders
  unsigned int          shader[2];

  // vert Shader:
  shader          [vert] = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource        (shader[vert], 1, &ShaderCode[vert], NULL);
  glCompileShader       (shader[vert]);
  
  checkCompileErrors    (shader[vert], "vert");

  // frag Shader:
  shader          [frag] = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource        (shader[frag], 1, &ShaderCode[frag], NULL);
  glCompileShader       (shader[frag]);

  checkCompileErrors    (shader[frag], "frag");
  
  // Shader Program:
  ID = glCreateProgram();

  glAttachShader        (ID, shader[vert]);
  glAttachShader        (ID, shader[frag]);
  glLinkProgram         (ID);

  checkCompileErrors    (ID, "PROGRAM");

  // delete the shaders as they're linked into our program now 
  // and no longer necessary. Think of it like deleting .o files
  // after linking and creating a binary.
  glDeleteShader        (shader[vert]);
  glDeleteShader        (shader[frag]);

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

void Shader::setVec2  (const std::string &name,
                                                float x,
                                                float y)      const
{
    glUniform2f (glGetUniformLocation(ID, name.c_str()),
        x, y
    );
}

void Shader::setVec3  (const std::string &name,
                                                float x,
                                                float y,
                                                float z)      const
{
    glUniform3f (glGetUniformLocation(ID, name.c_str()),
        x, y, z
    );
}

void Shader::setVec4  (const std::string &name,
                                                float x,
                                                float y,
                                                float z,
                                                float w)      const
{
    glUniform4f (glGetUniformLocation(ID, name.c_str()),
        x, y, z, w
    );
}

void Shader::setVec2  (const std::string& name, const vec2& vector) const
{

  glUniform2f (glGetUniformLocation(ID, name.c_str()), vector.x,
                                                       vector.y);
}

void Shader::setVec3  (const std::string& name, const vec3& vector) const
{

  glUniform3f (glGetUniformLocation(ID, name.c_str()), vector.x,
                                                       vector.y,
                                                       vector.z);

}

void Shader::setVec4  (const std::string& name, const vec4& vector) const
{
  
  glUniform4f (glGetUniformLocation(ID, name.c_str()), vector.x, 
                                                       vector.y, 
                                                       vector.z,
                                                       vector.w);

}

//------------------------------------- set matrix functions
void Shader::setMat2(const std::string &name,   const mat2 &matrix) const {
   glUniformMatrix2fv(
        glGetUniformLocation(ID, name.c_str()),
        1,
        GL_FALSE,
        matrix.data()
  );
}

void Shader::setMat3(const std::string &name,   const mat3 &matrix) const {
  glUniformMatrix3fv(
        glGetUniformLocation(ID, name.c_str()),
        1,
        GL_FALSE,
        matrix.data()
   );
}
void Shader::setMat4(const std::string &name,   const mat4 &matrix) const {
  glUniformMatrix4fv(
        glGetUniformLocation(ID, name.c_str()),
        1,
        GL_FALSE,
        matrix.data()
  );
}

//------------------------------------------------ set types
void Shader::setWeight(const std::string& name, const Weight& weight) const {
    setFloat(name + ".texture",  weight.texture);
    setFloat(name + ".vertex",   weight.vertex);
    setFloat(name + ".location", weight.location);
    setFloat(name + ".blend",    weight.blend);
}

void Shader::setMatrix(const std::string& name, const Matrix& matrix) const {
    setMat4(name + ".local",      matrix.local);
    setMat4(name + ".model",      matrix.model);
    setMat4(name + ".view",       matrix.view);
    setMat4(name + ".projection", matrix.projection);
    setMat4(name + ".clip",       matrix.clip);
    setMat3(name + ".normal",     matrix.normal);

}
// utility function for checking shader compilation/linking errors.
// ----------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, std::string type)  {

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
