#include "PXpch.h"
#include "SceneSerialization.h"
#include "yaml-cpp/yaml.h"

#include "Pixelate/Scene/Components.h"
#include "Pixelate/Scene/Entity.h"

#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Audio/Audio.h"

#include <fstream>
#include "glm/gtc/type_ptr.inl"
#include "../Asset/AssetManager.h"


namespace YAML {

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& other) {
			Node node;
			node.push_back(other.x);
			node.push_back(other.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& other) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			other.x = node[0].as<float>();
			other.y = node[1].as<float>();
			return true;
		}
	};


	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& other) {
			Node node;
			node.push_back(other.x);
			node.push_back(other.y);
			node.push_back(other.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& other) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			other.x = node[0].as<float>();
			other.y = node[1].as<float>();
			other.z = node[2].as<float>();
			return true;
		}

	
	};


	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& other) {
			Node node;
			node.push_back(other.x);
			node.push_back(other.y);
			node.push_back(other.z);
			node.push_back(other.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& other) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			other.x = node[0].as<float>();
			other.y = node[1].as<float>();
			other.z = node[2].as<float>();
			other.w = node[3].as<float>();
			return true;
		}


	};


	template<>
	struct convert<glm::u32vec4> {
		static Node encode(const glm::u32vec4& other) {
			Node node;
			node.push_back(other.x);
			node.push_back(other.y);
			node.push_back(other.z);
			node.push_back(other.w);
			return node;
		}

		static bool decode(const Node& node, glm::u32vec4& other) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			other.x = node[0].as<unsigned int>();
			other.y = node[1].as<unsigned int>();
			other.z = node[2].as<unsigned int>();
			other.w = node[3].as<unsigned int>();
			return true;
		}
	};


	template<>
	struct convert<glm::uvec2> {
		static Node encode(const glm::uvec2& other) {
			Node node;
			node.push_back(other.x);
			node.push_back(other.y);
			return node;
		}

		static bool decode(const Node& node, glm::uvec2& other) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			other.x = node[0].as<unsigned int>();
			other.y = node[1].as<unsigned int>();
			return true;
		}
	};


	template<>
	struct convert<glm::mat4> {

		static Node encode(const glm::mat4& other) {
			Node node;


			const float* pSource = (const float*)glm::value_ptr(other);
			for (int i = 0; i < 16; ++i) {
				node.push_back(pSource[i]);
			}

			return node;
		}

		static bool decode(const Node& node, glm::mat4& other) {
			if (!node.IsSequence() || node.size() != 16) {
				return false;
			}
			float temp[16];

			for (unsigned int i = 0; i < 16; i++) {
				temp[i] = node[i].as<float>();
			}

			other = glm::make_mat4(temp);

			return true;
		}


	};
}


