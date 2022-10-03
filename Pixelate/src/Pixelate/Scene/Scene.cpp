#include "PXpch.h"
#include "Scene.h"

#include "Pixelate/Rendering/Renderer2D.h"
#include "Components.h"
#include "Entity.h"
#include "Pixelate/Rendering/RenderCommand.h"
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "glm/gtx/quaternion.hpp"


#include "Pixelate/Core/Application.h"
#include "Pixelate/Scene/SceneManager.h"
#include "Pixelate/Audio/Audio.h"

#include "Pixelate/Rendering/SceneRenderer.h"
#include "Pixelate/Editor/EditorPanel.h"


namespace Pixelate {

	static Ref<Texture> s_AudioIcon; // temp

	Scene::Scene(const std::string& name /*= "Scene"*/) : m_Name(name)
	{
		Init();
	}


	Scene::~Scene() {
		m_Reg.clear();
	}

	void Scene::Init()
	{
		m_SceneEntity = m_Reg.create();


		TextureProperties props;
		props.TexFilter = TextureProperties::Filter::Linear;
		s_AudioIcon = Texture::Create("Resources/icons/scene-viewport/audio-source.png", props);

		m_Reg.emplace<PhysicsWorldComponent>(m_SceneEntity); // temp for now...



	}


	

	void Scene::OnUpdate(const Ref<SceneRenderer>& renderer, const Ref<EditorCamera>& camera)
	{

		auto renderGroup = m_Reg.view<SpriteRendererComponent>();
		auto boxColliderGroup = m_Reg.view<BoxColliderComponent>();
		auto circleColliderGroup = m_Reg.view<CircleColliderComponent>();
		auto polyColliderGroup = m_Reg.view<PolygonColliderComponent>();
		auto edgeColliderGroup = m_Reg.view<EdgeColliderComponent>();
		auto audioSrcGroup = m_Reg.view<AudioSourceComponent>();

		auto& editorPanel = EditorPanelManager::Get().GetPanel("SceneHierarcy");
		// find the primary camera to use for rendering.
		CameraComponent* targetCam = nullptr;

		bool renderSelectedEntity = renderer->GetOptions() & SceneRendererOptions::RenderEntitySelection;

		if (!camera)
		{
			auto allCameras = m_Reg.view<CameraComponent>();
			bool continueloop = true;
			for (auto entity : allCameras) {
				if (continueloop) {
					Entity e{ entity, this };
					auto& transformComp = e.GetComponent<TransformComponent>();
					auto& camComp = e.GetComponent<CameraComponent>();
					camComp.Camera.SetViewMatrix(transformComp.Transform);
					const auto& orthoSize = camComp.Camera.GetOrthographicSize();
					camComp.Camera.SetProjection({ -m_AspectRatio * orthoSize, m_AspectRatio * orthoSize, -orthoSize, orthoSize });

					if (camComp.PrimaryCamera) {
						targetCam = &camComp;
						continueloop = false;
					}

				}

			}

			if (targetCam != nullptr) {
				RenderCommand::SetClearColor(targetCam->ClearColor.r, targetCam->ClearColor.g, targetCam->ClearColor.b, targetCam->ClearColor.a);
				renderer->BeginScene(targetCam->Camera.GetViewProjectionMatrix());
			}
			else {
				PX_CORE_WARN("Currently no primary camera in the scene!\n");
			}

		}
		else
		{
			renderer->BeginScene(camera->GetViewProjectionMatrix());
		}


		for (auto entity : renderGroup) {
			Entity e{ entity, this };
			bool isEntitySelected = editorPanel->HasAnEntitySelected() && editorPanel->CurrentlySelectedEntity() == e;

			auto& transformComp = e.GetComponent<TransformComponent>();
			auto& spriteComp = e.GetComponent<SpriteRendererComponent>();

			renderer->SubmitSprite(transformComp, spriteComp, (int)entity);

			if (isEntitySelected && renderSelectedEntity) {
				AABB boundingBox;
				glm::vec4 color;

				auto [Pos, Rot, Scale] = transformComp.DecomposeTransform();

				boundingBox.Min = { -Scale.x / 2.0f + Pos.x, -Scale.y / 2.0f + Pos.y, 1.0f };
				boundingBox.Max = { Scale.x / 2.0f + Pos.x, Scale.y / 2.0f + Pos.y, 1.0f };

				color = { 0.0f, 1.0f, 1.0f, 1.0f };
				renderer->SubmitAABB(boundingBox, color);

			}

		
		}

		if (renderer->GetOptions() & SceneRendererOptions::RenderPhysicsPrimitives)
		{

			for (auto entity : boxColliderGroup) {
				Entity e{ entity, this };
				bool isEntitySelected = editorPanel->HasAnEntitySelected() && editorPanel->CurrentlySelectedEntity() == e;
				auto [Pos, rot, scale] = e.GetComponent<TransformComponent>().DecomposeTransform();

				AABB boundingBox;
				glm::vec4 color;

				auto& bcc = e.GetComponent<BoxColliderComponent>();
				if (isEntitySelected)
				{
					color = { 0.0f, 1.0f, 0.0f, 1.0f };
				}
				else {
					color = { 0.0f, 1.0f, 0.0f, 0.5f };
				}

				glm::vec2 transformPos = { Pos.x, Pos.y };
				const auto Position = bcc.Center + transformPos;
				const auto& Scale = bcc.Size;

				boundingBox.Min = { -Scale.x + Position.x, -Scale.y + Position.y, 1.0f };
				boundingBox.Max = { Scale.x + Position.x, Scale.y + Position.y, 1.0f };
				renderer->SubmitAABB(boundingBox, color);
			}

			for (auto entity : circleColliderGroup) {
				Entity e{ entity, this };
				bool isEntitySelected = editorPanel->HasAnEntitySelected() && editorPanel->CurrentlySelectedEntity() == e;
				auto [Pos, rot, scale] = e.GetComponent<TransformComponent>().DecomposeTransform();

				glm::vec4 color;
				auto& cc = e.GetComponent<CircleColliderComponent>();
				if (isEntitySelected)
				{
					color = { 0.0f, 1.0f, 0.0f, 1.0f };
				}
				else {
					color = { 0.0f, 1.0f, 0.0f, 0.5f };
				}

				glm::vec2 transformPos = { Pos.x, Pos.y };
				const auto Position = cc.Center + transformPos;

				renderer->SubmitCircle(Position, cc.Radius, color);
			}

			for (auto entity : edgeColliderGroup) {
				Entity e{ entity, this };
				bool isEntitySelected = editorPanel->HasAnEntitySelected() && editorPanel->CurrentlySelectedEntity() == e;
				auto [Pos, rot, scale] = e.GetComponent<TransformComponent>().DecomposeTransform();

				glm::vec4 color;
				auto& ecc = e.GetComponent<EdgeColliderComponent>();
				if (isEntitySelected)
				{
					color = { 0.0f, 1.0f, 0.0f, 1.0f };
				}
				else {
					color = { 0.0f, 1.0f, 0.0f, 0.5f };
				}

				glm::vec2 transformPos = { Pos.x, Pos.y };
				const auto PositionA = ecc.Point1 + transformPos;
				const auto PositionB = ecc.Point2 + transformPos;

				renderer->SubmitLine({ PositionA.x, PositionA.y, 0.0f }, { PositionB.x, PositionB.y, 0.0f }, color);

			}

			for (auto entity : polyColliderGroup) {

				Entity e{ entity, this };
				bool isEntitySelected = editorPanel->HasAnEntitySelected() && editorPanel->CurrentlySelectedEntity() == e;
				auto [Pos, rot, scale] = e.GetComponent<TransformComponent>().DecomposeTransform();

				glm::vec4 color;
				auto& ecc = e.GetComponent<PolygonColliderComponent>();
				if (isEntitySelected)
				{
					color = { 0.0f, 1.0f, 0.0f, 1.0f };
				}
				else {
					color = { 0.0f, 1.0f, 0.0f, 0.5f };
				}

				glm::vec2 transformPos = { Pos.x, Pos.y };

				std::vector<glm::vec4> verts;
				for (auto& Vertice : ecc.Vertices) {
					verts.emplace_back(Vertice.x + transformPos.x, Vertice.y + transformPos.y, 0.0f, 1.0f);
				}

				renderer->SubmitVertices(verts.data(), ecc.Vertices.size(), color);

			}
		}

		
		bool renderGizmos = renderer->GetOptions() & SceneRendererOptions::RenderGizmos;

		if (renderGizmos)
		{
			for (auto entity : audioSrcGroup)
			{
				Entity e{ entity, this };
				auto& transformComp = e.GetComponent<TransformComponent>();
				SpriteRendererComponent src;
				src.Texture = s_AudioIcon;
				src.Rect = Rect({ 0, 0 }, { 512, 512 });
				renderer->SubmitSprite(transformComp, src, (int)entity);

			}
		}

		RenderCommand::Clear();

		if (renderer->GetOptions() & SceneRendererOptions::RenderGrid)
			renderer->DrawSceneGrid(camera->GetOrthographicSize());
		renderer->EndScene();

	}


