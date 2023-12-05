#ifndef MODELS01_H_  // Include guards to prevent multiple inclusions
#define MODELS01_H_

#include "Headers/Shader.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include loader Model class
#include "Headers/Model.h"

// glfw include
#include <GLFW/glfw3.h>

#include "Headers/AnimationUtils.h"

class Models01
{
private:
    /* data */
    GLFWwindow *window01;
    Shader shader01;

    Model modelDartLegoBody;
    Model modelDartLegoHead;
    Model modelDartLegoMask;
    Model modelDartLegoLeftArm;
    Model modelDartLegoRightArm;
    Model modelDartLegoLeftHand;
    Model modelDartLegoRightHand;
    Model modelDartLegoLeftLeg;
    Model modelDartLegoRightLeg;

    // Variables donde se guardan las matrices de cada articulacion por 1 frame
    glm::mat4 modelMatrix; // modelMatrixDart
    float rotDartHead, rotDartLeftArm, rotDartLeftHand, rotDartRightArm, rotDartRightHand, rotDartLeftLeg, rotDartRightLeg;

    std::vector<std::vector<float>> keyFramesJoints; // keyFramesDartJoints
    std::vector<std::vector<glm::mat4>> keyFrames; // keyFramesDart
    int indexFrameDartJoints;
    int indexFrameDartJointsNext;
    float interpolationDartJoints;
    int maxNumPasosDartJoints;
    int numPasosDartJoints;
    int indexFrameDart;
    int indexFrameDartNext;
    float interpolationDart;
    int maxNumPasosDart;
    int numPasosDart;

    std::vector<float> matrixJoints;
	std::vector<glm::mat4> matrixDart;

public:
    Models01(/* args */);
    ~Models01();

    /*functions*/
    void setWindow01(GLFWwindow *window);
    void setShader01(Shader shader);

    void loadModels01();
    void destroyModel01();
    void renderModel01();
    void rotModel01_part();
    void rotModel01_full();

    glm::mat4 getModelMatrix();
    std::vector<std::vector<float>> getKeyFramesJoints();
    std::vector<std::vector<glm::mat4>> getKeyFramesDart();
    void setKeyFramesJoints(std::vector<std::vector<float>> kfj);
    void setKeyFrames(std::vector<std::vector<glm::mat4>> kf);

    void saveKeyFramesJoints(bool record);
    void saveKeyFrames(bool record, bool saveFrame, std::ofstream myfile);
};

// Models01::Models01(/* args */)
// {
// }

// Models01::~Models01()
// {
// }

#endif