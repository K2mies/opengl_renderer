#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <string>

class Texture {

  private:

    unsigned int ID;

    int          width;
    int          height;

    int          nrChannels;

  public:

    //------------------------------------------- Creates and loads a 2D texture.
    explicit Texture  (const std::string& imagePath, bool flipVertically = true);
    
    //---------------------------------------------------------------- Destructor
    ~Texture();

    //------------------------------ Copy constructors (stops accidental copying)
    Texture(const Texture&)              = delete;
    Texture& operator=(const Texture&)   = delete;

    //-------------------------------------- Bind this texture to a texture unit.
    void bind(unsigned int textureUnit = 0) const;

    //-------------------------------------------------------------------- Getter
    unsigned int getID()  const;

};

#endif
