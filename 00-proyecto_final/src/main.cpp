// ****************************************************************
// BIBLIOTECAS
// ****************************************************************

#define _USE_MATH_DEFINES
#include <cmath>
// glew include
#include <GL/glew.h>

// std includes
#include <string>
#include <iostream>

// glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

// GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

int screenWidth;
int screenHeight;

float cursor_x = -0.5460;
float cursor_y = 0.8330;

char CadenaVidaCazador[3];

char CadenaVidaSanador[3];

char CadenaVidaCaballero[3];

char CadenaVidaVengador[3];

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow *window;

Shader shader;
// Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;
// Shader para el terreno
Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
// Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;

// ****************************************************************
// CÁMARAS
// ****************************************************************
std::shared_ptr<FirstPersonCamera> camera1P(new FirstPersonCamera());

std::shared_ptr<ThirdPersonCamera> camera3P(new ThirdPersonCamera());

std::shared_ptr<ThirdPersonCamera> camera3PTablero(new ThirdPersonCamera());
std::shared_ptr<ThirdPersonCamera> camera3P3Pj(new ThirdPersonCamera());

std::string cameraState = "tercera_p";
bool enable_camera = true;
float distanceFromTarget = 7.0;

// ****************************************************************
// MODELOS BÁSICOS
// ****************************************************************
Sphere skyboxSphere(20, 20);
Box box_tablero;
Sphere esfera1(10, 10);
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);
Box boxIntro;
Box boxViewDepth;
// ****************************************************************
// MODELOS (OBF, FBX)
// ****************************************************************

// Decoración
Model modelDecoBosque;
// Cristales
Model model_cristal_amarillo;
Model model_cristal_azul;
Model model_cristal_rojo;
Model model_cristal_verde;

// objetos PJ y MS
Model ModelCazador;
Model ModelSanador;
Model ModelCaballero;
Model ModelVengador;

Model ModelEsqueleto;
Model ModelDemonio;
Model ModelHechicero;
Model ModelAnfiteres;
//*******************************************************************+

// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../media/textures/heightmap.png");

ShadowBox *shadowBox;

// ****************************************************************
// TEXTURAS
// ****************************************************************
GLuint textureTerrainKID;
GLuint texture_tableroID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint txs_active;
GLuint txs_main_title01, txs_main_title02, txs_main_title03;
GLuint txs_gameplay_ui_personajes;
GLuint txs_gameplay_efecto_ataque, txs_gameplay_efecto_defensa, txs_gameplay_efecto_velocidad;

bool iniciaPartida = false, presionarOpcion = false;
bool en_pantalla_de_juego = false;
bool ena_key_enter = true, ena_key_left = true, ena_key_right = true;
bool ena_key_z = true, ena_key_x = true, ena_key_c = true;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

// std::string fileNames[6] = {
// 	"../media/skybox/belfast_sunset_puresky/ft.tga",
// 	"../media/skybox/belfast_sunset_puresky/bk.tga",
// 	"../media/skybox/belfast_sunset_puresky/up.tga",
// 	"../media/skybox/belfast_sunset_puresky/dn.tga",
// 	"../media/skybox/belfast_sunset_puresky/rt.tga",
// 	"../media/skybox/belfast_sunset_puresky/lf.tga"};

std::string fileNames[6] = {
	"../media/skybox/anime_starry_night/ft.tga",
	"../media/skybox/anime_starry_night/bk.tga",
	"../media/skybox/anime_starry_night/up.tga",
	"../media/skybox/anime_starry_night/dn.tga",
	"../media/skybox/anime_starry_night/rt.tga",
	"../media/skybox/anime_starry_night/lf.tga"};

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixDecoBosque = glm::mat4(1.0f);

glm::mat4 ModelMatrixCazador = glm::mat4(1.0f);
glm::mat4 ModelMatrixSanador = glm::mat4(1.0f);
glm::mat4 ModelMatrixCaballero = glm::mat4(1.0f);
glm::mat4 ModelMatrixVengador = glm::mat4(1.0f);

glm::mat4 ModelMatrixEsqueleto = glm::mat4(1.0f);
glm::mat4 ModelMatrixDemonio = glm::mat4(1.0f);
glm::mat4 ModelMatrixHechicero = glm::mat4(1.0f);
glm::mat4 ModelMatrixAnfiteres = glm::mat4(1.0f);

// ****************************************************************
// VARIABLES DE ANIMACIÓN
// ****************************************************************
int modelSelected = 0;
bool enableCountSelected = true;
bool enableTirada = false;
bool CazadorSelected = true;
bool SanadorSelected = true;
bool CaballeroSelected = true;
bool VengadorSelected = true;
// ****************************************************************
// KEYFRAMES
// ****************************************************************

//--------------------------------------------------------------------------------------------
// bool Casilla_conbate=false ,Casilla_buffo = false, Casilla_jefe=false;
int combate = 0;
int diceValue = 1; // Valor del dado
int Tipo_Casilla = 0;
int buffo = 0;

// int keyTablero[28] = {0, 1, 2, 2, 1, 2, 3, 2, 1, 2, 2, 1, 2, 3, 2, 1, 2, 2, 1, 2, 3, 2, 1, 2, 2, 1, 2, 3};

//--------------------------------------------------------------------------------------------
//-----------------------------------------------------------------
// Definición de una estructura para un personaje
struct Personaje
{
	// Valores base de cada persoanje
	int TipoPj;	 // Tipo de Personaje
	int ataque;	 // Ataque de Personaje
	int defensa; // Defensa de Personaje
	int vida;	 // Vida de Personaje (Se modifica constante mente despues de cada accion)
	int vidaMx;	 // Vida MAX de Personje
	int PossTablero;	 // Posicion de Personaje en el Tableor
	int TiradaSalvacion; // Tirada de salvacion del personje
	int CD;				 // Cooldown de Personaje (Se modifica constante mente despues de cada accion)
	int CDMX;			 // Cooldown MAX de Personaje
	int BanderaPos1;
	int BanderaPos2;
	int BanderaPos3;
	int BanderaPos4;
};

// Función para inicializar un personaje basado en el tipo
struct Personaje inicializarPersonaje(int tipo_personaje)
{
	struct Personaje personaje;

	switch (tipo_personaje)
	{
	case 1:
		personaje.TipoPj = 1;
		personaje.ataque = 5;
		personaje.defensa = 2;
		personaje.vida = 6;
		personaje.vidaMx = 6;
		// personaje.movimiento = 5;
		personaje.PossTablero = 0;
		personaje.TiradaSalvacion = 4;
		personaje.CD = 3;
		personaje.CDMX = 3;
		personaje.BanderaPos1 = 0;
		personaje.BanderaPos2 = 0;
		personaje.BanderaPos3 = 0;
		personaje.BanderaPos4 = 0;

		break;
	case 2:
		personaje.TipoPj = 2;
		personaje.ataque = 2;
		personaje.defensa = 3;
		personaje.vida = 10;
		personaje.vidaMx = 10;
		// personaje.movimiento = 6;
		personaje.PossTablero = 0;
		personaje.TiradaSalvacion = 3;
		personaje.CD = 4;
		personaje.CDMX = 4;
		personaje.BanderaPos1 = 0;
		personaje.BanderaPos2 = 0;
		personaje.BanderaPos3 = 0;
		personaje.BanderaPos4 = 0;

		break;
	case 3:
		personaje.TipoPj = 3;
		personaje.ataque = 3;
		personaje.defensa = 5;
		personaje.vida = 8;
		personaje.vidaMx = 8;
		// personaje.movimiento = 3;
		personaje.PossTablero = 0;
		personaje.TiradaSalvacion = 4;
		personaje.CD = 2;
		personaje.CDMX = 2;
		personaje.BanderaPos1 = 0;
		personaje.BanderaPos2 = 0;
		personaje.BanderaPos3 = 0;
		personaje.BanderaPos4 = 0;

		break;
	case 4:
		personaje.TipoPj = 4;
		personaje.ataque = 4;
		personaje.defensa = 1;
		personaje.vida = 10;
		personaje.vidaMx = 10;
		// personaje.movimiento = 4;
		personaje.PossTablero = 0;
		personaje.TiradaSalvacion = 5;
		personaje.CD = 3;
		personaje.CDMX = 3;
		personaje.BanderaPos1 = 0;
		personaje.BanderaPos2 = 0;
		personaje.BanderaPos3 = 0;
		personaje.BanderaPos4 = 0;

		break;
	default:
		break;
	}

