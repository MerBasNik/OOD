//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Document.h"
#include <algorithm>
#include <stdexcept>

constexpr double BOUNDS_TOLERANCE = 1e-6;

Document::Document(const double width, const double height)
	: m_bounds{ 0, 0, width, height }
{
}

Frame Document::GetBounds() const
{
	return m_bounds;
}

int Document::AddShape(std::unique_ptr<IShape> shape)
{
	if (!shape)
	{
		throw std::invalid_argument("Фигура не задана");
	}
	CheckFrame(shape->GetFrame());

	const int id = m_nextId++;
	m_shapesInner.push_back({ id, std::move(shape) });
	NotifyObservers();
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

void Document::SetShapeFrame(const int id, const Frame& frame)
{
	const auto it = FindItem(id);
	if (it == m_shapesInner.end())
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	CheckFrame(frame);
	if (it->m_shape->GetFrame() == frame)
	{
		return;
	}
	it->m_shape->SetFrame(frame);
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
	const auto target = m_shapesInner.begin() + static_cast<std::ptrdiff_t>(index);
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
		CheckFrame(shape->GetFrame());
	}

	std::vector<DocumentItem> items;
	items.reserve(shapes.size());
	for (auto& shape : shapes)
	{
		items.push_back({ m_nextId++, std::move(shape) });
	}
	m_shapesInner = std::move(items);
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
	return *m_shapesInner.at(index).m_shape;
}

int Document::GetShapeId(const size_t index) const
{
	return m_shapesInner.at(index).m_id;
}

const IShape* Document::FindShape(const int id) const
{
	const auto it = std::ranges::find(m_shapesInner, id, &DocumentItem::m_id);
	return it != m_shapesInner.end() ? it->m_shape.get() : nullptr;
}

std::optional<size_t> Document::FindShapeIndex(const int id) const
{
	const auto it = std::ranges::find(m_shapesInner, id, &DocumentItem::m_id);
	if (it == m_shapesInner.end())
	{
		return std::nullopt;
	}
	return static_cast<size_t>(it - m_shapesInner.begin());
}

std::optional<int> Document::FindTopShape(const Point point) const
{
	for (auto it = m_shapesInner.rbegin(); it != m_shapesInner.rend(); ++it)
	{
		if (it->m_shape->HitTest(point))
		{
			return it->m_id;
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

std::vector<DocumentItem>::iterator Document::FindItem(const int id)
{
	return std::ranges::find(m_shapesInner, id, &DocumentItem::m_id);
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

void Document::NotifyObservers()
{
	const auto observers = m_observers;
	for (auto* observer : observers)
	{
		observer->DocumentUpdate();
	}
}
