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


#ifndef BSTONE_IMAGE_ENCODER_INCLUDED
#define BSTONE_IMAGE_ENCODER_INCLUDED


#include <cstdint>

#include <memory>


namespace bstone
{


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

class ImageEncoder
{
public:
	ImageEncoder() noexcept = default;

	virtual ~ImageEncoder() = default;


	virtual void encode_24(
		const std::uint8_t* src_buffer,
		int src_width,
		int src_height,
		std::uint8_t* dst_buffer,
		int dst_buffer_max_size,
		int& dst_size) = 0;
}; // ImageEncoder

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

enum class ImageEncoderType
{
	none,
	png,
}; // ImageEncoderType

using ImageEncodeUPtr = std::unique_ptr<ImageEncoder>;

ImageEncodeUPtr make_image_encoder(
	ImageEncoderType image_encoder_type);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


} // bstone


#endif // !BSTONE_IMAGE_ENCODER_INCLUDED