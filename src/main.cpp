#include "common.h"
#include "shaders.h"

//const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint WIDTH = 1024, HEIGHT = 768;

using namespace std;

int main(int argc, char* argv[]) {

	glfwInit();

	//configure GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//create a GLFWwindow object (will be used later by the functions)
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenGL", nullptr, nullptr);

	if (!window) {
		std::cerr << "FAILED TO CREATE A GLFW WINDOW\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		std::cerr << "FAILED TO INIT A GLEW WINDOW\n";
		return EXIT_FAILURE;
	}

	// to capture escape key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// have a nice dark background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// accept vertex that is closer to the camera
	glDepthFunc(GL_LESS);

	// create a vertex array object
	GLuint vArrayID;
	glGenVertexArrays(1, &vArrayID);
	glBindVertexArray(vArrayID);

	// BUILD AND COMPILE OUR SHADER PROGRAMS 
	shaders shader;
	GLint shaderPid = shader.load_shaders("vertex.vert", "fragment.frag");

	// get location for our MVP uniform
	GLuint matrixID = glGetUniformLocation(shaderPid, "MVP");

	/*
	 * Compute our mvp (model view projection)
	 */

	// projection matrix : 45° fov, 4:3 ratio, display range : 0.1 unit -> 100 units
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	// if we have an ortho camera (in world coordinates)
	//glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);

	// camera matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//  model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model = glm::mat4(1.0f);

	// our model view porjection (MVP( : multiplication of our 3 matrices
	glm::mat4 mvp = proj * view * model; // Remember, matrix multiplication is the other way around

	// load bmp texture

	const GLfloat vertexBufferData[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f, // triangle 2
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f, // triangle 3, and so on and so forth
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	// bind to the vertex data
	GLuint vBuffer;
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	// rgb color for each vertex
	const GLfloat colorBufferData[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	// bind the color data
	GLuint cBuffer;
	glGenBuffers(1, &cBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);

	bool escPressed = false;
	while (!glfwWindowShouldClose(window) && !escPressed) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// shader program
		glUseProgram(shaderPid);

		mvp = glm::rotate(mvp, glm::radians(.05f), glm::vec3(0.0f, 0.0f, 1.0f));

		// send MVP transform to the current shader (MVP uniform)
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		// match shader layout
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cBuffer);
		// must match shader layout
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);


		// draw the object
		//glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		// state machine
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// swap buffers and poll for events
		glfwSwapBuffers(window);
		glfwPollEvents();

		escPressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	}

	// clean everything
	glDeleteBuffers(1, &vBuffer);
	glDeleteBuffers(1, &cBuffer);
	glDeleteProgram(shaderPid);
	glDeleteVertexArrays(1, &vArrayID);
	glfwTerminate();

	return EXIT_SUCCESS;
}

