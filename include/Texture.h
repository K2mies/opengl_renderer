#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <string>

class Texture {

  public:

    //------------------------------------------------------- Default constructor
    Texture();

    //------------------------------------------- Creates and loads a 2D texture.
    explicit Texture  (const std::string& imagePath, bool flipVertically = true);
    
    //---------------------------------------------------------------- Destructor
    ~Texture();

    //------------------------------ Copy constructors (stops accidental copying)
    Texture(const Texture&)              = delete;
    Texture& operator=(const Texture&)   = delete;

    //---------------------------------------------------------- Move constructor
    Texture(Texture&& other)              noexcept;
    Texture& operator=(Texture&& other)   noexcept;

    //-------------------------------------------------------------- Load texture
    void load(const std::string& imagePath,          bool flipVertically = true);

    //-------------------------------------- Bind this texture to a texture unit.
    void bind(unsigned int textureUnit = 0) const;

    //-------------------------------------------------------------------- Getter
    unsigned int getID()  const;

  private:

    unsigned int ID;

    int          width;
    int          height;

    int          nrChannels;

};

#endif
