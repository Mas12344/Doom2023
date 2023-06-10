#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Resource_Manager.h"

#include "Model.h"
#include "Enemy.h"
#include "camera.h"
#include "Text_Renderer.h"


float deltaTime = 0.0f;
float lastFrame = 0.0f;

int screenWidth = 640;
int screenHeight = 480;


float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

Enemy *enemies[40];

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow* window)
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

TextRenderer *Text;



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    if (height == 0) return;
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void spawnEnemies() {
    for (int i = 0; i < 40; i++) {
        Enemy* enemy = new Enemy;
        enemies[i] = enemy;
    }
}

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(screenWidth, screenHeight, "Doom 2023 Copyright", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetErrorCallback(error_callback);

    stbi_set_flip_vertically_on_load(false);
    Text = new TextRenderer(screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    //Model sponza = Model("res/models/drone/Weatley.obj", "res/models/drone/");
    //sponza.m_modelmatrix = glm::scale(sponza.m_modelmatrix, glm::vec3(0.01f));
    ResourceManager::LoadShader("res/shaders/model.vs", "res/shaders/model.fs", nullptr, "simple");

    Text->Load("res/sans.ttf",20);
    spawnEnemies();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.09375f, 0.09375f, 0.09375f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 V = camera.GetViewMatrix();
        glm::mat4 P = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / screenHeight, 0.1f, 50.0f); //Wylicz macierz rzutowania
        auto s = ResourceManager::GetShader("simple").Use();
        s.SetMatrix4("V", V);
        s.SetMatrix4("P", P);
    
        for (int i = 0; i < 40; i++) {
            enemies[i]->spawnEnemy();
        }
        //Text->RenderText("Dupa", screenWidth/2, screenHeight/2, 1.0f); //napisz na ekranie napis "Dupa"
        
        //sponza.Draw("simple");
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}