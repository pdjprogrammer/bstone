/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2023 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

#include "SDL_video.h"

#include "bstone_exception.h"
#include "bstone_single_memory_pool.h"

#include "bstone_sys_sdl_exception.h"
#include "bstone_sys_sdl_gl_shared_library.h"

namespace bstone {
namespace sys {

namespace {

class SdlGlSharedLibrary final : public GlSharedLibrary
{
public:
	SdlGlSharedLibrary(const char* path);
	~SdlGlSharedLibrary() override;

	static void* operator new(std::size_t size);
	static void operator delete(void* ptr);

private:
	void* do_find_symbol(const char* name) noexcept override;
};

} // namespace

// ==========================================================================

using SdlGlSharedLibraryPool = SingleMemoryPool<SdlGlSharedLibrary>;
SdlGlSharedLibraryPool sdl_gl_shared_library_pool{};

// ==========================================================================

SdlGlSharedLibrary::SdlGlSharedLibrary(const char* path)
try
{
	sdl_ensure_result(SDL_GL_LoadLibrary(path));
}
BSTONE_STATIC_THROW_NESTED_FUNC

SdlGlSharedLibrary::~SdlGlSharedLibrary()
{
	SDL_GL_UnloadLibrary();
}

void* SdlGlSharedLibrary::operator new(std::size_t size)
try
{
	return sdl_gl_shared_library_pool.allocate(size);
}
BSTONE_STATIC_THROW_NESTED_FUNC

void SdlGlSharedLibrary::operator delete(void* ptr)
try
{
	sdl_gl_shared_library_pool.deallocate(ptr);
}
BSTONE_STATIC_THROW_NESTED_FUNC

void* SdlGlSharedLibrary::do_find_symbol(const char* name) noexcept
{
	return SDL_GL_GetProcAddress(name);
}

// ==========================================================================

GlSharedLibraryUPtr make_sdl_gl_shared_library(const char* path)
{
	return std::make_unique<SdlGlSharedLibrary>(path);
}

} // namespace sys
} // namespace bstone
