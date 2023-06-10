#include "HighScoreParser.h"

#include <fstream>
#include <ostream>

#pragma warning(disable : 4996)
#include <document.h>
#include <writer.h>
#pragma warning(default : 4996)

int HighScoreParser::ParseHighScores(const std::string& fileName)
{
    using namespace rapidjson;

    std::string filePath = "../Data/" + fileName;
    std::ifstream inputFile(filePath);

    // Check if file was opened successfully
    if (!inputFile.is_open()) 
    {
        return 0;
    }

    // Read the entire file into a string
    std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());

    // Close the input file
	inputFile.close();

    Document document;
    document.Parse(fileContents.c_str());

    if (document.HasParseError() || !document.IsArray()) 
    {
        return 0;
    }

    int biggestHighScore = 0;

    for (SizeType i = 0; i < document.Size(); ++i) 
    {
        const Value& scoreObj = document[i];
        if (scoreObj.IsObject() && scoreObj.HasMember("name") && scoreObj.HasMember("high_score")) 
        {
            const Value& nameValue = scoreObj["name"];
            const Value& highScoreValue = scoreObj["high_score"];

            if (nameValue.IsString() && highScoreValue.IsInt()) 
            {
                std::string name = nameValue.GetString();
                const int highScore = highScoreValue.GetInt();
                m_HighScores[name] = highScore;
                if (highScore > biggestHighScore)
                {
                    biggestHighScore = highScore;
                }
            }
        }
    }

    return biggestHighScore;
}
