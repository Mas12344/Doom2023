#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include <imgui.h>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    stbi_set_flip_vertically_on_load(false);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Shader modelShader("res/shaders/model.vs", "res/shaders/model.fs");

    Model spiderModel("res/models/spider.obj");


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.09375f, 0.09375f, 0.09375f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.use();

        glm::mat4 P = glm::perspective(glm::radians(50.0f), 640.f / 480.f, 0.1f, 100.0f);
        glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 10.0f, -25.0f/3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("P", P);
        modelShader.setMat4("V", V);

        glm::mat4 M(1.0f);
        M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));
        M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
        M = glm::rotate(M, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        M = glm::rotate(M, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("M", M);
        spiderModel.Draw(modelShader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}