# Тестовое задание для Unreal Engine Developer

Шаблон ситибилдера по мотивам игры planetbase https://store.steampowered.com/app/403190/Planetbase/

![1](https://github.com/user-attachments/assets/1ce61ad4-7b36-4788-9735-805da59bd96c)


# Структура проекта: 

* Настройки проекта - Project Settings|Moon. Здесь хранятся указатели на различные ассеты - для удобного доступа из кода. 
(Source/Moon/Public/ProjectSettings/MoonProjectSettings.h)

* BuildsManager – отвечает за постройку/снос зданий, а также, расчет ресурсов на основе этих конструкций.
(Source/Moon/Public/SubSystems/BuildsManager.h)

* HUDManager - отвечает за создания виджетов/визуализаторов и их управлением.
(Source/Moon/Public/SubSystems/HUDManager.h)

* MeshesInstanceManager - отвечает за создания и управление экземплярами меша (mesh instance).
(Source/Moon/Public/SubSystems/MeshesInstanceManager.h)

* MoonPlayerController - отвечает за инпуты и управление самим игровым процессом.
(Source/Moon/Public/Main/MeshesInstanceManager.h)

* MoonFunctionLibrary - Библиотека различных методов.
(Source/Moon/Public/MoonFunctionLibrary.h)

# Таблицы зданий/моcтов хранятся по пути Content/Data/DataTables




