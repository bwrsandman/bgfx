/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

#include <bx/math.h>

#include <type_traits>

namespace
{

struct PosColorTexCoord0Vertex
{
	float m_x;
	float m_y;
	float m_z;
	float m_u;
	float m_v;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	}

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;

void renderScreenSpaceQuad(uint8_t _view, bgfx::ProgramHandle _program, float _x, float _y, float _width, float _height)
{
	bgfx::TransientVertexBuffer tvb;
	bgfx::TransientIndexBuffer tib;

	if (bgfx::allocTransientBuffers(&tvb, PosColorTexCoord0Vertex::ms_layout, 4, &tib, 6) )
	{
		PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)tvb.data;

		float zz = 0.0f;

		const float minx = _x;
		const float maxx = _x + _width;
		const float miny = _y;
		const float maxy = _y + _height;

		float minu = 0.0f;
		float minv = 0.0f;
		float maxu = 1.0f;
		float maxv = 1.0f;

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		vertex[3].m_x = minx;
		vertex[3].m_y = maxy;
		vertex[3].m_z = zz;
		vertex[3].m_u = minu;
		vertex[3].m_v = maxv;

		uint16_t* indices = (uint16_t*)tib.data;

		indices[0] = 0;
		indices[1] = 2;
		indices[2] = 1;
		indices[3] = 0;
		indices[4] = 3;
		indices[5] = 2;

		bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);
		bgfx::setIndexBuffer(&tib);
		bgfx::setVertexBuffer(0, &tvb);
		bgfx::submit(_view, _program);
	}
}

const int TEXTURE_SIZE = 256;
const int HALF_TEXTURE_SIZE = TEXTURE_SIZE / 2;

#pragma pack(push, 1)
struct A8Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::A8;
    static constexpr const size_t aSz = 8;
    uint8_t a : aSz;
};
struct R8Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
    static constexpr const size_t rSz = 8;
    uint8_t r : rSz;
};
struct R8SPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8S;
    static constexpr const size_t rSz = 8;
    int8_t r : rSz;
};
struct R16Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R16;
    static constexpr const size_t rSz = 16;
    uint16_t r : rSz;
};
struct R16FPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R16F;
    static constexpr const size_t rSz = 16;
    uint16_t r : rSz;
};
struct R16SPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R16S;
    static constexpr const size_t rSz = 16;
    int16_t r : rSz;
};
struct R32FPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R32F;
    static constexpr const size_t rSz = 32;
    float r;
};
struct RG8Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RG8;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    uint8_t r : rSz;
    uint8_t g : gSz;
};
struct RGB8Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGB8;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    uint8_t r : rSz;
    uint8_t g : gSz;
    uint8_t b : bSz;
};
struct RGB8SPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGB8S;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    int8_t r : rSz;
    int8_t g : gSz;
    int8_t b : bSz;
};
struct RGB9E5FPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGB9E5F;
    static constexpr const size_t rSz = 9;
    static constexpr const size_t gSz = 9;
    static constexpr const size_t bSz = 9;
    static constexpr const size_t aSz = 5;
    uint32_t r : rSz;
    uint32_t g : gSz;
    uint32_t b : bSz;
    uint32_t a : aSz; // shared exponent
};
struct RGBA8Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    static constexpr const size_t aSz = 8;
    uint8_t r : rSz;
    uint8_t g : gSz;
    uint8_t b : bSz;
    uint8_t a : aSz;
};
struct BGRA8Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::BGRA8;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    static constexpr const size_t aSz = 8;
    uint8_t b : bSz;
    uint8_t g : gSz;
    uint8_t r : rSz;
    uint8_t a : aSz;
};
struct RGBA8SPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8S;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    static constexpr const size_t aSz = 8;
    int8_t r : rSz;
    int8_t g : gSz;
    int8_t b : bSz;
    int8_t a : aSz;
};
struct RGBA16Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA16;
    static constexpr const size_t rSz = 16;
    static constexpr const size_t gSz = 16;
    static constexpr const size_t bSz = 16;
    static constexpr const size_t aSz = 16;
    uint16_t r : rSz;
    uint16_t g : gSz;
    uint16_t b : bSz;
    uint16_t a : aSz;
};
struct RGBA16FPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA16F;
    static constexpr const size_t rSz = 16;
    static constexpr const size_t gSz = 16;
    static constexpr const size_t bSz = 16;
    static constexpr const size_t aSz = 16;
    uint16_t r : rSz;
    uint16_t g : gSz;
    uint16_t b : bSz;
    uint16_t a : aSz;
};
struct RGBA16SPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA16S;
    static constexpr const size_t rSz = 16;
    static constexpr const size_t gSz = 16;
    static constexpr const size_t bSz = 16;
    static constexpr const size_t aSz = 16;
    int16_t r : rSz;
    int16_t g : gSz;
    int16_t b : bSz;
    int16_t a : aSz;
};
struct RGBA32FPixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA32F;
    static constexpr const size_t rSz = 32;
    static constexpr const size_t gSz = 32;
    static constexpr const size_t bSz = 32;
    static constexpr const size_t aSz = 32;
    float r;
    float g;
    float b;
    float a;
};
struct B5G6R5Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::B5G6R5;
    static constexpr const size_t rSz = 5;
    static constexpr const size_t gSz = 6;
    static constexpr const size_t bSz = 5;
    uint16_t b : bSz;
    uint16_t g : gSz;
    uint16_t r : rSz;
};
struct BGRA4Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::BGRA4;
    static constexpr const size_t rSz = 4;
    static constexpr const size_t gSz = 4;
    static constexpr const size_t bSz = 4;
    static constexpr const size_t aSz = 4;
    uint16_t b : bSz;
    uint16_t g : gSz;
    uint16_t r : rSz;
    uint16_t a : aSz;
};
struct BGR5A1Pixel {
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::BGR5A1;
    static constexpr const size_t rSz = 5;
    static constexpr const size_t gSz = 5;
    static constexpr const size_t bSz = 5;
    static constexpr const size_t aSz = 1;
    uint16_t b : bSz;
    uint16_t g : gSz;
    uint16_t r : rSz;
    uint16_t a : aSz;
};
struct RGB10A2Pixel {
    static constexpr const char* str = "RGB10A2";
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGB10A2;
    static constexpr const size_t rSz = 10;
    static constexpr const size_t gSz = 10;
    static constexpr const size_t bSz = 10;
    static constexpr const size_t aSz = 2;
    uint32_t r : rSz;
    uint32_t g : gSz;
    uint32_t b : bSz;
    uint32_t a : aSz;
};
#pragma pack(pop)

