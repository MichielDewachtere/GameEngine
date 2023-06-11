#ifndef HIGHSCOREPARSER_H
#define HIGHSCOREPARSER_H

#include <map>
#include <string>
#include <vector>
#include <ostream>

#pragma warning(disable : 4996)
#include <document.h>
#pragma warning(default : 4996)

class HighScoreParser final
{
public:
	static int ParseHighScores(const std::string& fileName);
	static void UpdateHighScores(const std::string& playerName, int score);
	static std::vector<std::pair<std::string, int>> GetTopFive();

private:
	static void GenerateDefaultJson(const std::string& filename);

	inline static std::map<std::string, int> m_HighScores{};
	inline static rapidjson::Document m_Document{};
	inline static std::string m_FileName{};
};

#endif // HIGHSCOREPARSER_H