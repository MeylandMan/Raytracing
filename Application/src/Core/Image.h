#pragma once
#include <vector>
#include <string>
#include <chrono>

#include <glm/vec3.hpp>

using color = glm::vec3;
using data = std::vector<unsigned char>;

struct ImageParams {

	int Width;
	int Height;
	ImageParams(int width, int height)
		: Width(width), Height(height) {
	}
};

class Image {
public:
	Image(int width, int height);

	void Start();
	void Update();
	bool IsDone() const;
	double GetElapsedTime() const;
	const std::vector<unsigned char>& GetPixels() const;
	int GetWidth() const { return m_Params.Width; }
	int GetHeight() const { return m_Params.Height; }

	void SavePNG(const std::string& filename);

private:
	int m_CurrentLine;
	ImageParams m_Params;
	std::vector<unsigned char> m_Pixels;
	bool m_Running, m_Done;
	std::chrono::high_resolution_clock::time_point m_StartTime, m_EndTime;
};
