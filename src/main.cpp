#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Cube {
    glm::vec3 position;
    glm::vec3 color;
};

std::vector<Cube> world;

// Function to generate random terrain
void generateTerrain(int width, int depth, int maxHeight) {
    srand((unsigned)time(0));
    for (int x = -width / 2; x < width / 2; x++) {
        for (int z = -depth / 2; z < depth / 2; z++) {
            int height = rand() % maxHeight + 1; // random height
            for (int y = 0; y < height; y++) {
                Cube cube;
                cube.position = glm::vec3(x, y, z);
                if (y == height - 1)
                    cube.color = glm::vec3(0.1f, 0.8f, 0.1f); // grass top
                else
                    cube.color = glm::vec3(0.6f, 0.3f, 0.1f); // dirt
                world.push_back(cube);
            }
        }
    }
}

// Cube vertices
float vertices[] = {
    // positions          // colors
   -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,
   -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,

   -0.5f,  0.5f,  0.5f,   0.5f, 0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,   0.0f, 0.5f, 0.0f,
   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.5f,
   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.5f,
   -0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.0f,
   -0.5f,  0.5f,  0.5f,   0.5f, 0.0f, 0.0f,

    0.5f,  0.5f,  0.5f,   0.0f, 0.5f, 0.5f,
    0.5f,  0.5f, -0.5f,   0.5f, 0.0f, 0.5f,
    0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,
    0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,
    0.5f, -0.5f,  0.5f,   0.0f, 0.5f, 0.5f,
    0.5f,  0.5f,  0.5f,   0.0f, 0.5f, 0.5f,

   -0.5f, -0.5f, -0.5f,   0.3f, 0.3f, 0.3f,
    0.5f, -0.5f, -0.5f,   0.6f, 0.6f, 0.6f,
    0.5f, -0.5f,  0.5f,   0.9f, 0.9f, 0.9f,
    0.5f, -0.5f,  0.5f,   0.9f, 0.9f, 0.9f,
   -0.5f, -0.5f,  0.5f,   0.2f, 0.2f, 0.2f,
   -0.5f, -0.5f, -0.5f,   0.3f, 0.3f, 0.3f,

   -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f
};

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
uniform vec3 overrideColor;
void main()
{
    FragColor = vec4(overrideColor, 1.0);
}
)";

void processInput(GLFWwindow* window) {
    float cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float sensitivity = 0.1f;
    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity;
    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minecraft Clone", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    generateTerrain(20, 20, 5); // world size

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

        for (Cube& cube : world) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube.position);
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            unsigned int colorLoc = glGetUniformLocation(shaderProgram, "overrideColor");
            glUniform3fv(colorLoc, 1, glm::value_ptr(cube.color));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
