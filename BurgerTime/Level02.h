#ifndef LEVEL02_H
#define LEVEL02_H

#include <Scene.h>

class Level02 final : public real::Scene
{
public:
	explicit Level02();
	virtual ~Level02() override = default;
	Level02(const Level02& other) = delete;
	Level02(Level02&& other) = delete;
	Level02& operator=(const Level02& other) = delete;
	Level02& operator=(Level02&& other) = delete;

	void Load() override;
};

#endif // LEVEL02_H