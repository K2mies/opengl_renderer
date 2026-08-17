#ifndef SHADER_H
#define SHADER_H

// include glad to get all the required OpenGL headers 
#include <glad/glad.h> 

// include glm libraries to get the glm functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

// Types to use for custom setters ie: Light, Material ... etc
#include "ShaderTypes.h"

//---------------------------------------- Forward declarations

class Shader  {

  public:
    //------------------------------------------ the program id
    unsigned int ID;

    //------------------ constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);

    //---------------------------------- use/activate the shader
    void use();

    //-------------------------------- utility uniform functions
    void setBool  (const std::string &name, bool  value)  const;  
    void setInt   (const std::string &name, int   value)  const;   
    void setFloat (const std::string &name, float value)  const;

    //------------------------------------- set vector functions
    void setVec2  (const std::string& name,
                                            float x,
                                            float y)      const;
    void setVec3  (const std::string& name,
                                            float x,
                                            float y,
                                            float z)      const;
    void setVec4  (const std::string& name,
                                            float x,
                                            float y,
                                            float z,
                                            float w)      const;

    //------------------------------------- set matrix functions
//    void setMat2(const std::string &name,   const glm::mat2 &mat)
//
//                                                          const;
//
    void setMat3(const std::string &name,   const mat3 &matrix)

                                                          const;

    void setMat4(const std::string &name,   const mat4 &matrix)

                                                          const;

    //------------------------------------------------ set types
    void setWeight(const std::string& name, const Weight& weight)

                                                          const;

    void setMatrix(const std::string& name, const Matrix& matrix) 

                                                          const;
  private:
    //-- function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
