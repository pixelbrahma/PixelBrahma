#pragma once

#include <xhash>

namespace PixelBrahma
{
	// Universally unique identifier class
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		// Operator overloading to get the UUID
		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	// Template specialization of hash structure
	template<>
	struct hash<PixelBrahma::UUID>
	{
		// Operator overload to hash UUID to uint64_t
		std::size_t operator()(const PixelBrahma::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
