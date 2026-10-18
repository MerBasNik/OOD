//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_IDOCUMENTOBSERVER_H
#define OOD_EDITOR_IDOCUMENTOBSERVER_H

class IDocumentObserver
{
public:
	virtual ~IDocumentObserver() = default;
	virtual void DocumentUpdate() = 0;
};

#endif // OOD_EDITOR_IDOCUMENTOBSERVER_H
