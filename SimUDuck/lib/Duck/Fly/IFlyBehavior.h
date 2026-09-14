#ifndef LAB01_SIMUDUCK_IFLYBEHAVIOR_H
#define LAB01_SIMUDUCK_IFLYBEHAVIOR_H

class IFlyBehavior
{
public:
	virtual ~IFlyBehavior() = default;
	virtual void Fly() = 0;
	virtual int GetCount() const
	{
		return m_count;
	}
private:
	int m_count = 0;
};

#endif
