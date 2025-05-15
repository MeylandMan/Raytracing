#pragma once
#include <vector>
#include <string>
#include <chrono>

class Image {
public:
	Image(int width, int height);

	void Start();
	void Update();
	bool IsDone() const;
	double GetElapsedTime() const;
	const std::vector<unsigned char>& GetPixels() const;
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	void SavePNG(const std::string& filename);

private:
	int m_Width, m_Height;
	int m_CurrentLine;
	std::vector<unsigned char> m_Pixels;
	bool m_Running, m_Done;
	std::chrono::high_resolution_clock::time_point m_StartTime, m_EndTime;
};
