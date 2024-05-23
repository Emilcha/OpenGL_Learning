#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* filePath)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum data_type;
        if (nrChannels == 4) data_type = GL_RGBA;
        else data_type = GL_RGB;
        std::cout << "(+) Chargement de l'image \"" << filePath << "\" avec " << nrChannels << " canaux." << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, data_type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "(!) Erreur durant le chargement de la texture !" << std::endl;
    }
    stbi_image_free(data);
}


void Texture::use(GLuint texUnitIndex)
{
    glActiveTexture(GL_TEXTURE0 + texUnitIndex);
    glBindTexture(GL_TEXTURE_2D, textureID);
}