	return personaje;
}

// Definición de una estructura para los monstruos
struct Monstruo
{
	int ataque;	 // Ataque de Monstruo
	int defensa; // Defensa de Monstruo
	int vida;	 // vida de Monstruo
};

// Función para inicializar un monstruo basado en el tipo
struct Monstruo inicializarMonstruo(int tipo_monstruo)
{
	struct Monstruo monstruo;

	switch (tipo_monstruo)
	{
	case 0:
		monstruo.ataque = 2;
		monstruo.defensa = 3;
		monstruo.vida = 3;
		break;
	case 1:
		monstruo.ataque = 4;
		monstruo.defensa = 2;
		monstruo.vida = 5;
		break;
	case 2:
		monstruo.ataque = 3;
		monstruo.defensa = 1;
		monstruo.vida = 8;
		break;
	case 3:
		monstruo.ataque = 8;
		monstruo.defensa = 5;
		monstruo.vida = 50;
		break;
	default:
		// En caso de un tipo de monstruo no reconocido, se inicializan todos los valores a 0
		monstruo.ataque = 0;
		monstruo.defensa = 0;
		monstruo.vida = 0;
		break;
	}

	return monstruo;
}

//------------------------------------------------------------------
// inicializacion de Pj y Ms pricipales

struct Personaje Cazador = inicializarPersonaje(1);
struct Personaje Sanador = inicializarPersonaje(2);
struct Personaje Caballero = inicializarPersonaje(3);
struct Personaje Vengador = inicializarPersonaje(4);

struct Monstruo GeneradorMs = inicializarMonstruo(0);
struct Monstruo Anfiteres = inicializarMonstruo(3);

//--------------------------------------------------------------------
// conbate
void Combate(Personaje *Pj, Monstruo *Ms)
{
	printf("Entraste al combate.\n");
	// Dados
	int dadoPj = 1 + rand() % 6;
	int dadoMs = 1 + rand() % 6;

	// Cálculo del daño al monstruo y al personaje
	printf("El monstruo tiro dado de %d.\n", dadoMs);
	printf("El personaje tiro dado de %d.\n", dadoPj);

	int damage_monstruo = (Pj->ataque + dadoPj) - Ms->defensa; // Daño que sufre MS
	if (damage_monstruo < 0)
	{
		damage_monstruo = 0;
	}
	int damage_personaje = (Ms->ataque + dadoMs) - Pj->defensa; // Daño que sufre PJ
	if (damage_personaje < 0)
	{
		damage_personaje = 0;
	}

	// Daños
	Ms->vida -= damage_monstruo;
	Pj->vida -= damage_personaje;

	printf("La vida de Ms  %d \n", Ms->vida);
	printf("La vida de Pj %d \n", Pj->vida);

	// Información sobre el combate
	printf("El monstruo infligio %d de dano al personaje.\n", damage_personaje);
	printf("El personaje infligio %d de dano al monstruo.\n", damage_monstruo);
}

Monstruo MostruosGenerdor(Monstruo GeneradorMs)
{
	if (GeneradorMs.vida <= 0)
	{
		int Tipo_Ms = rand() % 2;
		GeneradorMs = inicializarMonstruo(Tipo_Ms);
		printf("se genero un nuevo moutruo \n");
	}

	return GeneradorMs;
}

Personaje TiradaDeSalvacion(Personaje Pj)
{
	printf("Se realizao tirada de salvacion \n");
	int DadoSalvacion = 1 + rand() % 6;
	if (Pj.TiradaSalvacion <= DadoSalvacion)
	{
		Pj.vida = Pj.vidaMx;
		printf("Tirada de salvacion exitosa\n");
	}
	else
	{
		printf("Pj Muerto y Su tirada de Salvacion fallo");
	}
	return Pj;
}

void PerderBuffo(int buffo, Personaje *Pj)
{
	if (buffo == 1)
	{
		Pj->ataque -= 2;
		printf("Buffo 1 perdido\n");
	}
	if (buffo == 2)
	{
		Pj->defensa -= 2;
		printf("Buffo 2 perdido\n");
	}
	if (buffo == 3)
	{
		// Pj->movimiento -= 2;
		printf("Buffo 3 perdido\n");
	}
}
int keyTablero(int TC)
{
	int KeyTC;
	switch (TC)
	{
	case 0:
		KeyTC = 0;
		break;
	case 1:
		KeyTC = 1;
		break;
	case 2:
		KeyTC = 2;
		break;
	case 3:
		KeyTC = 2;
		break;
	case 4:
		KeyTC = 1;
		break;
	case 5:
		KeyTC = 2;
		break;
	case 6:
		KeyTC = 3;
		break;
	case 7:
		KeyTC = 2;
		break;
	case 8:
		KeyTC = 1;
		break;
	case 9:
		KeyTC = 2;
		break;
	case 10:
		KeyTC = 2;
		break;
	case 11:
		KeyTC = 1;
		break;
	case 12:
		KeyTC = 2;
		break;
	case 13:
		KeyTC = 3;
		break;
	case 14:
		KeyTC = 2;
		break;
	case 15:
		KeyTC = 1;
		break;
	case 16:
		KeyTC = 2;
		break;
	case 17:
		KeyTC = 2;
		break;
	case 18:
		KeyTC = 1;
		break;
	case 19:
		KeyTC = 2;
		break;
	case 20:
		KeyTC = 3;
		break;
	case 21:
		KeyTC = 2;
		break;
	case 22:
		KeyTC = 1;
		break;
	case 23:
		KeyTC = 2;
		break;
	case 24:
		KeyTC = 2;
		break;
	case 25:
		KeyTC = 1;
		break;
	case 26:
		KeyTC = 2;
		break;
	case 27:
		KeyTC = 3;
		break;

	default:
		break;
	}
	return KeyTC;
}

void Tablero(Personaje *Pj, int dado, Monstruo *Ms, Monstruo *Anfiteres, int Tipo_Casilla)
{
	printf("Entraste al tablero.\n");
	if (Pj->CD < Pj->CDMX)
	{
		Pj->CD = Pj->CD + 1;
	}
	Pj->PossTablero += dado;

	buffo = 1 + rand() % 3;
	switch (Tipo_Casilla)

	{
	case 0:
		Pj->vida = Pj->vidaMx;
		break;
	case 1:
		if (buffo == 1)
		{
			Pj->ataque += 2;
			printf("Buffo 1\n");
			// buffoBool
		}
		if (buffo == 2)
		{
			Pj->defensa += 2;
			printf("Buffo 2\n");
		}
		if (buffo == 3)
		{
			// Pj->movimiento += 2;
			printf("Buffo 3\n");
		}
		break;

	case 2:
		Combate(Pj, Ms);
		printf("Combate con monstruo.\n");
		break;

	case 3:
		Combate(Pj, Anfiteres);
		printf("Combate con jefe.\n");
		break;
	default:
		break;
	}
}

glm::mat4 Movimiento (Personaje *Pj,int dado,glm::mat4 ModelMatrixPj){
	//----------------------Movimientos-------------------------------------
				if (Pj->PossTablero > 0 && Pj->PossTablero < 7)
				{
					ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 - (dado) * 6.2, 0.0, 0.0));
					Pj->BanderaPos1 = 6 - Pj->PossTablero;
				}
				if (Pj->PossTablero > 6 && Pj->PossTablero < 14)
				{
					if (Pj->BanderaPos1 > 0)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 - Pj->BanderaPos1 * 6.2, 0.0, 0.0 - (dado - Pj->BanderaPos1) * 6.2));
						Pj->BanderaPos1 = 0;
					}
					else if (Pj->BanderaPos1 < 1)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0, 0.0, 0.0 - (dado) * 6.2));
					}
					Pj->BanderaPos2 = 13 - Pj->PossTablero;
				}
				if (Pj->PossTablero > 13 && Pj->PossTablero < 21)
				{
					if (Pj->BanderaPos2 > 0)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 + (dado - Pj->BanderaPos2) * 6.2, 0.0, 0.0 - Pj->BanderaPos2 * 6.2));
						Pj->BanderaPos2 = 0;
					}
					else if (Pj->BanderaPos2 < 1)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 + (dado) * 6.2, 0.0, 0.0));
					}
					Pj->BanderaPos3 = 20 - Pj->PossTablero;
				}
				if (Pj->PossTablero > 20 && Pj->PossTablero < 28)
				{
					if (Pj->BanderaPos3 > 0)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 + Pj->BanderaPos3 * 6.2, 0.0, 0.0 + (dado - Pj->BanderaPos3) * 6.2));
						Pj->BanderaPos3 = 0;
					}
					else if (Pj->BanderaPos3 < 1)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0, 0.0, 0.0 + (dado) * 6.2));
					}
					Pj->BanderaPos4 = 27 - Pj->PossTablero;
				}
				if (Pj->PossTablero > 27)
				{
					if (Pj->BanderaPos4 > 0)
					{
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 - 1 * 6.2, 0.0, 0.0 + Pj->BanderaPos4));
						Pj->BanderaPos4 = 0;
					}else if(Pj->BanderaPos4 < 1){
						ModelMatrixPj = glm::translate(ModelMatrixPj, glm::vec3(0.0 - 1 * 6.2, 0.0, 0.0));
					}
					Pj->PossTablero = 0;
					Pj->vida = Pj->vidaMx;
				}
				return ModelMatrixPj;
				//------------------------Fin Movimienots-----------------------------------------------
}
//---------------------------------------------------------------constructores

