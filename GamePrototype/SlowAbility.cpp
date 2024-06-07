#include "pch.h"
#include "SlowAbility.h"

SlowAbility::SlowAbility(float amount)
  : Ability("Slow", "Slows down the time", 0, "textures/feather.png"), m_Slowdown(amount)
{
}

void SlowAbility::Update(GameState& game, Pot& pot, float elapsedSec)
{
  // Does nothing
}

void SlowAbility::Apply(GameState& game, Pot& pot)
{
  pot.SetViewingTime(pot.GetViewingTime() + m_Slowdown);
}

Ability* SlowAbility::Copy() const
{
  return new SlowAbility(m_Slowdown);
}
