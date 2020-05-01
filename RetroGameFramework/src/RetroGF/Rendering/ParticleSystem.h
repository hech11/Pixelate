#pragma once

#include <vector>
#include <GLM/glm/glm.hpp>

#define AOS 1

namespace RGF {

	struct ParticleProperties {
		glm::vec2 Position = { 0.0f, 0.0f };
		glm::vec2 Velocity = { 1.0f , 1.0f}, VelocityVariation1 = { 0.0f , 0.0f }, VelocityVariation2 = { 0.0f , 0.0f };
		glm::vec4 ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f }, ColorEnd = { 1.0f, 1.0f, 1.0f, 1.0f };
		float SizeBegin = 1.0f, SizeEnd = 1.0f, SizeVariation = 0.0f;
		float LifeTime = 1.0f;
	};

	// a pooled particle system
	class ParticleSystem {

		public :

			struct SystemInitData {
				unsigned int PoolLimit = 10000;
				unsigned int SpawnRate = 5;
			};

			ParticleSystem(const SystemInitData& initProps = SystemInitData());
			~ParticleSystem();

			// TODO: add timestep class
			void OnUpdate(float timeStep);
			void OnRender();

			void Emit(const ParticleProperties& props);

			SystemInitData InitData;

		private :

#if AOS

			struct Particle {
				glm::vec2 Position = { 0.0f, 0.0f };
				glm::vec2 Velocity = { 1.0f , 1.0f }, VelocityVariation1 = { 0.0f , 0.0f }, VelocityVariation2 = { 0.0f , 0.0f };
				glm::vec4 ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f }, ColorEnd = { 1.0f, 1.0f, 1.0f, 1.0f };
				float SizeBegin = 1.0f, SizeEnd = 1.0f, SizeVariation = 0.0f;

				float Rotation = 0.0f;

				float LifeTime = 1.0f;
				float LifeRemaining = 0.0f;

				unsigned char Active = false;
			};
			std::vector<Particle> m_ParticlePool;
#else
			struct Particle {
				std::vector<unsigned char> Active;
				std::vector<float> LifeRemaining;
				std::vector<float> LifeTime;

				std::vector<glm::vec2> Position;
				std::vector<glm::vec2> Velocity, VelocityVariation1, VelocityVariation2;
				std::vector<glm::vec4> ColorBegin, ColorEnd;
				std::vector<float> SizeBegin, SizeEnd, SizeVariation;

				std::vector<float> Rotation;


	};
			Particle m_ParticlePool;
#endif
			unsigned int m_PoolIndex;

	};

}