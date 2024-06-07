#pragma once
#include "Ability.h"

class HealthAbility : public Ability
{
public:
  HealthAbility(int health);

  // Inherited via Ability
  void Update(GameState& game, Pot& pot, float elapsedSec) override;
  void Apply(GameState& game, Pot& pot) override;

  Ability* Copy() const override;
private:
  int m_Health;

  // Inherited via Ability
};

