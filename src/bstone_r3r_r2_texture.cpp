/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2023 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

// 3D Renderer: 2D Texture

#include "bstone_exception.h"
#include "bstone_r3r_r2_texture.h"

namespace bstone {

void R3rR2Texture::update(const R3rR2TextureUpdateParam& param)
BSTONE_BEGIN_FUNC_TRY
	do_update(param);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void R3rR2Texture::generate_mipmaps()
BSTONE_BEGIN_FUNC_TRY
	do_generate_mipmaps();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

} // namespace bstone
