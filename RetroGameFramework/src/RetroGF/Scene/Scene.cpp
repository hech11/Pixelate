#include "RGFpch.h"
#include "Scene.h"

#include "RetroGF/Rendering/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "RetroGF/Rendering/RenderCommand.h"
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtx/matrix_decompose.hpp"



namespace RGF {

	Scene::Scene(const std::string& name /*= "Scene"*/) : m_Name(name)
	{
		Init();
	}

	Scene::~Scene() {
		m_Reg.clear();
	}

	void Scene::Init()
	{
		// TODO: Set stuff in here once reading the scene from a file.
	}

	


	void Scene::OnUpdate(float ts, EditorCamera& camera)
	{
		RenderCommand::Clear();
		Renderer2D::ResetStatistics();

		// Rendering
		Renderer2D::BeginScene(&camera.GetCamera());
		auto renderGroup = m_Reg.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : renderGroup) {
			auto[transformComp, spriteComp] = renderGroup.get<TransformComponent, SpriteRendererComponent>(entity);

			// Does not support rotation yet
			if (spriteComp.SpriteRect) {
				Renderer2D::DrawSprite(transformComp.Transform, spriteComp.SpriteRect, spriteComp.TintColor);
			} else {
				Renderer2D::DrawSprite(transformComp.Transform, spriteComp.TintColor);
			}

		}

		Renderer2D::EndScene();
	}

	void Scene::OnEvent(Event& e)
	{

	}

	Entity Scene::CreateEntity(const std::string& name) {
		auto entity = Entity(m_Reg.create(), this);
		entity.AddComponent<NameComponent>(name);
		glm::mat4 transform = glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f,1.0f });
		entity.AddComponent<TransformComponent>(transform);
		return entity;
	}

	void Scene::DeleteEntity(Entity entity) {
		m_Reg.destroy(entity.GetHandle());
	}

}