/// http://www.rosettacode.org/wiki/Color_wheel#C.2B.2B
void hsvToRgb(float h, float s, float v, float& r, float& g, float& b) {
  if (h < 0.0f)
  {
    h += 2.0f * bx::kPi;
  }
  float hp = h/bx::toRad(60.0f);
  float c = s * v;
  float x = c * (1 - bx::abs(bx::mod(hp, 2.0f) - 1));
  float m = v - c;
  if (hp <= 1) {
    r = c;
    g = x;
  } else if (hp <= 2) {
    r = x;
    g = c;
  } else if (hp <= 3) {
    g = c;
    b = x;
  } else if (hp <= 4) {
    g = x;
    b = c;
  } else if (hp <= 5) {
    r = x;
    b = c;
  } else {
    r = c;
    b = x;
  }
  r += m;
  g += m;
  b += m;
}

// https://en.cppreference.com/w/cpp/experimental/is_detected
namespace detail {
    template <class Default, class AlwaysVoid,
            template<class...> class Op, class... Args>
    struct detector {
        using value_t = std::false_type;
        using type = Default;
    };

    template <class Default, template<class...> class Op, class... Args>
    struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
        using value_t = std::true_type;
        using type = Op<Args...>;
    };

} // namespace detail

struct nonesuch{};

template <template<class...> class Op, class... Args>
using isDetected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <typename T>
using hasRed = decltype(T::r);

template <typename T>
using hasGreen = decltype(T::g);

template <typename T>
using hasBlue = decltype(T::b);

template <typename T>
using hasAlpha = decltype(T::a);

template<class Pixel, bool typeFloat>
Pixel ConstructPixel(float r, float g, float b, float a)
{
  Pixel p;
  if constexpr (isDetected<hasRed, Pixel>::value)
  {
    if constexpr (typeFloat)
    {
      if constexpr (sizeof(decltype(Pixel::r)) == sizeof(uint16_t))
        p.r = bx::halfFromFloat(r);
      else
        p.r = r;
    }
    else if constexpr (std::is_unsigned<decltype(Pixel::r)>())
      p.r = bx::round(r * ((1 << Pixel::rSz) - 1));
    else
      p.r = bx::round(r * ((1 << (Pixel::rSz - 1)) - 1));
  }
  if constexpr (isDetected<hasGreen, Pixel>::value)
  {
    if constexpr (typeFloat)
    {
      if constexpr (sizeof(decltype(Pixel::g)) == sizeof(uint16_t))
        p.g = bx::halfFromFloat(g);
      else
        p.g = g;
    }
    else if constexpr (std::is_unsigned<decltype(Pixel::g)>())
      p.g = bx::round(g * ((1 << Pixel::gSz) - 1));
    else
      p.g = bx::round(g * ((1 << (Pixel::gSz - 1)) - 1));
  }
  if constexpr (isDetected<hasBlue, Pixel>::value)
  {
    if constexpr (typeFloat)
    {
      if constexpr (sizeof(decltype(Pixel::b)) == sizeof(uint16_t))
        p.b = bx::halfFromFloat(b);
      else
        p.b = b;
    }
    else if constexpr (std::is_unsigned<decltype(Pixel::b)>())
      p.b = bx::round(b * ((1 << Pixel::bSz) - 1));
    else
      p.b = bx::round(b * ((1 << (Pixel::bSz - 1)) - 1));
  }
  if constexpr (isDetected<hasAlpha, Pixel>::value)
  {
    if constexpr (typeFloat)
    {
      if constexpr (sizeof(decltype(Pixel::a)) == sizeof(uint16_t))
        p.a = bx::halfFromFloat(a);
      else
        p.a = a;
    }
    else if constexpr (std::is_unsigned<decltype(Pixel::a)>())
      p.a = bx::round(a * ((1 << Pixel::aSz) - 1));
    else
      p.a = bx::round(a * ((1 << (Pixel::aSz - 1)) - 1));
  }

  return p;
}

