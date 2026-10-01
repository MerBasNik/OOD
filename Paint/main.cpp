//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Picture.h"
#include <iostream>

int main()
{
	Picture picture;

	auto shape = std::make_unique<IShape>();
	picture.AddShape(std::move(shape));
}