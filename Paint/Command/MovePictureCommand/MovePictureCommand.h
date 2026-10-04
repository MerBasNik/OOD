//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_MOVEPICTURECOMMAND_H
#define OOD_MOVEPICTURECOMMAND_H

#include "Picture.h"
#include "Command/ICommand.h"

class MovePictureCommand : public ICommand {
public:
	explicit MovePictureCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	double m_dx;
	double m_dy;
};

#endif //OOD_MOVEPICTURECOMMAND_H
