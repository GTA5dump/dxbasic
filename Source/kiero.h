#ifndef __KIERO_H__
#define __KIERO_H__
#include <stdint.h>

typedef uint64_t uint150_t;

namespace kiero
{
	struct Status
	{
		enum Enum
		{
			UnknownError = -1,
			NotSupportedError = -2,
			ModuleNotFoundError = -3,
			AlreadyInitializedError = -4,
			NotInitializedError = -5,
			Success = 0,
		};
	};

	struct RenderType
	{
		enum Enum
		{
			None,
			D3D11,
			Auto
		};
	};

	Status::Enum init(RenderType::Enum renderType);

	Status::Enum bind(uint16_t index, void** original, void* function);
	void unbind(uint16_t index, bool unbindall = false);
}

#endif // __KIERO_H__