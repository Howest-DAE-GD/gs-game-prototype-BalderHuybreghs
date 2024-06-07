#pragma once
#include "Ability.h"

class SlowAbility : public Ability
{
public:
  SlowAbility(float amount);

  // Inherited via Ability
  void Update(GameState& game, Pot& pot, float elapsedSec) override;
  void Apply(GameState& game, Pot& pot) override;
  Ability* Copy() const override;
private:
  float m_Slowdown;

  // Inherited via Ability
};

