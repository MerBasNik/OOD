//
// Created by Вадим Патрушев on 09.10.2026.
//

#ifndef OOD_PICTUREOBSERVER_H
#define OOD_PICTUREOBSERVER_H

#include <iostream>

class PictureObserver
{
public:
	PictureObserver() = default;
	void Update(const size_t countShapes)
	{
		std::cout << "Picture updated. Shapes: " << countShapes << std::endl;
		m_countCalls++;
	}

	int GetCountCalls() const
	{
		return m_countCalls;
	}

private:
	int m_countCalls = 0;
};

#endif // OOD_PICTUREOBSERVER_H
