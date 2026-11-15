#include "Timeline.h"
#include "Model/Shapes/GroupShape.h"
#include <algorithm>
#include <stdexcept>

Timeline::Timeline(const double duration)
	: m_duration(duration)
{
	if (duration <= 0)
	{
		throw std::invalid_argument("Длительность документа должна быть положительной");
	}
}

double Timeline::GetDuration() const
{
	return m_duration;
}

TimeInterval Timeline::GetInterval(const IShape& shape) const
{
	if (const GroupShape* group = shape.GetGroup())
	{
		if (group->GetChildCount() == 0)
		{
			return { 0, 0 };
		}
		TimeInterval interval = GetInterval(group->GetChild(0));
		for (size_t i = 1; i < group->GetChildCount(); ++i)
		{
			const TimeInterval child = GetInterval(group->GetChild(i));
			interval.m_start = std::min(interval.m_start, child.m_start);
			interval.m_end = std::max(interval.m_end, child.m_end);
		}
		return interval;
	}
	const auto found = m_intervals.find(shape.GetId());
	return found == m_intervals.end() ? TimeInterval{ 0, m_duration } : found->second;
}

void Timeline::SetInterval(const IShape& shape, const TimeInterval interval)
{
	CheckInterval(interval);
	if (!shape.GetGroup())
	{
		m_intervals.insert_or_assign(shape.GetId(), interval);
		return;
	}
	const TimeInterval previous = GetInterval(shape);
	if (previous.GetDuration() <= 0)
	{
		throw std::invalid_argument("Длительность группы должна быть положительной");
	}
	auto intervals = GetIntervals(shape);
	const double scale = interval.GetDuration() / previous.GetDuration();
	for (auto& [id, child] : intervals)
	{
		child = {
			interval.m_start + (child.m_start - previous.m_start) * scale,
			interval.m_start + (child.m_end - previous.m_start) * scale,
		};
	}
	SetIntervals(intervals);
}

std::map<int, TimeInterval> Timeline::GetIntervals(const IShape& shape) const
{
	std::map<int, TimeInterval> intervals;
	CollectIntervals(shape, intervals);
	return intervals;
}

const std::map<int, TimeInterval>& Timeline::GetIntervals() const
{
	return m_intervals;
}

void Timeline::CollectIntervals(const IShape& shape, std::map<int, TimeInterval>& intervals) const
{
	if (const GroupShape* group = shape.GetGroup())
	{
		for (size_t i = 0; i < group->GetChildCount(); ++i)
		{
			CollectIntervals(group->GetChild(i), intervals);
		}
	}
	else
	{
		intervals.emplace(shape.GetId(), GetInterval(shape));
	}
}

void Timeline::SetIntervals(const std::map<int, TimeInterval>& intervals)
{
	for (const auto& [id, interval] : intervals)
	{
		CheckInterval(interval);
	}
	for (const auto& [id, interval] : intervals)
	{
		m_intervals.insert_or_assign(id, interval);
	}
}

void Timeline::RemoveShape(const IShape& shape)
{
	if (const GroupShape* group = shape.GetGroup())
	{
		for (size_t i = 0; i < group->GetChildCount(); ++i)
		{
			RemoveShape(group->GetChild(i));
		}
	}
	else
	{
		m_intervals.erase(shape.GetId());
	}
}

void Timeline::Clear()
{
	m_intervals.clear();
}

void Timeline::CheckInterval(const TimeInterval interval) const
{
	if (interval.m_start < 0 || interval.m_end > m_duration || interval.m_start >= interval.m_end)
	{
		throw std::invalid_argument("Недопустимый интервал времени фигуры");
	}
}
