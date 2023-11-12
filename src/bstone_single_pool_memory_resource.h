/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2023 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

// Memory pool for single object.

#if !defined(BSTONE_SINGLE_POOL_MEMORY_RESOURCE_INCLUDED)
#define BSTONE_SINGLE_POOL_MEMORY_RESOURCE_INCLUDED

#include "bstone_cxx.h"
#include "bstone_assert.h"
#include "bstone_exception.h"
#include "bstone_memory_resource.h"

namespace bstone {

template<typename T>
class SinglePoolMemoryResource final : public MemoryResource
{
public:
	static constexpr auto object_size = static_cast<std::intptr_t>(sizeof(T));

public:
	SinglePoolMemoryResource() noexcept = default;
	SinglePoolMemoryResource(const SinglePoolMemoryResource&) = delete;
	SinglePoolMemoryResource(SinglePoolMemoryResource&&) noexcept = delete;
	~SinglePoolMemoryResource() override;

private:
	BSTONE_CXX_NODISCARD void* do_allocate(std::intptr_t size) override;
	void do_deallocate(void* ptr) noexcept override;

private:
	using Storage = unsigned char[object_size];

private:
	Storage storage_{};
	bool is_allocated_{};
};

// --------------------------------------------------------------------------

template<typename T>
SinglePoolMemoryResource<T>::~SinglePoolMemoryResource()
{
	BSTONE_ASSERT(!is_allocated_);
}

template<typename T>
BSTONE_CXX_NODISCARD void* SinglePoolMemoryResource<T>::do_allocate(std::intptr_t size)
{
	if (size != object_size)
	{
		BSTONE_THROW_STATIC_SOURCE("Size mismatch.");
	}

	if (is_allocated_)
	{
		BSTONE_THROW_STATIC_SOURCE("Out of memory.");
	}

	is_allocated_ = true;
	return storage_;
}

template<typename T>
void SinglePoolMemoryResource<T>::do_deallocate(void* ptr) noexcept
{
	if (ptr == nullptr)
	{
		return;
	}

	BSTONE_ASSERT(ptr == storage_);
	is_allocated_ = false;
}

} // namespace bstone

#endif // BSTONE_SINGLE_POOL_MEMORY_RESOURCE_INCLUDED
