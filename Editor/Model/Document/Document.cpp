//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Document.h"
#include "Model/Shapes/GroupShape.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

constexpr double BOUNDS_TOLERANCE = 1e-6;

Document::Document(const double width, const double height, const double duration)
	: m_bounds{ 0, 0, width, height }
	, m_duration(duration)
{
	if (m_duration <= 0)
	{
		throw std::invalid_argument("Длительность документа должна быть положительной");
	}
}

Frame Document::GetBounds() const
{
	return m_bounds;
}

double Document::GetDuration() const
{
	return m_duration;
}

int Document::AddShape(std::unique_ptr<IShape> shape)
{
	return AddShapeAt(std::move(shape), m_shapesInner.size());
}

int Document::AddShapeAt(std::unique_ptr<IShape> shape, const size_t index)
{
	if (!shape)
	{
		throw std::invalid_argument("Фигура не задана");
	}
	const int id = shape->GetId();
	InsertShape(std::move(shape), index);
	return id;
}

void Document::RemoveShape(const int id)
{
	const auto it = FindItem(id);
	if (it == m_shapesInner.end())
	{
		return;
	}
	m_shapesInner.erase(it);
	NotifyObservers();
}

std::unique_ptr<IShape> Document::DeleteShape(const int id)
{
	const auto it = FindItem(id);
	if (it == m_shapesInner.end())
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	auto shape = std::move(*it);
	m_shapesInner.erase(it);
	NotifyObservers();
	return shape;
}

void Document::InsertShape(std::unique_ptr<IShape> shape, const size_t index)
{
	if (!shape)
	{
		throw std::invalid_argument("Фигура не задана");
	}
	if (index > m_shapesInner.size())
	{
		throw std::out_of_range("Недопустимая позиция фигуры");
	}
	CheckShape(*shape);
	m_shapesInner.insert(m_shapesInner.begin() + index, std::move(shape));
	NotifyObservers();
}

int Document::GroupShapes(const std::vector<int>& shapeIds, std::unique_ptr<IShape> group, const size_t index)
{
	CheckGrouping(shapeIds, group.get(), index);

	const int id = group->GetId();
	GroupShape& composite = *group->GetGroup();
	for (const int shapeId : shapeIds)
	{
		const auto item = FindItem(shapeId);
		composite.AddChild(std::move(*item));
		m_shapesInner.erase(item);
	}
	m_shapesInner.insert(m_shapesInner.begin() + index, std::move(group));
	NotifyObservers();
	return id;
}

std::unique_ptr<IShape> Document::UngroupShape(const int groupId, const std::vector<size_t>& childIndices)
{
	const auto item = FindItem(groupId);
	const IShape* shape = item != m_shapesInner.end() ? item->get() : nullptr;
	CheckUngrouping(shape, childIndices);

	const auto index = item - m_shapesInner.begin();
	m_shapesInner.reserve(m_shapesInner.size() - 1 + childIndices.size());
	auto group = std::move(m_shapesInner[index]);
	m_shapesInner.erase(m_shapesInner.begin() + index);
	auto children = group->GetGroup()->ReleaseChildren();
	for (size_t i = 0; i < children.size(); ++i)
	{
		m_shapesInner.insert(m_shapesInner.begin() + childIndices[i], std::move(children[i]));
	}
	NotifyObservers();
	return group;
}

void Document::SetShapeFrame(const int id, const Frame& frame)
{
	IShape* shape = GetShapeById(id);
	if (!shape)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	CheckFrame(frame);
	if (shape->GetFrame() == frame)
	{
		return;
	}
	shape->SetFrame(frame);
	NotifyObservers();
}

void Document::SetShapeColors(const int id, const Color fillColor, const Color borderColor)
{
	IShape* shape = GetShapeById(id);
	if (!shape)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	if (shape->GetFillColor() == fillColor && shape->GetBorderColor() == borderColor)
	{
		return;
	}
	shape->SetFillColor(fillColor);
	shape->SetBorderColor(borderColor);
	NotifyObservers();
}

void Document::SetShapeTimeInterval(const int id, const TimeInterval& timeInterval)
{
	IShape* shape = GetShapeById(id);
	if (!shape)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	CheckTimeInterval(timeInterval);
	if (shape->GetTimeInterval() == timeInterval)
	{
		return;
	}
	shape->SetTimeInterval(timeInterval);
	NotifyObservers();
}

void Document::SetShapeIndex(const int id, const size_t index)
{
	const auto shape = FindItem(id);
	if (shape == m_shapesInner.end())
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	if (index >= m_shapesInner.size())
	{
		throw std::out_of_range("Недопустимая позиция фигуры");
	}
	const auto target = m_shapesInner.begin() + index;
	if (shape == target)
	{
		return;
	}
	if (shape < target)
	{
		std::rotate(shape, shape + 1, target + 1);
	}
	else
	{
		std::rotate(target, shape, shape + 1);
	}
	NotifyObservers();
}

void Document::ReplaceShapes(std::vector<std::unique_ptr<IShape>> shapes)
{
	for (const auto& shape : shapes)
	{
		if (!shape)
		{
			throw std::invalid_argument("Фигура не задана");
		}
		CheckShape(*shape);
	}

	m_shapesInner = std::move(shapes);
	NotifyObservers();
}

void Document::Clear()
{
	m_shapesInner.clear();
	NotifyObservers();
}

size_t Document::GetShapeCount() const
{
	return m_shapesInner.size();
}

const IShape& Document::GetShape(const size_t index) const
{
	return *m_shapesInner.at(index);
}

int Document::GetShapeId(const size_t index) const
{
	return m_shapesInner.at(index)->GetId();
}

