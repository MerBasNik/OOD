//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_IEDITORSTATEOBSERVER_H
#define OOD_EDITOR_IEDITORSTATEOBSERVER_H

class IEditorStateObserver
{
public:
	virtual ~IEditorStateObserver() = default;
	virtual void EditorStateUpdate() = 0;
};

#endif // OOD_EDITOR_IEDITORSTATEOBSERVER_H
