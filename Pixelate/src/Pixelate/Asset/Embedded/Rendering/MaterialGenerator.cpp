#include "PXpch.h"
#include "MaterialGenerator.h"


namespace Pixelate
{

	static const char* buffer = R"(Material:
  Name: Material
  Shader: 7968149936872318425
ReflectedUniformBuffers:
  - Binding: 0
    StructSize: 64
    Members:
      - Name: u_ViewProjection
        Type: 6
        Size: 64
        Offset: 0
ReflectedSamplerImages:
  - Texture:
      AssetHandle: 0
    Rect:
      PositionX: 0
      PositionY: 0
      ScaleX: 1024
      ScaleY: 1024
)";

	static const char* DefaultFilename = "Material.pxMaterial";




	void MaterialGenerator::GenerateToDisk(std::filesystem::path path)
	{
		path /= DefaultFilename;
		std::ofstream file(path);
		file << buffer;
		file.close();
	}
}