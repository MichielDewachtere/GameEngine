#ifndef LEVEL03_H
#define LEVEL03_H

#include <Scene.h>

class Level03 final : public real::Scene
{
public:
	explicit Level03();
	virtual ~Level03() override = default;
	Level03(const Level03& other) = delete;
	Level03(Level03&& other) = delete;
	Level03& operator=(const Level03& other) = delete;
	Level03& operator=(Level03&& other) = delete;

	void Load() override;
};

#endif // LEVEL03_H