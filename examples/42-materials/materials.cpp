/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"

namespace
{

struct PosTexCoordMaterialVertex
{
	float m_x;
	float m_y;
	float m_z;
	float m_u;
	float m_v;
	uint8_t m_materialID;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Uint8)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosTexCoordMaterialVertex::ms_layout;

static PosTexCoordMaterialVertex s_planeVertices[] =
{
	{20.0f, 20.0f, 1.0f, 0.0f, 0.0f, 5 },
	{16.0f, 20.0f, 1.0f, 0.1f, 0.0f, 5 },
	{12.0f, 20.0f, 1.0f, 0.2f, 0.0f, 5 },
	{8.0f, 20.0f, 1.0f, 0.3f, 0.0f, 5 },
	{4.0f, 20.0f, 1.0f, 0.4f, 0.0f, 5 },
	{0.0f, 20.0f, 1.0f, 0.5f, 0.0f, 5 },
	{-4.0f, 20.0f, 1.0f, 0.6f, 0.0f, 0 },
	{-8.0f, 20.0f, 1.0f, 0.7f, 0.0f, 0 },
	{-12.0f, 20.0f, 1.0f, 0.8f, 0.0f, 0 },
	{-16.0f, 20.0f, 1.0f, 0.9f, 0.0f, 0 },
	{-20.0f, 20.0f, 1.0f, 1.0f, 0.0f, 0 },
	{20.0f, 16.0f, 1.0f, 0.0f, 0.1f, 5 },
	{16.0f, 16.0f, 1.0f, 0.1f, 0.1f, 5 },
	{12.0f, 16.0f, 1.0f, 0.2f, 0.1f, 5 },
	{8.0f, 16.0f, 1.0f, 0.3f, 0.1f, 5 },
	{4.0f, 16.0f, 1.0f, 0.4f, 0.1f, 5 },
	{0.0f, 16.0f, 1.0f, 0.5f, 0.1f, 5 },
	{-4.0f, 16.0f, 1.0f, 0.6f, 0.1f, 0 },
	{-8.0f, 16.0f, 1.0f, 0.7f, 0.1f, 0 },
	{-12.0f, 16.0f, 1.0f, 0.8f, 0.1f, 0 },
	{-16.0f, 16.0f, 1.0f, 0.9f, 0.1f, 0 },
	{-20.0f, 16.0f, 1.0f, 1.0f, 0.1f, 0 },
	{20.0f, 12.0f, 1.0f, 0.0f, 0.2f, 5 },
	{16.0f, 12.0f, 1.0f, 0.1f, 0.2f, 5 },
	{12.0f, 12.0f, 1.0f, 0.2f, 0.2f, 5 },
	{8.0f, 12.0f, 1.0f, 0.3f, 0.2f, 5 },
	{4.0f, 12.0f, 1.0f, 0.4f, 0.2f, 5 },
	{0.0f, 12.0f, 1.0f, 0.5f, 0.2f, 5 },
	{-4.0f, 12.0f, 1.0f, 0.6f, 0.2f, 0 },
	{-8.0f, 12.0f, 1.0f, 0.7f, 0.2f, 0 },
	{-12.0f, 12.0f, 1.0f, 0.8f, 0.2f, 0 },
	{-16.0f, 12.0f, 1.0f, 0.9f, 0.2f, 0 },
	{-20.0f, 12.0f, 1.0f, 1.0f, 0.2f, 0 },
	{20.0f, 8.0f, 1.0f, 0.0f, 0.3f, 5 },
	{16.0f, 8.0f, 1.0f, 0.1f, 0.3f, 5 },
	{12.0f, 8.0f, 1.0f, 0.2f, 0.3f, 5 },
	{8.0f, 8.0f, 1.0f, 0.3f, 0.3f, 5 },
	{4.0f, 8.0f, 1.0f, 0.4f, 0.3f, 5 },
	{0.0f, 8.0f, 1.0f, 0.5f, 0.3f, 5 },
	{-4.0f, 8.0f, 1.0f, 0.6f, 0.3f, 0 },
	{-8.0f, 8.0f, 1.0f, 0.7f, 0.3f, 0 },
	{-12.0f, 8.0f, 1.0f, 0.8f, 0.3f, 0 },
	{-16.0f, 8.0f, 1.0f, 0.9f, 0.3f, 0 },
	{-20.0f, 8.0f, 1.0f, 1.0f, 0.3f, 0 },
	{20.0f, 4.0f, 1.0f, 0.0f, 0.4f, 5 },
	{16.0f, 4.0f, 1.0f, 0.1f, 0.4f, 5 },
	{12.0f, 4.0f, 1.0f, 0.2f, 0.4f, 5 },
	{8.0f, 4.0f, 1.0f, 0.3f, 0.4f, 5 },
	{4.0f, 4.0f, 1.0f, 0.4f, 0.4f, 5 },
	{0.0f, 4.0f, 1.0f, 0.5f, 0.4f, 0 },
	{-4.0f, 4.0f, 1.0f, 0.6f, 0.4f, 0 },
	{-8.0f, 4.0f, 1.0f, 0.7f, 0.4f, 0 },
	{-12.0f, 4.0f, 1.0f, 0.8f, 0.4f, 0 },
	{-16.0f, 4.0f, 1.0f, 0.9f, 0.4f, 0 },
	{-20.0f, 4.0f, 1.0f, 1.0f, 0.4f, 0 },
	{20.0f, 0.0f, 1.0f, 0.0f, 0.5f, 5 },
	{16.0f, 0.0f, 1.0f, 0.1f, 0.5f, 5 },
	{12.0f, 0.0f, 1.0f, 0.2f, 0.5f, 5 },
	{8.0f, 0.0f, 1.0f, 0.3f, 0.5f, 5 },
	{4.0f, 0.0f, 1.0f, 0.4f, 0.5f, 5 },
	{0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0 },
	{-4.0f, 0.0f, 1.0f, 0.6f, 0.5f, 0 },
	{-8.0f, 0.0f, 1.0f, 0.7f, 0.5f, 0 },
	{-12.0f, 0.0f, 1.0f, 0.8f, 0.5f, 0 },
	{-16.0f, 0.0f, 1.0f, 0.9f, 0.5f, 0 },
	{-20.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0 },
	{20.0f, -4.0f, 1.0f, 0.0f, 0.6f, 2 },
	{16.0f, -4.0f, 1.0f, 0.1f, 0.6f, 2 },
	{12.0f, -4.0f, 1.0f, 0.2f, 0.6f, 2 },
	{8.0f, -4.0f, 1.0f, 0.3f, 0.6f, 2 },
	{4.0f, -4.0f, 1.0f, 0.4f, 0.6f, 2 },
	{0.0f, -4.0f, 1.0f, 0.5f, 0.6f, 9 },
	{-4.0f, -4.0f, 1.0f, 0.6f, 0.6f, 9 },
	{-8.0f, -4.0f, 1.0f, 0.7f, 0.6f, 9 },
	{-12.0f, -4.0f, 1.0f, 0.8f, 0.6f, 9 },
	{-16.0f, -4.0f, 1.0f, 0.9f, 0.6f, 9 },
	{-20.0f, -4.0f, 1.0f, 1.0f, 0.6f, 9 },
	{20.0f, -8.0f, 1.0f, 0.0f, 0.7f, 2 },
	{16.0f, -8.0f, 1.0f, 0.1f, 0.7f, 2 },
	{12.0f, -8.0f, 1.0f, 0.2f, 0.7f, 2 },
	{8.0f, -8.0f, 1.0f, 0.3f, 0.7f, 2 },
	{4.0f, -8.0f, 1.0f, 0.4f, 0.7f, 2 },
	{0.0f, -8.0f, 1.0f, 0.5f, 0.7f, 9 },
	{-4.0f, -8.0f, 1.0f, 0.6f, 0.7f, 9 },
	{-8.0f, -8.0f, 1.0f, 0.7f, 0.7f, 9 },
	{-12.0f, -8.0f, 1.0f, 0.8f, 0.7f, 9 },
	{-16.0f, -8.0f, 1.0f, 0.9f, 0.7f, 9 },
	{-20.0f, -8.0f, 1.0f, 1.0f, 0.7f, 9 },
	{20.0f, -12.0f, 1.0f, 0.0f, 0.8f, 2 },
	{16.0f, -12.0f, 1.0f, 0.1f, 0.8f, 2 },
	{12.0f, -12.0f, 1.0f, 0.2f, 0.8f, 2 },
	{8.0f, -12.0f, 1.0f, 0.3f, 0.8f, 2 },
	{4.0f, -12.0f, 1.0f, 0.4f, 0.8f, 2 },
	{0.0f, -12.0f, 1.0f, 0.5f, 0.8f, 9 },
	{-4.0f, -12.0f, 1.0f, 0.6f, 0.8f, 9 },
	{-8.0f, -12.0f, 1.0f, 0.7f, 0.8f, 9 },
	{-12.0f, -12.0f, 1.0f, 0.8f, 0.8f, 9 },
	{-16.0f, -12.0f, 1.0f, 0.9f, 0.8f, 9 },
	{-20.0f, -12.0f, 1.0f, 1.0f, 0.8f, 9 },
	{20.0f, -16.0f, 1.0f, 0.0f, 0.9f, 2 },
	{16.0f, -16.0f, 1.0f, 0.1f, 0.9f, 2 },
	{12.0f, -16.0f, 1.0f, 0.2f, 0.9f, 2 },
	{8.0f, -16.0f, 1.0f, 0.3f, 0.9f, 2 },
	{4.0f, -16.0f, 1.0f, 0.4f, 0.9f, 2 },
	{0.0f, -16.0f, 1.0f, 0.5f, 0.9f, 9 },
	{-4.0f, -16.0f, 1.0f, 0.6f, 0.9f, 9 },
	{-8.0f, -16.0f, 1.0f, 0.7f, 0.9f, 9 },
	{-12.0f, -16.0f, 1.0f, 0.8f, 0.9f, 9 },
	{-16.0f, -16.0f, 1.0f, 0.9f, 0.9f, 9 },
	{-20.0f, -16.0f, 1.0f, 1.0f, 0.9f, 9 },
	{20.0f, -20.0f, 1.0f, 0.0f, 1.0f, 2 },
	{16.0f, -20.0f, 1.0f, 0.1f, 1.0f, 2 },
	{12.0f, -20.0f, 1.0f, 0.2f, 1.0f, 2 },
	{8.0f, -20.0f, 1.0f, 0.3f, 1.0f, 2 },
	{4.0f, -20.0f, 1.0f, 0.4f, 1.0f, 2 },
	{0.0f, -20.0f, 1.0f, 0.5f, 1.0f, 2 },
	{-4.0f, -20.0f, 1.0f, 0.6f, 1.0f, 9 },
	{-8.0f, -20.0f, 1.0f, 0.7f, 1.0f, 9 },
	{-12.0f, -20.0f, 1.0f, 0.8f, 1.0f, 9 },
	{-16.0f, -20.0f, 1.0f, 0.9f, 1.0f, 9 },
	{-20.0f, -20.0f, 1.0f, 1.0f, 1.0f, 9 },
};

static const uint16_t s_planeTriList[] =
{
	0, 1, 12,
	12, 11, 0,
	1, 2, 13,
	13, 12, 1,
	2, 3, 14,
	14, 13, 2,
	3, 4, 15,
	15, 14, 3,
	4, 5, 16,
	16, 15, 4,
	5, 6, 17,
	17, 16, 5,
	6, 7, 18,
	18, 17, 6,
	7, 8, 19,
	19, 18, 7,
	8, 9, 20,
	20, 19, 8,
	9, 10, 21,
	21, 20, 9,
	11, 12, 23,
	23, 22, 11,
	12, 13, 24,
	24, 23, 12,
	13, 14, 25,
	25, 24, 13,
	14, 15, 26,
	26, 25, 14,
	15, 16, 27,
	27, 26, 15,
	16, 17, 28,
	28, 27, 16,
	17, 18, 29,
	29, 28, 17,
	18, 19, 30,
	30, 29, 18,
	19, 20, 31,
	31, 30, 19,
	20, 21, 32,
	32, 31, 20,
	22, 23, 34,
	34, 33, 22,
	23, 24, 35,
	35, 34, 23,
	24, 25, 36,
	36, 35, 24,
	25, 26, 37,
	37, 36, 25,
	26, 27, 38,
	38, 37, 26,
	27, 28, 39,
	39, 38, 27,
	28, 29, 40,
	40, 39, 28,
	29, 30, 41,
	41, 40, 29,
	30, 31, 42,
	42, 41, 30,
	31, 32, 43,
	43, 42, 31,
	33, 34, 45,
	45, 44, 33,
	34, 35, 46,
	46, 45, 34,
	35, 36, 47,
	47, 46, 35,
	36, 37, 48,
	48, 47, 36,
	37, 38, 49,
	49, 48, 37,
	38, 39, 50,
	50, 49, 38,
	39, 40, 51,
	51, 50, 39,
	40, 41, 52,
	52, 51, 40,
	41, 42, 53,
	53, 52, 41,
	42, 43, 54,
	54, 53, 42,
	44, 45, 56,
	56, 55, 44,
	45, 46, 57,
	57, 56, 45,
	46, 47, 58,
	58, 57, 46,
	47, 48, 59,
	59, 58, 47,
	48, 49, 60,
	60, 59, 48,
	49, 50, 61,
	61, 60, 49,
	50, 51, 62,
	62, 61, 50,
	51, 52, 63,
	63, 62, 51,
	52, 53, 64,
	64, 63, 52,
	53, 54, 65,
	65, 64, 53,
	55, 56, 67,
	67, 66, 55,
	56, 57, 68,
	68, 67, 56,
	57, 58, 69,
	69, 68, 57,
	58, 59, 70,
	70, 69, 58,
	59, 60, 71,
	71, 70, 59,
	60, 61, 72,
	72, 71, 60,
	61, 62, 73,
	73, 72, 61,
	62, 63, 74,
	74, 73, 62,
	63, 64, 75,
	75, 74, 63,
	64, 65, 76,
	76, 75, 64,
	66, 67, 78,
	78, 77, 66,
	67, 68, 79,
	79, 78, 67,
	68, 69, 80,
	80, 79, 68,
	69, 70, 81,
	81, 80, 69,
	70, 71, 82,
	82, 81, 70,
	71, 72, 83,
	83, 82, 71,
	72, 73, 84,
	84, 83, 72,
	73, 74, 85,
	85, 84, 73,
	74, 75, 86,
	86, 85, 74,
	75, 76, 87,
	87, 86, 75,
	77, 78, 89,
	89, 88, 77,
	78, 79, 90,
	90, 89, 78,
	79, 80, 91,
	91, 90, 79,
	80, 81, 92,
	92, 91, 80,
	81, 82, 93,
	93, 92, 81,
	82, 83, 94,
	94, 93, 82,
	83, 84, 95,
	95, 94, 83,
	84, 85, 96,
	96, 95, 84,
	85, 86, 97,
	97, 96, 85,
	86, 87, 98,
	98, 97, 86,
	88, 89, 100,
	100, 99, 88,
	89, 90, 101,
	101, 100, 89,
	90, 91, 102,
	102, 101, 90,
	91, 92, 103,
	103, 102, 91,
	92, 93, 104,
	104, 103, 92,
	93, 94, 105,
	105, 104, 93,
	94, 95, 106,
	106, 105, 94,
	95, 96, 107,
	107, 106, 95,
	96, 97, 108,
	108, 107, 96,
	97, 98, 109,
	109, 108, 97,
	99, 100, 111,
	111, 110, 99,
	100, 101, 112,
	112, 111, 100,
	101, 102, 113,
	113, 112, 101,
	102, 103, 114,
	114, 113, 102,
	103, 104, 115,
	115, 114, 103,
	104, 105, 116,
	116, 115, 104,
	105, 106, 117,
	117, 116, 105,
	106, 107, 118,
	118, 117, 106,
	107, 108, 119,
	119, 118, 107,
	108, 109, 120,
	120, 119, 108
};

class ExampleMaterials : public entry::AppI
{
public:
	ExampleMaterials(const char* _name, const char* _description, const char* _url)
		: entry::AppI(_name, _description, _url)
	{
	}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
	{
		Args args(_argc, _argv);

		m_width  = _width;
		m_height = _height;
		m_debug  = BGFX_DEBUG_NONE;
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
		PosTexCoordMaterialVertex::init();

		// Create static vertex buffer.
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			  bgfx::makeRef(s_planeVertices, sizeof(s_planeVertices) )
			, PosTexCoordMaterialVertex::ms_layout
			);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_planeTriList, sizeof(s_planeTriList) ) );

		// Create texture sampler uniforms.
		s_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Sampler);

		// Create program from shaders.
		m_program = loadProgram("vs_materials", "fs_materials");

		// Load diffuse texture.
		m_textureColor = loadTexture("textures/texturearray.dds");

		m_timeOffset = bx::getHPCounter();

		imguiCreate();
	}

	virtual int shutdown() override
	{
		imguiDestroy();

		// Cleanup.
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_program);
		bgfx::destroy(m_textureColor);
		bgfx::destroy(s_texColor);

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

			ImGui::SetNextWindowPos(
				  ImVec2(m_width - m_width / 5.0f - 10.0f, 10.0f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::SetNextWindowSize(
				  ImVec2(m_width / 5.0f, m_height / 3.5f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::Begin("Settings"
				, nullptr
				, 0
				);

			ImGui::End();

			imguiEndFrame();

			float time = (float)( (bx::getHPCounter()-m_timeOffset)/double(bx::getHPFrequency() ) );

			const bx::Vec3 at  = { 0.0f, 0.0f,   0.0f };
			const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

			// Set view and projection matrix for view 0.
			{
				float view[16];
				bx::mtxLookAt(view, eye, at);

				float proj[16];
				bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
				bgfx::setViewTransform(0, view, proj);

				// Set view 0 default viewport.
				bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
			}

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			// Set model matrix for rendering.
//			bgfx::setTransform(mtx);

			// Set vertex and index buffer.
			bgfx::setVertexBuffer(0, m_vbh);
			bgfx::setIndexBuffer(m_ibh);

			// Bind textures.
			bgfx::setTexture(0, s_texColor,  m_textureColor);

			// Set render states.
			bgfx::setState(0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LESS
					| BGFX_STATE_MSAA
					);

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, m_program);

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
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	bgfx::UniformHandle s_texColor;
	bgfx::ProgramHandle m_program;
	bgfx::TextureHandle m_textureColor;
	int64_t m_timeOffset;
};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	  ExampleMaterials
	, "42-materials"
	, "Rendering different levels from a texture array."
	, "https://bkaradzic.github.io/bgfx/examples.html#materials"
	);
