#pragma once
#include "Ability.h"

class IngredientAbility : public Ability
{
public:
  IngredientAbility(int amount);

  // Inherited via Ability
  void Update(GameState& game, Pot& pot, float elapsedSec) override;
  void Apply(GameState& game, Pot& pot) override;
  Ability* Copy() const override;
private:
  int m_Amount;
};

