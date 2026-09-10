#ifndef QUAKBEHAVIOR_H
#define QUAKBEHAVIOR_H

struct IQuackBehavior
{
	virtual ~IQuackBehavior(){};
	virtual void Quack() = 0;
};

#endif
