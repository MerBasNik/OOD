//
// Created by Вадим Патрушев on 12.10.2026.
//

#ifndef OOD_IPICTUREOBSERVER_H
#define OOD_IPICTUREOBSERVER_H
// TODO: избавиться в интерфейсе от ResetCount and GetCount
class IPictureObserver
{
public:
	virtual ~IPictureObserver() = default;
	virtual void Update(size_t countShapes) = 0;
	virtual int GetCountCalls() const = 0;
	virtual void ResetCountCalls() = 0;
};

#endif // OOD_IPICTUREOBSERVER_H
