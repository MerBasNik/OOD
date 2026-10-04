//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_MOVESHAPECOMMAND_H
#define OOD_MOVESHAPECOMMAND_H

#include "../ICommand.h"
#include "Shape.h"

class MoveShapeCommand : public ICommand
{
public:
	explicit MoveShapeCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	Point m_position{};
};

#endif //OOD_MOVESHAPECOMMAND_H
