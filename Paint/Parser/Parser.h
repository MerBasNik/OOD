//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_PARSER_H
#define OOD_PARSER_H

#include "Command/ICommand.h"
#include <sstream>

class Parser
{
public:
	std::unique_ptr<ICommand> ParseCommand(std::stringstream& input);
};

#endif // OOD_PARSER_H
