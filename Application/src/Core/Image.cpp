#include "Image.h"
#include <fstream>
#include <iostream>

Image::Image(int width, int height)
	: m_Width(width), m_Height(height), m_CurrentLine(0), m_Running(false), m_Done(false)
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
	if (m_CurrentLine >= m_Height) {
		m_Done = true;
		m_Running = false;
		m_EndTime = std::chrono::high_resolution_clock::now();
		return;
	}
	for (int i = 0; i < m_Width; ++i) {
		double r = double(i) / (m_Width - 1);
		double g = double(m_CurrentLine) / (m_Height - 1);
		double b = 0.0;
		int ir = int(255.999 * r);
		int ig = int(255.999 * g);
		int ib = int(255.999 * b);
		int idx = ((m_Height - 1 - m_CurrentLine) * m_Width + i) * 3;
		m_Pixels[idx + 0] = static_cast<unsigned char>(ir);
		m_Pixels[idx + 1] = static_cast<unsigned char>(ig);
		m_Pixels[idx + 2] = static_cast<unsigned char>(ib);
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

void Image::SavePPM(const std::string& filename) {
	std::ofstream image_file(filename);
	if (!image_file) {
		std::cerr << "Error while loading the file: " << filename << "\n";
		return;
	}
	image_file << "P3\n" << m_Width << " " << m_Height << "\n255\n";
	for (int j = 0; j < m_Height; ++j) {
		for (int i = 0; i < m_Width; ++i) {
			int idx = (j * m_Width + i) * 3;
			image_file << (int)m_Pixels[idx] << " "
				<< (int)m_Pixels[idx + 1] << " "
				<< (int)m_Pixels[idx + 2] << " ";
		}
		image_file << "\n";
	}
	image_file.close();
}
