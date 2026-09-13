//
// Created by Вадим Патрушев on 12.09.2026.
//

#ifndef OOD_IFLYACTION_H
#define OOD_IFLYACTION_H

struct IFlyAction
{
	virtual void FlyAction() = 0;
	virtual ~IFlyAction() = default;
};

#endif // OOD_IFLYACTION_H
