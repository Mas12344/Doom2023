#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Resource_Manager.h"

#include "Model.h"
#include "Enemy.h"
#include "camera.h"
#include "Text_Renderer.h"
#include "GameObject.h"
#include "Raycaster.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int screenWidth = 640;
int screenHeight = 480;
float Health = 100.0;

float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

Enemy *enemies[40];
float point_lights[40 * 3];

Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));

Raycaster *raycaster = new Raycaster;

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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        raycaster->CastRay();
}

TextRenderer *Text;

float enemydist(Camera& cam, Enemy* e) {
    float x = e->GetModelMatrix()[3].x - cam.Position.x;
    float y = e->GetModelMatrix()[3].y - cam.Position.y;
    float z = e->GetModelMatrix()[3].z - cam.Position.z;
    return x * x + y * y + z * z;
}

void swapenemy(int i, int j) {
    Enemy* p = enemies[i];
    enemies[i] = enemies[j];
    enemies[j] = p;
}

void sortEnemies(Camera& cam, Enemy* enem[40]) {
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < 40 - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < 40; j++)
            if (enemydist(cam, enem[j]) < enemydist(cam, enem[min_idx]))
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swapenemy(min_idx, i);

        enem[i]->targetpos = cam.Position;
    }

}

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

void spawnEnemies(std::shared_ptr<Model> model) {
    for (int i = 0; i < 40; i++) {
        Enemy* enemy = new Enemy(model);
        enemies[i] = enemy;
    }
}

int checkIfWin() {
    int enemiesKilled = 0;
    for (int i = 0; i < 40; i++) {
        if (enemies[i]->dead) enemiesKilled++;
    }
    return enemiesKilled;
}

bool checkIfGameOver() {
    return Health <= 0 ? true : false;
}

bool checkIfEndGame() {
    return checkIfWin() >= 10 || checkIfGameOver();
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
    glfwSwapInterval(1);

    stbi_set_flip_vertically_on_load(false);


    auto labirynt_model = std::make_shared<Model>("res/models/labirynt/LabiryntDowna.obj", "res/models/labirynt/");    
    auto drone_model = std::make_shared<Model>("res/models/drone/Weatley.obj", "res/models/drone/");

    auto pgun_model = std::make_shared<Model>("res/models/portalgun/portal-gun.obj", "res/models/portalgun/");

    GameObject labirynt = GameObject(labirynt_model);

    glm::mat4 Mlabirynt = labirynt.ApplyTransform(
        Transformation(
            TranformType::Scale,
            glm::vec3(0.f),
            glm::vec3(1.f),
            0.0f
        )
    );

    GameObject pgun = GameObject(pgun_model);

    Text = new TextRenderer(screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ResourceManager::LoadShader("res/shaders/model.vs", "res/shaders/model.fs", nullptr, "simple");

    Text->Load("res/sans.ttf",20);
    spawnEnemies(drone_model);

    while (!glfwWindowShouldClose(window))
    {   
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        

        processInput(window);
        sortEnemies(camera, enemies);

        for (int i = 0; i < 40; i++) {
            if (!enemies[i]->dead) {
                // Update the enemy's position based on the target position
                glm::vec3 direction = enemies[i]->targetpos - enemies[i]->getCoords();
                float distance = glm::length(direction);
                glm::vec3 moveVector = glm::normalize(direction) * 7.0f * deltaTime;
                if (distance >2.5f) {
                    enemies[i]->ApplyTransform(Transformation(
                        TranformType::Translate,
                        glm::vec3(),
                        moveVector,
                        0.0f
                    ));
                }
                else {
                    Health -= 0.1f;
                }
            }
        }

        raycaster->Update(camera, deltaTime,screenWidth, screenHeight, enemies);

        glClearColor(0.02f, 0.16f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 V = camera.GetViewMatrix();
        glm::vec3 lightdir = glm::vec3(0.f, 0.f, -1.f);
        lightdir = glm::vec4(lightdir,0.f)*V;
        glm::mat4 P = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / screenHeight, 0.01f, 50.0f); //Wylicz macierz rzutowania
        auto s = ResourceManager::GetShader("simple").Use();
        s.SetMatrix4("V", V);
        s.SetMatrix4("P", P);
   
        for (int i = 0; i < 40; ++i) {
            auto pos = enemies[i]->getCoords();
            point_lights[i * 3 + 0] = pos[0];
            point_lights[i * 3 + 1] = pos[1];
            point_lights[i * 3 + 2] = pos[2];
        }

        s.SetVector3fv("lights", 40, point_lights);
        s.SetVector3f("viewPos", camera.Position);
        s.SetVector3f("cameradir", lightdir);

        labirynt.GetModel()->Draw("simple", Mlabirynt);
        if (!checkIfEndGame()) {
            for (int i = 0; i < 40; i++) {
                auto m = enemies[i]->GetModelMatrix();
                enemies[i]->GetModel()->Draw("simple", m);
            }

            glm::mat4 Mpgun = glm::mat4(1);
            if (camera.Position.z < -18.0f)
                camera.Position.z = 18.0f;
            if (camera.Position.z > 18.0f)
                camera.Position.z = 18.0f;
            if (camera.Position.x < -18.0f)
                camera.Position.x = -18.0f;
            if (camera.Position.x > 18.0f)
                camera.Position.x = -18.0f;
            Mpgun = glm::inverse(camera.GetViewMatrix());
            pgun.SetModelMatrix(Mpgun);
            pgun.ApplyTransform(
                Transformation(
                    TranformType::Translate,
                    glm::vec3(),
                    glm::vec3(0.04f, -0.04f, -0.15f),
                    0.0f
                )
            );
            pgun.ApplyTransform(
                Transformation(
                    TranformType::Rotate,
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec3(),
                    glm::radians(180.f)
                )
            );

            Mpgun = pgun.ApplyTransform(
                Transformation(
                    TranformType::Scale,
                    glm::vec3(),
                    glm::vec3(0.012f)
                )
            );

            pgun.GetModel()->Draw("simple", Mpgun);

            std::stringstream ss;
            ss << "FPS: " << 1.f / deltaTime;
            Text->RenderText(ss.str(), 15.f, 15.f, 1.0f);
            ss.str("");
            ss.clear();
            ss << checkIfWin() << "/10 enemies killed";
            Text->RenderText(ss.str(), screenWidth / 2, 15.0f, 1.0f);
            Text->RenderText(".", screenWidth / 2, screenHeight / 2, 1.0f);
            ss.str("");
            ss.clear();
            ss << "HP: "<<Health;
            Text->RenderText(ss.str(), screenWidth/2, screenHeight/10, 1.0f);
        }
        else if(checkIfWin()>=10){
            Text->RenderText("You win!", screenWidth / 2 - 25.0f, screenHeight / 2, 1.0f);
        }
        else if (checkIfGameOver()) {
            Text->RenderText("You died", screenWidth / 2 - 25.0f, screenHeight / 2, 1.0f);
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}