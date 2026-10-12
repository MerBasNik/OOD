//
// Created by Вадим Патрушев on 09.10.2026.
//

#ifndef OOD_PICTUREOBSERVER_H
#define OOD_PICTUREOBSERVER_H

#include "IPictureObserver.h"
#include <iostream>

class PictureObserver : public IPictureObserver
{
public:
	PictureObserver() = default;
	void Update(const size_t countShapes) override
	{
		std::cout << "Picture updated. Shapes: " << countShapes << std::endl;
		m_countCalls++;
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
};

#endif // OOD_PICTUREOBSERVER_H
