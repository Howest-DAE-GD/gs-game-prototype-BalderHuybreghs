#include "pch.h"
#include "IngredientAbility.h"

IngredientAbility::IngredientAbility(int amount)
  : Ability("Ingredient", "Removes an ingredient", 0, "textures/pepper.png"), m_Amount(amount)
{
}

void IngredientAbility::Update(GameState& game, Pot& pot, float elapsedSec)
{
}

void IngredientAbility::Apply(GameState& game, Pot& pot)
{
  if (pot.GetIngredients() > 0) {
    pot.SetIngredients(pot.GetIngredients() - m_Amount);
  }
}

Ability* IngredientAbility::Copy() const
{
  return new IngredientAbility(m_Amount);
}
