/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

#include <bx/math.h>

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

struct RGBA8Pixel {
    static constexpr const char* str = "RGBA8";
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8;
    static constexpr const bimg::UnpackFn unpackFn = bx::unpackRgba8;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    static constexpr const size_t aSz = 8;
    uint32_t r : rSz;
    uint32_t g : gSz;
    uint32_t b : bSz;
    uint32_t a : aSz;
};

struct BGRA8Pixel {
    static constexpr const char* str = "BGRA8";
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::BGRA8;
    static constexpr const bimg::UnpackFn unpackFn = bx::unpackBgra8;
    static constexpr const size_t rSz = 8;
    static constexpr const size_t gSz = 8;
    static constexpr const size_t bSz = 8;
    static constexpr const size_t aSz = 8;
    uint32_t b : bSz;
    uint32_t g : gSz;
    uint32_t r : rSz;
    uint32_t a : aSz;
};

struct RGBA4Pixel {
    static constexpr const char* str = "RGBA4";
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA4;
    static constexpr const bimg::UnpackFn unpackFn = bx::unpackRgba4;
    static constexpr const size_t rSz = 4;
    static constexpr const size_t gSz = 4;
    static constexpr const size_t bSz = 4;
    static constexpr const size_t aSz = 4;
    uint16_t r : rSz;
    uint16_t g : gSz;
    uint16_t b : bSz;
    uint16_t a : aSz;
};

struct RGB5A1Pixel {
    static constexpr const char* str = "RGB5A1";
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGB5A1;
    static constexpr const bimg::UnpackFn unpackFn = bx::unpackRgb5a1;
    static constexpr const size_t rSz = 5;
    static constexpr const size_t gSz = 5;
    static constexpr const size_t bSz = 5;
    static constexpr const size_t aSz = 1;
    uint16_t r : rSz;
    uint16_t g : gSz;
    uint16_t b : bSz;
    uint16_t a : aSz;
};

struct RGB10A2Pixel {
    static constexpr const char* str = "RGB10A2";
    static constexpr const bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGB10A2;
    static constexpr const bimg::UnpackFn unpackFn = bx::unpackRgb10A2;
    static constexpr const size_t rSz = 10;
    static constexpr const size_t gSz = 10;
    static constexpr const size_t bSz = 10;
    static constexpr const size_t aSz = 2;
    uint32_t r : rSz;
    uint32_t g : gSz;
    uint32_t b : bSz;
    uint32_t a : aSz;
};

constexpr const char* FormatStrings[] =
{
  RGBA8Pixel::str,
  BGRA8Pixel::str,
  RGBA4Pixel::str,
  RGB5A1Pixel::str,
  RGB10A2Pixel::str,

  "imageConvert",
  "imageConvert",
  "imageConvert",
  "imageConvert",
  "imageConvert",
};

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

template<class Pixel>
bgfx::TextureHandle createTexture(bool convert = false)
{
  const bgfx::Memory* mem = bgfx::alloc(TEXTURE_SIZE * TEXTURE_SIZE * sizeof(Pixel));
  Pixel* texbuf = (Pixel*)mem->data;
  bx::memSet(texbuf, 0xff, mem->size);
  int x, y;
  for (y = 0 ; y < TEXTURE_SIZE; ++y)
  {
    for (x = 0 ; x < TEXTURE_SIZE; ++x)
    {
      float distance = bx::min(1.0f
              , bx::sqrt( (float)( (x - HALF_TEXTURE_SIZE) * (x - HALF_TEXTURE_SIZE) + (y - HALF_TEXTURE_SIZE) * (y - HALF_TEXTURE_SIZE) ) ) / (float)HALF_TEXTURE_SIZE
      );
      float angle = bx::atan2((float)(y - HALF_TEXTURE_SIZE), (float)(x - HALF_TEXTURE_SIZE));
      float r = 0, g = 0, b = 0;
      float a = 1.0f - distance;
      hsvToRgb(angle, 1.0f, 1.0f, r, g, b);

      texbuf[x + y * TEXTURE_SIZE].r = bx::round(r * ((1 << Pixel::rSz) - 1));
      texbuf[x + y * TEXTURE_SIZE].g = bx::round(g * ((1 << Pixel::gSz) - 1));
      texbuf[x + y * TEXTURE_SIZE].b = bx::round(b * ((1 << Pixel::bSz) - 1));
      texbuf[x + y * TEXTURE_SIZE].a = bx::round(a * ((1 << Pixel::aSz) - 1));
    }
  }

  if (convert)
  {
    const bgfx::Memory* mem2 = bgfx::alloc(TEXTURE_SIZE * TEXTURE_SIZE * sizeof(RGBA8Pixel));
    bx::memSet(mem2->data, 0xff, mem2->size);
    bimg::imageConvert(mem2->data, sizeof(RGBA8Pixel) * 8, bx::packRgba8, mem->data, sizeof(Pixel) * 8, Pixel::unpackFn, TEXTURE_SIZE, TEXTURE_SIZE, 1, TEXTURE_SIZE * sizeof(Pixel), TEXTURE_SIZE * sizeof(RGBA8Pixel));
    mem = mem2;
  }

  const uint32_t flags = 0
                         | BGFX_SAMPLER_U_CLAMP
                         | BGFX_SAMPLER_V_CLAMP
                         | BGFX_SAMPLER_MIN_POINT
                         | BGFX_SAMPLER_MAG_POINT
  ;
  bgfx::TextureHandle handle = bgfx::createTexture2D(TEXTURE_SIZE, TEXTURE_SIZE, false, 1, convert ? RGBA8Pixel::format : Pixel::format, flags, mem);
  bgfx::setName(handle, Pixel::str);
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

    m_textures[0] = createTexture<RGBA8Pixel>();
    m_textures[1] = createTexture<BGRA8Pixel>();
    m_textures[2] = createTexture<RGBA4Pixel>();
    m_textures[3] = createTexture<RGB5A1Pixel>();
    m_textures[4] = createTexture<RGB10A2Pixel>();
    m_textures[5] = createTexture<RGBA8Pixel>(true);
    m_textures[6] = createTexture<BGRA8Pixel>(true);
    m_textures[7] = createTexture<RGBA4Pixel>(true);
    m_textures[8] = createTexture<RGB5A1Pixel>(true);
    m_textures[9] = createTexture<RGB10A2Pixel>(true);

		for (int i = 0; i < sizeof(FormatStrings) / sizeof(FormatStrings[0]); ++i)
		{
			bgfx::setViewName(i + 1, FormatStrings[i]);
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
				uint16_t textH = h / 16;
				bgfx::dbgTextPrintf(x * textW + (offsetX + textW) / 8, y * textH + (offsetY + textH) / 16, 0x0f, FormatStrings[i]);
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
	bgfx::TextureHandle m_textures[sizeof(FormatStrings) / sizeof(FormatStrings[0])];
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	  ExamplePixelFormats
	, "47-pixelformats"
	, "Texture formats."
	, "https://bkaradzic.github.io/bgfx/examples.html#pixelformats"
	);
