#include "TestTexture2D.h"
#include "Common.h"

/* glm */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

namespace test {

	TestTexture2D::TestTexture2D()
        :m_Renderer(Renderer()), m_TranslationA(glm::vec3(200, 200, 0)), m_TranslationB(glm::vec3(100, 100, 0)),
        m_ModelA(glm::mat4(1.0f)), m_ModelB(glm::mat4(1.0f)), m_MVP(glm::mat4(1.0f))
    {
		/* Define Vertex postions */
		float vertex[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			 50.0f, -50.0f, 1.0f, 0.0f, //1
			 50.0f,  50.0f, 1.0f, 1.0f, //2
			-50.0f,  50.0f, 0.0f, 1.0f  //3
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

        /* Init vertex array obj */
        m_VertexArray = new VertexArray();
        /* Create and asign data to vertex buffer */
        m_VertexBuffer = new VertexBuffer(vertex, 4 * 4 * sizeof(float));

        /* Init vertex layout */
        m_VertexBufferLayout = new VertexBufferLayout();

        /* Create layout */
        m_VertexBufferLayout->Push<float>(2);
        m_VertexBufferLayout->Push<float>(2);

        /* Bind vertex buffer to layout */
        m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

        /* Create and asign data to vertex index buffer */
        m_IndexBuffer = new IndexBuffer(indices, 6);

        /* Init and load shader source */
        m_Shader = new Shader("res/shaders/basic");

        //m_Shader->SetUniform4f("u_Color", 0.2f, 0.0f, 0.8f, 1.0f);

        /* Creating screen projection matrix */
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        /* Creating view matrix (view matrices are just the oposite transform of model matrices)*/
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        /* Creating model transform matrix */
        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(100, 100, 0);
        m_ModelA = glm::translate(glm::mat4(1.0f), translationA);
        m_ModelB = glm::translate(glm::mat4(1.0f), translationB);

        m_VPMatrix = proj * view;
        //glm::mat4 mvp = vpmatrix * m_ModelA;
        //shader.SetUniformMat4f("u_MVP", mvp);

        /* Load and bind texture */
        m_Texture = new Texture("res/textures/infimp.png");
        int TextureSlot = 0;
        m_Texture->Bind(TextureSlot);
        m_Shader->SetUniform1i("u_Texture", TextureSlot);
	}

	TestTexture2D::~TestTexture2D() {
        delete m_VertexArray;
        delete m_VertexBuffer;
        delete m_VertexBufferLayout;
        delete m_IndexBuffer;

        delete m_Shader;
        delete m_Texture;

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestTexture2D::OnUpdate(float deltaTime) {

	}

	void TestTexture2D::OnRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Renderer.Clear();

        m_Shader->Bind();

        m_ModelA = glm::translate(glm::mat4(1.0f), m_TranslationA);
        m_MVP = m_VPMatrix * m_ModelA;
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        /* Draw call for first obj*/
        m_Renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

        m_ModelB = glm::translate(glm::mat4(1.0f), m_TranslationB);
        m_MVP = m_VPMatrix * m_ModelB;
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        /* Draw call for second obj*/
        m_Renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestTexture2D::OnImGuiRender() {
        ImGui::SliderFloat2("Translation A##translateA", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat2("Translation B##translateB", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}