// ****************************************************************
// Cristales (Multiple Lights)
// ****************************************************************
std::vector<glm::vec3> poss_cristal_amarillo = {
	glm::vec3(5.0, 0.0, 0.0)};
std::vector<glm::vec3> poss_cristal_azul = {
	glm::vec3(-5.0, 0.0, 0.0)};
std::vector<glm::vec3> poss_cristal_rojo = {
	glm::vec3(0.0, 0.0, 5.0)};
std::vector<glm::vec3> poss_cristal_verde = {
	glm::vec3(0.0, 0.0, -5.0)};

double deltaTime;
double currTime, lastTime;

// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>> collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>> collidersSBB;

// ****************************************************************
// OpenAL (defines)
// ****************************************************************
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = {0.0, 0.0, 4.0};
ALfloat listenerVel[] = {0.0, 0.0, 0.0};
ALfloat listenerOri[] = {0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
// Source 0
ALfloat source0Pos[] = {-2.0, 0.0, 0.0};
ALfloat source0Vel[] = {0.0, 0.0, 0.0};
// Source 1
ALfloat source1Pos[] = {2.0, 0.0, 0.0};
ALfloat source1Vel[] = {0.0, 0.0, 0.0};
// Source 2
ALfloat source2Pos[] = {2.0, 0.0, 0.0};
ALfloat source2Vel[] = {0.0, 0.0, 0.0};
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint al_sources[NUM_SOURCES];
ALuint al_music_sources[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> al_music_play = {true, true, true};
std::vector<bool> al_sounds_play = {true, true, true};
std::vector<bool> sourcesPlay = {true, true, true};

// Framesbuffers
GLuint depthMap, depthMapFBO;

// Definición del frame buffer
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 400;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
				 int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

void initParticleBuffers()
{
	// Generate the buffers
	glGenBuffers(1, &initVel);	 // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++)
	{

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++)
	{
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen)
{

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
								  glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
								  nullptr);

	if (window == nullptr)
	{
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../shaders/colorShader.vs", "../shaders/colorShader.fs");
	shaderSkybox.initialize("../shaders/skyBox.vs", "../shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../shaders/iluminacion_textura_animation_shadow.vs", "../shaders/multipleLights_shadow.fs");
	// shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../shaders/terrain_shadow.vs", "../shaders/terrain_shadow.fs");
	shaderTexture.initialize("../shaders/texturizado.vs", "../shaders/texturizado.fs");
	shaderViewDepth.initialize("../shaders/texturizado.vs", "../shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../shaders/shadow_mapping_depth.vs", "../shaders/shadow_mapping_depth.fs");

	// ****************************************************************
	// init(): MODELOS BÁSICOS
	// ****************************************************************
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));

	box_tablero.init();
	box_tablero.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	// ****************************************************************
	// init(): MODELOS (OBJ, FBX)
	// ****************************************************************

	// Decoración Tablero
	modelDecoBosque.loadModel("../media/models/DecoBosque/DecoBosque.obj");
	modelDecoBosque.setShader(&shaderMulLighting);
	// Cristales
	model_cristal_amarillo.loadModel("../media/models/cristales/cristal_amarillo.obj");
	model_cristal_amarillo.setShader(&shaderMulLighting);
	model_cristal_azul.loadModel("../media/models/cristales/cristal_azul.obj");
	model_cristal_azul.setShader(&shaderMulLighting);
	model_cristal_rojo.loadModel("../media/models/cristales/cristal_rojo.obj");
	model_cristal_rojo.setShader(&shaderMulLighting);
	model_cristal_verde.loadModel("../media/models/cristales/cristal_verde.obj");
	model_cristal_verde.setShader(&shaderMulLighting);


	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	// ****************************************************************
	// init(): CÁMARAS
	// ****************************************************************
	// Nota. Para una cámara en tercera persona, no se ocupa un posición
	camera1P->setPosition(glm::vec3(0.0, 10.0, 0.0));

	// camera3P->setPosition(glm::vec3(0.0, 5.0, 4.0));
	//  camera3P->setAngleTarget(90.0f); // Ángulo de inclinación para mirar hacia abajo (90 grados)
	//  camera3P->setYaw(0.0f);   // Ángulo de giro horizontal (puede ser 0 o cualquier otro valor)
	camera3P->setSensitivity(1.0f);
	camera3P->setDistanceFromTarget(distanceFromTarget);

	// models*************************************************************
	ModelCazador.loadModel("../media/models/Cazador/Cazadora.fbx");
	ModelCazador.setShader(&shaderMulLighting);
	ModelSanador.loadModel("../media/models/Sanador/Sanador.fbx");
	ModelSanador.setShader(&shaderMulLighting);
	ModelCaballero.loadModel("../media/models/Caballero/Caballero.fbx");
	ModelCaballero.setShader(&shaderMulLighting);
	ModelVengador.loadModel("../media/models/Vengador/Vengador.fbx");
	ModelVengador.setShader(&shaderMulLighting);
	ModelEsqueleto.loadModel("../media/models/Esqueleto/Esqueleto.fbx");
	ModelEsqueleto.setShader(&shaderMulLighting);
	ModelDemonio.loadModel("../media/models/Demonio/Demonio.fbx");
	ModelDemonio.setShader(&shaderMulLighting);
	ModelHechicero.loadModel("../media/models/Hechicero/Hechicero.fbx");
	ModelHechicero.setShader(&shaderMulLighting);
	ModelAnfiteres.loadModel("../media/models/Dragon/Dragon.fbx");
	ModelAnfiteres.setShader(&shaderMulLighting);

	//*******************************************************************+

	// ****************************************************************
	// init(): SKYBOX
	// ****************************************************************
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++)
	{
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData())
		{
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
						 skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// ****************************************************************
	// init(): MAPA DE MEZCLAZ
	// ****************************************************************

	// Definiendo la textura a utilizar
	Texture textureK("../media/textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureK.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainKID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainKID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureK.getData())
	{
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureK.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureK.getChannels() == 3 ? GL_RGB : GL_RGBA, textureK.getWidth(), textureK.getHeight(), 0,
					 textureK.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureK.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureK.freeImage();

	// Definiendo la textura
	Texture textureR("../media/textures/uniformed_fire.png");
	textureR.loadImage();											  // Cargar la textura
	glGenTextures(1, &textureTerrainRID);							  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);				  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureR.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
					 textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../media/textures/stone_dust.png");
	textureG.loadImage();											  // Cargar la textura
	glGenTextures(1, &textureTerrainGID);							  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);				  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureG.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
					 textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../media/textures/galaxy_universe.png");
	textureB.loadImage();											  // Cargar la textura
	glGenTextures(1, &textureTerrainBID);							  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);				  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureB.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
					 textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../media/textures/terrain_blendmap.png");
	textureBlendMap.loadImage();									  // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID);					  // Creando el id de la textura
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);			  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureBlendMap.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
					 textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture texture_screen_01("../media/textures_screen/inicio_jugar.PNG");
	texture_screen_01.loadImage();									  // Cargar la textura
	glGenTextures(1, &txs_main_title01);							  // Creando el id de la textura
	glBindTexture(GL_TEXTURE_2D, txs_main_title01);					  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_screen_01.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_screen_01.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_screen_01.getWidth(), texture_screen_01.getHeight(), 0,
					 texture_screen_01.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_screen_01.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_screen_01.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture texture_screen_02("../media/textures_screen/inicio_opciones.PNG");
	texture_screen_02.loadImage();									  // Cargar la textura
	glGenTextures(1, &txs_main_title02);							  // Creando el id de la textura
	glBindTexture(GL_TEXTURE_2D, txs_main_title02);					  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_screen_02.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_screen_02.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_screen_02.getWidth(), texture_screen_02.getHeight(), 0,
					 texture_screen_02.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_screen_02.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_screen_02.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture texture_screen_03("../media/textures_screen/inicio_salir.PNG");
	texture_screen_03.loadImage();									  // Cargar la textura
	glGenTextures(1, &txs_main_title03);							  // Creando el id de la textura
	glBindTexture(GL_TEXTURE_2D, txs_main_title03);					  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_screen_03.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_screen_03.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_screen_03.getWidth(), texture_screen_03.getHeight(), 0,
					 texture_screen_03.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_screen_03.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_screen_03.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen("../media/textures/ui_personajes_00.png");
	textureScreen.loadImage();										  // Cargar la textura
	glGenTextures(1, &txs_gameplay_ui_personajes);					  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, txs_gameplay_ui_personajes);		  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureScreen.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
					 textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria

	// ****************************************************************
	// init(): TABLERO
	// ****************************************************************

	// Definiendo la textura
	Texture texture_tablero("../media/textures/tablero.png");
	texture_tablero.loadImage();									  // Cargar la textura
	glGenTextures(1, &texture_tableroID);							  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, texture_tableroID);				  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_tablero.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_tablero.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_tablero.getWidth(), texture_tablero.getHeight(), 0,
					 texture_tablero.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_tablero.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_tablero.freeImage(); // Liberamos memoria

	// ****************************************************************
	// init(): EFECTOS
	// ****************************************************************

	// Definiendo la textura
	Texture texture_efecto_01("../media/textures_screen/efecto_ataque.PNG");
	texture_efecto_01.loadImage();									  // Cargar la textura
	glGenTextures(1, &txs_gameplay_efecto_ataque);					  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, txs_gameplay_efecto_ataque);		  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_efecto_01.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_efecto_01.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_efecto_01.getWidth(), texture_efecto_01.getHeight(), 0,
					 texture_efecto_01.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_efecto_01.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_efecto_01.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture texture_efecto_02("../media/textures_screen/efecto_defensa.PNG");
	texture_efecto_02.loadImage();									  // Cargar la textura
	glGenTextures(1, &txs_gameplay_efecto_defensa);					  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, txs_gameplay_efecto_defensa);		  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_efecto_02.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_efecto_02.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_efecto_02.getWidth(), texture_efecto_02.getHeight(), 0,
					 texture_efecto_02.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_efecto_02.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_efecto_02.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture texture_efecto_03("../media/textures_screen/efecto_velocidad.PNG");
	texture_efecto_03.loadImage();									  // Cargar la textura
	glGenTextures(1, &txs_gameplay_efecto_velocidad);				  // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, txs_gameplay_efecto_velocidad);	  // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture_efecto_03.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture_efecto_03.getChannels() == 3 ? GL_RGB : GL_RGBA, texture_efecto_03.getWidth(), texture_efecto_03.getHeight(), 0,
					 texture_efecto_03.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_efecto_03.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture_efecto_03.freeImage(); // Liberamos memoria

	// ****************************************************************
	// init(): OpenAL
	// ****************************************************************
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR)
	{
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else
	{
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../media/music/20210702_The First Town.wav");
	buffer[1] = alutCreateBufferFromFile("../media/sounds/zapsplat_fantasy_magic_wand_ping_bell_chime_classic_simple_001_92714.wav");
	buffer[2] = alutCreateBufferFromFile("../media/sounds/zapsplat_multimedia_game_sound_fun_magic_game_positive_bonus_award_level_up_003_61003.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR)
	{
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, al_sources);

	if (alGetError() != AL_NO_ERROR)
	{
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else
	{
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(al_sources[0], AL_PITCH, 1.0f);
	alSourcef(al_sources[0], AL_GAIN, 0.3f);
	alSourcefv(al_sources[0], AL_VELOCITY, source0Vel);
	alSourcei(al_sources[0], AL_BUFFER, buffer[0]);
	alSourcei(al_sources[0], AL_LOOPING, AL_TRUE);

	alSourcef(al_sources[1], AL_PITCH, 1.0f);
	alSourcef(al_sources[1], AL_GAIN, 0.5f);
	alSourcefv(al_sources[1], AL_VELOCITY, source1Vel);
	alSourcei(al_sources[1], AL_BUFFER, buffer[1]);
	alSourcei(al_sources[1], AL_LOOPING, AL_FALSE);

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	box_tablero.destroy();
	esfera1.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	rayModel.destroy();
	boxIntro.destroy();
	boxViewDepth.destroy();
	// models
	ModelCazador.destroy();
	ModelSanador.destroy();
	ModelCaballero.destroy();
	ModelVengador.destroy();
	ModelEsqueleto.destroy();
	ModelDemonio.destroy();
	ModelHechicero.destroy();
	ModelAnfiteres.destroy();

	// Custom objects Delete
	modelDecoBosque.destroy();
	// Cristales
	model_cristal_amarillo.destroy();
	model_cristal_azul.destroy();
	model_cristal_rojo.destroy();
	model_cristal_verde.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureTerrainKID);
	glDeleteTextures(1, &texture_tableroID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &txs_main_title01);
	glDeleteTextures(1, &txs_main_title02);
	glDeleteTextures(1, &txs_main_title03);
	glDeleteTextures(1, &txs_gameplay_ui_personajes);
	glDeleteTextures(1, &txs_gameplay_efecto_ataque);
	glDeleteTextures(1, &txs_gameplay_efecto_defensa);
	glDeleteTextures(1, &txs_gameplay_efecto_velocidad);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes)
{
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	// ****************************************************************
	// scrollCallback(): CÁMARAS
	// ****************************************************************
	distanceFromTarget -= yoffset;
	camera3P->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod)
{
	if (state == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication)
{
	if (exitApp || glfwWindowShouldClose(window) != 0)
	{
		return false;
	}

	// ****************************************************************
	// processInput(): TITLE SCREEM
	// ****************************************************************
	if (!iniciaPartida)
	{
		if (txs_active == txs_main_title01)
		{
			if (ena_key_enter && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				iniciaPartida = true;
				txs_active = txs_gameplay_ui_personajes;
				alSourcePlay(al_sources[1]);
			}
			else if (ena_key_left && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				ena_key_left = false;
				txs_active = txs_main_title02;
				alSourcePlay(al_sources[1]);
			}
			else if (ena_key_right && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				ena_key_right = false;
				txs_active = txs_main_title03;
				alSourcePlay(al_sources[1]);
			}
		}
		else if (txs_active == txs_main_title02)
		{
			if (ena_key_right && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				ena_key_right = false;
				txs_active = txs_main_title01;
				alSourcePlay(al_sources[1]);
			}
		}
		else if (txs_active == txs_main_title03)
		{
			if (ena_key_left && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				ena_key_left = false;
				txs_active = txs_main_title01;
				alSourcePlay(al_sources[1]);
			}
			else if (ena_key_enter && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				std::cout << "JUEGO CERRADO" << std::endl;
				exit(0);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		{
			ena_key_enter = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		{
			ena_key_left = true;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		{
			ena_key_right = true;
		}
	}

	// ****************************************************************
	// processInput(): CÁMARAS
	// ****************************************************************
	if (cameraState == "libre")
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera1P->moveFrontCamera(true, deltaTime * 2);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera1P->moveFrontCamera(false, deltaTime * 2);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera1P->moveRightCamera(false, deltaTime * 2);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera1P->moveRightCamera(true, deltaTime * 2);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera1P->mouseMoveCamera(offsetX, offsetY, deltaTime * 2);
	}
	else if (cameraState == "tercera_p")
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera3P->mouseMoveCamera(offsetX, 0, deltaTime);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera3P->mouseMoveCamera(0, offsetY, deltaTime);
	}
	offsetX = 0;
	offsetY = 0;

	// ****************************************************************
	// processInput(): EFECTOS
	// ****************************************************************
	if (ena_key_z && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		txs_active = txs_gameplay_efecto_ataque;
	}
	else if (ena_key_x && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		txs_active = txs_gameplay_efecto_defensa;
	}
	else if (ena_key_c && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		txs_active = txs_gameplay_efecto_velocidad;
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
	{
		ena_key_z = true;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
	{
		ena_key_x = true;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
	{
		ena_key_c = true;
	}

	// ****************************************************************
	// processInput(): FLECHAS
	// ****************************************************************
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		cursor_y += 0.001;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		cursor_y -= 0.001;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		cursor_x -= 0.001;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		cursor_x += 0.001;
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE)
	{
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

		// if (fabs(axes[3]) > 0.2)
		// {
		// 	camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
		// }
		// if (fabs(axes[4]) > 0.2)
		// {
		// 	camera->mouseMoveCamera(0.0, axes[4], deltaTime);
		// }

		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount << std::endl;
		if (buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if (!isJump && buttons[0] == GLFW_PRESS)
		{
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	// ****************************************************************
	// processInput(): MOVIMIENTOS DE MODELOS
	// ****************************************************************

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		enableCountSelected = false;
		CazadorSelected = true;
		SanadorSelected = true;
		CaballeroSelected = true;
		VengadorSelected = true;
		modelSelected++;
		if (modelSelected > 4)
			modelSelected = 1;

		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !enableTirada)
	{
		enableTirada = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && enableTirada)
	{
		enableTirada = false;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !enableTirada)
	{
		enableTirada = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && enableTirada)
	{
		enableTirada = false;
	}

	//--------------------------------------------------------
	if (modelSelected == 1 && enableTirada)
	{

		// tirada de salvacion antes de realizar su acciones
		if (CazadorSelected && Cazador.vida > 0)
		{
			if (CazadorSelected && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			{ // tira el dado y avanza

				CazadorSelected = false;

				int dado = 1 + rand() % 6;
				printf("Entro a accion 1 del cazador y el dado dio %d\n", dado);

				GeneradorMs = MostruosGenerdor(GeneradorMs);

				Tipo_Casilla = keyTablero(Cazador.PossTablero + dado);

				Tablero(&Cazador, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);

				if (Tipo_Casilla == 1)
				{
					PerderBuffo(buffo, &Cazador);
				}

				//----------------------Movimientos-------------------------------------
				ModelMatrixCazador=Movimiento(&Cazador,dado,ModelMatrixCazador);
				//------------------------Fin Movimienots-----------------------------------------------

				printf("Entro a accion 1 del cazador y su ataque es %d\n", Cazador.ataque);
				printf("Entro a accion 1 del cazador y su defensa es %d\n", Cazador.defensa);
				printf("Entro a accion 1 del cazador y su vida es %d\n", Cazador.vida);
				printf("Entro a accion 1 del cazador y su poss es %d\n", Cazador.PossTablero);

				printf("Habilidad en CD Tira el Dado (Accion 1) CD %d / %d\n", Cazador.CD, Cazador.CDMX);
			}

			if (CazadorSelected && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			{ // diplica su dañao y avanza
				if (Cazador.CD == Cazador.CDMX)
				{
					Cazador.ataque = Cazador.ataque * 2;
					int dado = 1 + rand() % 6;
					printf("Entro a accion 2 del cazador y el dado dio %d\n", dado);
					printf("Entro a accion 2 del cazador y su ataque es %d\n", Cazador.ataque);

					GeneradorMs = MostruosGenerdor(GeneradorMs);
					Tipo_Casilla = keyTablero(Cazador.PossTablero + dado);

					Tablero(&Cazador, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
					Cazador.ataque = Cazador.ataque / 2;

					if (Tipo_Casilla == 1)
					{
						PerderBuffo(buffo, &Cazador);
					}

					//----------------------Movimientos-------------------------------------
					ModelMatrixCazador=Movimiento(&Cazador,dado,ModelMatrixCazador);
					//------------------------Fin Movimienots-----------------------------------------------

					printf("Entro a accion 2 del cazador y su ataque es %d\n", Cazador.ataque);
					printf("Entro a accion 2 del cazador y su defensa es %d\n", Cazador.defensa);
					printf("Entro a accion 2 del cazador y su vida es %d\n", Cazador.vida);
					printf("Entro a accion 2 del cazador y su poss es %d\n", Cazador.PossTablero);
					printf("Entro a accion Ms vida %d\n", GeneradorMs.vida);

					Cazador.CD = 0;
					CazadorSelected = false;
				}
				else
				{
					printf("Habilidad en CD Tira el Dado (Accion 1) CD %d / %d\n", Cazador.CD, Cazador.CDMX);
				}
			}
		}
		else if (CazadorSelected && Cazador.vida <= 0)
		{
			Cazador = TiradaDeSalvacion(Cazador);

			CazadorSelected = false;
		}
	}

	if (modelSelected == 2 && enableTirada)
	{

		// tirada de salvacion antes de realizar su acciones

		if (SanadorSelected && Sanador.vida > 0)
		{

			if (SanadorSelected && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			{ // tira el dado y avanza
				SanadorSelected = false;
				int dado = 1 + rand() % 6;
				GeneradorMs = MostruosGenerdor(GeneradorMs);
				Tipo_Casilla = keyTablero(Sanador.PossTablero + dado);
				Tablero(&Sanador, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
				if (Tipo_Casilla == 1)
				{
					PerderBuffo(buffo, &Sanador);
				}

				//----------------------Movimientos-------------------------------------
				ModelMatrixSanador=Movimiento(&Sanador,dado,ModelMatrixSanador);
				//------------------------Fin Movimienots-----------------------------------------------

				printf("Entro a accion 1 del sanador y su ataque es %d\n", Sanador.ataque);
				printf("Entro a accion 1 del sanador y su defensa es %d\n", Sanador.defensa);
				printf("Entro a accion 1 del sanador y su vida es %d\n", Sanador.vida);
				printf("Entro a accion 1 del sanador y su poss es %d\n", Sanador.PossTablero);

				printf("Entro a accion Ms vida %d\n", GeneradorMs.vida);
			}
			if (SanadorSelected && glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			{
				SanadorSelected = false;
				if (Sanador.CD == Sanador.CDMX)
				{
					if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
					{
						Cazador.vida = Cazador.vida + 2;
					}
					if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
					{
						Caballero.vida = Caballero.vida + 2;
					}
					if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
					{
						Vengador.vida = Vengador.vida + 2;
					}
					int dado = 1 + rand() % 6;
					GeneradorMs = MostruosGenerdor(GeneradorMs);
					Tipo_Casilla = keyTablero(Sanador.PossTablero + dado);
					Tablero(&Sanador, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
					if (Tipo_Casilla == 1)
					{
						PerderBuffo(buffo, &Sanador);
					}
					//----------------------Movimientos-------------------------------------
					ModelMatrixSanador=Movimiento(&Sanador,dado,ModelMatrixSanador);
					//------------------------Fin Movimienots-----------------------------------------------
					printf("Entro a accion 2 del sanador y su ataque es %d\n", Sanador.ataque);
					printf("Entro a accion 2 del sanador y su defensa es %d\n", Sanador.defensa);
					printf("Entro a accion 2 del sanador y su vida es %d\n", Sanador.vida);
					printf("Entro a accion 2 del sanador y su poss es %d\n", Sanador.PossTablero);
				}
				else
					printf("Habilidad en CD Tira el Dado (Accion 1) CD %d / %d\n", Sanador.CD, Sanador.CDMX);
			}
		}
		if (SanadorSelected && Sanador.vida <= 0)
		{
			Sanador = TiradaDeSalvacion(Sanador);
			SanadorSelected = false;
		}
	}
	if (modelSelected == 3 && enableTirada)
	{
		if (CaballeroSelected && Caballero.vida > 0)
		{

			if (CaballeroSelected && glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			{ // tira el dado y avanza
				CaballeroSelected = false;
				int dado = 1 + rand() % 6;
				GeneradorMs = MostruosGenerdor(GeneradorMs);
				Tipo_Casilla = keyTablero(Caballero.PossTablero + dado);
				Tablero(&Caballero, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
				if (Tipo_Casilla == 1)
				{
					PerderBuffo(buffo, &Caballero);
				}
				//----------------------Movimientos-------------------------------------
				ModelMatrixCaballero=Movimiento(&Caballero,dado,ModelMatrixCaballero);
				//------------------------Fin Movimienots-----------------------------------------------
				printf("Entro a accion 1 del caballero y su ataque es %d\n", Caballero.ataque);
				printf("Entro a accion 1 del caballero y su defensa es %d\n", Caballero.defensa);
				printf("Entro a accion 1 del caballero y su vida es %d\n", Caballero.vida);
				printf("Entro a accion 1 del caballero y su poss es %d\n", Caballero.PossTablero);
			}
			if (CaballeroSelected && glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			{ // diplica su dañao y avanza
				CaballeroSelected = false;
				Caballero.defensa = Caballero.defensa + 2;
				int dado = 1 + rand() % 6;
				MostruosGenerdor(GeneradorMs);
				Tipo_Casilla = keyTablero(Caballero.PossTablero + dado);
				Tablero(&Caballero, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
				if (Tipo_Casilla == 1)
				{
					PerderBuffo(buffo, &Caballero);
				}
				//----------------------Movimientos-------------------------------------
				ModelMatrixCaballero=Movimiento(&Caballero,dado,ModelMatrixCaballero);
				//------------------------Fin Movimienots-----------------------------------------------
				Caballero.defensa = Caballero.defensa - 2;
				printf("Entro a accion 2 del caballero y su ataque es %d\n", Caballero.ataque);
				printf("Entro a accion 2 del caballero y su defensa es %d\n", Caballero.defensa);
				printf("Entro a accion 2 del caballero y su vida es %d\n", Caballero.vida);
				printf("Entro a accion 2 del caballero y su poss es %d\n", Caballero.PossTablero);
			}
		}
		if (CaballeroSelected && Caballero.vida <= 0)
		{
			Caballero = TiradaDeSalvacion(Caballero);
			CaballeroSelected = false;
		}
	}

	if (modelSelected == 4 && enableTirada)
	{
		if (VengadorSelected && glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		{ // tira el dado y avanza
			VengadorSelected = false;
			int dado = 1 + rand() % 5;
			MostruosGenerdor(GeneradorMs);
			Tipo_Casilla = keyTablero(Vengador.PossTablero + dado);
			Tablero(&Vengador, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
			if (Tipo_Casilla == 1)
			{
				PerderBuffo(buffo, &Vengador);
			}
			//----------------------Movimientos-------------------------------------
			ModelMatrixVengador=Movimiento(&Vengador,dado,ModelMatrixVengador);
			//------------------------Fin Movimienots-----------------------------------------------
			printf("Entro a accion 1 del vengador y su ataque es %d\n", Vengador.ataque);
			printf("Entro a accion 1 del vengador y su defensa es %d\n", Vengador.defensa);
			printf("Entro a accion 1 del vengador y su vida es %d\n", Vengador.vida);
			printf("Entro a accion 1 del vengador y su poss es %d\n", Vengador.PossTablero);
		}
		if (VengadorSelected && glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		{ // diplica su dañao y avanza .
			VengadorSelected = false;
			Vengador.ataque = Vengador.ataque + (10 - Vengador.vida);
			int dado = 1 + rand() % 5;
			MostruosGenerdor(GeneradorMs);
			Tipo_Casilla = keyTablero(Vengador.PossTablero + dado);
			Tablero(&Vengador, dado, &GeneradorMs, &Anfiteres, Tipo_Casilla);
			if (Tipo_Casilla == 1)
			{
				PerderBuffo(buffo, &Vengador);
			}
			//----------------------Movimientos-------------------------------------
			ModelMatrixVengador=Movimiento(&Vengador,dado,ModelMatrixVengador);
			//------------------------Fin Movimienots-----------------------------------------------
			printf("Entro a accion 2 del vengador y su ataque es %d\n", Vengador.ataque);
			printf("Entro a accion 2 del vengador y su defensa es %d\n", Vengador.defensa);
			printf("Entro a accion 2 del vengador y su vida es %d\n", Vengador.vida);
			printf("Entro a accion 2 del vengador y su poss es %d\n", Vengador.PossTablero);
		}
		if (VengadorSelected && Vengador.vida <= 0)
		{
			Vengador = TiradaDeSalvacion(Vengador);
			VengadorSelected = false;
		}
		 
	}
	//--------------------------------------------------------
	// Cambiar tipo de camara
	//if (enable_camera && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//	{
//		cameraState = "libre";
//		enable_camera = false;
//	}
//	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
//	{
//		enable_camera = true;
//	}
	glfwPollEvents();
	return continueApplication;
}

void prepareScene()
{

	terrain.setShader(&shaderTerrain);
}

void prepareDepthScene()
{

	terrain.setShader(&shaderDepth);
}

void renderSolidScene()
{
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del agua
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainKID);
	shaderTerrain.setInt("backgroundTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));

	terrain.setPosition(glm::vec3(100, 0, 100));
	terrain.render();

	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	// ****************************************************************
	// MODELOS (OBJ, FBX)
	// ****************************************************************

	glm::mat4 renderMatrix_DecoBosque = glm::mat4(modelMatrixDecoBosque);
	modelDecoBosque.render(renderMatrix_DecoBosque);

	// Cristales (render)
	for (int i = 0; i < poss_cristal_amarillo.size(); i++)
	{
		poss_cristal_amarillo[i].y = terrain.getHeightTerrain(poss_cristal_amarillo[i].x, poss_cristal_amarillo[i].z);
		model_cristal_amarillo.setPosition(poss_cristal_amarillo[i]);
		model_cristal_amarillo.render();
	}
	for (int i = 0; i < poss_cristal_azul.size(); i++)
	{
		poss_cristal_azul[i].y = terrain.getHeightTerrain(poss_cristal_azul[i].x, poss_cristal_azul[i].z);
		model_cristal_azul.setPosition(poss_cristal_azul[i]);
		model_cristal_azul.render();
	}
	for (int i = 0; i < poss_cristal_rojo.size(); i++)
	{
		poss_cristal_rojo[i].y = terrain.getHeightTerrain(poss_cristal_rojo[i].x, poss_cristal_rojo[i].z);
		model_cristal_rojo.setPosition(poss_cristal_rojo[i]);
		model_cristal_rojo.render();
	}
	for (int i = 0; i < poss_cristal_verde.size(); i++)
	{
		poss_cristal_verde[i].y = terrain.getHeightTerrain(poss_cristal_verde[i].x, poss_cristal_verde[i].z);
		model_cristal_verde.setPosition(poss_cristal_verde[i]);
		model_cristal_verde.render();
	}

	// Models Huesos

	glm::mat4 renderMatrixCazador = glm::mat4(ModelMatrixCazador);
	renderMatrixCazador = glm::scale(renderMatrixCazador, glm::vec3(0.02f, 0.02f, 0.02f));
	glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(ModelMatrixCazador[3][0], ModelMatrixCazador[3][2]));
	glm::vec3 ejex = glm::vec3(ModelMatrixCazador[0]);
	glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixCazador[0] = glm::vec4(ejex, 0.0);
	ModelMatrixCazador[1] = glm::vec4(ejey, 0.0);
	ModelMatrixCazador[2] = glm::vec4(ejez, 0.0);
	ModelMatrixCazador[3][1] = terrain.getHeightTerrain(ModelMatrixCazador[3][0], ModelMatrixCazador[3][2]);
	ModelCazador.setAnimationIndex(1);
	ModelCazador.render(renderMatrixCazador);

	glm::mat4 renderMatrixSanador = glm::mat4(ModelMatrixSanador);
	renderMatrixSanador = glm::scale(renderMatrixSanador, glm::vec3(0.02f, 0.02f, 0.02f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixSanador[0] = glm::vec4(ejex, 0.0);
	ModelMatrixSanador[1] = glm::vec4(ejey, 0.0);
	ModelMatrixSanador[2] = glm::vec4(ejez, 0.0);
	ModelMatrixSanador[3][1] = terrain.getHeightTerrain(ModelMatrixSanador[3][0], ModelMatrixSanador[3][2]);
	ModelSanador.setAnimationIndex(0);
	ModelSanador.render(renderMatrixSanador);

	glm::mat4 renderMatrixCaballero = glm::mat4(ModelMatrixCaballero);
	renderMatrixCaballero = glm::scale(renderMatrixCaballero, glm::vec3(0.02f, 0.02f, 0.02f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixCaballero[0] = glm::vec4(ejex, 0.0);
	ModelMatrixCaballero[1] = glm::vec4(ejey, 0.0);
	ModelMatrixCaballero[2] = glm::vec4(ejez, 0.0);
	ModelMatrixCaballero[3][1] = terrain.getHeightTerrain(ModelMatrixCaballero[3][0], ModelMatrixCaballero[3][2]);
	ModelCaballero.setAnimationIndex(0);
	ModelCaballero.render(renderMatrixCaballero);

	glm::mat4 renderMatrixVengador = glm::mat4(ModelMatrixVengador);
	renderMatrixVengador = glm::scale(renderMatrixVengador, glm::vec3(0.02f, 0.02f, 0.02f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixVengador[0] = glm::vec4(ejex, 0.0);
	ModelMatrixVengador[1] = glm::vec4(ejey, 0.0);
	ModelMatrixVengador[2] = glm::vec4(ejez, 0.0);
	ModelMatrixVengador[3][1] = terrain.getHeightTerrain(ModelMatrixVengador[3][0], ModelMatrixVengador[3][2]);
	ModelVengador.setAnimationIndex(0);
	ModelVengador.render(renderMatrixVengador);

	glm::mat4 renderMatrixAnfiteres = glm::mat4(ModelMatrixAnfiteres);
	renderMatrixAnfiteres = glm::scale(renderMatrixAnfiteres, glm::vec3(5.0f, 5.0f, 5.0f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixAnfiteres[0] = glm::vec4(ejex, 0.0);
	ModelMatrixAnfiteres[1] = glm::vec4(ejey, 0.0);
	ModelMatrixAnfiteres[2] = glm::vec4(ejez, 0.0);
	ModelMatrixAnfiteres[3][1] = terrain.getHeightTerrain(ModelMatrixAnfiteres[3][0], ModelMatrixAnfiteres[3][2]);
	ModelAnfiteres.setAnimationIndex(1);
	ModelAnfiteres.render(renderMatrixAnfiteres);

	glm::mat4 renderMatrixEsqueleto = glm::mat4(ModelMatrixEsqueleto);
	renderMatrixEsqueleto = glm::scale(renderMatrixEsqueleto, glm::vec3(0.1f, 0.1f, 0.1f));
	renderMatrixEsqueleto = glm::rotate(renderMatrixEsqueleto,glm::radians(90.0f),glm::vec3(-1.0f, 0.0f, 0.0f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixEsqueleto[0] = glm::vec4(ejex, 0.0);
	ModelMatrixEsqueleto[1] = glm::vec4(ejey, 0.0);
	ModelMatrixEsqueleto[2] = glm::vec4(ejez, 0.0);
	ModelMatrixEsqueleto[3][1] = terrain.getHeightTerrain(ModelMatrixEsqueleto[3][0], ModelMatrixEsqueleto[3][2]);
	ModelEsqueleto.render(renderMatrixEsqueleto);
	
	glm::mat4 renderMatrixDemonio = glm::mat4(ModelMatrixDemonio);
	renderMatrixDemonio = glm::scale(renderMatrixDemonio, glm::vec3(0.2f, 0.2f, 0.2f));
	//renderMatrixDemonio = glm::rotate(renderMatrixDemonio,glm::radians(90.0f),glm::vec3(-1.0f, 0.0f, 0.0f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixDemonio[0] = glm::vec4(ejex, 0.0);
	ModelMatrixDemonio[1] = glm::vec4(ejey, 0.0);
	ModelMatrixDemonio[2] = glm::vec4(ejez, 0.0);
	ModelMatrixDemonio[3][1] = terrain.getHeightTerrain(ModelMatrixDemonio[3][0], ModelMatrixDemonio[3][2]);
	ModelDemonio.render(renderMatrixDemonio);
	
	glm::mat4 renderMatrixHechicero = glm::mat4(ModelMatrixHechicero);
	renderMatrixHechicero = glm::scale(renderMatrixHechicero, glm::vec3(0.01f, 0.01f, 0.01f));
	//renderMatrixHechicero = glm::rotate(renderMatrixHechicero,glm::radians(90.0f),glm::vec3(-1.0f, 0.0f, 0.0f));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	ModelMatrixHechicero[0] = glm::vec4(ejex, 0.0);
	ModelMatrixHechicero[1] = glm::vec4(ejey, 0.0);
	ModelMatrixHechicero[2] = glm::vec4(ejez, 0.0);
	ModelMatrixHechicero[3][1] = terrain.getHeightTerrain(ModelMatrixHechicero[3][0], ModelMatrixHechicero[3][2]);
	ModelHechicero.render(renderMatrixHechicero);
	
	

	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void renderAlphaScene(bool render = true)
{
	// ****************************************************************
	// renderAlphaScene(): update the position with alpha objects
	// ****************************************************************

	// ****************************************************************
	// renderAlphaScene(): sorter with alpha objects
	// ****************************************************************

	// ****************************************************************
	// renderAlphaScene(): MODELOS TRANSPARENTES
	// ****************************************************************
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	box_tablero.setScale(glm::vec3(50.0f, 0.05f, 50.0f));
	box_tablero.setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	box_tablero.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_tableroID);
	shaderMulLighting.setInt("texture1", 0);
	box_tablero.render();
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (render)
	{
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, txs_active);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		// Coordenadas de 1 a 0 (nota: alineadas a la izquierda)

		// std::cout << "cursor_x: " << cursor_x << std::endl;
		// std::cout << "cursor_y: " << cursor_y << std::endl;

		sprintf(CadenaVidaCazador, "%d/6", Cazador.vida);
		modelText->render(CadenaVidaCazador, -0.5460, 0.8330);
		sprintf(CadenaVidaSanador, "%d/10", Sanador.vida);
		modelText->render(CadenaVidaSanador, -0.0540, 0.8330);
		sprintf(CadenaVidaCaballero, "%d/8", Caballero.vida);
		modelText->render(CadenaVidaCaballero, 0.4229, 0.8330);
		sprintf(CadenaVidaVengador, "%d/10", Vengador.vida);
		modelText->render(CadenaVidaVengador, 0.8939, 0.8330);

		// cursor_x: -0.5460
		// cursor_y: 0.8330

		// cursor_x: -0.0540
		// cursor_y: 0.8330

		// cursor_x: 0.4229
		// cursor_y: 0.8330

		// cursor_x: 0.8939
		// cursor_y: 0.8330
	}
}

void renderScene()
{
	renderSolidScene();
	renderAlphaScene(false);
}

void applicationLoop()
{
	bool psi = true;

	// ****************************************************************
	// Variables de las CÁMARAS
	// ****************************************************************
	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	// ****************************************************************
	// applicationLoop(): POSICIÓN INICIAL DE LOS MODELOS
	// ****************************************************************

	modelMatrixDecoBosque = glm::translate(modelMatrixDecoBosque, glm::vec3(0.0, 0.0, 0.0));

	// ModelMatrixCazador = glm::translate(ModelMatrixCazador, glm::vec3(10.0, 0.0, -50.0));

	// ModelMatrixSanador = glm::translate(ModelMatrixSanador, glm::vec3(15.0, 0.0, -50.0));

	// ModelMatrixCaballero = glm::translate(ModelMatrixCaballero, glm::vec3(20.0, 0.0, -50.0));
	// X = (160/512) * 50 = 15.625
	// Y = (224/512) * 50 = 21.875
	// 6.25
	// 224, 226

	ModelMatrixVengador = glm::translate(ModelMatrixVengador, glm::vec3(16.625, 0.0, 23.565));

	ModelMatrixCaballero = glm::translate(ModelMatrixCaballero, glm::vec3(14.625, 0.0, 23.565));

	ModelMatrixCazador = glm::translate(ModelMatrixCazador, glm::vec3(16.625, 0.0, 20.885));

	ModelMatrixSanador = glm::translate(ModelMatrixSanador, glm::vec3(14.625, 0.0, 20.885));

	ModelMatrixAnfiteres = glm::translate(ModelMatrixAnfiteres, glm::vec3(0.0, 0.0, 0.0));

	ModelMatrixEsqueleto = glm::translate(ModelMatrixEsqueleto, glm::vec3(5.0, 0.0, 0.0));

	ModelMatrixEsqueleto = glm::translate(ModelMatrixDemonio, glm::vec3(0.0, 0.0, 0.5));

	ModelMatrixEsqueleto = glm::translate(ModelMatrixHechicero, glm::vec3(0.5, 0.0, 0.0));

	// Variables to interpolation key frames
	// ***

	lastTime = TimeManager::Instance().GetTime();

	txs_active = txs_main_title01;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera1P.get(), 15.0f, 0.1f, 45.0f);

	std::cout << "BEFORE PSI" << std::endl;
	while (psi)
	{
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667)
		{
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
												(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

		// ****************************************************************
		// applicationLoop(), while(psi): CÁMARAS
		// ****************************************************************
		// pruebas camara
		if (modelSelected == 1)
		{
			target = ModelMatrixCazador[3];
			angleTarget = glm::angle(glm::quat_cast(ModelMatrixCazador));
			axis = glm::axis(glm::quat_cast(ModelMatrixCazador));
		}
		else if (modelSelected == 2)
		{
			target = ModelMatrixSanador[3];
			angleTarget = glm::angle(glm::quat_cast(ModelMatrixSanador));
			axis = glm::axis(glm::quat_cast(ModelMatrixSanador));
		}
		else if (modelSelected == 3)
		{
			target = ModelMatrixCaballero[3];
			angleTarget = glm::angle(glm::quat_cast(ModelMatrixCaballero));
			axis = glm::axis(glm::quat_cast(ModelMatrixCaballero));
		}
		else if (modelSelected == 4)
		{
			target = ModelMatrixVengador[3];
			angleTarget = glm::angle(glm::quat_cast(ModelMatrixVengador));
			axis = glm::axis(glm::quat_cast(ModelMatrixVengador));
		}
		camera3P->setAngleTarget(angleTarget);
		camera3P->setCameraTarget(target);
		camera3P->updateCamera();
		view = camera3P->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
								glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
								glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
									 glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
									 glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
									 glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
								 glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
								 glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
								 glm::value_ptr(lightSpaceMatrix));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		glm::vec3 color_fog = glm::vec3(71.5 / 255, 106.7 / 255, 132.6 / 255);
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(color_fog));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(color_fog));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(color_fog));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera1P->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera1P->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		// Usamos size() para determinar el número de lámparas que tenemos
		int pointLights_num = poss_cristal_amarillo.size() + poss_cristal_azul.size() + poss_cristal_rojo.size() + poss_cristal_verde.size();
		// shaderMulLighting.setInt("pointLightCount", 1);
		// shaderTerrain.setInt("pointLightCount", 1);

		// cristal_amarillo
		for (int i = 0; i < poss_cristal_amarillo.size(); i++)
		{
			glm::mat4 matrixAdjust = glm::mat4(1.0);
			matrixAdjust = glm::translate(matrixAdjust, poss_cristal_amarillo[i]);
			matrixAdjust = glm::scale(matrixAdjust, glm::vec3(0.5));
			matrixAdjust = glm::translate(matrixAdjust, glm::vec3(0, 10.55, 0));

			glm::vec3 pos_matrixAdjust = glm::vec3(matrixAdjust[3]);
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(1.0, 1.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);

			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(1.0, 1.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		// cristal_azul
		for (int i = 0; i < poss_cristal_azul.size(); i++)
		{
			glm::mat4 matrixAdjust = glm::mat4(1.0);
			matrixAdjust = glm::translate(matrixAdjust, poss_cristal_azul[i]);
			matrixAdjust = glm::scale(matrixAdjust, glm::vec3(0.5));
			matrixAdjust = glm::translate(matrixAdjust, glm::vec3(0, 10.55, 0));

			glm::vec3 pos_matrixAdjust = glm::vec3(matrixAdjust[3]);
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);

			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		// cristal_rojo
		for (int i = 0; i < poss_cristal_rojo.size(); i++)
		{
			glm::mat4 matrixAdjust = glm::mat4(1.0);
			matrixAdjust = glm::translate(matrixAdjust, poss_cristal_rojo[i]);
			matrixAdjust = glm::scale(matrixAdjust, glm::vec3(0.5));
			matrixAdjust = glm::translate(matrixAdjust, glm::vec3(0, 10.55, 0));

			glm::vec3 pos_matrixAdjust = glm::vec3(matrixAdjust[3]);
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);

			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		// cristal_verde
		for (int i = 0; i < poss_cristal_verde.size(); i++)
		{
			glm::mat4 matrixAdjust = glm::mat4(1.0);
			matrixAdjust = glm::translate(matrixAdjust, poss_cristal_verde[i]);
			matrixAdjust = glm::scale(matrixAdjust, glm::vec3(0.5));
			matrixAdjust = glm::translate(matrixAdjust, glm::vec3(0, 10.55, 0));

			glm::vec3 pos_matrixAdjust = glm::vec3(matrixAdjust[3]);
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);

			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(pos_matrixAdjust));
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		// ... FIN: Propiedades PointLights
		// ****************************************************************

		// OpenAL

		// Set up the listener
		ALfloat listenerPos[] = {0.0, 0.0, 0.0};				 // Listener position (x, y, z)
		ALfloat listenerVel[] = {0.0, 0.0, 0.0};				 // Listener velocity
		ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0}; // Listener orientation (at, up)
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);

		// std::cout << "TITLE SCREEN" << std::endl;
		// ****************************************************************
		// applicationLoop(): while(psi): Render de la imagen de frente
		// ****************************************************************
		if (!iniciaPartida)
		{
			if (al_music_play[0])
			{
				al_music_play[0] = false;
				alSourcePlay(al_sources[0]);
			}

			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, txs_active);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}
		else if (!en_pantalla_de_juego)
		{
			en_pantalla_de_juego = true;
			alDeleteSources(1, al_sources);
		}

		// std::cout << "GAME STARTED" << std::endl;
		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setFloat("near_plane", near_plane);
		shaderViewDepth.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
				 collidersOBB.begin();
			 it != collidersOBB.end(); it++)
		{
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
				 collidersSBB.begin();
			 it != collidersSBB.end(); it++)
		{
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		/**********Render de transparencias***************/
		renderAlphaScene();

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
					  std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
				 collidersSBB.begin();
			 it != collidersSBB.end(); it++)
		{
			bool isCollision = false;
			for (std::map<std::string,
						  std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
					 collidersSBB.begin();
				 jt != collidersSBB.end(); jt++)
			{
				if (it != jt && testSphereSphereIntersection(
									std::get<0>(it->second), std::get<0>(jt->second)))
				{
					std::cout << "Hay collision entre " << it->first << " y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
					  std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
				 collidersOBB.begin();
			 it != collidersOBB.end(); it++)
		{
			bool isColision = false;
			for (std::map<std::string,
						  std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
					 collidersOBB.begin();
				 jt != collidersOBB.end(); jt++)
			{
				if (it != jt &&
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second)))
				{
					std::cout << "Hay colision entre " << it->first << " y el modelo" << jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
					  std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
				 collidersSBB.begin();
			 it != collidersSBB.end(); it++)
		{
			bool isCollision = false;
			for (std::map<std::string,
						  std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
					 collidersOBB.begin();
				 jt != collidersOBB.end(); jt++)
			{
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second)))
				{
					std::cout << "Hay colision del " << it->first << " y el modelo" << jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin();
			 itCollision != collisionDetection.end(); itCollision++)
		{
			std::map<std::string, std::tuple<AbstractModel::SBB,
											 glm::mat4, glm::mat4>>::iterator sbbBuscado =
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
											 glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end())
			{
				if (!itCollision->second)
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end())
			{
				if (!itCollision->second)
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else
				{
					// ***
				}
			}
		}

		glfwSwapBuffers(window);

		// ****************************************************************
		// applicationLoop(): while(psi): OpenAL (sound data)
		// ****************************************************************

		// Listener for the First person camera
		listenerPos[0] = camera1P->getPosition().x;
		listenerPos[1] = camera1P->getPosition().y;
		listenerPos[2] = camera1P->getPosition().z;
		alListenerfv(AL_POSITION, listenerPos);

		listenerOri[0] = camera1P->getFront().x;
		listenerOri[1] = camera1P->getFront().y;
		listenerOri[2] = camera1P->getFront().z;
		listenerOri[3] = camera1P->getUp().x;
		listenerOri[4] = camera1P->getUp().y;
		listenerOri[5] = camera1P->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for (unsigned int i = 0; i < sourcesPlay.size(); i++)
		{
			if (sourcesPlay[i])
			{
				sourcesPlay[i] = false;
				alSourcePlay(al_sources[i]);
			}
		}
	}
}

int main(int argc, char **argv)
{
	init(1280, 720, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}