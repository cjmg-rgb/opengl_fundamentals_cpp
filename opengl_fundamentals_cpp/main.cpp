#include "config.h"

void bufferFrameResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void initializeGLEW()
{
	glewExperimental = GL_TRUE;
	glewInit();
}

GLFWwindow* createWindow(int& w, int& h)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);


	GLFWwindow* window = glfwCreateWindow(500, 500, "Fundamentals", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, bufferFrameResizeCallback);

	return window;
}

unsigned int loadShader(const unsigned int type, const std::string& fileDir)
{
	char infoLog[512];
	int success;
	std::ifstream file;
	std::string temp = "";
	std::string src = "";

	file.open(fileDir);
	if (file.is_open())
		while (std::getline(file, temp))
			src += temp + "\n";
	
	const char* srcChar = src.c_str();
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &srcChar, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Failed " << fileDir << "\n";
		std::cout << infoLog << "\n";
	}

	return shader;
}

unsigned int createShader(const unsigned int vertex, const unsigned int fragment)
{
	char infoLog[512];
	int success;
	unsigned int id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Failed Program" << "\n";
		std::cout << infoLog << "\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glUseProgram(0);

	return id;
}

unsigned int loadTexture(unsigned int type, const char* fileDir)
{
	int imgWidth, imgHeight;
	unsigned char* image = SOIL_load_image(fileDir, &imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);

	if (!image)
	{
		std::cout << "Failed to load image\n";
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(type, texture);

	glTexImage2D(type, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(type);

	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);
	glBindTexture(type, 0);


	return texture;
}

void modelMatrixConfig(glm::mat4& model, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
	model = glm::mat4(1.f);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, scale);
		
}

void modelControl(GLFWwindow* window, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{

	if (glfwGetKey(window, GLFW_KEY_W)) pos.z -= 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_S)) pos.z += 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_A)) pos.x -= 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_D)) pos.x += 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_E)) rot.z -= 1.f;
	else if (glfwGetKey(window, GLFW_KEY_Q)) rot.z += 1.f;
	else if (glfwGetKey(window, GLFW_KEY_N)) rot.y += 1.f;
	else if (glfwGetKey(window, GLFW_KEY_M)) rot.y -= 1.f;
}

int main()
{
	// GLFW
	glfwInit();

	// Window
	int winWidth, winHeight;
	GLFWwindow* window = createWindow(winWidth, winHeight);
	glfwMakeContextCurrent(window);

	// GLEW
	initializeGLEW();

	// Shader
	unsigned int vertex = loadShader(GL_VERTEX_SHADER, "vertex.glsl");
	unsigned int fragment = loadShader(GL_FRAGMENT_SHADER, "fragment.glsl");
	unsigned int program = createShader(vertex, fragment);

	// Buffer
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	// Texture
	unsigned int texture0 = loadTexture(GL_TEXTURE_2D, "images/luffy.png");
	
	// Uniform
	glm::mat4 ModelMatrix(1.f);
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ViewMatrix(1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camPos(0.f, 0.f, 1.f);
	glm::vec3 camFront(0.f, 0.f, -.1f);
	ViewMatrix = glm::lookAt(camPos, camPos + camFront, worldUp);

	glm::mat4 ProjectionMatrix(1.f);
	float fov = 90.f;
	float nearPlane = .1f;
	float farPlane = 1000.f;
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(winWidth) / winHeight, nearPlane, farPlane);
	
	
	glm::vec3 lightPos0(0.f, 0.f, -1.f);

	glUniform3fv(glGetUniformLocation(program, "lightPos0"), 1, glm::value_ptr(lightPos0));
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, glm::value_ptr(camPos));


	// Loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update Model Matrix
		
		modelControl(window, position, rotation, scale);
		modelMatrixConfig(ModelMatrix, position, rotation, scale);
		
		// Update Projection Matrix
		glfwGetFramebufferSize(window, &winWidth, &winHeight);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(winWidth) / winHeight, nearPlane, farPlane);



		// Program
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		
		// Texture
		glBindTextureUnit(0, texture0);

		// Vertex
		glBindVertexArray(VAO);

		// Draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/*	GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << error << std::endl;
		}*/
		
		// Inputs
		glfwPollEvents();

		// Swap Buffers
		glfwSwapBuffers(window);
	}

	return 0;
}