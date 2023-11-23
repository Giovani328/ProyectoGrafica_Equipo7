/*---------------------------------------------------------*/
/* ----------------  Proyecto Museo "Nintendo"                -----------*/
/*-----------------    2024-1   ---------------------------*/
/*------------- Alumnos:  Baez Cadena Duilio Giovani   y Lozano Neri Israel                   ---------------*/
/*------------- No. Cuenta       318047355                    317044447     ---------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

		
//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(20.0f, 20.0f, 450.0f));
float MovementSpeed = 5.0f;
float lastX = SCR_WIDTH / 1.0f;
float lastY = SCR_HEIGHT / 1.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(78.0f, 11.4f, -55.7f);
glm::vec3 lightPosition2(78.0f, 11.4f, -78.7f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;


int estado = 0;
int estadoCarro = 0;
int estadoHongo = 0;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
rotRodDer = 0.0f,                   //Agregados nuevos
giroMonito = 0.0f,
movBrazoDer = 0.0f,
movCabeza = 0.0f,
movBrazoIzq = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
rotIncDer = 0.0f,
giroMonitoInc = 0.0f,
movBrazoDerInc = 0.0f,
movCabezaInc = 0.0f,
movBrazoIzqInc = 0.0f,
subeMoneda = 20.0f,
rotMoneda = 0.0f,

//Variables Hongo
rotHongo = 0.0f,
hongo_x = -122.0f,
hongo_y = 32.0f,
hongo_z = -140.0f;

bool	activate_hongo_animation = false;

//Variables Carro

float	car_pos_ini_x = -90.0f,
car_pos_ini_z = -140.0f,
car_x = 0.0f,
car_z = 0.0f,
pos_vuelta = 0.0f,
car_direction = 0.0f,
car_theta1 = 4.7123f;

bool	activate_car_animation = false;

int		car_state = 0;

//VARIABLES ESPADA
float espadaY = -1.0f,
rotEspada = 0.0f,
rotEspadaX = 0.0f,
rotEspadaY = 1.0f,
rotEspadaZ = 0.0f;

int espada_state = 0;

bool	activate_espada_animation = false;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;                                             //agregar a estructura nuevos
	float rotRodDer;
	float giroMonito;
	float movBrazoDer;
	float movCabeza;
	float movBrazoIzq;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;
bool flag = false;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);

	/*std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
	*/
	KeyFrame[0].posX = posX; //posX+15.0f;       //posiciones en 0 en inicio
	KeyFrame[0].posX = posX; //posY+20.0f;
	KeyFrame[0].posX = posX; //posZ+50.0f;

	KeyFrame[0].rotRodIzq = rotRodIzq;               //Primer caso seteamos
	KeyFrame[0].rotRodDer = rotRodDer;
	KeyFrame[0].giroMonito = giroMonito;

	KeyFrame[0].movBrazoDer = movBrazoDer;
	KeyFrame[0].movCabeza = movCabeza;
	KeyFrame[0].movBrazoIzq = movBrazoIzq;

	for (int i = 1; i < MAX_FRAMES; i++) {                          // de 1 a menor que 9(MAX FRAMES) entonces serian 8
		KeyFrame[i].posX = posX + 1.5f; //posX+15.0f;    //1.5 EN X Y y Z PARA QUE SUBA
		KeyFrame[i].posY = posX + 1.5f; //posY+20.0f;
		KeyFrame[i].posZ = posX + 1.5f; //posZ+50.0f;

		KeyFrame[i].rotRodIzq = rotRodIzq + 50.0f;     //UNA ADELANTE
		KeyFrame[i].rotRodDer = rotRodDer - 20.0f;     //UNA ATRAS
		KeyFrame[i].giroMonito = giroMonito + 80.0f;   //GIRO MUÑECO

		KeyFrame[i].movBrazoDer = movBrazoDer + 10.0f;    //DERECHO E IZQUIERDO HACIA LA MISMA DIRECCION
		KeyFrame[i].movCabeza = movCabeza + 100.0f;         //GIRA A LA DERECHA
		KeyFrame[i].movBrazoIzq = movBrazoIzq + 10.0f;
	}
	FrameIndex = 9;

}


