/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2023 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

// OpenGL 3D Renderer: Sampler Manager

#include "bstone_exception.h"
#include "bstone_single_memory_pool.h"

#include "bstone_r3r_tests.h"

#include "bstone_gl_r3r_context.h"
#include "bstone_gl_r3r_r2_texture.h"
#include "bstone_gl_r3r_sampler.h"
#include "bstone_gl_r3r_sampler_mgr.h"

namespace bstone {

class GlR3rSamplerMgrImpl final : public GlR3rSamplerMgr
{
public:
	GlR3rSamplerMgrImpl(GlR3rContext& context);
	~GlR3rSamplerMgrImpl() override;

	static void* operator new(std::size_t size);
	static void operator delete(void* ptr);

	R3rSamplerUPtr create(const R3rSamplerInitParam& param) override;

	void notify_destroy(const R3rSampler* sampler) noexcept override;

	void set(R3rSampler* sampler) override;

	const R3rSamplerState& get_current_state() const noexcept override;

private:
	GlR3rContext& context_;
	const R3rDeviceFeatures& device_features_;

	GlR3rSamplerUPtr default_sampler_;
	GlR3rSampler* current_sampler_;

private:
	void initialize_default_sampler();

	void set();
};

// ==========================================================================

using GlR3rSamplerMgrImplPool = SingleMemoryPool<GlR3rSamplerMgrImpl>;
GlR3rSamplerMgrImplPool gl_r3r_sampler_mgr_impl_pool{};

// ==========================================================================

GlR3rSamplerMgrImpl::GlR3rSamplerMgrImpl(GlR3rContext& context)
try
	:
	context_{context},
	device_features_{context_.get_device_features()},
	default_sampler_{},
	current_sampler_{}
{
	initialize_default_sampler();

	current_sampler_ = default_sampler_.get();
}
BSTONE_STATIC_THROW_NESTED_FUNC

GlR3rSamplerMgrImpl::~GlR3rSamplerMgrImpl() = default;

void* GlR3rSamplerMgrImpl::operator new(std::size_t size)
try
{
	return gl_r3r_sampler_mgr_impl_pool.allocate(size);
}
BSTONE_STATIC_THROW_NESTED_FUNC

void GlR3rSamplerMgrImpl::operator delete(void* ptr)
try
{
	gl_r3r_sampler_mgr_impl_pool.deallocate(ptr);
}
BSTONE_STATIC_THROW_NESTED_FUNC

R3rSamplerUPtr GlR3rSamplerMgrImpl::create(const R3rSamplerInitParam& param)
{
	return make_gl_r3r_sampler(context_, param);
}

void GlR3rSamplerMgrImpl::notify_destroy(const R3rSampler* sampler) noexcept
{
	if (current_sampler_ == sampler)
	{
		if (device_features_.is_sampler_available)
		{
			current_sampler_ = nullptr;
		}
		else
		{
			current_sampler_ = default_sampler_.get();
		}
	}
}

void GlR3rSamplerMgrImpl::set(R3rSampler* sampler)
{
	const auto new_sampler = (sampler ? sampler : default_sampler_.get());

	if (current_sampler_ == new_sampler)
	{
		return;
	}

	current_sampler_ = static_cast<GlR3rSampler*>(new_sampler);
	set();
}

const R3rSamplerState& GlR3rSamplerMgrImpl::get_current_state() const noexcept
{
	return current_sampler_->get_state();
}

void GlR3rSamplerMgrImpl::initialize_default_sampler()
try
{
	auto param = R3rSamplerInitParam{};
	auto& state = param.state;

	state.mag_filter = R3rFilterType::nearest;
	state.min_filter = R3rFilterType::nearest;

	state.mipmap_mode = R3rMipmapMode::none;

	state.address_mode_u = R3rAddressMode::repeat;
	state.address_mode_v = R3rAddressMode::repeat;

	state.anisotropy = 0;

	default_sampler_ = make_gl_r3r_sampler(context_, param);
}
BSTONE_STATIC_THROW_NESTED_FUNC

void GlR3rSamplerMgrImpl::set()
try
{
	if (device_features_.is_sampler_available)
	{
		current_sampler_->set();
	}
}
BSTONE_STATIC_THROW_NESTED_FUNC

// ==========================================================================

GlR3rSamplerMgrUPtr make_gl_r3r_sampler_mgr(GlR3rContext& context)
{
	return std::make_unique<GlR3rSamplerMgrImpl>(context);
}

} // namespace bstone
