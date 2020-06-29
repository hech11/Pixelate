#include "RGFpch.h"
#include "ParticleSystem.h"

#include <GLM/glm/gtc/constants.hpp>
#define GLM_ENABLE_EXERIMENTAL
#include <GLM/glm/gtx/compatibility.hpp>

#include "RetroGF/Utility/Random.h"


#include "Renderer2D.h"
#include "RetroGF/Debug/Instrumentor.h"

namespace RGF {

	
#if AOS

	ParticleSystem::ParticleSystem(const SystemInitData& initProps) 
		: InitData(initProps)
	{
		RGF_PROFILE_FUNCTION();
		m_PoolIndex = 0;
		m_ParticlePool.resize(InitData.PoolLimit);
	}

	ParticleSystem::~ParticleSystem() {

	}

	void ParticleSystem::OnUpdate(float timeStep) {
		RGF_PROFILE_FUNCTION();
		for (auto& particle : m_ParticlePool) {
			if(particle.Active < 1)
				continue;

			if (particle.LifeRemaining <= 0.0f) {
				particle.Active = 0;
				continue;
			}

			particle.LifeRemaining -= timeStep;
			particle.Position += particle.Velocity * timeStep;

			// TODO: Add rotation support soon
			particle.Rotation += 0.01f*timeStep;
		}
	}
	void ParticleSystem::OnRender() {
		RGF_PROFILE_FUNCTION();
		for (auto& particle : m_ParticlePool) {
			if(particle.Active < 1)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			Renderer2D::DrawSprite({ particle.Position.x,particle.Position.y, 1.0f }, particle.Rotation, {size, size, 1.0f}, glm::lerp(particle.ColorEnd, particle.ColorBegin, life));
		}
	}


	void ParticleSystem::Emit(const ParticleProperties& props) {
		RGF_PROFILE_FUNCTION();
		Particle& particle = m_ParticlePool[m_PoolIndex];


		particle.Active = 1;
		particle.Position = props.Position;
		particle.Rotation = 0.0f;


		particle.Velocity = props.Velocity;
		particle.Velocity.x += RGF::Random::GetRandomInRange(props.VelocityVariation1.x, props.VelocityVariation2.x);
		particle.Velocity.y += RGF::Random::GetRandomInRange(props.VelocityVariation1.y, props.VelocityVariation2.y);



		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		particle.LifeTime = props.LifeTime;
		particle.LifeRemaining = props.LifeTime;


		particle.SizeBegin += RGF::Random::GetRandomInRange(RGF::Random::GetRandomInRange(0.0f, props.SizeVariation), props.SizeVariation);
		particle.SizeEnd = props.SizeEnd;



		m_PoolIndex = ++m_PoolIndex % m_ParticlePool.size();

	}

	#else

	ParticleSystem::ParticleSystem(const SystemInitData& initProps)
		: InitData(initProps)
	{
		RGF_PROFILE_FUNCTION();
		m_PoolIndex = 0;



	}

	ParticleSystem::~ParticleSystem() {
		RGF_PROFILE_FUNCTION();

	}

	void ParticleSystem::OnUpdate(float timeStep) {
		RGF_PROFILE_FUNCTION();
		for (int i = 0; i < InitData.PoolLimit; i++) {
			if (!m_ParticlePool.Active[i])
				continue;

			if (m_ParticlePool.LifeRemaining[i] <= 0.0f) {
				m_ParticlePool.Active[i] = 0;
				continue;
			}

			m_ParticlePool.LifeRemaining[i] -= timeStep;
			m_ParticlePool.Position[i] += m_ParticlePool.Velocity[i] * timeStep;

			// TODO: Add rotation support soon
		}
	}
	void ParticleSystem::OnRender() {
		RGF_PROFILE_FUNCTION();
		for (int i = 0; i < InitData.PoolLimit; i++) {
			if (!m_ParticlePool.Active[i])
				continue;

			float life = m_ParticlePool.LifeRemaining[i] / m_ParticlePool.LifeTime[i];
			glm::vec4 color = glm::lerp(m_ParticlePool.ColorEnd[i], m_ParticlePool.ColorBegin[i], life);
			float size = glm::lerp(m_ParticlePool.SizeEnd[i], m_ParticlePool.SizeBegin[i], life);

			Renderer2D::DrawSprite({ m_ParticlePool.Position[i].x,m_ParticlePool.Position[i].y, 1.0f }, { size, size, 1.0f }, color);
		}
	}


	void ParticleSystem::Emit(const ParticleProperties& props) {
		RGF_PROFILE_FUNCTION();
		Particle& particle = m_ParticlePool;


		particle.Active[m_PoolIndex] = 1;
		particle.LifeTime[m_PoolIndex] = props.LifeTime;
		particle.LifeRemaining[m_PoolIndex] = props.LifeTime;

		particle.Position[m_PoolIndex] = props.Position;
		particle.Rotation[m_PoolIndex] = 0.0f;


		particle.Velocity[m_PoolIndex] = props.Velocity;
		particle.Velocity[m_PoolIndex].x += RGF::Random::GetRandomInRange(props.VelocityVariation1.x, props.VelocityVariation2.x);
		particle.Velocity[m_PoolIndex].y += RGF::Random::GetRandomInRange(props.VelocityVariation1.y, props.VelocityVariation2.y);



		particle.ColorBegin[m_PoolIndex] = props.ColorBegin;
		particle.ColorEnd[m_PoolIndex] = props.ColorEnd;



		particle.SizeBegin[m_PoolIndex] = props.SizeBegin + RGF::Random::GetRandomInRange(RGF::Random::GetRandomInRange(0.0f, props.SizeVariation), props.SizeVariation);
		particle.SizeEnd[m_PoolIndex] = props.SizeEnd;



		m_PoolIndex = ++m_PoolIndex % InitData.PoolLimit;

	}


#endif



}