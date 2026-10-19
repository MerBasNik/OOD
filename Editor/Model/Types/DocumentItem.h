//
// Created by Вадим Патрушев on 19.10.2026.
//

#ifndef OOD_DOCUMENTITEM_H
#define OOD_DOCUMENTITEM_H

#include "Model/Shapes/IShape.h"
#include <memory>

struct DocumentItem
{
	int m_id;
	std::unique_ptr<IShape> m_shape;
};

#endif //OOD_DOCUMENTITEM_H
