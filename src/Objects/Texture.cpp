#include "Texture.h"

#include "stb_image.h"

#include <iostream>
#include <stdexcept>
#include <string>

Texture::Texture() 
  : ID(0),
  width(0),
  height(0),
  nrChannels(0) {}


Texture::Texture  (const std::string& imagePath, bool flipVertically) 
  : Texture()
{
    load(imagePath, flipVertically);
}

void Texture::load(
    const std::string& imagePath,
    bool flipVertically
)
{
    // Delete any texture we already own.
    if (ID != 0)
    {
        glDeleteTextures(1, &ID);

        ID = 0;
        width = 0;
        height = 0;
        nrChannels = 0;
    }

    // OpenGL considers the bottom-left the texture origin,
    // while most image files use the top-left.
    stbi_set_flip_vertically_on_load(flipVertically);

    // Create the OpenGL texture object.
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Configure wrapping.
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    // Configure filtering.
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    // Load the image into CPU memory.
    unsigned char* data = stbi_load(
        imagePath.c_str(),
        &width,
        &height,
        &nrChannels,
        0
    );

    if (data == nullptr)
    {
        std::string errorMessage =
            "Failed to load texture: " + imagePath;

        if (stbi_failure_reason() != nullptr)
        {
            errorMessage += "\nstb_image error: ";
            errorMessage += stbi_failure_reason();
        }

        glDeleteTextures(1, &ID);
        ID = 0;

        throw std::runtime_error(errorMessage);
    }

    GLenum format;

    if (nrChannels == 1)
    {
        format = GL_RED;
    }
    else if (nrChannels == 2)
    {
        format = GL_RG;
    }
    else if (nrChannels == 3)
    {
        format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        stbi_image_free(data);

        glDeleteTextures(1, &ID);

        ID = 0;

        throw std::runtime_error(
            "Unsupported number of image channels in: " +
            imagePath
        );
    }

    /*
     * OpenGL normally assumes each row starts on a four-byte
     * boundary. Setting this to 1 also handles RGB images whose
     * row width is not divisible by four.
     */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Upload the image to the GPU.
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    // Generate mipmaps.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Restore the default unpack alignment.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Free CPU memory.
    stbi_image_free(data);

    // Unbind.
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout
        << "Loaded texture: " << imagePath
        << " (" << width
        << "x" << height
        << ", " << nrChannels
        << " channels)"
        << std::endl;
}

Texture::Texture(Texture&& other) noexcept
    :
    ID(other.ID),
    width(other.width),
    height(other.height),
    nrChannels(other.nrChannels)
{
    other.ID = 0;
    other.width = 0;
    other.height = 0;
    other.nrChannels = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        if (ID != 0)
        {
            glDeleteTextures(1, &ID);
        }

        ID = other.ID;
        width = other.width;
        height = other.height;
        nrChannels = other.nrChannels;

        other.ID = 0;
        other.width = 0;
        other.height = 0;
        other.nrChannels = 0;
    }

    return *this;
}

Texture::~Texture()
{
    if (ID != 0)
    {
        glDeleteTextures(1, &ID);
    }
}

void Texture::bind(unsigned int textureUnit) const
{
    glActiveTexture (GL_TEXTURE0 + textureUnit);
    glBindTexture   (GL_TEXTURE_2D, ID);
}

unsigned int Texture::getID() const
{
    return ID;
}
