#ifndef QUAKBEHAVIOR_H
#define QUAKBEHAVIOR_H

struct IQuackBehavior
{
	virtual ~IQuackBehavior() {};
	virtual void Quack() const = 0;
};

#endif
