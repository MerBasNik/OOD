//
// Created by Вадим Патрушев on 01.11.2026.
//

#ifndef OOD_EDITOR_CHANGESHAPECOLORSCOMMAND_H
#define OOD_EDITOR_CHANGESHAPECOLORSCOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"
#include <optional>

class ChangeShapeColorsCommand : public ICommand
{
public:
	ChangeShapeColorsCommand(Document& document, int shapeId, Color fillColor, Color borderColor);
	void Execute() override;
	void Unexecute() override;
	std::vector<int> GetShapeIds() const override;

private:
	Document& m_document;
	int m_shapeId;
	Color m_fillColor;
	Color m_borderColor;
	std::optional<Color> m_oldFillColor;
	std::optional<Color> m_oldBorderColor;
};

#endif // OOD_EDITOR_CHANGESHAPECOLORSCOMMAND_H
