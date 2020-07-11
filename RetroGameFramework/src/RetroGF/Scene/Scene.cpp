#include "RGFpch.h"
#include "Scene.h"

#include "RetroGF/Rendering/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "RetroGF/Rendering/RenderCommand.h"


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
			Renderer2D::DrawSprite(transformComp.Position, transformComp.Rotation, transformComp.Scale, spriteComp.SpriteRect, spriteComp.TintColor);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnEvent(Event& e)
	{

	}

	Ref<Entity> Scene::CreateEntity() {
		auto entity = CreateRef<Entity>(m_Reg.create(), this);
		entity->AddComponent<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		return entity;
	}

}
