#include "pch.h"
#include "utils.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "MathUtils.h"

RectangleShape::RectangleShape(const Point2f& size, const Color4f& color, bool filled)
  : Shape(color, filled), m_Size(size)
{
}

RectangleShape::RectangleShape(const RectangleShape& other)
  : RectangleShape(other.GetSize(), other.GetColor(), other.IsFilled())
{
}

bool RectangleShape::operator==(const Shape& other) const
{
  const RectangleShape* otherRectangle = dynamic_cast<const RectangleShape*>(&other);

  if (!otherRectangle) {
    return false;
  }

  return m_Color == otherRectangle->GetColor() && GetSize() == otherRectangle->GetSize() && otherRectangle->IsFilled() == m_Filled;
}

Point2f RectangleShape::GetSize() const
{
  return m_Size;
}

void RectangleShape::Draw(const Point2f& position) const
{
  const Rectf rect{
    position.x - m_Size.x / 2.f,
    position.y - m_Size.y / 2.f,
    m_Size.x,
    m_Size.y
  };

  utils::SetColor(m_Color);

  if (m_Filled) {
    utils::FillRect(rect);
  } else {
    utils::DrawRect(rect);
  }
}

Shape* RectangleShape::Copy() const
{
  return new RectangleShape(*this);
}