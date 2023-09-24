/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2023 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

// Thread-safe, automatically clearing arena memory resource.

#if !defined(BSTONE_TS_AUTO_ARENA_MEMORY_RESOURCE_INCLUDED)
#define BSTONE_TS_AUTO_ARENA_MEMORY_RESOURCE_INCLUDED

#include <mutex>

#include "bstone_auto_arena_memory_resource.h"

namespace bstone {

class TsAutoArenaMemoryResource : public MemoryResource
{
public:
	TsAutoArenaMemoryResource() = default;
	~TsAutoArenaMemoryResource() override = default;

	void reserve(IntP capacity, MemoryResource& memory_resource);

private:
	void* do_allocate(IntP size) override;
	void do_deallocate(void* ptr) override;

private:
	using Mutex = std::mutex;
	using MutexLock = std::unique_lock<Mutex>;

private:
	Mutex mutex_{};
	AutoArenaMemoryResource arena_{};
};

} // namespace bstone

#endif // BSTONE_TS_AUTO_ARENA_MEMORY_RESOURCE_INCLUDED