namespace Pixelate {

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec2& other) {
		out << YAML::Flow;
		out << YAML::BeginSeq << other.x << other.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec3& other) {
		out << YAML::Flow;
		out << YAML::BeginSeq << other.x << other.y << other.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec4& other) {
		out << YAML::Flow;
		out << YAML::BeginSeq << other.x << other.y << other.z << other.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::u32vec4& other) {
		out << YAML::Flow;
		out << YAML::BeginSeq << other.x << other.y << other.z << other.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::mat4& other) {

		const float* pSource = (const float*)glm::value_ptr(other);


		out << YAML::Flow;
		out << YAML::BeginSeq << 
			pSource[0] <<  pSource[1]  << pSource[2]  << pSource[3] << 
			pSource[4] <<  pSource[5]  << pSource[6]  << pSource[7] << 
			pSource[8] <<  pSource[9]  << pSource[10] << pSource[11] << 
			pSource[12] << pSource[13] << pSource[14] << pSource[15] <<
			YAML::EndSeq;

		return out;
	}

	void SceneSerialization::Serialize(const Ref<Scene>& scene, const std::string& filepath) {
		YAML::Emitter data;
		data << YAML::BeginMap;
		data << YAML::Key << "Scene";
		data << YAML::Value << scene->GetName();
		data << YAML::Key << "Entities";
		data << YAML::Value << YAML::BeginSeq;

		scene->GetReg().each([&](auto entity) {
			Entity e = { entity, scene.get() };

			if (!e.HasComponent<UUIDComponent>()) {
				return;
			}



			data << YAML::BeginMap;
			data << YAML::Key << "Entity";
			data << YAML::Value << e.GetComponent<UUIDComponent>().UUID;


			//TODO: Support all components
			if (e.HasComponent<NameComponent>()) {

				data << YAML::Key << "NameComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "Name" << YAML::Value << e.GetComponent<NameComponent>().Name;
				data << YAML::EndMap;



				
			}

			if (e.HasComponent<TagComponent>()) {

				data << YAML::Key << "TagComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "Tag" << YAML::Value << e.GetComponent<TagComponent>().Tag;
				data << YAML::EndMap;

			}

			// Transform comp -- all entities that the user can create all have a transform
			data << YAML::Key << "TransformComponent";

			data << YAML::BeginMap;
			data << YAML::Key << "Transform" << YAML::Value << e.GetComponent<TransformComponent>().Transform;
			data << YAML::EndMap;

			


///////////////////////////////////////////////////// Rendering Components /////////////////////////////////////////////////////

			if (e.HasComponent<SpriteRendererComponent>()) {
				auto& src = e.GetComponent<SpriteRendererComponent>();
				data << YAML::Key << "SpriteRendererComponent";


				data << YAML::BeginMap;
				// Texture
				data << YAML::Key << "Texture";
				data << YAML::BeginMap;

				if (src.Texture)
					data << YAML::Key << "AssetHandle" << YAML::Value << (UUID)AssetManager::GetMetadata(src.Texture->Handle).Handle;
				else
					data << YAML::Key << "AssetHandle" << (AssetHandle)0;

				data << YAML::EndMap;


				// Sprite rect
				data << YAML::Key << "Rect";
				data << YAML::BeginMap;

				data << YAML::Key << "Position" << YAML::Value << src.Rect.Position;
				data << YAML::Key << "Scale" << YAML::Value << src.Rect.Scale;
				

				data << YAML::EndMap;
				data << YAML::Key << "TintColor" << YAML::Value << src.TintColor;
				data << YAML::EndMap;

			}



			if (e.HasComponent<CameraComponent>()) {

				data << YAML::Key << "CameraComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "ClearColor" << YAML::Value << e.GetComponent<CameraComponent>().ClearColor;
				data << YAML::Key << "OrthoSize" << YAML::Value << e.GetComponent<CameraComponent>().Camera.GetOrthographicSize();
				data << YAML::Key << "Primary" << YAML::Value << e.GetComponent<CameraComponent>().PrimaryCamera;
				data << YAML::EndMap;

			}



///////////////////////////////////////////////////// Audio Components /////////////////////////////////////////////////////

			if (e.HasComponent<AudioSourceComponent>()) {
				data << YAML::Key << "AudioSourceComponent";
				auto& asc = e.GetComponent<AudioSourceComponent>();

				if (asc.Source) {
					data << YAML::BeginMap;
					data << YAML::Key << "AssetHandle" << YAML::Value << (UUID)AssetManager::GetMetadata(asc.Source->Handle).Handle;
					data << YAML::Key << "Gain" << YAML::Value << asc.Source->GetGain();
					data << YAML::Key << "ShouldLoop" << YAML::Value << asc.Source->IsLooping();
					data << YAML::EndMap;
				}

			}
			if (e.HasComponent<AudioListenerComponent>()) {
				data << YAML::Key << "AudioListenerComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "Spatial" << YAML::Value << e.GetComponent<AudioListenerComponent>().Listener.IsSpatial();
				data << YAML::EndMap;

			}



///////////////////////////////////////////////////// Physics Components /////////////////////////////////////////////////////
			if (e.HasComponent<RigidBodyComponent>()) {
				data << YAML::Key << "RigidBodyComponent";
				auto& rbc = e.GetComponent<RigidBodyComponent>();

				data << YAML::BeginMap;
				data << YAML::Key << "GravityScale" << YAML::Value << rbc.Definition.GravityScale;
				data << YAML::Key << "SleepingState" << YAML::Value << (int)rbc.Definition.State;
				data << YAML::Key << "CollisionDetectionMode" << YAML::Value << (int)rbc.Definition.DetectionMode;
				data << YAML::Key << "BodyType" << YAML::Value << (int)rbc.Definition.Type;
				data << YAML::Key << "FixedRotation" << YAML::Value << rbc.Definition.FixedRotation;
				data << YAML::EndMap;

			}

			if (e.HasComponent<BoxColliderComponent>()) {
				data << YAML::Key << "BoxColliderComponent";
				auto& bcc = e.GetComponent<BoxColliderComponent>();

				data << YAML::BeginMap;
				data << YAML::Key << "Center" << YAML::Value << bcc.Center;
				data << YAML::Key << "Size" << YAML::Value << bcc.Size;
				data << YAML::Key << "IsTrigger" << YAML::Value << bcc.IsTrigger;
				data << YAML::EndMap;

			}




///////////////////////////////////////////////////// Scripting Components /////////////////////////////////////////////////////
			if (e.HasComponent<ScriptingBehaviourComponent>()) {
				data << YAML::Key << "ScriptingBehaviourComponent";
				auto& sbc = e.GetComponent<ScriptingBehaviourComponent>();

				data << YAML::BeginMap;
				data << YAML::Key << "ClassName" << sbc.Behaviour.ClassName;
				auto& scriptFields = sbc.Behaviour.AllFields;
				data << YAML::Key << "Fields" << YAML::Value;
				data << YAML::BeginSeq;

				for (unsigned int i = 0; i < scriptFields.size(); i++) {
					data << YAML::BeginMap;
					data << YAML::Key << "FieldName" << YAML::Value << scriptFields[i].VariableName;
					data << YAML::Key << "FieldType" << YAML::Value << (int)scriptFields[i].Type;

					switch (scriptFields[i].Type)
					{
						case PropertyType::Bool:
							data << YAML::Key << "FieldValue" << YAML::Value << (bool)scriptFields[i].Value;
							break;
						case PropertyType::Float:
							data << YAML::Key << "FieldValue" << YAML::Value << *(float*)&scriptFields[i].Value;
							break;
						case PropertyType::Int: {
							data << YAML::Key << "FieldValue" << YAML::Value << (int)scriptFields[i].Value;
							break;
						}
						case PropertyType::Vec2:
							break;
						case PropertyType::Vec3:
							break;
						case PropertyType::Vec4:
							break;
						default:
							break;

					}
					data << YAML::EndMap;

				}
				data << YAML::EndSeq;
				data << YAML::EndMap;


///////////////////////////////////////////////////// -------- END -------- /////////////////////////////////////////////////////

			}


			data << YAML::EndMap;

		});

		data << YAML::EndSeq;
		data << YAML::EndMap;


		std::ofstream file(filepath);
		file << data.c_str();

		file.close();
	}

 	Ref<Scene> SceneSerialization::Deserialize(const std::string& filepath) {
		std::ifstream file(filepath);
		YAML::Node data = YAML::Load(file);

		if (!data["Scene"]) {
			PX_CORE_ERROR("Could not load scene!\n");
			return nullptr;
		}
		std::string name = data["Scene"].as<std::string>();

		Ref<Scene> resultScene = CreateRef<Scene>(name);


		YAML::Node entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				unsigned long long uuid = entity["Entity"].as<unsigned long long>();
				std::string name, tag = "No tag";
				if (auto nameComp = entity["NameComponent"]) {
					name = nameComp["Name"].as<std::string>();
				}

				if (auto tagComp = entity["TagComponent"]) {
					tag = tagComp["Tag"].as<std::string>();
				}


				Entity e = resultScene->CreateEntityWithUUID(uuid, name);
				e.GetComponent<TagComponent>().Tag = tag;

				// All created entities have a transform
				auto transformComp = entity["TransformComponent"];
				e.GetComponent<TransformComponent>().Transform = transformComp["Transform"].as<glm::mat4>();

///////////////////////////////////////////////////// Rendering Components /////////////////////////////////////////////////////


				if (auto spriteRendererComp = entity["SpriteRendererComponent"]) {
					auto& comp = e.AddComponent<SpriteRendererComponent>();
					auto texture = spriteRendererComp["Texture"];
					if (texture["AssetHandle"].as<uint64_t>() != 0) {
						Ref<Texture> tex;


						AssetHandle handle = texture["AssetHandle"].as<uint64_t>();

						if (AssetManager::IsAssetHandleValid(handle)) {
							tex = AssetManager::GetAsset<Texture>(handle);
						}

						//TODO: Texture manager needs a redo
// 						if (auto id = TextureManager::IsTextureValid(filepath)) {
// 							tex = TextureManager::GetTexture(id);
// 
// 						}
// 						else {
// 							tex = Texture::Create(filepath);
// 							TextureManager::DirectAdd(tex);
// 						}
// 						


						comp.Texture = tex;
						auto r = spriteRendererComp["Rect"];
						comp.Rect = Rect(r["Position"].as<glm::uvec2>(), r["Scale"].as<glm::uvec2>());
					}
					comp.TintColor = spriteRendererComp["TintColor"].as<glm::vec4>();

				}


				if (auto cameraComp = entity["CameraComponent"]) {
					auto& comp = e.AddComponent<CameraComponent>();
					comp.Camera = OrthographicCamera();
					comp.Camera.SetOrthographicSize(cameraComp["OrthoSize"].as<float>());

					comp.PrimaryCamera = cameraComp["Primary"].as<bool>();

				}

///////////////////////////////////////////////////// Audio Components /////////////////////////////////////////////////////


				if (auto ascYaml = entity["AudioSourceComponent"]) {
					auto& comp = e.AddComponent<AudioSourceComponent>();
					if (ascYaml["AssetHandle"]) {
						comp.FilePath = AssetManager::GetFilePathString(AssetManager::GetMetadata(ascYaml["AssetHandle"].as<uint64_t>()));
						bool loop = ascYaml["ShouldLoop"].as<bool>();
						float gain = ascYaml["Gain"].as<float>();

						comp.Source = Audio::CreateAudioSource(comp.FilePath, loop, false);
						if(comp.Source == nullptr)
							comp.FilePath = "";
						else
							comp.Source->SetGain(gain);
					}
				}

				if (auto audioListenerComp = entity["AudioListenerComponent"]) {
					auto& comp = e.AddComponent<AudioListenerComponent>();
					comp.Listener = AudioListener();
					comp.Listener.SetSpatial(audioListenerComp["Spatial"].as<bool>());
				}


///////////////////////////////////////////////////// Physics Components /////////////////////////////////////////////////////


				if (auto rbYaml = entity["RigidBodyComponent"]) {
					auto& comp = e.AddComponent<RigidBodyComponent>();

					bool rotate = rbYaml["FixedRotation"].as<bool>();
					float gravScale = rbYaml["GravityScale"].as<float>();
					SleepingState state = (SleepingState) rbYaml["SleepingState"].as<int>();
					CollisionDetectionMode mode = (CollisionDetectionMode) rbYaml["CollisionDetectionMode"].as<int>();
					BodyType type = (BodyType) rbYaml["BodyType"].as<int>();

					comp.Definition.GravityScale = gravScale;
					comp.Definition.State = state;
					comp.Definition.DetectionMode = mode;
					comp.Definition.Type = type;
					comp.Definition.FixedRotation = rotate;

			
				}

				if (auto bccYaml = entity["BoxColliderComponent"]) {
					auto& comp = e.AddComponent<BoxColliderComponent>();

					comp.Center = bccYaml["Center"].as<glm::vec2>();
					comp.Size = bccYaml["Size"].as<glm::vec2>();
					comp.IsTrigger = bccYaml["IsTrigger"].as<bool>();
				}


///////////////////////////////////////////////////// Scripting Components /////////////////////////////////////////////////////

				if (auto sbcYaml = entity["ScriptingBehaviourComponent"]) {
					auto& sbc = e.AddComponent<ScriptingBehaviourComponent>();

					sbc.Behaviour = ScriptBehaviour();
					sbc.Behaviour.ClassName = sbcYaml["ClassName"].as<std::string>();

					ScriptingMaster::CreateEntityScript(e, sbc.Behaviour);
					auto& allFields = sbc.Behaviour.AllFields;
					const auto& fieldData = sbcYaml["Fields"];


					int iterator = 0;
					for (auto field : fieldData) {
						
						switch ((PropertyType)field["FieldType"].as<int>()) {
							case PropertyType::Bool: {
								bool temp = field["FieldValue"].as<bool>();
								allFields[iterator].Value = std::move(&temp);
								break;
							}
							case PropertyType::Float: {
								float temp = field["FieldValue"].as<float>();
								allFields[iterator].Value = std::move(&temp);
								break;
							}
							case PropertyType::Int: {
								int temp = field["FieldValue"].as<int>();
								allFields[iterator].Value = std::move(&temp);
								break;
							}
							case PropertyType::Vec2:
								break;
							case PropertyType::Vec3:
								break;
							case PropertyType::Vec4:
								break;
							default:
								break;

						}
						sbc.Behaviour.SetPropertyValue(allFields[iterator].Value, allFields[iterator].Field);
						iterator++;

					}
					
				}


			}
		}

		file.close();
		return resultScene;
 	}

}