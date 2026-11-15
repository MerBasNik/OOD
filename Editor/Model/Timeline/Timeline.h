#ifndef OOD_TIMELINE_H
#define OOD_TIMELINE_H

#include "Model/Types/TimeInterval.h"
#include <map>

class IShape;

class Timeline
{
public:
	explicit Timeline(double duration = DEFAULT_DURATION);
	double GetDuration() const;
	TimeInterval GetInterval(const IShape& shape) const;
	void SetInterval(const IShape& shape, TimeInterval interval);
	std::map<int, TimeInterval> GetIntervals(const IShape& shape) const;
	const std::map<int, TimeInterval>& GetIntervals() const;
	void SetIntervals(const std::map<int, TimeInterval>& intervals);
	void RemoveShape(const IShape& shape);
	void Clear();

private:
	double m_duration;
	std::map<int, TimeInterval> m_intervals;

	void CheckInterval(TimeInterval interval) const;
	void CollectIntervals(const IShape& shape, std::map<int, TimeInterval>& intervals) const;
};

#endif
