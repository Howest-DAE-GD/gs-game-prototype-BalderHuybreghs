#include "pch.h"
#include "utils.h"
#include "CircleShape.h"
#include "RectangleShape.h"
#include "MathUtils.h"

CircleShape::CircleShape(float radius, const Color4f& color, bool filled)
  : Shape(color, filled), m_Radius(radius)
{
}

CircleShape::CircleShape(const CircleShape& other)
  : CircleShape(other.GetRadius(), other.GetColor(), other.IsFilled())
{
}

bool CircleShape::operator==(const Shape& other) const
{
  const CircleShape* otherCircle = dynamic_cast<const CircleShape*>(&other);

  if (!otherCircle) {
    return false;
  }

  return otherCircle->GetColor() == m_Color && otherCircle->GetSize() == GetSize() && otherCircle->IsFilled() == m_Filled;
}

void CircleShape::Draw(const Point2f& position) const
{
  utils::SetColor(m_Color);

  if (m_Filled) {
    utils::FillEllipse(position.x, position.y, m_Radius, m_Radius);
  } else {
    utils::DrawEllipse(position.x, position.y, m_Radius, m_Radius);
  }
}

Shape* CircleShape::Copy() const
{
  return new CircleShape(*this);
}

Point2f CircleShape::GetSize() const
{
  return Point2f{ m_Radius * 2.f, m_Radius * 2.f };
}

float CircleShape::GetRadius() const
{
  return m_Radius;
}
