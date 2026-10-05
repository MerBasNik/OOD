//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_MOVEPICTURECOMMAND_H
#define OOD_MOVEPICTURECOMMAND_H

#include "Command/ICommand.h"
#include "Picture.h"

class MovePictureCommand : public ICommand
{
public:
	explicit MovePictureCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	Point m_position{};
};

#endif // OOD_MOVEPICTURECOMMAND_H
