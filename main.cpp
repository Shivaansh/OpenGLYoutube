#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//Video link - https://www.youtube.com/watch?v=45MIykWJ-C4
//Video timestamp - 25:17

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main() {

	//Using GLFW to create window

	glfwInit();

	//specify version and profile info. Profile: core and compatibility, core is only modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLfloat triangleVertices[] =
	{
		-0.5f, -0.65f, 0.0f, //XYZ coords of first vertex
		-0.63f, -0.41f, 0.0f,
		-0.19f, -0.82f, 0.0f,
	};

	//Specify window properties
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Make window active by adding to current context
	glfwMakeContextCurrent(window);

	//Load OpenGL configurations via glad
	gladLoadGL();

	//specify window viewport, which is area of window from bottom left to top right corner (using size)
	glViewport(0, 0, 800, 800);


	//reference for shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//specify shader source, to be compiled later (using 1 string as source
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//compile shader into machine code
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//need to wrap shaders in a shader program
	//1. create program
	GLuint shaderProgram = glCreateProgram();
	//2. attach shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//3. wrapping
	glLinkProgram(shaderProgram);
	//4. can delete shaders now that they're wrapped in shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Vertex Array Object (VAO): Stores pointers to one or more VBOs, allows to switch between different VBOs
	//VBO: Buffer for sharing vertices between CPU and GPU, NOT a frame buffer.
	//Due to slow speed of CPU-GPU data transfer, vertex data is sent in batches
	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO); //MUST BE GENERATED BEFORE VBO
	glGenBuffers(1, &VBO); //creating buffer object: 1 object, pointing to array reference

	//Binding: Making a certain object the current object.
	glBindVertexArray(VAO); //Bind VAO to store vertex buffers
	
	//Doc for glBindBuffer https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //ARRAY_BUFFER coz we need a Vertex Buffer
	//See docs for other kind of buffers
	//populate buffer
	//params: type, size, data, use/purpose of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); //this method puts data in VBO as it is currently binded
	//Doc for glBufferData https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glBufferData.xml

	//Configure VAO, so that OpenGL can read a VBO
	/*
	* 0: Position of vertex attribute (a way of communicating with vertex shader from outside)
	* 3: Number of values per vertex (3 floats)
	* GL_FLOAT: type of values
	* GL_FALSE: If using coordinates as integers
	* 3 * sizeof(float): stride of vertices (amount of data per vertex)
	* (void*) 0: Pointer to where vertices begin in array
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable VA at position 0
	glEnableVertexAttribArray(0); //0 coz that's position of vertex attribute

	//Optional step to prevent change
	glBindBuffer(GL_ARRAY_BUFFER, 0); //bind VBO to attribute at position 0
	glBindVertexArray(0); //bind VAO to attribute at position 0

	//QUESTION: Why do we bind VertexArray to 0, when binding for VAO has already been done?

	//glClearColor(0.07f, 0.13f, 0.17f, 1.0f); -> not really needed for anything

	//Clean back buffer and assign colors to it
	glClear(GL_COLOR_BUFFER_BIT);
	//Swap buffers
	glfwSwapBuffers(window);

	//Check if window should remain open
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //Window background color

		//Clean back buffer and assign colors to it
		glClear(GL_COLOR_BUFFER_BIT);

		//activate shader program
		glUseProgram(shaderProgram); //Start using a shader program
		glBindVertexArray(VAO); //Specify VAO as active vertex array as this contains VBO with vertex data to draw
		glDrawArrays(GL_TRIANGLES, 0, 3); //Draw a triangle Primitive starting at index 0, draw 3 vertices contained in VAO
		glfwSwapBuffers(window); //Generate images every frame by swapping front and back buffers
		//record window activity while open and active
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//destroy window
	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}