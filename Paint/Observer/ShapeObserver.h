//
// Created by Вадим Патрушев on 08.10.2026.
//

#ifndef OOD_SHAPEOBSERVER_H
#define OOD_SHAPEOBSERVER_H

#include "IShapeObserver.h"

#include <iostream>

class ShapeObserver : public IShapeObserver
{
public:
	ShapeObserver() = default;
	void Update() override
	{
		std::cout << "Shape updated: " << std::endl;
		m_countCalls++;
	}

	int GetCountCalls() const
	{
		return m_countCalls;
	}

private:
	int m_countCalls = 0;
};

#endif // OOD_SHAPEOBSERVER_H
