//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_CHANGECOLOR_H
#define OOD_CHANGECOLOR_H
#include "Command/ICommand.h"



class ChangeColor : public ICommand
{
public:
	ChangeColor(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	Color m_color;
};



#endif //OOD_CHANGECOLOR_H
