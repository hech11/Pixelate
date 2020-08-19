#pragma once

#include "Pixelate/Core/Random.h"
#include <xhash>


namespace Pixelate {

	class UUID {
		public :
			UUID()  : m_UUID(Random::GetURandom())
			{ 
			}

			UUID(unsigned long long uuid) : m_UUID(uuid)
			{
			}

			UUID(const UUID& uuid) : m_UUID(uuid.m_UUID)
			{
			}

			operator unsigned long long() { return m_UUID; }
			operator const unsigned long long() const { return m_UUID; }


		private :
			unsigned long long m_UUID;
	};

}

namespace std {

	// Used for std::unordered_map
	template <>
	struct hash<Pixelate::UUID> {

		std::size_t operator()(const Pixelate::UUID& id) const {
			return hash<unsigned long long>()((unsigned long long)id);
		}
	};

}