/*
BStone: A Source port of
Blake Stone: Aliens of Gold and Blake Stone: Planet Strike

Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2021 Boris I. Bendovsky (bibendovsky@hotmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/


//
// OpenGL texture object manager (implementation).
//


#include "bstone_detail_ren_3d_gl_texture_mgr.h"

#include "bstone_exception.h"
#include "bstone_ren_3d_tests.h"
#include "bstone_ren_3d_texture_2d.h"

#include "bstone_detail_ren_3d_gl_context.h"
#include "bstone_detail_ren_3d_gl_error.h"
#include "bstone_detail_ren_3d_gl_utils.h"
#include "bstone_detail_ren_3d_gl_texture_2d.h"


namespace bstone
{
namespace detail
{


// ==========================================================================
// Ren3dGlTextureMgrException
//

class Ren3dGlTextureMgrException :
	public Exception
{
public:
	explicit Ren3dGlTextureMgrException(
		const char* message)
		:
		Exception{"REN_3D_GL_TEXTURE_MGR", message}
	{
	}
}; // Ren3dGlTextureMgrException

//
// Ren3dGlTextureMgrException
// ==========================================================================


// ==========================================================================
// Ren3dGlTextureMgrImpl
//

class Ren3dGlTextureMgrImpl final :
	public Ren3dGlTextureMgr
{
public:
	Ren3dGlTextureMgrImpl(
		const Ren3dGlContextPtr context);

	~Ren3dGlTextureMgrImpl() override;


	Ren3dGlContextPtr get_context() const noexcept override;


	Ren3dTexture2dUPtr create(
		const Ren3dCreateTexture2dParam& param) override;


	void set(
		const Ren3dGlTexture2dPtr texture_2d) override;


private:
	const Ren3dGlContextPtr context_;


	[[noreturn]]
	static void fail(
		const char* message);

	[[noreturn]]
	static void fail_nested(
		const char* message);


	void unbind();
}; // Ren3dGlTextureMgrImpl

//
// Ren3dGlTextureMgrImpl
// ==========================================================================


// ==========================================================================
// Ren3dGlTextureMgrImpl
//

Ren3dGlTextureMgrImpl::Ren3dGlTextureMgrImpl(
	const Ren3dGlContextPtr context)
try
	:
	context_{context}
{
	if (!context_)
	{
		throw Ren3dGlTextureMgrException{"Null context."};
	}

	unbind();
}
catch (...)
{
	fail_nested(__func__);
}

Ren3dGlTextureMgrImpl::~Ren3dGlTextureMgrImpl() = default;

Ren3dGlContextPtr Ren3dGlTextureMgrImpl::get_context() const noexcept
{
	return context_;
}

Ren3dTexture2dUPtr Ren3dGlTextureMgrImpl::create(
	const Ren3dCreateTexture2dParam& param)
try
{
	return Ren3dGlTexture2dFactory::create(this, param);
}
catch (...)
{
	fail_nested(__func__);
}

void Ren3dGlTextureMgrImpl::set(
	const Ren3dGlTexture2dPtr texture_2d)
try
{
	if (texture_2d)
	{
		texture_2d->set();
	}
	else
	{
		unbind();
	}
}
catch (...)
{
	fail_nested(__func__);
}

[[noreturn]]
void Ren3dGlTextureMgrImpl::fail(
	const char* message)
{
	throw Ren3dGlTextureMgrException{message};
}

[[noreturn]]
void Ren3dGlTextureMgrImpl::fail_nested(
	const char* message)
{
	std::throw_with_nested(Ren3dGlTextureMgrException{message});
}

void Ren3dGlTextureMgrImpl::unbind()
try
{
	glBindTexture(GL_TEXTURE_2D, 0);
	Ren3dGlError::ensure_debug();
}
catch (...)
{
	fail_nested(__func__);
}

//
// Ren3dGlTextureMgrImpl
// ==========================================================================


// ==========================================================================
// Ren3dGlTextureMgrFactory
//

Ren3dGlTextureMgrUPtr Ren3dGlTextureMgrFactory::create(
	const Ren3dGlContextPtr context)
{
	return std::make_unique<Ren3dGlTextureMgrImpl>(context);
}

//
// Ren3dGlTextureMgrFactory
// ==========================================================================


} // detail
} // bstone
