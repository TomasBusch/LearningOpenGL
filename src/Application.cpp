#include "Application.h"

#include <iostream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"

#include "Mesh.h"
#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"

Application::Application()
    : m_GLSL_Version("#version 150"), m_Window(nullptr), m_Width(960), m_Height(512), m_Renderer(Renderer()), m_Camera(Camera(960, 512))
{
}

Application::Application(const std::string GLSL_Version, unsigned int width, unsigned int height) 
	: m_GLSL_Version(GLSL_Version), m_Window(nullptr), m_Width(width), m_Height(height), m_Renderer(Renderer()), m_Camera(Camera(width, height))
{
}

Application::Application(const std::string GLSL_Version, unsigned int width, unsigned int height, const std::string windowName)
    : m_GLSL_Version(GLSL_Version), m_Window(nullptr), m_Width(width), m_Height(height), m_Renderer(Renderer()), m_Camera(Camera(width, height))
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

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_Width, m_Height, windowName.c_str(), NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    /* Sync framerate with vsync of monitor */
    glfwSwapInterval(1);


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

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glBlendEquation(GL_FUNC_ADD));

    GLCall(glEnable(GL_DEPTH_TEST));

    return 0;
}

int Application::Run() {

    Vertex vertices[4] = {
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::float32(0)},
        {glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(2.0f, 0.0f), glm::float32(0)},
        {glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(2.0f, 2.0f), glm::float32(0)},
        {glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 2.0f), glm::float32(0)},
    };

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    /* Init vertex array obj */
    VertexArray* m_VertexArray = new VertexArray();
    /* Create and asign data to vertex buffer */
    VertexBuffer* m_VertexBuffer = new VertexBuffer(nullptr, 4 * sizeof(Vertex));
    //VertexBuffer* m_VertexBuffer = new VertexBuffer(vertices, 4 * sizeof(Vertex));
    /* Init vertex layout */
    VertexBufferLayout* m_VertexBufferLayout = new VertexBufferLayout();
    /* Create layout */
    m_VertexBufferLayout->Push<glm::vec3>(1, (const void*)offsetof(Vertex, Position));
    m_VertexBufferLayout->Push<glm::vec4>(1, (const void*)offsetof(Vertex, Color));
    m_VertexBufferLayout->Push<glm::vec3>(1, (const void*)offsetof(Vertex, Normal));
    m_VertexBufferLayout->Push<glm::vec2>(1, (const void*)offsetof(Vertex, UV));
    m_VertexBufferLayout->Push<glm::float32>(1, (const void*)offsetof(Vertex, TextureID));

    /* Bind vertex buffer to layout */
    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

    /* Create and asign data to vertex index buffer */
    IndexBuffer* m_IndexBuffer = new IndexBuffer(nullptr, 6);
    //IndexBuffer* m_IndexBuffer = new IndexBuffer(indices, 6);

    Shader* m_Shader = new Shader("res/shaders/basic");

    Texture* texture1 = new Texture("res/textures/concrete_layers_02/concrete_layers_02_diff_2k.jpg");
    Texture* texture2 = new Texture("res/textures/wooden_garage_door/wooden_garage_door_diff_2k.jpg");

    texture1->Bind(0);
    texture2->Bind(1);
    int samplers[2] = { 0, 1 };
    m_Shader->SetUniform1iv("u_Textures", 2, samplers);

    glm::mat4 model_Matrix = glm::mat4(1.0f);

    glm::vec3 translationA(0, 0, 0.0f);
    model_Matrix = glm::translate(model_Matrix, translationA);
    model_Matrix = glm::scale(model_Matrix, glm::vec3(100, 100, 100));
    model_Matrix = glm::rotate(model_Matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_Shader->SetUniformMat4f("u_Model", model_Matrix);


    glfwSetWindowUserPointer(m_Window, this);

    auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        instance->updateViewportSize(width, height);
        instance->m_Camera.setViewportSize(width, height);
        glViewport(0, 0, width, height);
    };

    auto mouse_callback = [](GLFWwindow* window, double xposIn, double yposIn) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        instance->m_Camera.mouse_callback(window, xposIn, yposIn);
    };

    auto scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset) {
        Application* instance = (Application*)glfwGetWindowUserPointer(window);
        instance->m_Camera.scroll_callback(window, xoffset, yoffset);
    };
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_Window, mouse_callback);
    glfwSetScrollCallback(m_Window, scroll_callback);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glm::vec3 slider = glm::vec3(0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(m_Window)) {

        processInput(m_Window);

        /* Modify vertex data here */
        vertices[0].Color = glm::vec4(slider, 1.0f);

        /* Set dynamic Vertex Buffer Data*/
        m_VertexBuffer->SetData(0, sizeof(vertices), vertices);

        /* Modify index data here */

        /* Set dynamic Index Buffer Data*/
        m_IndexBuffer->SetData(0, sizeof(indices), indices);

        m_Renderer.Clear();

        m_Shader->SetUniformMat4f("u_Projection", m_Camera.getProjectionMatrix());
        m_Shader->SetUniformMat4f("u_View", m_Camera.getViewMatrix());

        m_Renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

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

void Application::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        m_Camera.toggleLock();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Camera.processKeyboard(FORWARD, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Camera.processKeyboard(BACKWARD, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Camera.processKeyboard(LEFT, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Camera.processKeyboard(RIGHT, 1.0f);
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

    ImGui::SliderFloat3("Colors", &slider.r, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}