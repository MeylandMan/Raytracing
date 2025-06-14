#include "Image.h"

#include "Color.h"

#include <fstream>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


Image::Image(int width, int height)
	: m_CurrentLine(0), m_Running(false), m_Done(false),
		m_Params(ImageParams(width, height))
{
	m_Pixels.resize(width * height * 3, 0);
}

void Image::Start() {
	m_CurrentLine = 0;
	m_Done = false;
	m_Running = true;
	m_StartTime = std::chrono::high_resolution_clock::now();
}

void Image::Update() {
	if (!m_Running || m_Done) return;

	if (m_CurrentLine >= m_Params.Height) {
		m_Done = true;
		m_Running = false;
		m_EndTime = std::chrono::high_resolution_clock::now();
		return;
	}

	for (int i = 0; i < m_Params.Width; ++i) {

		auto col = color(double(i) / (m_Params.Width - 1), double(m_CurrentLine) / (m_Params.Height - 1), 0.0);
		write_color(m_Params, col, m_Pixels, i, m_CurrentLine);
	}
	++m_CurrentLine;
}

bool Image::IsDone() const {
	return m_Done;
}

double Image::GetElapsedTime() const {
	if (!m_Done) return 0.0;
	return std::chrono::duration<double>(m_EndTime - m_StartTime).count();
}

const std::vector<unsigned char>& Image::GetPixels() const {
	return m_Pixels;
}

void Image::SavePNG(const std::string& filename) {

	std::vector<unsigned char> flipped(m_Pixels.size());
	for (int y = 0; y < m_Params.Height; ++y) {
		memcpy(
			&flipped[y * m_Params.Width * 3],
			&m_Pixels[(m_Params.Height - 1 - y) * m_Params.Width * 3],
			m_Params.Width * 3
		);
	}
	if (!stbi_write_png(filename.c_str(), m_Params.Width, m_Params.Height, 3, flipped.data(), m_Params.Width * 3)) {
		std::cerr << "Error while saving the file to PNG : " << filename << "\n";
	}
}

