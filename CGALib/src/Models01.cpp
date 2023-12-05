#include "Headers/Models01.h"

// class Models01
// {
// private:
//     /* data */
// public:
//     Models01(/* args */);
//     ~Models01();
// };

// Shader shader01;

Models01::Models01(/* args */)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;

	std::vector<std::vector<float>> keyFramesJoints;
	std::vector<std::vector<glm::mat4>> keyFramesDart;
	int indexFrameDartJoints = 0;
	int indexFrameDartJointsNext = 1;
	float interpolationDartJoints = 0.0;
	int maxNumPasosDartJoints = 20;
	int numPasosDartJoints = 0;
	int indexFrameDart = 0;
	int indexFrameDartNext = 1;
	float interpolationDart = 0.0;
	int maxNumPasosDart = 200;
	int numPasosDart = 0;
}

Models01::~Models01()
{
}

void Models01::setWindow01(GLFWwindow *window01){
	window01 = window01;
}

void Models01::setShader01(Shader shader){
    shader01 = shader;
}

void Models01::loadModels01(){
    modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shader01);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shader01);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shader01);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shader01);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shader01);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shader01);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shader01);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shader01);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shader01);
}

void Models01::destroyModel01(){
    modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
}

void Models01::renderModel01(){
	// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixBody = glm::mat4(modelMatrix);
		modelMatrixBody = glm::scale(modelMatrixBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixBody);
		glm::mat4 modelMatrixHead = glm::mat4(modelMatrixBody);
		modelMatrixHead = glm::rotate(modelMatrixHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixHead);
		modelDartLegoMask.render(modelMatrixHead);

		glm::mat4 modelMatrixLeftArm = glm::mat4(modelMatrixBody);
		modelMatrixLeftArm = glm::translate(modelMatrixLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixLeftArm = glm::translate(modelMatrixLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixLeftArm);
		glm::mat4 modelMatrixLeftHand = glm::mat4(modelMatrixLeftArm);
		modelMatrixLeftHand = glm::translate(modelMatrixLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixLeftHand = glm::translate(modelMatrixLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixLeftHand);
		glm::mat4 modelMatrixRightArm = glm::mat4(modelMatrixBody);
		modelMatrixRightArm = glm::translate(modelMatrixRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixRightArm = glm::rotate(modelMatrixRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixRightArm = glm::rotate(modelMatrixRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixRightArm = glm::rotate(modelMatrixRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixRightArm = glm::translate(modelMatrixRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixRightArm);
		glm::mat4 modelMatrixRightHand = glm::mat4(modelMatrixRightArm);
		modelMatrixRightHand = glm::translate(modelMatrixRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixRightHand = glm::rotate(modelMatrixRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixRightHand = glm::rotate(modelMatrixRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixRightHand = glm::rotate(modelMatrixRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixRightHand = glm::translate(modelMatrixRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixRightHand);
		glm::mat4 modelMatrixLeftLeg = glm::mat4(modelMatrixBody);
		modelMatrixLeftLeg = glm::translate(modelMatrixLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixLeftLeg = glm::rotate(modelMatrixLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixLeftLeg = glm::translate(modelMatrixLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixLeftLeg);
		glm::mat4 modelMatrixRightLeg = glm::mat4(modelMatrixBody);
		modelMatrixRightLeg = glm::translate(modelMatrixRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixRightLeg = glm::rotate(modelMatrixRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixRightLeg = glm::translate(modelMatrixRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);
}

void Models01::rotModel01_part(){
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window01, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (glfwGetKey(window01, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window01, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	// Rotación completa Dart
	if (glfwGetKey(window01, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrix = glm::rotate(modelMatrix, 0.02f, glm::vec3(0, 1, 0));
	else if (glfwGetKey(window01, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrix = glm::rotate(modelMatrix, -0.02f, glm::vec3(0, 1, 0));
	if (glfwGetKey(window01, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.02, 0.0, 0.0));
	else if (glfwGetKey(window01, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.02, 0.0, 0.0));
	
}

void Models01::rotModel01_full(){
	// Rotación completa Dart
	if (glfwGetKey(window01, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrix = glm::rotate(modelMatrix, 0.02f, glm::vec3(0, 1, 0));
	else if (glfwGetKey(window01, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrix = glm::rotate(modelMatrix, -0.02f, glm::vec3(0, 1, 0));
	if (glfwGetKey(window01, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.02, 0.0, 0.0));
	else if (glfwGetKey(window01, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.02, 0.0, 0.0));
	
}

glm::mat4 Models01::getModelMatrix(){
	return modelMatrix;
}
std::vector<std::vector<float>> Models01::getKeyFramesJoints(){
	return keyFramesJoints;
}
    
std::vector<std::vector<glm::mat4>> Models01::getKeyFramesDart(){
	return keyFrames;
}
void Models01::setKeyFramesJoints(std::vector<std::vector<float>> kfj){
	keyFramesJoints= kfj;
}    
void Models01::setKeyFrames(std::vector<std::vector<glm::mat4>> kf){
	keyFrames = kf;
}

void Models01::saveKeyFramesJoints(bool record){
	// if (record){
	// 		matrixJoints.push_back(rotDartHead);
	// 		matrixJoints.push_back(rotDartLeftArm);
	// 		matrixJoints.push_back(rotDartLeftHand);
	// 		matrixJoints.push_back(rotDartRightArm);
	// 		matrixJoints.push_back(rotDartRightHand);
	// 		matrixJoints.push_back(rotDartLeftLeg);
	// 		matrixJoints.push_back(rotDartRightLeg);

	// 		if(saveFrame){
	// 			appendFrame(myfile, matrixJoints);
	// 			saveFrame = false;
	// 		}
	// }
	// else if(keyFramesJoints.size() > 0){
	// 	interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
	// 	numPasosDartJoints++;
	// 	if(interpolationDartJoints > 1.0){
	// 		numPasosDartJoints = 0;
	// 		interpolationDartJoints = 0;
	// 		indexFrameDartJoints = indexFrameDartJointsNext;
	// 		indexFrameDartJointsNext++;
	// 	}
	// 	if(indexFrameDartJointsNext > keyFramesJoints.size() -1)
	// 		indexFrameDartJointsNext = 0;
	// 	rotDartHead = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		0, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// 	rotDartLeftArm = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		1, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// 	rotDartLeftHand = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		2, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// 	rotDartRightArm = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		3, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// 	rotDartRightHand = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		4, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// 	rotDartLeftLeg = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		5, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// 	rotDartRightLeg = interpolate(
	// 		keyFramesJoints, indexFrameDartJoints,indexFrameDartJointsNext,
	// 		6, interpolationDartJoints // 0 es por el orden en que hicimos matrixJoints.push_back()
	// 	);
	// }
}

void Models01::saveKeyFrames(bool record, bool saveFrame, std::ofstream myfile){
	if(record){
			matrixDart.push_back(modelMatrix);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDart);
			}
		}
	else if(keyFrames.size() > 0){
		interpolationDart = numPasosDart / (float) maxNumPasosDart;
		numPasosDart++;
		if(interpolationDart > 1.0){
			numPasosDart = 0;
			interpolationDart = 0;
			indexFrameDart = indexFrameDartNext;
			indexFrameDartNext++;
		}
		if(indexFrameDartNext > keyFrames.size() - 1){
			indexFrameDartNext = 0;
		}
		modelMatrix = interpolate(
			keyFrames, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
			
	}
}