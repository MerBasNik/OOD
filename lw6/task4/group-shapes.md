# Задание 4

## Группировка фигур и Undo

| Момент            | Документ               | Выделение |
|-------------------|------------------------|-----------|
| До группировки    | A, B, C, D(E, F), H    | D и B     |
| После группировки | A, C, H, G(B, D(E, F)) | G         |
| После Undo        | A, B, C, D(E, F), H    | B и D     |

## Группировка

1. По Cmd+G окно вызывает контроллер, тот читает выделение из EditorState и проверяет
   id в списке фигур в документе. Контроллер сбрасывает прежнее preview и создает одну команду
   группировки. Группировка применяется сразу при выполнении команды
   [Обработка клавиш](../../Editor/View/Window/EditorWindow.cpp),
   [Подготовка команды](../../Editor/Controller/EditorController.cpp)
2. История вызывает команду, которая запоминает индексы B и D и сортирует их id по этим индексам.
   Документ извлекает B и D, помещает их в G в изначальном порядке и вставляет группу поверх трех
   оставшихся фигур. Команда получает id созданной группы
   [Выполнение команды](../../Editor/Command/GroupShapesCommand/GroupShapesCommand.cpp),
   [Сохранение индексов](../../Editor/Command/GroupShapesCommand/GroupShapesCommand.cpp),
   [Изменение дерева](../../Editor/Model/Document/Document.cpp)
3. После Execute в историю добавляется команда. Выделяется G
   [Запись в историю](../../Editor/Command/CommandHistory/CommandHistory.cpp),
   [Обновление документа](../../Editor/Controller/EditorController.cpp),
   [Выделение группы](../../Editor/Controller/EditorController.cpp)
4. История вызывает Unexecute у той же команды. По сохраненным индексам документ возвращает B
   между A и C, а D между C и H. История сдвигает позицию Undo и возвращает id освобожденных фигур,
   а контроллер выделяет B и D. Отдельный вызов пользователем
   [Отмена группировки](../../Editor/Command/GroupShapesCommand/GroupShapesCommand.cpp),
   [Вставка по индексам](../../Editor/Model/Document/Document.cpp),
   [Undo контроллера](../../Editor/Controller/EditorController.cpp)