	void Scene::OnRuntimeStart() {
		// Init Physics
		auto scene = m_Reg.view<PhysicsWorldComponent>();
		auto& physicsWorld = m_Reg.get<PhysicsWorldComponent>(scene.front());


		auto rbView = m_Reg.view<RigidBodyComponent>();

		for (auto entity : rbView) {
			Entity e{ entity, this };
			auto& transformComp = e.GetComponent<TransformComponent>();
			auto& ridBodComp = e.GetComponent<RigidBodyComponent>();
			
			auto [pos, rotationQ, scale] = transformComp.DecomposeTransform();
			float rotation = glm::degrees(glm::eulerAngles(rotationQ)).z;
			
			const RigidBodyDef& def = ridBodComp.Definition;

			ridBodComp.Definition.Position = pos;
			ridBodComp.Definition.Angle = rotation;

			ridBodComp.RigidBody.Init(physicsWorld, {(unsigned int)entity, this}, ridBodComp.Definition);

			b2Shape* shape = nullptr;
			if (e.HasComponent<BoxColliderComponent>()) {

				auto& cc = e.GetComponent<BoxColliderComponent>();

				const auto& colliderDat = cc.ColliderData;
				const auto& isTrigger = cc.IsTrigger;

				shape = cc.ColliderData = new b2PolygonShape;
				cc.ColliderData->SetAsBox(cc.Size.x, cc.Size.y, {cc.Center.x, cc.Center.y}, rotation);

				
				ridBodComp.RigidBody.AddCollider(colliderDat, def.CategoryLayer, Physics::GetLayerRuleMask(def.CategoryLayer), def.Material->Mass, def.Material->Friction, def.Material->Bounciness, isTrigger);
				
			}

			if (e.HasComponent<CircleColliderComponent>()) {
				auto& cc = e.GetComponent<CircleColliderComponent>();

				const auto& colliderDat = cc.ColliderData;
				const auto& isTrigger = cc.IsTrigger;


				shape = cc.ColliderData = new b2CircleShape;
				cc.ColliderData->m_radius = cc.Radius;
				cc.ColliderData->m_p = { cc.Center.x, cc.Center.y };

				
				ridBodComp.RigidBody.AddCollider(colliderDat, def.CategoryLayer, Physics::GetLayerRuleMask(def.CategoryLayer), def.Material->Mass, def.Material->Friction, def.Material->Bounciness, isTrigger);
			}

			if (e.HasComponent<EdgeColliderComponent>()) {
				auto& ecc = e.GetComponent<EdgeColliderComponent>();


				const auto& colliderDat = ecc.ColliderData;
				const auto& isTrigger = ecc.IsTrigger;

				ecc.ColliderData = new b2EdgeShape;
				ecc.ColliderData->Set({ ecc.Point1.x, ecc.Point1.y }, { ecc.Point2.x, ecc.Point2.y });

		
				ridBodComp.RigidBody.AddCollider(colliderDat, def.CategoryLayer, Physics::GetLayerRuleMask(def.CategoryLayer), def.Material->Mass, def.Material->Friction, def.Material->Bounciness, isTrigger);

			}

			if (e.HasComponent<PolygonColliderComponent>()) {
				auto& pcc = e.GetComponent<PolygonColliderComponent>();

				const auto& colliderDat = pcc.ColliderData;
				const auto& isTrigger = pcc.IsTrigger;

				if (ridBodComp.Definition.Type != BodyType::Static) {
					pcc.ColliderData = new b2PolygonShape;
					b2PolygonShape* shape = (b2PolygonShape*)pcc.ColliderData;

					shape->Set(pcc.Vertices.data(), pcc.Vertices.size());

				} else {
					pcc.ColliderData = new b2ChainShape;
					b2ChainShape* shape = (b2ChainShape*)pcc.ColliderData;
					
					shape->CreateChain(pcc.Vertices.data(), pcc.Vertices.size());


				}

				ridBodComp.RigidBody.AddCollider(colliderDat, def.CategoryLayer, Physics::GetLayerRuleMask(def.CategoryLayer), def.Material->Mass, def.Material->Friction, def.Material->Bounciness, isTrigger);


			}

		}


		// Init Scripts
		auto sbcView = m_Reg.view<ScriptingBehaviourComponent>();
		for (auto entity : sbcView) {
			Entity e{ entity, this };
			auto& sbc = e.GetComponent<ScriptingBehaviourComponent>();

			ScriptingMaster::OnEntityCreate(sbc.Behaviour);
		}


		m_IsPlaying = true;

	}

