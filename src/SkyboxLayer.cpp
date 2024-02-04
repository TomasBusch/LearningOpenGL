#include "SkyboxLayer.h"

#include "Common.h"
#include "Model.h"

SkyboxLayer::SkyboxLayer() 
    :m_Camera(nullptr)
{
    std::vector<float> skyboxVertices = {
        -1, -1,  1.0, //0
         1, -1,  1.0, //1
        -1,  1,  1.0, //2
         1,  1,  1.0, //3
        -1, -1, -1.0, //4
         1, -1, -1.0, //5
        -1,  1, -1.0, //6
         1,  1, -1.0  //7
    };

    std::vector<uint32_t> skyboxIndices = {
        //Top
        2, 6, 7,
        7, 3, 2,

        //Bottom
        5, 4, 0,
        0, 1, 5,

        //Left
        6, 2, 0,
        0, 4, 6,

        //Right
        1, 3, 7,
        7, 5, 1,

        //Front
        0, 2, 3,
        3, 1, 0,

        //Back
        7, 6, 4,
        4, 5, 7
    };

    m_SkyboxVertices = skyboxVertices;
    m_SkyboxIndices = skyboxIndices;


    //TODO camera logic

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(m_SkyboxVertices.data(), m_SkyboxVertices.size() * sizeof(float));
    m_VertexBufferLayout = std::make_unique<VertexBufferLayout>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(m_SkyboxIndices.data(), m_SkyboxIndices.size());

    m_VertexBufferLayout->Push<glm::vec3>(1, 0);

    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

    m_Shader = std::make_unique<Shader>("res/shaders/skybox");
    m_Shader->Bind();

    m_Skybox = std::make_unique<CubeMap>("res/textures/skybox");
    

    //TODO make singleton texture slot tracker for asignment
    m_Skybox->Bind(2);

    m_Shader->SetUniform1i("skybox", 2);

    glm::mat4 model_Matrix = glm::mat4(1.0f);

    m_Shader->SetUniformMat4f("u_Model", model_Matrix);
    
}

void SkyboxLayer::onUpdate() {
    m_Shader->Bind();

    glm::mat4 view = glm::mat4(glm::mat3(m_Camera->getViewMatrix())); // remove translation from the view matrix
    m_Shader->SetUniformMat4f("u_Projection", m_Camera->getProjectionMatrix());
    m_Shader->SetUniformMat4f("u_View", view);

    glDepthFunc(GL_LEQUAL);

    m_Renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

    glDepthFunc(GL_LESS);
}