template<class Pixel, bool typeFloat = false>
bgfx::TextureHandle createTexture()
{
  const bgfx::Memory* mem = bgfx::alloc(TEXTURE_SIZE * TEXTURE_SIZE * sizeof(Pixel));
  Pixel* texbuf = (Pixel*)mem->data;
  bx::memSet(texbuf, 0xff, mem->size);
  int x, y;
  for (y = 0 ; y < TEXTURE_SIZE; ++y)
  {
    for (x = 0; x < TEXTURE_SIZE; ++x)
    {
      float distance = bx::min(1.0f, bx::sqrt((float) ((x - HALF_TEXTURE_SIZE) * (x - HALF_TEXTURE_SIZE) +
                                                       (y - HALF_TEXTURE_SIZE) * (y - HALF_TEXTURE_SIZE))) /
                                     (float) HALF_TEXTURE_SIZE
      );
      float angle = bx::atan2((float) (y - HALF_TEXTURE_SIZE), (float) (x - HALF_TEXTURE_SIZE));
      float r = 0, g = 0, b = 0;
      float a = 1.0f - distance;
      hsvToRgb(angle, 1.0f, 1.0f - distance, r, g, b);
      texbuf[x + y * TEXTURE_SIZE] = ConstructPixel<Pixel, typeFloat>(r, g, b, a);
    }
  }

  Pixel r = ConstructPixel<Pixel, typeFloat>(1.0f, 0.0f, 0.0f, 1.0f);
  Pixel g = ConstructPixel<Pixel, typeFloat>(0.0f, 1.0f, 0.0f, 1.0f);
  Pixel b = ConstructPixel<Pixel, typeFloat>(0.0f, 0.0f, 1.0f, 1.0f);

  for (y = 0; y < 16; ++y)
  {
    for (x = 0; x < 16; ++x)
    {
      texbuf[x + (TEXTURE_SIZE - 36 + y) * TEXTURE_SIZE] = r;
      texbuf[x + 16 + (TEXTURE_SIZE - 36 + y) * TEXTURE_SIZE] = g;
      texbuf[x + 32 + (TEXTURE_SIZE - 36 + y) * TEXTURE_SIZE] = b;
    }
  }

  if constexpr (isDetected<hasAlpha, Pixel>::value)
  {
    for (y = 0; y < 16; ++y)
    {
      for (x = 0; x < 48; ++x)
      {
        Pixel a = ConstructPixel<Pixel, typeFloat>(1.0f, 1.0f, 1.0f, (float) (48 - x) / 48.0f);
        texbuf[x + (TEXTURE_SIZE - 20 + y) * TEXTURE_SIZE] = a;
      }
    }
  }

  const uint32_t flags = 0
                         | BGFX_SAMPLER_U_CLAMP
                         | BGFX_SAMPLER_V_CLAMP
                         | BGFX_SAMPLER_MIN_POINT
                         | BGFX_SAMPLER_MAG_POINT
  ;
  bgfx::TextureHandle handle = bgfx::createTexture2D(TEXTURE_SIZE, TEXTURE_SIZE, false, 1, Pixel::format, flags, mem);
  return handle;
}

