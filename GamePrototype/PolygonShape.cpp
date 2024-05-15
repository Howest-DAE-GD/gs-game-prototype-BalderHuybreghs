#include "pch.h"
#include "PolygonShape.h"
#include "utils.h"

PolygonShape::PolygonShape(std::vector<Point2f> points, const Color4f& color, bool filled)
  : Shape(color, filled), m_Points(points)
{
}

PolygonShape::PolygonShape(const PolygonShape& other)
  : PolygonShape(other.GetPoints(), other.GetColor(), other.IsFilled())
{

}

bool PolygonShape::operator==(const Shape& other) const
{
  const PolygonShape* otherPolygon = dynamic_cast<const PolygonShape*>(&other);

  if (!otherPolygon) {
    return false;
  }

  bool basics{ otherPolygon->GetColor() == m_Color && otherPolygon->IsFilled() == m_Filled && m_Points.size() != otherPolygon->GetPoints().size() };
  if (!basics) {
    return false;
  }

  auto otherPoints{ otherPolygon->GetPoints() };
  for (int i = 0; i < m_Points.size(); i++)
  {
    if (!(m_Points[i] == otherPoints[i])) {
      return false;
    }
  }

  return true;
}

void PolygonShape::Draw(const Point2f& position) const
{
  utils::SetColor(m_Color);

  glPushMatrix();
  glTranslatef(position.x, position.y, 0.f);

  if (m_Filled) {
    utils::FillPolygon(m_Points);
  } else {
    utils::DrawPolygon(m_Points);
  }

  glPopMatrix();
}

Shape* PolygonShape::Copy() const
{
    return new PolygonShape(*this);
}

Point2f PolygonShape::GetSize() const
{
  return Point2f{};
}

std::vector<Point2f> PolygonShape::GetPoints() const
{
  return m_Points;
}
