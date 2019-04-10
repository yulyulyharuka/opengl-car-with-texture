#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <Camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// build and compile our shader zprogram
	// ------------------------------------
	Shader squareShader("../Shaders/7.4.camera.vs", "../Shaders/7.4.camera.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.7f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, //0
		 0.5f, -0.7f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, //1
		-0.5f, -0.2f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, //2
		 0.5f, -0.2f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, //3
		 0.5f, -0.7f, -0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,//4
		 0.5f, -0.2f, -0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,//5
		-0.5f, -0.2f, -0.5f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,//6
		-0.5f, -0.7f, -0.5f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,//7
		 0.5f,  0.2f, -0.65f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,//8
		-0.5f,  0.2f, -0.65f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,//9
		-0.5f,  0.2f, -1.5f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,//10
		 0.5f,  0.2f, -1.5f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,//11
		 0.5f, -0.7f, -1.5f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,//12
		-0.5f, -0.7f, -1.5f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,//13
		 0.45f, -0.2f, -0.499f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//14
		-0.45f, -0.2f, -0.499f, 0.77f, 1.0f,  1.0f, 0.0f, 1.0f,//15
		0.45f,  0.15f, -0.63f, 0.77f, 1.0f,  1.0f, 1.0f, 1.0f,//16
		-0.45f,  0.15f, -0.63f, 0.77f, 1.0f,  1.0f, 1.0f, 0.0f,//17

		 0.5f,  -0.2f, -1.5f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,//18
		 -0.5f, -0.2f, -1.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,// 19

		 //0.51f,  0.15f, -0.7f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//8 20
		 //0.51f,  0.15f, -1.4f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//11 21
		 //0.51f, -0.15f, -0.6f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//5 22
		 //0.51f,  -0.15f, -1.4f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//18 23

		 0.501f,  0.15f, -0.7f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//8 20
		 0.501f,  0.15f, -1.0f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//11 21
		 0.501f, -0.15f, -0.6f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//5 22
		 0.501f,  -0.15f, -1.0f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//18 23

		 0.501f,  0.15f, -1.1f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//8 24
		 0.501f,  0.15f, -1.45f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//11 25
		 0.501f, -0.15f, -1.1f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//5 26
		 0.501f,  -0.15f, -1.45f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//18 27

		 -0.501f,  0.15f, -0.7f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//8 28
		 -0.501f,  0.15f, -1.0f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//11 29
		 -0.501f, -0.15f, -0.6f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//5 30
		 -0.501f,  -0.15f, -1.0f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//18 31

		 -0.501f,  0.15f, -1.1f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//8 32
		 -0.501f,  0.15f, -1.45f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//11 33
		 -0.501f, -0.15f, -1.1f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//5 34
		 -0.501f,  -0.15f, -1.45f, 0.77f, 1.0f,  1.0f, 0.0f, 0.0f,//18 35

		 -0.45f,  0.15f, -1.501f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//10 36
		 0.45f,  0.15f, -1.501f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//11 37
		 0.45f, -0.2f, -1.501f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//12 38
		 -0.45f, -0.2f, -1.501f, 0.77f, 1.0f, 1.0f, 0.0f, 0.0f,//13 39

		 -0.4f, -0.425f,  0.01f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //40
		 0.4f, -0.425f,  0.01f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //41
		 -0.4f, -0.25f,  0.01f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //42
		 0.4f, -0.25f,  0.01f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //43
	};
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3,
		1, 3, 4,
		3, 4, 5,
		2, 3, 6,
		3, 5, 6,
		0, 2, 7,
		2, 7, 6,
		0, 1, 7,
		1, 4, 7,
		6, 5, 8,
		6, 8, 9,
		8, 9, 10,
		8, 10, 11,
		5, 8, 18,
		8, 11, 18,
		4,5,12,
		5,12,18,
		10, 13, 12,
		10, 11, 12,
		9, 10, 19,
		6, 9, 19,
		6, 7, 13,
		6, 13, 19,
		7, 4, 13,
		13,12,4,
		14, 15, 17,
		14, 17, 16,

		20,21,22,
		22,23,21,

		24,25,26,
		26,27,25,

		28,29,30,
		30,31,29,

		32,33,34,
		34,35,33,

		36,37,38,
		38,39,36,

		40,41,42,
		41,42,43
	};

	//SQUARES
	unsigned int VBO, VAOSQ, EBO, VAOC, VBOC, EBOC, VAOWG, VBOWG, EBOWG, VAOFL, VBOFL, EBOFL;
	glGenVertexArrays(1, &VAOSQ);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAOSQ);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//CIRCLES
	glGenVertexArrays(1, &VAOC);
	glGenBuffers(1, &VBOC);
	glGenBuffers(1, &EBOC);
	glBindVertexArray(VAOC);
	float verticesCircle[] = {
		0.4f, 0.4f, 0.0f, 0.0f, 0.0f,  0.0f, //0 tengah
		0.4f, 0.01f, 0.0f, 0.0f, 0.0f,  0.0f, //1 atas
		0.675f, 0.125f, 0.0f, 0.0f, 0.0f,  0.0f, //2 kanan atas
		0.79f, 0.4f, 0.0f, 0.0f, 0.0f,  0.0f, //3 kanan
		0.675f, 0.675f, 0.0f, 0.0f, 0.0f,  0.0f, //4 kanan bawah
		0.4f, 0.79f, 0.0f, 0.0f, 0.0f,  0.0f, //5 bawah
		0.125f, 0.675f, 0.0f, 0.0f, 0.0f,  0.0f, //6 kiri bawah
		0.001f, 0.4f, 0.0f, 0.0f, 0.0f,  0.0f, //7 kiri
		0.125f, 0.125f, 0.0f, 0.0f, 0.0f,  0.0f, //8 kiri atas

		0.4f, 0.01f, -0.3f, 0.0f, 0.0f,  0.0f, //9 atas
		0.675f, 0.125f, -0.3f, 0.0f, 0.0f,  0.0f, //10 kanan atas
		0.79f, 0.4f, -0.3f, 0.0f, 0.0f,  0.0f, //11 kanan
		0.675f, 0.675f, -0.3f, 0.0f, 0.0f,  0.0f, //12 kanan bawah
		0.4f, 0.79f, -0.3f, 0.0f, 0.0f,  0.0f, //13 bawah
		0.125f, 0.675f, -0.3f, 0.0f, 0.0f,  0.0f, //14 kiri bawah
		0.001f, 0.4f, -0.3f, 0.0f, 0.0f,  0.0f, //15 kiri
		0.125f, 0.125f, -0.3f, 0.0f, 0.0f,  0.0f, //16 kiri atas
	};
	unsigned int indicesCircle[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,1,

		1,2,9,
		2,9,10,
		2,3,10,
		3,10,11,
		3,4,11,
		4,11,12,
		4,5,12,
		5,12,13,
		5,6,13,
		6,13,14,
		6,7,14,
		7,14,15,
		7,8,15,
		8,15,16,
		8,1,16,
		1,16,9,

		10,9,10,
		10,10,11,
		10,11,12,
		10,12,13,
		10,13,14,
		10,14,15,
		10,15,16,
		10,16,9,
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCircle), verticesCircle, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCircle), indicesCircle, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Wheel Glass
	glGenVertexArrays(1, &VAOWG);
	glGenBuffers(1, &VBOWG);
	glGenBuffers(1, &EBOWG);
	glBindVertexArray(VAOWG);
	float verticesWheelGlass[] = {
		0.4f, 0.4f, 0.0f, 1.0f, 1.0f,  1.0f, //0 tengah
		0.4f, 0.01f, 0.0f, 1.0f, 1.0f,  1.0f, //1 atas
		0.675f, 0.125f, 0.0f, 1.0f, 1.0f,  1.0f, //2 kanan atas
		0.79f, 0.4f, 0.0f, 1.0f, 1.0f,  1.0f, //3 kanan
		0.675f, 0.675f, 0.0f, 1.0f, 1.0f,  1.0f, //4 kanan bawah
		0.4f, 0.79f, 0.0f, 1.0f, 1.0f,  1.0f, //5 bawah
		0.125f, 0.675f, 0.0f, 1.0f, 1.0f,  1.0f, //6 kiri bawah
		0.001f, 0.4f, 0.0f, 1.0f, 1.0f,  1.0f, //7 kiri
		0.125f, 0.125f, 0.0f, 1.0f, 1.0f,  1.0f, //8 kiri atas

		0.24f, 0.24f, 0.01f, 0.0f, 0.0f,  0.0f, //9
		0.4f, 0.165f, 0.01f, 0.0f, 0.0f,  0.0f, //10
		0.585f, 0.215f, 0.01f, 0.0f, 0.0f,  0.0f, //11
		0.63f, 0.4f, 0.01f, 0.0f, 0.0f,  0.0f, //12
		0.55f, 0.55f, 0.01f, 0.0f, 0.0f,  0.0f, //13
		0.4f, 0.62f, 0.01f, 0.0f, 0.0f,  0.0f, //14
		0.26f, 0.54f, 0.01f, 0.0f, 0.0f,  0.0f, //15
		0.18f, 0.4f, 0.01f, 0.0f, 0.0f,  0.0f, //16
	};
	unsigned int indicesWheelGlass[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,1,

		0,9,10,
		0,11,12,
		0,13,14,
		0,15,16
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBOWG);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWheelGlass), verticesWheelGlass, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWG);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesWheelGlass), indicesWheelGlass, GL_STATIC_DRAW);
		// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Front Lamp
	glGenVertexArrays(1, &VAOFL);
	glGenBuffers(1, &VBOFL);
	glGenBuffers(1, &EBOFL);
	glBindVertexArray(VAOFL);
	float verticesFrontLamp[] = {
		0.4f, 0.4f, 0.0f, 1.0f, 1.0f,  0.0f, //0 tengah
		0.4f, 0.01f, 0.0f, 1.0f, 1.0f,  0.0f, //1 atas
		0.675f, 0.125f, 0.0f, 1.0f, 1.0f,  0.0f, //2 kanan atas
		0.79f, 0.4f, 0.0f, 1.0f, 1.0f,  0.0f, //3 kanan
		0.675f, 0.675f, 0.0f, 1.0f, 1.0f,  0.0f, //4 kanan bawah
		0.4f, 0.79f, 0.0f, 1.0f, 1.0f,  0.0f, //5 bawah
		0.125f, 0.675f, 0.0f, 1.0f, 1.0f,  0.0f, //6 kiri bawah
		0.001f, 0.4f, 0.0f, 1.0f, 1.0f,  0.0f, //7 kiri
		0.125f, 0.125f, 0.0f, 1.0f, 1.0f,  0.0f, //8 kiri atas
	};
	unsigned int indicesFrontLamp[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,1,

		0,9,10,
		0,11,12,
		0,13,14,
		0,15,16
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBOFL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFrontLamp), verticesFrontLamp, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFL);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesFrontLamp), indicesFrontLamp, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("../Texture/metal-texture.jpg" , &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//// set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// load image, create texture and generate mipmaps
	//data = stbi_load("../Texture/awesomeface.png", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	squareShader.use();
	squareShader.setInt("texture1", 0);
	//squareShader.setInt("texture2", 1);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		squareShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		squareShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		squareShader.setMat4("view", view);

		// render boxes
		glBindVertexArray(VAOSQ);
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float angle = 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		squareShader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, sizeof(indices) / 3 * sizeof(int), GL_UNSIGNED_INT, 0);


		// render circle
		glBindVertexArray(VAOC);
		glm::mat4 modelCircle = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float angleCircle = 0;

		// roda kiri depan
		modelCircle = glm::rotate(modelCircle, glm::radians(angleCircle), glm::vec3(1.0f, 0.3f, 0.5f));
		modelCircle = glm::scale(modelCircle, glm::vec3(0.5f));
		modelCircle = glm::rotate(modelCircle, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCircle = glm::translate(modelCircle, glm::vec3(0.1f, -1.8f, 1.1f));
		squareShader.setMat4("model", modelCircle);

		// roda kiri belakang
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 5 * sizeof(float), GL_UNSIGNED_INT, 0);
		modelCircle = glm::translate(modelCircle, glm::vec3(2.0f, 0.0f, 0.0f));
		squareShader.setMat4("model", modelCircle);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 5 * sizeof(float), GL_UNSIGNED_INT, 0);

		//roda kanan belakang
		modelCircle = glm::translate(modelCircle, glm::vec3(0.0f, 0.0f, -1.9f));
		squareShader.setMat4("model", modelCircle);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 5 * sizeof(float), GL_UNSIGNED_INT, 0);

		//roda kanan depan
		modelCircle = glm::translate(modelCircle, glm::vec3(-2.0f, 0.0f, -0.0f));
		squareShader.setMat4("model", modelCircle);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 5 * sizeof(float), GL_UNSIGNED_INT, 0);

		// render wheel glass
		glBindVertexArray(VAOWG);
		glm::mat4 modelWheelGlass = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float angleWheelGlass = 0;
		//kiri depan
		modelWheelGlass = glm::rotate(modelWheelGlass, glm::radians(angleWheelGlass), glm::vec3(1.0f, 0.3f, 0.5f));
		modelWheelGlass = glm::scale(modelWheelGlass, glm::vec3(0.3f));
		modelWheelGlass = glm::rotate(modelWheelGlass, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelWheelGlass = glm::translate(modelWheelGlass, glm::vec3(0.4f, -2.75f, 1.85f));

		squareShader.setMat4("model", modelWheelGlass);

		glDrawElements(GL_TRIANGLES, sizeof(indices) / 3 * sizeof(int), GL_UNSIGNED_INT, 0);
		//kiri belakang
		modelWheelGlass = glm::translate(modelWheelGlass, glm::vec3(3.35f, 0.0f, 0.0f));
		squareShader.setMat4("model", modelWheelGlass);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 3 * sizeof(int), GL_UNSIGNED_INT, 0);

		//kiri belakang
		modelWheelGlass = glm::translate(modelWheelGlass, glm::vec3(0.0f, 0.0f, -3.75f));
		modelWheelGlass = glm::rotate(modelWheelGlass, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelWheelGlass = glm::translate(modelWheelGlass, glm::vec3(-0.8f, 0.0f, 0.0f));
		squareShader.setMat4("model", modelWheelGlass);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 3 * sizeof(int), GL_UNSIGNED_INT, 0);

		//kiri depan
		modelWheelGlass = glm::translate(modelWheelGlass, glm::vec3(3.35f, 0.0f, 0.0f));
		squareShader.setMat4("model", modelWheelGlass);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 3 * sizeof(int), GL_UNSIGNED_INT, 0);

		//lampu depan
		glBindVertexArray(VAOFL);
		glm::mat4 modelFrontLamp = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float angleFrontLamp = 0;

		//kiri
		modelFrontLamp = glm::scale(modelFrontLamp, glm::vec3(0.2f));
		modelFrontLamp = glm::translate(modelFrontLamp, glm::vec3(-1.9f, -2.1f, 0.1f));
		squareShader.setMat4("model", modelFrontLamp);
		glDrawElements(GL_TRIANGLES, sizeof(indicesFrontLamp) / 5 * sizeof(int), GL_UNSIGNED_INT, 0);

		//kanan
		modelFrontLamp = glm::translate(modelFrontLamp, glm::vec3(3.0f, 0.0f, 0.0f));
		squareShader.setMat4("model", modelFrontLamp);
		glDrawElements(GL_TRIANGLES, sizeof(indicesFrontLamp) / 5 * sizeof(int), GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOSQ);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