const IShape* Document::FindShape(const int id) const
{
	for (const auto& root : m_shapesInner)
	{
		if (const auto* found = std::as_const(*root).FindShape(id))
		{
			return found;
		}
	}
	return nullptr;
}

IShape* Document::GetShapeById(const int id)
{
	for (auto& root : m_shapesInner)
	{
		if (auto* found = root->FindShape(id))
		{
			return found;
		}
	}
	return nullptr;
}

std::optional<int> Document::FindRootShapeId(const int id) const
{
	for (const auto& root : m_shapesInner)
	{
		if (std::as_const(*root).FindShape(id))
		{
			return root->GetId();
		}
	}
	return std::nullopt;
}

std::optional<int> Document::FindTargetShape(const int rootId, const Point point, const double time) const
{
	const IShape* root = FindShape(rootId);
	if (!root)
	{
		return std::nullopt;
	}
	return FindTargetInShape(*root, point, time);
}

std::optional<int> Document::FindTargetInShape(const IShape& shape, const Point point, const double time) const
{
	if (!shape.GetTimeInterval().IsContains(time))
	{
		return std::nullopt;
	}

	const GroupShape* group = shape.GetGroup();
	if (!group)
	{
		if (shape.HitTest(point))
		{
			return shape.GetId();
		}
		return std::nullopt;
	}

	for (size_t i = group->GetChildCount(); i > 0; --i)
	{
		const auto targetId = FindTargetInShape(group->GetChild(i - 1), point, time);
		if (targetId)
		{
			return targetId;
		}
	}
	return std::nullopt;
}

std::optional<size_t> Document::FindShapeIndex(const int id) const
{
	const auto it = std::ranges::find_if(m_shapesInner, [id](const auto& shape) { return shape->GetId() == id; });
	if (it == m_shapesInner.end())
	{
		return std::nullopt;
	}
	return static_cast<size_t>(it - m_shapesInner.begin());
}

std::optional<int> Document::FindTopShape(const Point point, const double time) const
{
	for (auto it = m_shapesInner.rbegin(); it != m_shapesInner.rend(); ++it)
	{
		if (FindTargetShape((*it)->GetId(), point, time).has_value())
		{
			return (*it)->GetId();
		}
	}
	return std::nullopt;
}

void Document::AddObserver(IDocumentObserver& observer)
{
	if (std::ranges::find(m_observers, &observer) == m_observers.end())
	{
		m_observers.push_back(&observer);
	}
}

void Document::RemoveObserver(IDocumentObserver& observer)
{
	std::erase(m_observers, &observer);
}

std::vector<std::unique_ptr<IShape>>::iterator Document::FindItem(const int id)
{
	return std::ranges::find_if(m_shapesInner, [id](const auto& shape) { return shape->GetId() == id; });
}

void Document::CheckFrame(const Frame& frame) const
{
	if (frame.m_width <= 0 || frame.m_height <= 0)
	{
		throw std::invalid_argument("Размеры фигуры должны быть положительными");
	}
	if (!m_bounds.IsContainsFrame(frame, BOUNDS_TOLERANCE))
	{
		throw std::invalid_argument("Фигура выходит за пределы холста");
	}
}

void Document::CheckTimeInterval(const TimeInterval& timeInterval) const
{
	if (timeInterval.m_start < 0 || timeInterval.m_end > m_duration || timeInterval.m_start >= timeInterval.m_end)
	{
		throw std::invalid_argument("Недопустимый интервал времени фигуры");
	}
}

void Document::CheckShape(const IShape& shape) const
{
	CheckFrame(shape.GetFrame());
	CheckTimeInterval(shape.GetTimeInterval());
}

void Document::CheckGrouping(const std::vector<int>& shapeIds, const IShape* group, const size_t index) const
{
	const GroupShape* composite = group ? group->GetGroup() : nullptr;
	if (!composite || composite->GetChildCount() != 0)
	{
		throw std::invalid_argument("Для группировки нужна пустая группа");
	}
	if (shapeIds.size() < 2)
	{
		throw std::invalid_argument("Для группировки нужны как минимум две фигуры");
	}
	for (size_t i = 0; i < shapeIds.size(); ++i)
	{
		if (!FindShapeIndex(shapeIds[i]))
		{
			throw std::invalid_argument("Фигуры группы должны быть объектами верхнего уровня");
		}
		const auto previousEnd = shapeIds.begin() + i;
		if (std::find(shapeIds.begin(), previousEnd, shapeIds[i]) != previousEnd)
		{
			throw std::invalid_argument("Фигуры группы должны быть разными");
		}
	}
	if (index > m_shapesInner.size() - shapeIds.size())
	{
		throw std::out_of_range("Недопустимая позиция группы");
	}
}

void Document::CheckUngrouping(const IShape* group, const std::vector<size_t>& childIndices) const
{
	const GroupShape* composite = group ? group->GetGroup() : nullptr;
	if (!composite)
	{
		throw std::invalid_argument("Группа не найдена");
	}
	if (childIndices.size() != composite->GetChildCount())
	{
		throw std::invalid_argument("Для каждого ребёнка должна быть задана позиция");
	}
	for (size_t i = 0; i < childIndices.size(); ++i)
	{
		if (childIndices[i] > m_shapesInner.size() - 1 + i)
		{
			throw std::out_of_range("Недопустимая позиция дочерней фигуры");
		}
		if (i > 0 && childIndices[i] <= childIndices[i - 1])
		{
			throw std::out_of_range("Позиции дочерних фигур должны идти по возрастанию");
		}
	}
}

void Document::NotifyObservers()
{
	const auto observers = m_observers;
	for (auto* observer : observers)
	{
		observer->DocumentUpdate();
	}
}
