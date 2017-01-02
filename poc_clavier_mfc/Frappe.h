#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <chrono>

class Keystroke
{
public:
	Keystroke();
	~Keystroke();
	inline const std::vector<std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>> getKeystockes() { return keystrokes; }
	inline const auto getLastKeyDuration() { return (keystrokes.back().second - keystrokes.back().first).count(); }
	inline const auto getCurrentKeyDuration() { return (currentKeyUp - currentKeyDown).count(); }
	inline void setDown(std::chrono::high_resolution_clock::time_point t) { currentKeyDown = t; }
	inline void setUp(std::chrono::high_resolution_clock::time_point t) { currentKeyUp = t; }
private:
	std::chrono::high_resolution_clock::time_point currentKeyDown;
	std::chrono::high_resolution_clock::time_point currentKeyUp;
	std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point> currentKey;
	std::vector<std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>> keystrokes;
	inline std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point> makePair() { currentKey = std::make_pair(currentKeyDown, currentKeyUp); return currentKey; }
	inline void add_keystroke(std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point> keys) { keystrokes.push_back(keys); }
};

