//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "GroupShape.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

GroupShape::GroupShape(std::vector<std::unique_ptr<IShape>> children)
{
	for (auto& child : children)
	{
		AddChild(std::move(child));
	}
}

size_t GroupShape::GetChildCount() const
{
	return m_children.size();
}

const IShape& GroupShape::GetChild(const size_t index) const
{
	return *m_children.at(index);
}

void GroupShape::AddChild(std::unique_ptr<IShape> child)
{
	if (!child)
	{
		throw std::invalid_argument("Фигура не задана");
	}
	m_children.push_back(std::move(child));
}

std::vector<std::unique_ptr<IShape>> GroupShape::ReleaseChildren()
{
	return std::exchange(m_children, {});
}

bool GroupShape::HitTest(const Point point) const
{
	return std::ranges::any_of(m_children, [point](const auto& child) {
		return child->HitTest(point);
	});
}

std::unique_ptr<IShape> GroupShape::Clone() const
{
	auto clone = std::make_unique<GroupShape>();
	for (const auto& child : m_children)
	{
		clone->AddChild(child->Clone());
	}
	return clone;
}

Frame GroupShape::GetFrame() const
{
	if (m_children.empty())
	{
		return { 0, 0, 0, 0 };
	}
	const Frame first = m_children.front()->GetFrame();
	double left = first.m_left;
	double top = first.m_top;
	double right = first.GetRight();
	double bottom = first.GetBottom();
	for (const auto& child : m_children)
	{
		const Frame frame = child->GetFrame();
		left = std::min(left, frame.m_left);
		top = std::min(top, frame.m_top);
		right = std::max(right, frame.GetRight());
		bottom = std::max(bottom, frame.GetBottom());
	}
	return { left, top, right - left, bottom - top };
}

void GroupShape::SetFrame(const Frame& frame)
{
	const Frame oldFrame = GetFrame();
	if (oldFrame.m_width <= 0 || oldFrame.m_height <= 0 || frame.m_width <= 0 || frame.m_height <= 0)
	{
		throw std::invalid_argument("Размеры группы должны быть положительными");
	}
	const double oldRight = oldFrame.m_left + oldFrame.m_width;
	const double oldBottom = oldFrame.m_top + oldFrame.m_height;
	for (const auto& child : m_children)
	{
		const Frame childFrame = child->GetFrame();
		const double left = Scale(childFrame.m_left, oldFrame.m_left, oldRight, frame.m_left, frame.GetRight());
		const double top = Scale(childFrame.m_top, oldFrame.m_top, oldBottom, frame.m_top, frame.GetBottom());
		const double right = Scale(childFrame.GetRight(), oldFrame.m_left, oldRight, frame.m_left, frame.GetRight());
		const double bottom = Scale(childFrame.GetBottom(), oldFrame.m_top, oldBottom, frame.m_top, frame.GetBottom());
		child->SetFrame({ left, top, right - left, bottom - top });
	}
}

Color GroupShape::GetFillColor() const
{
	return m_children.empty() ? 0 : m_children.front()->GetFillColor();
}

Color GroupShape::GetBorderColor() const
{
	return m_children.empty() ? 0 : m_children.front()->GetBorderColor();
}

void GroupShape::SetFillColor(const Color color)
{
	for (const auto& child : m_children)
	{
		child->SetFillColor(color);
	}
}

void GroupShape::SetBorderColor(const Color color)
{
	for (const auto& child : m_children)
	{
		child->SetBorderColor(color);
	}
}

void GroupShape::Accept(IShapeVisitor& visitor) const
{
	visitor.Visit(*this);
}

TimeInterval GroupShape::GetTimeInterval() const
{
	if (m_children.empty())
	{
		return { 0, 0 };
	}
	TimeInterval interval = m_children.front()->GetTimeInterval();
	for (const auto& child : m_children)
	{
		const TimeInterval childInterval = child->GetTimeInterval();
		interval.m_start = std::min(interval.m_start, childInterval.m_start);
		interval.m_end = std::max(interval.m_end, childInterval.m_end);
	}
	return interval;
}

void GroupShape::SetTimeInterval(const TimeInterval timeInterval)
{
	const TimeInterval oldInterval = GetTimeInterval();
	if (oldInterval.GetDuration() <= 0 || timeInterval.GetDuration() <= 0)
	{
		throw std::invalid_argument("Длительность группы должна быть положительной");
	}
	for (const auto& child : m_children)
	{
		const TimeInterval childInterval = child->GetTimeInterval();
		child->SetTimeInterval({
			Scale(childInterval.m_start, oldInterval.m_start, oldInterval.m_end, timeInterval.m_start, timeInterval.m_end),
			Scale(childInterval.m_end, oldInterval.m_start, oldInterval.m_end, timeInterval.m_start, timeInterval.m_end),
		});
	}
}

GroupShape* GroupShape::GetGroup()
{
	return this;
}

const GroupShape* GroupShape::GetGroup() const
{
	return this;
}

double GroupShape::Scale(const double value, const double oldStart, const double oldEnd, const double newStart,
	const double newEnd)
{
	if (value == oldStart)
	{
		return newStart;
	}
	if (value == oldEnd)
	{
		return newEnd;
	}
	return newStart + (value - oldStart) * (newEnd - newStart) / (oldEnd - oldStart);
}
