#pragma once
#include "Texture.h"
#include "Game.h"
#include "GameState.h"

static const float BOX_MARGIN = 20.f;

class Pot;



class Ability
{
public:
  virtual ~Ability();

  void Draw(const Rectf& rect) const; // Draw cannot be overriden, all abilities are drawn the same
  void DrawInfo(const Point2f& position) const; // Draws an info box with the name and description

  virtual void Update(GameState& game, Pot& pot, float elapsedSec) = 0; // Each ability can do or modify something at update?

  virtual void Apply(GameState& game, Pot& pot) = 0; // Each abilty can be applied to the pot

  void UpdateLifetime();

  std::string GetDescription() const;
  std::string GetName() const;
  float GetLifetime() const;

  virtual Ability* Copy() const = 0;
protected:
  Ability(const std::string& name, const std::string& description, float lifetime, const std::string& texture);

private:
  // How long this ability has left to live (in rounds)
  int m_Lifetime;

  const std::string m_Name;
  const std::string m_Description;

  const Texture* m_TexturePtr;
  const Texture* m_NameTexturePtr;
  const Texture* m_DescriptionTexturePtr;
};

