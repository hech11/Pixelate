#include "PXpch.h"
#include "MaterialGenerator.h"


namespace Pixelate
{

	static const char* buffer = R"(Material:
  - Name: DefaultMaterial
    Shader: 9493763084305907703
    Table:
      - Binding: 0
        StructSize: 64
        Members:
          - Name: u_ViewProjection
            Type: 6
            Size: 64
            Offset: 0
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