#ifndef HIGHSCOREPARSER_H
#define HIGHSCOREPARSER_H

#include <map>
#include <string>


class HighScoreParser final
{
public:
	static int ParseHighScores(const std::string& fileName);

private:
	inline static std::map<std::string, int> m_HighScores{};
};

#endif // HIGHSCOREPARSER_H