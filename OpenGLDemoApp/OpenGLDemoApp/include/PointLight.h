#pragma once
#include "../include/BaseLight.h"
class PointLight :
    public BaseLight
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat constant, GLfloat linear, GLfloat quadratic);
    PointLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity, glm::vec3 position, glm::vec3 attenuationConstants);

    
    void UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation,
        GLint diffuseIntensityLocation, GLint positionLocation,
        GLint constantLocation, GLint linearLocation, GLint quadraticLocation) const;

    void SetLightPosition(glm::vec3 position);

    ~PointLight();



protected:
    glm::vec3 position;

    GLfloat constant, linear, quadratic;
    

};

