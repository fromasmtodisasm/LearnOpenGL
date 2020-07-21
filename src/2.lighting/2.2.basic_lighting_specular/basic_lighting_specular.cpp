#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const static float pi=3.141593, k=pi/180;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 n;
};
struct Icosahedron
{
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<glm::u16vec3> indices;
		Mesh()
		{
			{
				indices.resize(20);
				indices[0] = glm::u16vec3(0, 2, 1);
				indices[1] = glm::u16vec3(0, 3, 2);
				indices[2] = glm::u16vec3(0, 4, 3);
				indices[3] = glm::u16vec3(0, 5, 4);
				indices[4] = glm::u16vec3(0, 1, 5);
				indices[5] = glm::u16vec3(6, 1, 7);
				indices[6] = glm::u16vec3(7, 1, 2);
				indices[7] = glm::u16vec3(7, 2, 8);
				indices[8] = glm::u16vec3(8, 2, 3);
				indices[9] = glm::u16vec3(8, 3, 9);
				indices[10] = glm::u16vec3(9, 3, 4);
				indices[11] = glm::u16vec3(9, 4, 10);
				indices[12] = glm::u16vec3(1, 4, 5);
				indices[13] = glm::u16vec3(1, 5, 6);
				indices[14] = glm::u16vec3(6, 5, 1);
				indices[15] = glm::u16vec3(7, 11, 6);
				indices[16] = glm::u16vec3(8, 11, 7);
				indices[17] = glm::u16vec3(9, 11, 8);
				indices[18] = glm::u16vec3(1, 11, 9);
				indices[19] = glm::u16vec3(6, 11, 10);

				{
					GLfloat R = 0.75;
					GLfloat a = 4 * R / sqrt(10 + 2 * sqrt(5));
					GLfloat alpha = acos((1 - a * a / 2 / R / R));

					vertices.resize(12);
					vertices[0].pos = glm::vec3(0, 0, R);
					vertices[1].pos = glm::vec3(R * sin(alpha) * sin(0), R * sin(alpha) * cos(0), R * cos(alpha));
					vertices[2].pos = glm::vec3(R * sin(alpha) * sin(72 * k), R * sin(alpha) * cos(72 * k), R * cos(alpha));
					vertices[3].pos = glm::vec3(R * sin(alpha) * sin(2 * 72 * k), R * sin(alpha) * cos(2 * 72 * k), R * cos(alpha));
					vertices[4].pos = glm::vec3(R * sin(alpha) * sin(3 * 72 * k), R * sin(alpha) * cos(3 * 72 * k), R * cos(alpha));
					vertices[5].pos = glm::vec3(R * sin(alpha) * sin(4 * 72 * k), R * sin(alpha) * cos(4 * 72 * k), R * cos(alpha));
					vertices[6].pos = glm::vec3(R * sin(pi - alpha) * sin(-36 * k), R * sin(pi - alpha) * cos(-36 * k), R * cos(pi - alpha));
					vertices[7].pos = glm::vec3(R * sin(pi - alpha) * sin(36 * k), R * sin(pi - alpha) * cos(36 * k), R * cos(pi - alpha));
					vertices[8].pos = glm::vec3(R * sin(pi - alpha) * sin((36 + 72) * k), R * sin(pi - alpha) * cos((36 + 72) * k), R * cos(pi - alpha));
					vertices[9].pos = glm::vec3(R * sin(pi - alpha) * sin((36 + 2 * 72) * k), R * sin(pi - alpha) * cos((36 + 2 * 72) * k), R * cos(pi - alpha));
					vertices[10].pos = glm::vec3(R * sin(pi - alpha) * sin((36 + 3 * 72) * k), R * sin(pi - alpha) * cos((36 + 3 * 72) * k), R * cos(pi - alpha));
					vertices[11].pos = glm::vec3(0, 0, -R);
					{
						for (int i = 0; i < 20; i++)
						{
							auto v = vertices;
							auto idx = indices;
							glm::vec3 normal = glm::normalize(
								glm::cross(
									glm::vec3(v[idx[i][1]].pos) - glm::vec3(v[idx[i][0]].pos),
									glm::vec3(v[idx[i][2]].pos) - glm::vec3(v[idx[i][1]].pos)
								)
							);
							v[idx[i][0]].n = normal;
							v[idx[i][1]].n = normal;
							v[idx[i][2]].n = normal;
						}
					}
				}
			}
		};

	};
	static Mesh mesh;

	static std::vector<Vertex> Tesselate(int level)
	{

	}
};

Icosahedron::Mesh Icosahedron::mesh;

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
    Shader lightingShader("2.2.basic_lighting.vs", "2.2.basic_lighting.fs");
    Shader lampShader("2.2.lamp.vs", "2.2.lamp.fs");

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    {
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(cubeVAO);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

    }

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    {
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// note that we update the lamp's position attribute's stride to reflect the updated buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
    }
    unsigned int icosahedronVAO;
	Icosahedron icosahedron;
    {
		unsigned int icosahedronVBO, icosahedronIBO;
		glGenVertexArrays(1, &icosahedronVAO);
		glGenBuffers(1, &icosahedronVBO);
		glGenBuffers(1, &icosahedronIBO);

		glBindVertexArray(icosahedronVAO);

		glBindBuffer(GL_ARRAY_BUFFER, icosahedronVBO);
		glBufferData(GL_ARRAY_BUFFER, icosahedron.mesh.vertices.size() * sizeof(Vertex), icosahedron.mesh.vertices.data(), GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedronIBO);
		glBufferData(GL_ARRAY_BUFFER, icosahedron.mesh.indices.size() * sizeof(glm::u16vec3), icosahedron.mesh.indices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

    }


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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        //glBindVertexArray(cubeVAO);
        glBindVertexArray(icosahedronVAO);
        glDrawElements(GL_TRIANGLES, 20*3, GL_UNSIGNED_SHORT, nullptr);


        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
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