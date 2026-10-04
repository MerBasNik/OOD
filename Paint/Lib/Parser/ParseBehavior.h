//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_PARSEBEHAVIOR_H
#define OOD_PARSEBEHAVIOR_H

#include "IShapeBehavior.h"
#include <iosfwd>

std::unique_ptr<IShapeBehavior> ParseBehavior(const std::string& typeName, std::stringstream& params);

#endif //OOD_PARSEBEHAVIOR_H
