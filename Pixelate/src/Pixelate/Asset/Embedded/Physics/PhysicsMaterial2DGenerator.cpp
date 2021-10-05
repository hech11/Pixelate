#include "PXpch.h"
#include "PhysicsMaterial2DGenerator.h"

#include "Pixelate/Physics/PhysicsMaterial2D.h"

namespace Pixelate {

	static const char* DefaultFilename = "Physics Material2D.pxPhysicsMaterial2D";



	void PhysicsMaterial2DGenerator::GenerateToDisk(std::filesystem::path path) {

		// TODO: If file already exists
		path /= DefaultFilename;
		PhysicsMaterial2DSerialization::Serialize(path, CreateRef<PhysicsMaterial2D>());

	}

}