class ExamplePixelFormats : public entry::AppI
{
public:
    ExamplePixelFormats(const char* _name, const char* _description, const char* _url)
		: entry::AppI(_name, _description, _url)
	{
	}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
	{
		Args args(_argc, _argv);

		m_width  = _width;
		m_height = _height;
		m_debug  = BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS;
		m_reset  = BGFX_RESET_VSYNC;

		bgfx::Init init;
		init.type     = args.m_type;
		init.vendorId = args.m_pciId;
		init.resolution.width  = m_width;
		init.resolution.height = m_height;
		init.resolution.reset  = m_reset;
		bgfx::init(init);

		// Enable debug text.
		bgfx::setDebug(m_debug);

		// Set view 0 clear state.
		bgfx::setViewClear(0
				, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
				, 0x303030ff
				, 1.0f
				, 0
				);

		// Create vertex stream declaration.
		PosColorTexCoord0Vertex::init();

		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

		// Create program from shaders.
		m_program = loadProgram("vs_pixelformats", "fs_pixelformats");

    int i = 0;

    #define insertTextureFormat(format, floatFormat) \
      m_textures[i] = createTexture<format##Pixel, floatFormat>(); \
      m_formats[i] = bgfx::TextureFormat::format; \
      ++i;

    insertTextureFormat(A8, false);
    insertTextureFormat(R8, false);
    insertTextureFormat(R8S, false);
    insertTextureFormat(R16, false);
    insertTextureFormat(R16F, true);
    insertTextureFormat(R16S, false);
    insertTextureFormat(R32F, true);
    insertTextureFormat(RG8, false);
    insertTextureFormat(RGB8, false);
    insertTextureFormat(RGB8S, false);
    insertTextureFormat(RGB9E5F, false);
    insertTextureFormat(BGRA8, false);
    insertTextureFormat(RGBA8, false);
    insertTextureFormat(RGBA8S, false);
    insertTextureFormat(RGBA16, false);
    insertTextureFormat(RGBA16F, true);
    insertTextureFormat(RGBA16S, false);
    insertTextureFormat(RGBA32F, true);
    insertTextureFormat(B5G6R5, false);
    insertTextureFormat(BGRA4, false);
    insertTextureFormat(BGR5A1, false);
    insertTextureFormat(RGB10A2, false);

    #undef insertTextureFormat

    for (i = 0; i < sizeof(m_textures)/ sizeof(m_textures[0]); ++i)
    {
      const char* formatName = bimg::getName(bimg::TextureFormat::Enum(m_formats[i]));
      bgfx::setName(m_textures[i], formatName); \
      bgfx::setViewName(i + 1, formatName); \
    }

		imguiCreate();
	}

	int shutdown() override
	{
		imguiDestroy();

		// Cleanup.
		bgfx::destroy(m_program);

		bgfx::destroy(s_texColor);

		for (auto texture: m_textures)
			bgfx::destroy(texture);

		// Shutdown bgfx.
		bgfx::shutdown();

		return 0;
	}

	bool update() override
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		{
			imguiBeginFrame(m_mouseState.m_mx
				,  m_mouseState.m_my
				, (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
				,  m_mouseState.m_mz
				, uint16_t(m_width)
				, uint16_t(m_height)
				);

			showExampleDialog(this);

			imguiEndFrame();
			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to viewZ 0.
			bgfx::touch(0);
			bgfx::dbgTextClear();

			// bgfx::dbgTextPrintf(0, 0, 0x0f, "API %d, Renderer %s", BGFX_API_VERSION, bgfx::getRendererName(bgfx::getRendererType()));

			uint16_t splits = 5;
			uint16_t offsetX = m_width / 2;
			uint16_t offsetY = m_height / 16;
			uint16_t w = uint16_t((m_width - offsetX) / splits);
			uint16_t h = uint16_t((m_height - offsetY) / splits);
			for (int i = 0; i < sizeof(m_textures) / sizeof(m_textures[0]); ++i)
			{
				bgfx::ViewId viewId = i + 1;
				uint16_t x = i % splits;
				uint16_t y = i / splits;
				int currentTexture = i;

				// Set view default viewport.
				bgfx::setViewRect(viewId, offsetX + x * w, offsetY + y * h, w, h);

				const bgfx::Caps *caps = bgfx::getCaps();

				float ortho[16];
				bx::mtxOrtho(ortho, 0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);

				// Set view and projection matrix.
				bgfx::setViewTransform(viewId, NULL, ortho);

				bgfx::setTexture(0, s_texColor, m_textures[currentTexture]);
				renderScreenSpaceQuad(viewId, m_program, 0.0f, 0.0f, 1280.0f, 720.0f);
				uint16_t textW = w / 8;
				uint16_t textH = h / 15;
				bgfx::dbgTextPrintf(x * textW + (offsetX + textW) / 8, y * textH + (offsetY + textH) / 15, 0x0f, bimg::getName(bimg::TextureFormat::Enum(m_formats[i])));
			}

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

			return true;
		}

		return false;
	}

	entry::MouseState m_mouseState;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	bgfx::UniformHandle s_texColor;
	bgfx::ProgramHandle m_program;
	bgfx::TextureFormat::Enum m_formats[22];
	bgfx::TextureHandle m_textures[sizeof(m_formats) / sizeof(m_formats[0])];
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	  ExamplePixelFormats
	, "47-pixelformats"
	, "Texture formats."
	, "https://bkaradzic.github.io/bgfx/examples.html#pixelformats"
	);
