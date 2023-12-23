#pragma once

#include <stdint.h>

class UUID
{
public:
	/// <summary>
	/// The UUID constructor that generates a new random UUID
	/// </summary>
	UUID();
	/// <summary>
	/// The UUID constructor that sets the UUID to a specific value
	/// </summary>
	/// <param name="uuid">The UUID value</param>
	UUID(uint64_t uuid);
	/// <summary>
	/// The UUID constructor that copies the value from another UUID
	/// </summary>
	/// <param name="">The other UUID</param>
	UUID(const UUID&) = default;

	operator uint64_t() const { return m_UUID; }
private:
	uint64_t m_UUID;
};

typedef UUID NodeID;
typedef UUID PinID;
typedef UUID LinkID;

// Extend the STD library hash to return the value of UUID as its hash

namespace std
{
	template<typename T> struct hash;

	template<>
	struct hash<UUID>
	{
		size_t operator()(const UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}