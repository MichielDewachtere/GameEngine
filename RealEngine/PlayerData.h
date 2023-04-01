#pragma once
#include "Observer.h"

namespace real
{
	class PlayerData final : public Observer
	{
	public:
		PlayerData() = default;
		virtual ~PlayerData() override = default;
		PlayerData(const PlayerData& other) = delete;
		PlayerData& operator=(const PlayerData& other) = delete;
		PlayerData(PlayerData&& other) = delete;
		PlayerData& operator=(PlayerData&& other) = delete;

		virtual void Notify(GameEvent event, GameObject* actor) override;
	};
}