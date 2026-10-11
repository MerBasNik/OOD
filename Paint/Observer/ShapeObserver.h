//
// Created by Вадим Патрушев on 08.10.2026.
//

#ifndef OOD_SHAPEOBSERVER_H
#define OOD_SHAPEOBSERVER_H

#include <iostream>

class ShapeObserver
{
public:
	ShapeObserver() = default;
	static void Update()
	{
		std::cout << "Shape updated: " << std::endl;
	}
};

#endif //OOD_SHAPEOBSERVER_H
