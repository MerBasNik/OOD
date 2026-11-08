//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_TIMEINTERVAL_H
#define OOD_TIMEINTERVAL_H

constexpr double DEFAULT_DURATION = 10;

struct TimeInterval
{
	double m_start = 0;
	double m_end = DEFAULT_DURATION;

	bool operator==(const TimeInterval& other) const
	{
		return m_start == other.m_start && m_end == other.m_end;
	}

	bool IsContains(const double time) const
	{
		return time >= m_start && time < m_end;
	}

	double GetDuration() const
	{
		return m_end - m_start;
	}
};

#endif // OOD_TIMEINTERVAL_H
