//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_IDANCEBEHAVIOR_H
#define OOD_IDANCEBEHAVIOR_H

struct IDanceBehavior
{
	virtual ~IDanceBehavior() {}
	virtual void Dance() const = 0;
};

#endif // OOD_IDANCEBEHAVIOR_H
