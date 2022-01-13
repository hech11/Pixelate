#include "PXpch.h"
#include "Material.h"
#include <utility>


namespace Pixelate
{


	std::pair< Pixelate::Ref<Pixelate::UniformBuffer>, Pixelate::ShaderMember> Material::FindUniformStorage(const std::string& name, int binding)
	{

		if (m_UBOs.size())
		{

			for (auto&& [member, buffer] : m_UBOs)
			{
				if (member.Name == name)
					return std::make_pair(buffer[binding], member);
			}
		}

	}


}