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
		Ref<Material> result = MaterialSerialization::Deserialize(filepath);

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
		out << YAML::Key << "Material" << YAML::BeginSeq;

		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << material->GetName();
		out << YAML::Key << "Shader" << YAML::Value << (uint64_t)material->GetShader()->Handle;

		out << YAML::Key << "Table" << YAML::BeginSeq;
		out << YAML::BeginMap;
		for (auto& table : material->GetUniformTable())
		{
			out << YAML::Key << "Binding" << YAML::Value << table.ReflectedUniformBuffer.Binding;
			out << YAML::Key << "StructSize" << YAML::Value << table.ReflectedUniformBuffer.StructSize;


			out << YAML::Key << "Members" << YAML::BeginSeq;
			for (auto& member : table.ReflectedUniformBuffer.Members)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Name" << YAML::Value << member.Name;
				out << YAML::Key << "Type" << YAML::Value << (int)member.Type;
				out << YAML::Key << "Size" << YAML::Value << member.Size;
				out << YAML::Key << "Offset" << YAML::Value << member.Offset;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
		}
		out << YAML::EndSeq;

		out << YAML::EndMap;
		out << YAML::EndMap;

		out << YAML::EndSeq;
		out << YAML::EndMap;


		std::ofstream file(path);
		file << out.c_str();

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
		if (!node) {
			PX_CORE_ERROR("Could not deduce the header of the material!\n");
			return result;
		}

		for (auto n : node) {
			std::string name = n["Name"].as<std::string>();
			uint64_t handle = n["Shader"].as<uint64_t>();

			Ref<Shader> shader = AssetManager::GetAsset<Shader>(handle);
			result = CreateRef<Material>(shader, name, false);


			YAML::Node tableNode = n["Table"];
			for (auto table : tableNode)
			{
				ShaderUniform entry;
				entry.Binding = table["Binding"].as<int>();
				entry.StructSize = table["StructSize"].as<int>();
				YAML::Node memberNode = table["Members"];

				for (auto member : memberNode)
				{
					ShaderMember e2;
					
					e2.Name = member["Name"].as<std::string>();
					e2.Offset = member["Offset"].as<int>();
					e2.Type = (ShaderBaseType)member["Type"].as<int>();
					e2.Size = member["Size"].as<int>();
					entry.Members.push_back(e2);
				}

				result->AddUniformBufferEntry(entry);
			}

		}

		return result;


	}


}