	void Scene::OnRuntimeUpdate(float ts) {
		// Updating Scripts

		auto sbcView = m_Reg.view<ScriptingBehaviourComponent>();
		for (auto entity : sbcView) {
			Entity e{ entity, this };
			auto& sbc = e.GetComponent<ScriptingBehaviourComponent>();
			void* params = &ts;

			ScriptingMaster::OnEntityUpdate(sbc.Behaviour, &params);
		}


		// Spatial Audio ----- TEMP

		auto alcView = m_Reg.view<AudioListenerComponent>();
		AudioListener* al = nullptr;
		bool cont = true;
		for (auto entity : alcView) {
			if (cont) {

				Entity e{ entity, this };
				auto& alc = e.GetComponent<AudioListenerComponent>();

				if (alc.Listener.IsSpatial()) {
					auto& transform = e.GetComponent<TransformComponent>();
					auto[pos, rot, scale] = transform.DecomposeTransform();
					alc.Listener.SetPosition(pos);
					al = &alc.Listener;
					cont = false;
				}
			}

		}

		if (al != nullptr) {
			auto ascView = m_Reg.view<AudioSourceComponent>();
			for (auto entity : ascView) {

				Entity e{ entity, this };
				auto& asc = e.GetComponent<AudioSourceComponent>();
				auto& transform = e.GetComponent<TransformComponent>();
				auto [pos, rot, scale] = transform.DecomposeTransform();
				asc.Source->SetPosition(pos);

			}
		}
	


		// Physics
		auto scene = m_Reg.view<PhysicsWorldComponent>();
		auto& physicsWorld = m_Reg.get<PhysicsWorldComponent>(scene.front());

		auto& time = Application::GetApp().GetTime();

		physicsWorld.World->Step(physicsWorld.FixedTimeStep, physicsWorld.VelocityIterations, physicsWorld.PositionIterations);
// 		while (physicsWorld.CurrentSimulationTime < time.GetElapsedSeconds()) {
// 			physicsWorld.CurrentSimulationTime += physicsWorld.FixedTimeStep;
// 		}

		auto rbView = m_Reg.view<RigidBodyComponent>();

		for (auto entity : rbView) {

			Entity e{ entity, this };

			auto& transformComp = e.GetComponent<TransformComponent>();
			const auto& ridBodComp = e.GetComponent<RigidBodyComponent>();


			const auto& position = ridBodComp.RigidBody.GetPosition();
			const auto& angle = ridBodComp.RigidBody.GetAngle();
			auto [pos, rotationQ, scale] = transformComp.DecomposeTransform();

			auto rotation = glm::eulerAngles(rotationQ);

			transformComp.Transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) *
				glm::toMat4(glm::quat({ rotation.x, rotation.y, angle })) *
				glm::scale(glm::mat4(1.0f), scale);
			
		}

		

	}

	void Scene::OnRuntimeStop() {





		// calling on destroy in scripts
		auto sbcView = m_Reg.view<ScriptingBehaviourComponent>();
		for (auto entity : sbcView) {
			Entity e{ entity, this };
			auto& sbc = e.GetComponent<ScriptingBehaviourComponent>();

			ScriptingMaster::OnEntityDestroy(sbc.Behaviour);
		}


		auto audioView = m_Reg.view<AudioSourceComponent>();
		for (auto entity : audioView) {
			Entity e{ entity, this };
			auto& asc = e.GetComponent<AudioSourceComponent>();


			asc.Source->Stop();
		}


		auto scene = m_Reg.view<PhysicsWorldComponent>();
		auto& physicsWorld = m_Reg.get<PhysicsWorldComponent>(scene.front());

		auto rbView = m_Reg.view<RigidBodyComponent>();

		for (auto entity : rbView) {
			Entity e{ entity, this };
			auto& transformComp = e.GetComponent<TransformComponent>();
			auto& ridBodComp = e.GetComponent<RigidBodyComponent>();

			ridBodComp.RigidBody.Destroy();

		}
	}


	void Scene::SetGameViewport(unsigned int width, unsigned int height) {
		m_AspectRatio = (float)width / (float)height;
	}

	Entity Scene::CreateEntity(const std::string& name) {
		auto entity = Entity(m_Reg.create(), this);
		entity.AddComponent<UUIDComponent>().UUID;
		auto& id = entity.GetComponent<UUIDComponent>().UUID;

		m_EntityMap[id] = entity;
		entity.AddComponent<TagComponent>("No tag");
		entity.AddComponent<NameComponent>(name);
		glm::mat4 transform = glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f,1.0f });
		entity.AddComponent<TransformComponent>(transform);
		return entity;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
		auto entity = Entity(m_Reg.create(), this);
		entity.AddComponent<UUIDComponent>().UUID;
		auto& id = entity.GetComponent<UUIDComponent>().UUID = uuid;

		m_EntityMap[id] = entity;

		entity.AddComponent<TagComponent>("No tag");
		entity.AddComponent<NameComponent>(name);
		glm::mat4 transform = glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f,1.0f });
		entity.AddComponent<TransformComponent>(transform);

		return entity;
	}

	void Scene::DeleteEntity(Entity entity) {
		m_Reg.destroy(entity.GetHandle());
	}

}
