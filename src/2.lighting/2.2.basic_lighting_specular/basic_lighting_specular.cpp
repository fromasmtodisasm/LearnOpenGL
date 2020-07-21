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
    {
		unsigned int icosahedronVBO, icosahedronIBO;
        glm::vec3 vertices[12][2]; // 3p, 3c
        glm::u16vec3 indices[20];

        {
		   indices[0][0]=0;
		   indices[0][1]=2;
		   indices[0][2]=1;
		 
		   indices[1][0]=0;
		   indices[1][1]=3;
		   indices[1][2]=2;
		 
		   indices[2][0]=0;
		   indices[2][1]=4;
		   indices[2][2]=3;
		 
		   indices[3][0]=0;
		   indices[3][1]=5;
		   indices[3][2]=4;
		 
		   indices[4][0]=0;
		   indices[4][1]=1;
		   indices[4][2]=5;
		 
		   indices[5][0]=6;
		   indices[5][1]=1;
		   indices[5][2]=7;
		 
		   indices[6][0]=7;
		   indices[6][1]=1;
		   indices[6][2]=2;
		 
		   indices[7][0]=7;
		   indices[7][1]=2;
		   indices[7][2]=8;
		 
		   indices[8][0]=8;
		   indices[8][1]=2;
		   indices[8][2]=3;
		 
		   indices[9][0]=8;
		   indices[9][1]=3;
		   indices[9][2]=9;
		 
		   indices[10][0]=9;
		   indices[10][1]=3;
		   indices[10][2]=4;
		 
		   indices[11][0]=9;
		   indices[11][1]=4;
		   indices[11][2]=10;
		 
		   indices[12][0]=10;
		   indices[12][1]=4;
		   indices[12][2]=5;
		 
		   indices[13][0]=10;
		   indices[13][1]=5;
		   indices[13][2]=6;
		 
		   indices[14][0]=6;
		   indices[14][1]=5;
		   indices[14][2]=1;
		 
		   indices[15][0]=7;
		   indices[15][1]=11;
		   indices[15][2]=6;
		 
		   indices[16][0]=8;
		   indices[16][1]=11;
		   indices[16][2]=7;
		 
		   indices[17][0]=9;
		   indices[17][1]=11;
		   indices[17][2]=8;
		 
		   indices[18][0]=10;
		   indices[18][1]=11;
		   indices[18][2]=9;
		 
		   indices[19][0]=6;
		   indices[19][1]=11;
		   indices[19][2]=10;

        }

		{
			GLfloat R=0.75;
			GLfloat a=4*R/sqrt(10+2*sqrt(5));
			GLfloat alpha=acos((1-a*a/2/R/R));

			vertices[0][0]= glm::vec3(0, 0, R);

			vertices[1][0] = glm::vec3(R * sin(alpha) * sin(0),	R * sin(alpha) * cos(0), R * cos(alpha));

			vertices[2][0] = glm::vec3(R*sin(alpha)*sin(72*k), R*sin(alpha)*cos(72*k), R*cos(alpha));

			vertices[3][0] = glm::vec3(R*sin(alpha)*sin(2*72*k),  R*sin(alpha)*cos(2*72*k), R*cos(alpha));

			vertices[4][0] = glm::vec3(R*sin(alpha)*sin(3*72*k), R*sin(alpha)*cos(3*72*k), R*cos(alpha));

			vertices[5][0] = glm::vec3(R*sin(alpha)*sin(4*72*k), R*sin(alpha)*cos(4*72*k), R*cos(alpha));

			vertices[6][0] = glm::vec3(R*sin(pi-alpha)*sin(-36*k), R*sin(pi-alpha)*cos(-36*k), R*cos(pi-alpha));

			vertices[7][0] = glm::vec3(R*sin(pi-alpha)*sin(36*k), R*sin(pi-alpha)*cos(36*k), R*cos(pi-alpha));

			vertices[8][0] = glm::vec3(R*sin(pi-alpha)*sin((36+72)*k), R*sin(pi-alpha)*cos((36+72)*k), R*cos(pi-alpha));

			vertices[9][0] = glm::vec3(R*sin(pi-alpha)*sin((36+2*72)*k), R*sin(pi-alpha)*cos((36+2*72)*k), R*cos(pi-alpha));

			vertices[10][0] = glm::vec3(R*sin(pi-alpha)*sin((36+3*72)*k), R*sin(pi-alpha)*cos((36+3*72)*k), R*cos(pi-alpha));

			vertices[11][0] = glm::vec3(0, 0, -R);

			{
				for (int i = 0; i < 20; i++)
				{
					auto v = vertices;
					auto idx = indices;
					glm::vec3 normal = glm::normalize(
						glm::cross(
							glm::vec3(
								v[idx[i][1]][0]
								) - 
							glm::vec3(
								v[idx[i][0]][0]
								) 
							,
							glm::vec3(
								v[idx[i][2]][0]
								) - 
							glm::vec3(
								v[idx[i][1]][0]
								) 
						)
					);
					v[idx[i][0]][1] = normal;

					v[idx[i][1]][1] = normal;

					v[idx[i][2]][1] = normal;
				}
			}
		}

		glGenVertexArrays(1, &icosahedronVAO);
		glGenBuffers(1, &icosahedronVBO);
		glGenBuffers(1, &icosahedronIBO);

		glBindVertexArray(icosahedronVAO);

		glBindBuffer(GL_ARRAY_BUFFER, icosahedronVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedronIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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