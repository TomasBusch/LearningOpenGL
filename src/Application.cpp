#include "Application.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"

#include "Model.h"
#include "Texture.h"
#include "CubeMap.h"
#include "Scene.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Batch.h"

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
    const uint32_t MAX_VERTICES = pow(2, 20);
    const uint32_t MAX_INDICES = MAX_VERTICES;

    //Batch batch(MAX_VERTICES, MAX_INDICES);
    Scene* scene = new Scene(m_Width, m_Height, MAX_VERTICES, MAX_INDICES);

    //this is a hack will rework later
    m_Camera = scene->getCamera();

    srand(static_cast <unsigned> (time(0)));
    float positionx   = 0.0f;
    float positiony   = 0.0f;
    float positionz   = 0.0f;
    float rotationamp = 0.0f;
    float rotationx   = 0.0f;
    float rotationy   = 0.0f;
    float rotationz   = 0.0f;

    for (int i = 0; i < 100; i++) {
        Model* model = new Model("res/obj/monkey.obj");

        positionx = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 20)) - 10);
        positiony = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 20)) - 10);
        positionz = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 20)) - 10);
        rotationamp = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX / 360))- 180);
        rotationx = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 2)) - 1);
        rotationy = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 2)) - 1);
        rotationz = static_cast <float> ((rand()) / (static_cast <float> (RAND_MAX   / 2)) - 1);

        model->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(positionx, positiony, positionz)));
        model->transformModel(glm::rotate(glm::mat4(1.0f), glm::radians(rotationamp), glm::vec3(rotationx, rotationy, rotationz)));

        //model->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
        //model->transformModel(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

        scene->addModel(model);
    }

    ///* Init vertex array obj */
    //VertexArray* m_VertexArray = new VertexArray();
    ///* Create and asign data to vertex buffer */
    //VertexBuffer* m_VertexBuffer = new VertexBuffer(nullptr, MAX_VERTICES * sizeof(Vertex));
    ///* Init vertex layout */
    //VertexBufferLayout* m_VertexBufferLayout = new VertexBufferLayout();
    ///* Create layout */
    //m_VertexBufferLayout->Push<glm::vec3>(1, (const void*)offsetof(Vertex, Position));
    //m_VertexBufferLayout->Push<glm::vec4>(1, (const void*)offsetof(Vertex, Color));
    //m_VertexBufferLayout->Push<glm::vec3>(1, (const void*)offsetof(Vertex, Normal));
    //m_VertexBufferLayout->Push<glm::vec2>(1, (const void*)offsetof(Vertex, UV));
    //m_VertexBufferLayout->Push<glm::float32>(1, (const void*)offsetof(Vertex, MaterialID));

    ///* Bind vertex buffer to layout */
    //m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

    ///* Create and asign data to vertex index buffer */
    //IndexBuffer* m_IndexBuffer = new IndexBuffer(nullptr, MAX_INDICES);

    //Shader* m_Shader = new Shader("res/shaders/basic");

    Texture* textureAlbedo = new Texture("res/textures/test/container2.png");
    Texture* textureSpecular = new Texture("res/textures/test/container2_specular.png");

    textureAlbedo->Bind(0);
    textureSpecular->Bind(1);
    //m_Shader->SetUniform1i("material.diffuse", 0);
    //m_Shader->SetUniform1i("material.specular", 1);
    //m_Shader->SetUniform1f("material.shininess", 128.0f);

    //m_Shader->SetUniform3f("dirLight.direction", glm::vec3(-0.5f, -0.5f, -0.5f));
    //m_Shader->SetUniform3f("dirLight.ambient"  , glm::vec3(0.005f, 0.005f, 0.005f));
    //m_Shader->SetUniform3f("dirLight.diffuse"  , glm::vec3(1.0f, 1.0f, 1.0f));
    //m_Shader->SetUniform3f("dirLight.specular" , glm::vec3(1.0f, 1.0f, 1.0f));

    CubeMap* skybox = new CubeMap("res/textures/skybox");
    skybox->Bind(2);
    //m_Shader->SetUniform1i("skybox", 2);

    //for (int i = 0; i < 4; i++) {
    //    if (i == 0) {
    //        m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(0.7f, 0.2f, 2.0f));
    //    }
    //    if (i == 1) {
    //        m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(-0.7f, 0.2f, 2.0f));
    //    }
    //    if (i == 2) {
    //        m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(1.7f, 2.2f, 2.0f));
    //    }
    //    if (i == 3) {
    //        m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(-1.7f, 0.2f, -2.0f));
    //    }
    //    m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
    //    m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
    //    m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);

    //    m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    //    m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    //    m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //}


    //glm::mat4 model_Matrix = glm::mat4(1.0f);

    //m_Shader->SetUniformMat4f("u_Model", model_Matrix);

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

        ///* Modify vertex data here */
        //vertices[0].Color = glm::vec4(slider, 1.0f);

        ///* Set dynamic Vertex Buffer Data*/
        //m_VertexBuffer->SetData(0, sizeof(Vertex) * cube->getMeshes().at(0).getVertices().size(), cube->getMeshes().at(0).getVertices().data());

        ///* Modify index data here */

        ///* Set dynamic Index Buffer Data*/
        //m_IndexBuffer->SetData(0, sizeof(uint32_t) * cube->getMeshes().at(0).getIndices().size(), cube->getMeshes().at(0).getIndices().data());

        m_Renderer.Clear();

        //m_Shader->SetUniformMat4f("u_Projection", m_Camera.getProjectionMatrix());
        //m_Shader->SetUniformMat4f("u_View", m_Camera.getViewMatrix());
        //m_Shader->SetUniform3f("u_ViewPos", m_Camera.getPosition());

        //m_Shader->SetUniform3f("spotLight.position", m_Camera.getPosition());
        //m_Shader->SetUniform3f("spotLight.direction", m_Camera.getDirection());
        //m_Shader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
        //m_Shader->SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));

        //m_Shader->SetUniform3f("spotLight.ambient",  glm::vec3(0.0f, 0.0f, 0.0f));
        //m_Shader->SetUniform3f("spotLight.diffuse",  glm::vec3(1.0f, 1.0f, 1.0f));
        //m_Shader->SetUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //m_Shader->SetUniform1f("spotLight.constant", 1.0f);
        //m_Shader->SetUniform1f("spotLight.linear", 0.09f);
        //m_Shader->SetUniform1f("spotLight.quadratic", 0.032f);

        //batch.begin();

        //while (!batch.isEmpty()) {

        //    //auto start = std::chrono::high_resolution_clock::now();

        //    batch.generateDrawQueue();
        //    batch.generateBatch(*m_VertexBuffer, *m_IndexBuffer);

        //    //auto end = std::chrono::high_resolution_clock::now();
        //    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        //    //std::cout << duration.count() << std::endl;

        //    m_Renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        //}

        scene->draw();

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