//
// Created by Вадим Патрушев on 12.09.2026.
//

#ifndef OOD_IFLYOBSERVER_H
#define OOD_IFLYOBSERVER_H

struct IFlyObserver
{
	virtual void FlyAction() = 0;
	virtual ~IFlyObserver() = default;
};

#endif //OOD_IFLYOBSERVER_H
