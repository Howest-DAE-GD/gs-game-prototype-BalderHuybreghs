#include "pch.h"
#include "Ability.h"
#include "utils.h"
#include "Pot.h"

using namespace utils;

Ability::Ability(const std::string& name, const std::string& description, float lifetime, const std::string& texture)
  : m_Name(name), m_Description(description), m_Lifetime(0)
{
  m_TexturePtr = new Texture(texture);
  m_NameTexturePtr = new Texture(name, "fonts/roboto.ttf", 16, Color4f{ 1.f, 1.f, 1.f, 1.f });
  m_DescriptionTexturePtr = new Texture(description, "fonts/roboto.ttf", 14, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

Ability::~Ability()
{
  delete m_TexturePtr;
  delete m_NameTexturePtr;
  delete m_DescriptionTexturePtr;
}

void Ability::Draw(const Rectf& rect) const
{
  m_TexturePtr->Draw(rect);
}

void Ability::DrawInfo(const Point2f& position) const
{
  // Also a margin
  const float boxWidth{ m_NameTexturePtr->GetWidth() + m_DescriptionTexturePtr->GetWidth() + BOX_MARGIN };
  const float boxHeight{ m_NameTexturePtr->GetHeight() + m_DescriptionTexturePtr->GetHeight() + BOX_MARGIN + 2.f };

  const Rectf boxRect{
    position.x,
    position.y - boxHeight,
    boxWidth,
    boxHeight
  };

  const Point2f namePosition{
    position.x + BOX_MARGIN / 2.f,
    position.y - BOX_MARGIN / 2.f - m_NameTexturePtr->GetHeight()
  };

  const Point2f descriptionPosition{
    position.x + BOX_MARGIN / 2.f,
    position.y - BOX_MARGIN / 2.f - m_NameTexturePtr->GetHeight() - 2.f - m_DescriptionTexturePtr->GetHeight()
  };

  SetColor(Color4f{ .4f, .5f, .7f, 1.f });
  FillRect(boxRect);

  m_NameTexturePtr->Draw(namePosition);
  m_DescriptionTexturePtr->Draw(descriptionPosition);
}

void Ability::UpdateLifetime()
{
  --m_Lifetime;
}

std::string Ability::GetDescription() const
{
  return m_Description;
}

std::string Ability::GetName() const
{
  return m_Name;
}

float Ability::GetLifetime() const
{
  return m_Lifetime;
}
