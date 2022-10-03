#include "PXpch.h"
#include "MaterialManager.h"

#include "Pixelate/Asset/AssetManager.h"

#include <yaml-cpp/yaml.h>


namespace Pixelate
{
	std::vector<Pixelate::Ref<Pixelate::Material>> MaterialManager::s_Materials;



	/////// MaterialManager ///////
	Ref<Material> MaterialManager::Load(const std::string& filepath)
	{
		Ref<Material> result = AssetManager::GetAsset<Material>(filepath);
		for (int i = 0; i < s_Materials.size(); i++)
		{
			if (s_Materials[i] == result)
				return s_Materials[i];
		}

		s_Materials.push_back(result);
		return result;
	}

	Ref<Material> MaterialManager::LoadExternalResource(const std::string& filepath)
	{
		std::filesystem::path p = std::filesystem::relative(filepath, "resources");

		Ref<Material> result = AssetManager::GetAsset<Material>(AssetManager::GetMetadata(p.c_str()).Handle, true);

		for (int i = 0; i < s_Materials.size(); i++)
		{
			if (s_Materials[i] == result)
				return s_Materials[i];
		}

		s_Materials.push_back(result);

		return result;
	}



	/////// MaterialSerialization ///////

	void MaterialSerialization::Serialize(const std::filesystem::path& path, const Ref<Material>& material)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Material";

		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << material->GetName();
		out << YAML::Key << "Shader" << YAML::Value << (uint64_t)material->GetShader()->Handle;
		out << YAML::EndMap;


		out << YAML::Key << "ReflectedUniformBuffers";
		out << YAML::Value << YAML::BeginSeq;

		for (auto& table : material->GetUniformTable())
		{
			for (auto&& [binding, ubs] : table->ReflectedUniformBuffers)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Binding" << YAML::Value << binding;
				out << YAML::Key << "StructSize" << YAML::Value << ubs.StructSize;

				out << YAML::Key << "Members" << YAML::BeginSeq;

				for (auto& member : ubs.Members)
				{
					out << YAML::BeginMap;
					out << YAML::Key << "Name" << YAML::Value << member.Name;


					switch (member.Type)
					{
					case ShaderBaseType::Int32:
					{
						int value = material->Get<int>(member.Name, binding);
						out << YAML::Key << "Value" << YAML::Value << value;
						break;
					}

					case ShaderBaseType::Float:
					{
						float value = material->Get<float>(member.Name, binding);
						out << YAML::Key << "Value" << YAML::Value << value;
						break;
					}
					}

					out << YAML::Key << "Type" << YAML::Value << (int)member.Type;
					out << YAML::Key << "Size" << YAML::Value << member.Size;
					out << YAML::Key << "Offset" << YAML::Value << member.Offset;

					out << YAML::EndMap;

				}

				out << YAML::EndSeq;
				out << YAML::EndMap;

			}
		}



		out << YAML::EndSeq;



		if (material->GetReflectedSampledImages().size())
		{
			out << YAML::Key << "ReflectedSamplerImages";
			out << YAML::Value << YAML::BeginSeq;

			for (auto&& [binding, image] : material->GetReflectedSampledImages())
			{
				out << YAML::BeginMap;


				out << YAML::Key << "Texture";
				out << YAML::BeginMap;
				out << YAML::Key << "AssetHandle" << YAML::Value << (UUID)AssetManager::GetMetadata(image.Texture->Handle).Handle;
				out << YAML::EndMap;


				out << YAML::Key << "Rect";
				out << YAML::BeginMap;

				out << YAML::Key << "PositionX" << YAML::Value << image.Rect.Position.x;
				out << YAML::Key << "PositionY" << YAML::Value << image.Rect.Position.y;
				out << YAML::Key << "ScaleX" << YAML::Value << image.Rect.Scale.x;
				out << YAML::Key << "ScaleY" << YAML::Value << image.Rect.Scale.y;
				out << YAML::EndMap;

				out << YAML::EndMap;
			}

			out << YAML::EndSeq;
		}


		out << YAML::EndMap;


		std::ofstream file(path);
		file << out.c_str();

		PX_CORE_MSG("%s\n", out.c_str());

		file.close();
	}

	Ref<Material> MaterialSerialization::Deserialize(const std::filesystem::path& path)
	{
		Ref<Material> result = nullptr;
		if (!FileSystem::Exists(path)) {
			return result;
		}

		std::ifstream stream(path);

		YAML::Node data = YAML::Load(stream);

		YAML::Node node = data["Material"];
		YAML::Node tableNode = data["ReflectedUniformBuffers"];
		YAML::Node sampledImageNodes = data["ReflectedSamplerImages"];

		if (!node) {
			PX_CORE_ERROR("Could not deduce the header of the material!\n");
			return result;
		}


		std::string name = node["Name"].as<std::string>();
		uint64_t handle = node["Shader"].as<uint64_t>();

		Ref<Shader> shader;
		if (AssetManager::IsResource(handle))
		{
			shader = AssetManager::GetAsset<Shader>(handle, true);
		}
		else 
		{
			shader = AssetManager::GetAsset<Shader>(handle, false);
		}


		if (!shader)
		{
			shader = Shader::Create("resources/shaders/ErrorShader.pxShader");
		}
		result = CreateRef<Material>(shader, name, false);




		for (auto reflectedBuffer : tableNode)
		{
			ShaderUniform entry;
			entry.Binding = reflectedBuffer["Binding"].as<int>();
			entry.StructSize = reflectedBuffer["StructSize"].as<int>();
			YAML::Node memberNode = reflectedBuffer["Members"];

			result->AddUniformBufferEntry(entry);
			auto& uniformTable = result->GetUniformTable()[entry.Binding];

			for (auto member : memberNode)
			{
				ShaderMember e2;

				e2.Name = member["Name"].as<std::string>();
				e2.Offset = member["Offset"].as<int>();
				e2.Type = (ShaderBaseType)member["Type"].as<int>();
				e2.Size = member["Size"].as<int>();


				auto value = member["Value"];
				if (value)
				{
					switch (e2.Type)
					{
					case ShaderBaseType::Float:
					{
						float v = value.as<float>();
						memcpy((uint8_t*)uniformTable->Data.data() + e2.Offset, &v, e2.Size);
						break;
					}
					case ShaderBaseType::Int32:
					{
						int v = value.as<int32_t>();
						memcpy((uint8_t*)uniformTable->Data.data() + e2.Offset, &v, e2.Size);
						break;
					}
					}

				}
				uniformTable->ReflectedUniformBuffers[entry.Binding].Members.push_back(e2);

			}
		}


		for (auto reflectedSample : sampledImageNodes)
		{

			YAML::Node textureNode = reflectedSample["Texture"];
			uint64_t assetHandle = textureNode["AssetHandle"].as<uint64_t>();
			Ref<Texture> texture = AssetManager::GetAsset<Texture>((AssetHandle)assetHandle);


			YAML::Node rectNode = reflectedSample["Rect"];

			uint32_t posx = rectNode["PositionX"].as<uint32_t>();
			uint32_t posy = rectNode["PositionY"].as<uint32_t>();
			uint32_t scalex = rectNode["ScaleX"].as<uint32_t>();
			uint32_t scaley = rectNode["ScaleY"].as<uint32_t>();

			SampledImage2DContainer cont = { texture, {{posx, posy}, {scalex, scaley}} };

			result->AddSampledImageEntry(0, cont);
		}



		return result;


	}


}
