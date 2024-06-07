#include "pch.h"
#include "HealthAbility.h"

HealthAbility::HealthAbility(int attempts)
  : Ability("Healing", "Heals you", 0, "textures/quarts.png"), m_Health(attempts)
{

}

void HealthAbility::Update(GameState& game, Pot& pot, float elapsedSec)
{
  // Does nothing
}

void HealthAbility::Apply(GameState& game, Pot& pot)
{
  if (game.attempts) {
    game.attempts -= (int)m_Health;
  }
}

Ability* HealthAbility::Copy() const
{
  return new HealthAbility(m_Health);
}
