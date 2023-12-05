# Инструкция по установке dislin для Windows 11:
1) Установить [Python](https://www.python.org/downloads/);
2) Скачать [dislin](https://www.dislin.de/win64.html) - dl_11_py.zip;
3) Распаковать в папку рядом с Python-файлом, который использует библиотеку dislin;
4) В Python-файле, который использует библиотеку dislin, добавить следующий код `import dislin.dislin as dislin`;
5) Далее использовать функции из библиотеки dislin как `dislin.`, например `dislin.getver()`.

# Пример
Например если Python-файл, который использует библиотеку dislin, называется `main.py`, то структура католога должны быть следующей:

```
├───dislin
├───main.py
```

```
# main.py
import dislin.dislin as dislin
print("Версия dislin: {}".format(dislin.getver()))
```

```
# main.py out
Версия dislin: 11.5
```

# Возможные проблемы
## Не найден указанный модуль
Иногда код `import dislin.dislin as dislin` может завершится исключением `не найдет указанный модуль`.

## Решение
1) Переименовать файл `dislin.pyd` в `dislin_save.pyd`;
2) Переименовать файл `dislin{Python Version}.pyd` в `dislin.pyd`.

### Пример
Если установленная на персональном компьютере версия Python `3.10.7`, то ближайшая для него версия dislin находится в файле `dislin39.pyd`, этот файл необходимо переименовать в `dislin.pyd`. Главная (major, см. [Семантическое Версионирование 2.0.0](https://semver.org/lang/ru/)) версия Python должна совпадать, остальные нет.  
Команда для того чтобы узнать версию Python:
```
python.exe --version
```