void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotRodDer = KeyFrame[0].rotRodDer;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazoDer = KeyFrame[0].movBrazoDer;
	movCabeza = KeyFrame[0].movCabeza;
	movBrazoIzq = KeyFrame[0].movBrazoIzq;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	rotIncDer = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;         //agregar la rodilla el brazo y lo que faltaba
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;
	movCabezaInc = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps;
	movBrazoIzqInc = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;


}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			rotRodDer += rotIncDer;                          //Agregamos las rotaciones que faltan
			giroMonito += giroMonitoInc;

			movBrazoDer += movBrazoDerInc;
			movCabeza += movCabezaInc;
			movBrazoIzq += movBrazoIzqInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	PlaySound(TEXT("Mario.wav"), NULL, SND_ASYNC);

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC 20241", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------                                                                 // modelos
	Model piso("resources/objects/piso/piso.obj");
	Model bulbasaur("resources/objects/bulbasaur/bulbasaur2.obj");
	Model meetwo("resources/objects/meetwo/meetwo.obj");
	Model charmander("resources/objects/charmander/charmander.obj");
	Model base("resources/objects/base/base.obj");
	Model estante("resources/objects/estante/estante.obj");
	Model pokebola("resources/objects/pokebola/pokebola.obj");
	Model lamparas("resources/objects/lamparas/lamparas.obj");
	Model moneda("resources/objects/Moneda/moneda_de_mario.obj");
	Model maquinita("resources/objects/maquinita/maquinita.obj");
	Model switchN("resources/objects/switch/switch.obj");
	Model mesa("resources/objects/mesa/mesa.obj");
	Model arbol("resources/objects/arbol/arbol.obj");
	Model escudo("resources/objects/escudoZ/Escudo zelda.obj");
	Model anuncio("resources/objects/anuncio/anuncio.obj");
	Model marioCartel("resources/objects/marioCartel/marioCartel.obj");
	Model cuadro("resources/objects/cuadro/cuadro.obj");
	Model mario("resources/objects/mario/Mario.obj");
	Model planta("resources/objects/planta/planta.obj");
	Model pikachu("resources/objects/pikachu/pikachu.obj");
	Model kart("resources/objects/kart/kart.obj");
	Model espada("resources/objects/espada/espada3.obj");
	Model cuboMario("resources/objects/cuboMario/cuboMario.obj");
	Model hongoRojo("resources/objects/hongoRojo/hongoRojo.obj");


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------


	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", ambientColor);
		staticShader.setVec3("dirLight.diffuse", diffuseColor);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));

		//lampara toca charmander
		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.09f);
		staticShader.setFloat("pointLight[0].quadratic", 0.082f);

		//lampara charmander
		staticShader.setVec3("pointLight[1].position", glm::vec3(78.0, 57.0f, -60.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.08f);
		staticShader.setFloat("pointLight[1].linear", 0.09f);
		staticShader.setFloat("pointLight[1].quadratic", 0.082f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(78.0, 57.0f, -120.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.08f);
		staticShader.setFloat("pointLight[2].linear", 0.09f);
		staticShader.setFloat("pointLight[2].quadratic", 0.082f);

		staticShader.setVec3("pointLight[3].position", glm::vec3(78.0, 17.0f, -121.0f));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 0.8f);
		staticShader.setFloat("pointLight[3].linear", 0.09f);
		staticShader.setFloat("pointLight[3].quadratic", 0.0292f);

		staticShader.setVec3("pointLight[4].position", glm::vec3(78.0, 57.0f, -90.0f));
		staticShader.setVec3("pointLight[4].ambient", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[4].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[4].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[4].constant", 0.08f);
		staticShader.setFloat("pointLight[4].linear", 0.09f);
		staticShader.setFloat("pointLight[4].quadratic", 0.082f);

		staticShader.setVec3("pointLight[5].position", glm::vec3(78.0, 14.0f,-85.0f));
		staticShader.setVec3("pointLight[5].ambient", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[5].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[5].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[5].constant", 0.8f);
		staticShader.setFloat("pointLight[5].linear", 0.09f);
		staticShader.setFloat("pointLight[5].quadratic", 0.0292f);

		staticShader.setVec3("pointLight[6].position", glm::vec3(-52.0, 57.0f, -118.7f));
		staticShader.setVec3("pointLight[6].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[6].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[6].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[6].constant", 0.08f);
		staticShader.setFloat("pointLight[6].linear", 0.09f);
		staticShader.setFloat("pointLight[6].quadratic", 0.082f);

		staticShader.setVec3("pointLight[7].position", glm::vec3(-52.0, 40.0f, -118.7f));
		staticShader.setVec3("pointLight[7].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[7].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[7].specular", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[7].constant", 0.08f);
		staticShader.setFloat("pointLight[7].linear", 0.09f);
		staticShader.setFloat("pointLight[7].quadratic", 0.0292f);



		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//squirtle.Draw(animShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//casaDoll.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));                  //para velocidad
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// Modelos Animacion

		model = glm::translate(glm::mat4(1.0f), glm::vec3(83.9f, 11.4f, -116.5f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		bulbasaur.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(73.5f, 11.4f, -78.0f));
		model = glm::scale(model, glm::vec3(0.005f));
		staticShader.setMat4("model", model);
		meetwo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(76.0f, 11.4f, -55.7f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		charmander.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(76.0f, 25.0f, -180.7f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		pikachu.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(76.0f, 13.4f, -155.7f));
		model = glm::scale(model, glm::vec3(0.028f));
		staticShader.setMat4("model", model);
		pokebola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(76.0f, 57.0f, -116.7f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		lamparas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(76.0f, 57.0f, -85.7f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		lamparas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(76.0f, 57.0f, -55.7f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		lamparas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.9f, -70.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		base.Draw(staticShader);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, 10.0f, -180.7f));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, subeMoneda, -180.7f));
		model = glm::rotate(model, glm::radians(rotMoneda), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.5f));
		staticShader.setMat4("model", model);
		moneda.Draw(staticShader);

			if(estado==0)
			{
				subeMoneda++;
				if(subeMoneda > 30.0f)
					estado = 1;
			}
			if(estado==1)
			{
				subeMoneda--;
				if (subeMoneda < 20.0f)
					estado = 0;
			}

			rotMoneda=rotMoneda+5.0f;


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.0f, -115.7f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		mario.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-127.0f, 30.0f, -140.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		cuboMario.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(hongo_x, hongo_y, hongo_z));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		hongoRojo.Draw(staticShader);

		if (activate_hongo_animation) {
			if (estadoHongo == 0) {
				hongo_y += 1;
				if (hongo_y >= 42.0f)
					estadoHongo = 1;
			}
			if (estadoHongo == 1)
			{
				hongo_z -= 1;
				if (hongo_z < -150.0f)
					estadoHongo = 2;
			}
			if (estadoHongo == 2) {
				hongo_y -= 1;
				if (hongo_y <= -1.0f) {
					estadoHongo = 3;
				}
			}
			if (estadoHongo == 3) {
				hongo_z -= 1;
				if (hongo_z <= -180.0f) {
					estadoHongo = 4;
				}
			}
			if (estadoHongo == 4) {
				hongo_z += 1;
				if (hongo_z >= -70.0f) {
					estadoHongo = 3;
				}
			}
		}

		model = glm::translate(glm::mat4(1.0f), glm::vec3(car_pos_ini_x+car_x, -1.0f, car_pos_ini_z+car_z));
		model = glm::rotate(model, glm::radians(car_direction), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.07f));
		staticShader.setMat4("model", model);
		kart.Draw(staticShader);

			if (activate_car_animation) {
				if (car_state == 0) {
					car_z += 0.0f;
					car_x += 1.0f;
					if (car_x >= 40.0f) {
						car_state = 1;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_x = 0.0f;
					}
				}
				//dar vuelta
				if (car_state == 1) {
					if (car_theta1 >= 4.6 && car_theta1 <= 6.28) {
						car_x = 10.0f * cos(car_theta1);
						car_z = (10.0f * sin(car_theta1)) + 10.f;
						car_theta1 += 0.01f;
						car_direction -= .55f;
						if (car_theta1 >= 6.28) {
							// cambio de estado reinicio de variables
							// y reinico de variables de cambio
							car_state = 2;
							car_pos_ini_x = car_pos_ini_x + car_x;
							car_pos_ini_z = car_pos_ini_z + car_z;
							car_z = 0;
							car_x = 0;
						}
					}
				}
				//movimiento Recto
				if (car_state == 2) {
					car_x += 0.0f;
					car_z += 1.0f;
					car_direction -= .1f;
					if (car_z >= 30.0f) {
						car_state = 3;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_pos_ini_z = car_pos_ini_z + car_z;
						car_z = 0;
						car_x = 0;
						car_theta1 = 0;
					}
				}

				//Elipse 1
				if (car_state == 3) {
					car_x = (10.0f * cos(car_theta1) - 10.0f);
					car_z = 10.0f * sin(car_theta1);
					car_theta1 += 0.01;
					car_direction -= .55f;
					if (car_theta1 >= 1.65) {
						car_state = 4;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_pos_ini_z = car_pos_ini_z + car_z;
						car_z = 0;
						car_x = 0;
						car_theta1 = 0;
					}
				}

				if (car_state == 4) {
					car_x -= 1.0f;
					if (car_x <= -40.0f) {
						car_state = 5;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_z = 0;
						car_x = 0;
						car_theta1 = 4.71238898;//inicializando variable para el siguiente estado
					}
				}

				if (car_state == 5) {
					car_x = 10.0f * cos(car_theta1);
					car_z = (10.0f * sin(car_theta1) + 10.0f);
					car_theta1 -= 0.01f;
					car_direction += .55f;
					if (car_direction >= -90.0f) {
						car_state = 6;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_pos_ini_z = car_pos_ini_z + car_z;
						car_z = 0;
						car_x = 0;
						car_theta1 = 0.0f;
					}
				}

				if (car_state == 6) {
					car_z -= 1.0f;
					if (car_z <= -50.0f) {
						car_state = 7;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_pos_ini_z = car_pos_ini_z + car_z;
						car_z = 0;
						car_x = 0;
						car_theta1 = 0.0f;
					}
				}

				if (car_state == 7) {
					car_x = (10.0f * cos(car_theta1) - 10.0f);
					car_z = 10.0f * sin(car_theta1);
					car_theta1 -= 0.01f;
					car_direction += .55f;
					if (car_direction >= 0.0f) {
						car_state = 8;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_pos_ini_z = car_pos_ini_z + car_z;
						car_z = 0;
						car_x = 0;
						car_theta1 = 0.0f;
					}
				}

				if (car_state == 8) {
					car_z += 0.0f;
					car_x += 1.0f;
					if (car_x >= 20.0f) {
						car_state = 0;
						car_pos_ini_x = car_pos_ini_x + car_x;
						car_x = 0.0f;
						car_state = -1;
					}
				}

			}



		model = glm::translate(glm::mat4(1.0f), glm::vec3(-117.0f, 9.5f, -188.3f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(160.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, -1.0f, -190.0f));
		model = glm::scale(model, glm::vec3(0.18f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, 57.0f, -115.7f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		lamparas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.8f, 25.0f));
		model = glm::scale(model, glm::vec3(16.5f));
		staticShader.setMat4("model", model);
		maquinita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, -1.8f, 25.0f));
		model = glm::scale(model, glm::vec3(16.5f));
		staticShader.setMat4("model", model);
		maquinita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 40.0f, -8.0f));
		model = glm::scale(model, glm::vec3(21.5f));
		staticShader.setMat4("model", model);
		switchN.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, -1.8f, 25.0f));
		model = glm::scale(model, glm::vec3(16.5f));
		staticShader.setMat4("model", model);
		maquinita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-91.0f, 20.0f, 27.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		escudo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-80.0f, espadaY, -25.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(rotEspada), glm::vec3(rotEspadaX, rotEspadaY, rotEspadaZ));
		staticShader.setMat4("model", model);
		espada.Draw(staticShader);

		if (activate_espada_animation) {
			if (espada_state == 0) {
				espadaY++;
				if (espadaY >= 40.0f) {
					espada_state = 1;
					rotEspadaX = 0.0f;
					rotEspadaY = 1.0f;
					rotEspadaZ = 0.0f;
				}
			}
			if (espada_state == 1) {
				rotEspada += 10.0f;
				if (rotEspada >= 360.0f) {
					rotEspadaX = 0.0f;
					rotEspadaY = 0.0f;
					rotEspadaZ = 1.0f;
					espada_state = 2;
					rotEspada = 0.0f;

				}
			}
			if (espada_state == 2) {
				rotEspada += 10.0f;
				if (rotEspada >= 180.0f) {
					espada_state = 3;
				}
			}
			if (espada_state == 3) {
				espadaY--;
				if (espadaY <= 25.0f) {
					espada_state = 4;
				}
			}
		}

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-87.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.18f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 48.0f, 102.0f));
		model = glm::scale(model, glm::vec3(0.018f));
		staticShader.setMat4("model", model);
		anuncio.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(33.0f, 35.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.0030f));
		staticShader.setMat4("model", model);
		marioCartel.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(123.0f, 20.0f, 50.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cuadro.Draw(staticShader);



		//Modelos extras
		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(0.18f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 0.0f, -90.0f));
		model = glm::scale(model, glm::vec3(0.18f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(0.18f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(110.0f, -4.3f, 208.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -4.3f, 208.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.5f, 128.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.5f, 168.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.5f, 208.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.5f, 248.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.5f, 128.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.5f, 168.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.5f, 208.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.5f, 248.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3( - 115.0f, -1.5f, 128.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, -1.5f, 168.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, -1.5f, 208.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, -1.5f, 248.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-85.0f, -4.3f, 208.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);



		model = glm::translate(glm::mat4(1.0f), glm::vec3(-175.0f, 0.0f, 90.0f));
		staticShader.setMat4("model", model);
		//museo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		
	}

	skybox.Terminate();
	glfwTerminate();

	
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	/* 
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movBrazoDer += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		movBrazoIzq += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		movBrazoDer -= 100.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		movBrazoIzq -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		movCabeza += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		movCabeza -= 2.0f;
	*/

	//Espada animacion

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
		activate_espada_animation ^= true;// compuerta para cambiar el estado
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS && espada_state == 4) {
		espadaY = -1.0f,
			rotEspada = 0.0f,
			rotEspadaX = 0.0f,
			rotEspadaY = 1.0f,
			rotEspadaZ = 0.0f;

		espada_state = 0;
		activate_espada_animation = false;
	}



	//Carro animacion

	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
		activate_car_animation ^= true;// compuerta para cambiar el estado
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS && car_state == -1) {
		car_pos_ini_x = -90.0f;
		car_pos_ini_z = -140.0f;
		car_state = 0;
		activate_car_animation = false;
	}

	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
		activate_hongo_animation ^= true;// compuerta para cambiar el estado
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS && car_state == 4) {
		rotHongo = 0.0f,
			hongo_x = -122.0f,
			hongo_y = 32.0f,
			hongo_z = -140.0f;
		activate_hongo_animation = false;
	}

	


	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{

		saveFrame();
		std::cout << "Frame Index =" << FrameIndex << std::endl;
		if (play == false && (FrameIndex >= 0))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}

		/*if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}*/
	}

	//To Save a KeyFrame
	/*
	if (key == GLFW_KEY_L && action == GLFW_PRESS)                   // se comento
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}