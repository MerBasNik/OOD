//
// Created by Вадим Патрушев on 09.10.2026.
//

#ifndef OOD_PICTUREOBSERVERUNSUBANDSUB_H
#define OOD_PICTUREOBSERVERUNSUBANDSUB_H

#include "IShapeObserver.h"
#include "Picture.h"
#include <iostream>

class PictureObserverUnsubAndSub : public IPictureObserver
{
public:
	PictureObserverUnsubAndSub(PictureObserver& observer, Picture* picture)
		: m_observer(observer)
		, m_picture(picture) {};

	void Update(const size_t countShapes) override
	{
		std::cout << "Picture updated. Shapes: " << countShapes << std::endl;
		m_countCalls++;
		m_picture->DeleteObserver(m_observer);
		m_picture->AddObserver(m_observer);
	}

	int GetCountCalls() const override
	{
		return m_countCalls;
	}

	void ResetCountCalls() override
	{
		m_countCalls = 0;
	}

private:
	int m_countCalls = 0;
	IPictureObserver& m_observer;
	Picture* m_picture;
};

#endif // OOD_PICTUREOBSERVERUNSUBANDSUB_H
