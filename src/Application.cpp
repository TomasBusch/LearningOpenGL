#include "Application.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"

#include "Model.h"
#include "Texture.h"
#include "CubeMap.h"
#include "SceneLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Batch.h"
#include "SkyboxLayer.h"

Application::Application()
    : m_GLSL_Version("#version 150"), m_Window(nullptr), m_Width(960), m_Height(512), m_Renderer(Renderer()), m_Camera(new Camera(960, 512))
{
}

Application::Application(const std::string GLSL_Version, uint32_t width, uint32_t height) 
	: m_GLSL_Version(GLSL_Version), m_Window(nullptr), m_Width(width), m_Height(height), m_Renderer(Renderer()), m_Camera(new Camera(width, height))
{
}

Application::Application(const std::string GLSL_Version, uint32_t width, uint32_t height, const std::string windowName)
    : m_GLSL_Version(GLSL_Version), m_Window(nullptr), m_Width(width), m_Height(height), m_Renderer(Renderer()), m_Camera(new Camera(width, height))
{
    Init(windowName);
}

int Application::Init(const std::string windowName) {
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //For MSAA anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_Width, m_Height, windowName.c_str(), NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    /* Sync framerate with vsync of monitor */
    glfwSwapInterval(0);


    if (glewInit() != GLEW_OK) {
        std::cout << "Error: GlEW Not initialized" << std::endl;
    }


    glViewport(0, 0, m_Width, m_Height);

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glBlendEquation(GL_FUNC_ADD));

    GLCall(glEnable(GL_DEPTH_TEST));

    glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //For anti-aliasing
    glEnable(GL_MULTISAMPLE);

    //Enable gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);


    return 0;
}

int Application::Run() {
    const uint32_t MAX_VERTICES = 150000;
    const uint32_t MAX_INDICES = MAX_VERTICES;

    SceneLayer* sceneLayer = new SceneLayer(m_Width, m_Height, MAX_VERTICES, MAX_INDICES);
    SkyboxLayer* skyLayer = new SkyboxLayer();

    //this is a hack will rework later
    m_Camera = new Camera(m_Width, m_Height);
    sceneLayer->m_Camera = m_Camera;
    skyLayer->m_Camera = m_Camera;

    srand(static_cast <unsigned> (time(0)));
    float positionx   = 0.0f;
    float positiony   = 0.0f;
    float positionz   = 0.0f;
    float rotationamp = 0.0f;
    float rotationx   = 0.0f;
    float rotationy   = 0.0f;
    float rotationz   = 0.0f;

    for (int i = 0; i < 100; i++) {
        Model* model = new Model("res/obj/cube.obj");

        positionx = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 20)) - 10);
        positiony = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 20)) - 10);
        positionz = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 20)) - 10);
        rotationamp = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX / 360))- 180);
        rotationx = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 2)) - 1);
        rotationy = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 2)) - 1);
        rotationz = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 2)) - 1);

        model->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(positionx, positiony, positionz)));
        model->transformModel(glm::rotate(glm::mat4(1.0f), glm::radians(rotationamp), glm::vec3(rotationx, rotationy, rotationz)));

        sceneLayer->addModel(model);
    }

    Texture* textureAlbedo = new Texture("res/textures/test/container2.png");
    Texture* textureSpecular = new Texture("res/textures/test/container2_specular.png");

    textureAlbedo->Bind(0);
    textureSpecular->Bind(1);

    glfwSetWindowUserPointer(m_Window, this);

    auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        instance->updateViewportSize(width, height);
        instance->m_Camera->setViewportSize(width, height);
        glViewport(0, 0, width, height);
    };

    auto mouse_callback = [](GLFWwindow* window, double xposIn, double yposIn) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        instance->m_Camera->mouse_callback(window, xposIn, yposIn);
    };

    auto scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        instance->m_Camera->scroll_callback(window, xoffset, yoffset);
    };
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_Window, mouse_callback);
    glfwSetScrollCallback(m_Window, scroll_callback);

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    glClearColor(0.001f, 0.001f, 0.001f, 1.0f);

    glm::vec3 slider = glm::vec3(0.0f, 0.0f, 0.0f);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
            instance->m_Camera->toggleLock(window);
        }
    };
    glfwSetKeyCallback(m_Window, key_callback);

    while (!glfwWindowShouldClose(m_Window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(m_Window, deltaTime);

        m_Renderer.Clear();

        sceneLayer->onUpdate();
        skyLayer->onUpdate();

        ImGUIMenu(slider);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void Application::processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Camera->processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Camera->processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_Camera->processKeyboard(DOWN, deltaTime);
}

void Application::updateViewportSize(int width, int height) {
    m_Width = width;
    m_Height = height;
}

void Application::ImGUIMenu(glm::vec3& slider) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static int counter = 0;

    ImGui::Begin("Controls");                          // Create a window.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    ImGui::SliderFloat3("Sun Rotation", &slider.r, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}