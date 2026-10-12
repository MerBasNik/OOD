//
// Created by Вадим Патрушев on 12.10.2026.
//

#ifndef OOD_ISHAPEOBSERVER_H
#define OOD_ISHAPEOBSERVER_H

class IShapeObserver
{
public:
	virtual ~IShapeObserver() = default;
	virtual void Update() = 0;
	virtual void ResetCountCalls() = 0;
};

#endif // OOD_ISHAPEOBSERVER_H
