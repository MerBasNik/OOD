//
// Created by Вадим Патрушев on 19.10.2026.
//

#ifndef OOD_EDITOR_RESIZECASE_H
#define OOD_EDITOR_RESIZECASE_H

#include "Model/Types/Frame.h"
#include "Model/Types/Point.h"
#include <string>

struct ResizeCase
{
	std::string m_name;
	Point m_handle;
	Point m_offset;
	Frame m_expected;
};

#endif // OOD_EDITOR_RESIZECASE_H
