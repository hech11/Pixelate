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


	

	void Scene::OnUpdate(float ts, const Ref<EditorCamera>& camera, Entity selectedEntity,bool hasEntityBeenSelected)
	{


		// Rendering the scene editor viewport
		Renderer2D::ResetStatistics();

		Renderer2D::BeginScene(camera.get());
		Renderer2D::DrawSceneGrid(camera->GetOrthographicSize());

		auto renderGroup = m_Reg.view<TransformComponent>();

		for (auto entity : renderGroup) {
			Entity e{ entity, this };
			auto& transformComp = e.GetComponent<TransformComponent>();

			if (e.HasComponent<SpriteRendererComponent>()) {
				auto& spriteComp = e.GetComponent<SpriteRendererComponent>();

				if (spriteComp.Texture) {
					Renderer2D::DrawSprite(transformComp, spriteComp, (int)entity);
				} else {
					Renderer2D::DrawSprite(transformComp.Transform, spriteComp.TintColor, (int)entity);
				}

				if (hasEntityBeenSelected && selectedEntity == e) {
					AABB boundingBox;
					glm::vec4 color;

					auto [Pos, Rot, Scale] = transformComp.DecomposeTransform();

					boundingBox.Min = { -Scale.x / 2.0f + Pos.x, -Scale.y / 2.0f + Pos.y, 1.0f };
					boundingBox.Max = { Scale.x / 2.0f + Pos.x, Scale.y / 2.0f + Pos.y, 1.0f };

					color = { 0.0f, 1.0f, 1.0f, 1.0f };
					Renderer2D::DrawAABB(boundingBox, color);

					if (e.HasComponent<BoxColliderComponent>()) {
						auto& bcc = e.GetComponent<BoxColliderComponent>();
						color = { 0.0f, 1.0f, 0.0f, 1.0f };

						glm::vec2 transformPos = { Pos.x, Pos.y };
						const auto Position = bcc.Center + transformPos;
						const auto& Scale = bcc.Size;

						boundingBox.Min = { -Scale.x + Position.x, -Scale.y + Position.y, 1.0f };
						boundingBox.Max = { Scale.x + Position.x, Scale.y + Position.y, 1.0f };
						Renderer2D::DrawAABB(boundingBox, color);

					}

					if (e.HasComponent<CircleColliderComponent>()) {
						auto& cc = e.GetComponent<CircleColliderComponent>();
						color = { 0.0f, 1.0f, 0.0f, 1.0f };

						glm::vec2 transformPos = { Pos.x, Pos.y };
						const auto Position = cc.Center + transformPos;

						Renderer2D::DrawCircle(Position, cc.Radius, color);
					}

					if (e.HasComponent<EdgeColliderComponent>()) {
						auto& ecc = e.GetComponent<EdgeColliderComponent>();
						color = { 0.0f, 1.0f, 0.0f, 1.0f };

						glm::vec2 transformPos = { Pos.x, Pos.y };
						const auto PositionA = ecc.Point1 + transformPos;
						const auto PositionB = ecc.Point2 + transformPos;

						Renderer2D::DrawLine({ PositionA.x, PositionA.y, 0.0f}, { PositionB.x, PositionB.y, 0.0f }, color);

					}

					if (e.HasComponent<PolygonColliderComponent>()) {
						auto& ecc = e.GetComponent<PolygonColliderComponent>();
						color = { 0.0f, 1.0f, 0.0f, 1.0f };

						glm::vec2 transformPos = { Pos.x, Pos.y };

						std::vector<glm::vec4> verts;
						for (int i = 0; i < ecc.Vertices.size(); i++) {
							verts.push_back({ ecc.Vertices[i].x + transformPos.x, ecc.Vertices[i].y + transformPos.y, 0.0f, 1.0f });
						}

						Renderer2D::DrawVerticies(verts.data(), ecc.Vertices.size(), color);

					}






				}



			}

			if (e.HasComponent<AudioSourceComponent>())
				Renderer2D::DrawSprite(transformComp.Transform, s_AudioIcon, { {0, 0}, {512, 512} }, { 1.0f, 1.0f, 1.0f, 1.0f }, (int)entity);



		}

		Renderer2D::EndScene();

	}


	void Scene::OnGameViewportRender()
	{
		// find the primary camera to use for rendering.
		CameraComponent* renderCam = nullptr;
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
					renderCam = &camComp;
					continueloop = false;
				}

			}

		}

		// if there was no camera found in the scene
		if (renderCam != nullptr) {

			RenderCommand::SetClearColor(renderCam->ClearColor.r, renderCam->ClearColor.g, renderCam->ClearColor.b, renderCam->ClearColor.a);

			RenderCommand::Clear();
			Renderer2D::ResetStatistics();

			Renderer2D::BeginScene(&renderCam->Camera);

			auto renderGroup = m_Reg.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : renderGroup) {
				auto [transformComp, spriteComp] = renderGroup.get<TransformComponent, SpriteRendererComponent>(entity);

				if (spriteComp.Texture) {
					Renderer2D::DrawSprite(transformComp, spriteComp, (int)entity);
				}
				else {
					Renderer2D::DrawSprite(transformComp.Transform, spriteComp.TintColor, (int)entity);
				}


			}
			Renderer2D::EndScene();
		} else {
			PX_CORE_WARN("Currently no primary camera in the scene!\n");
		}
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
			
			RigidBodyDef def;

			ridBodComp.Definition.Position = pos;
			ridBodComp.Definition.Angle = rotation;

			ridBodComp.RigidBody.Init(physicsWorld, {(unsigned int)entity, this}, ridBodComp.Definition);


			if (e.HasComponent<BoxColliderComponent>()) {

				auto& cc = e.GetComponent<BoxColliderComponent>();

				cc.ColliderData = new b2PolygonShape;
				cc.ColliderData->SetAsBox(cc.Size.x, cc.Size.y, {cc.Center.x, cc.Center.y}, rotation);


				ridBodComp.RigidBody.AddCollider(cc.ColliderData, 1.0f, 1.0f, cc.IsTrigger);
				
			}

			if (e.HasComponent<CircleColliderComponent>()) {
				auto& cc = e.GetComponent<CircleColliderComponent>();

				cc.ColliderData = new b2CircleShape;
				cc.ColliderData->m_radius = cc.Radius;
				cc.ColliderData->m_p = { cc.Center.x, cc.Center.y };

				ridBodComp.RigidBody.AddCollider(cc.ColliderData, 1.0f, 1.0f, cc.IsTrigger);
			}

			if (e.HasComponent<EdgeColliderComponent>()) {
				auto& ecc = e.GetComponent<EdgeColliderComponent>();

				ecc.ColliderData = new b2EdgeShape;
				ecc.ColliderData->Set({ ecc.Point1.x, ecc.Point1.y }, { ecc.Point2.x, ecc.Point2.y });

				ridBodComp.RigidBody.AddCollider(ecc.ColliderData, 1.0f, 1.0f, ecc.IsTrigger);

			}

			if (e.HasComponent<PolygonColliderComponent>()) {
				auto& pcc = e.GetComponent<PolygonColliderComponent>();

				if (ridBodComp.Definition.Type != BodyType::Static) {
					pcc.ColliderData = new b2PolygonShape;
					b2PolygonShape* shape = (b2PolygonShape*)pcc.ColliderData;

					shape->Set(pcc.Vertices.data(), pcc.Vertices.size());

				} else {
					pcc.ColliderData = new b2ChainShape;
					b2ChainShape* shape = (b2ChainShape*)pcc.ColliderData;
					
					shape->CreateChain(pcc.Vertices.data(), pcc.Vertices.size());


				}

				ridBodComp.RigidBody.AddCollider(pcc.ColliderData, 1.0f, 1.0f, pcc.IsTrigger);

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
