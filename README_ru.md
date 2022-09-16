# Dimon Run

**Dimon run** - некоммерческая 2D игра, разработанная командой DreamTeam. 
Игра написана на языке C++ с использованием [Qt Framework](https://www.qt.io/) версии и концепции Entity Component System.

Игра является учебным проектом студентов 5 группы 1 курса Факультета Прикладной Математики и Информатики БГУ (2 семестр 2021-2022).

Участники команды:

  * Егор Тилигузов([@tiliguzov](https://github.com/tiliguzov))
  * Штанов Илья([@ManFormTheMoon](https://github.com/ManFormTheMoon))
  * Сивакова Кристина([@CCivakova](https://github.com/CCivakova))
  * Семенов Михаил([@Marmotikon](https://github.com/Marmotikon))

Люди, которые не повелись на деньги:

  * Егор Ходасевич ([@Khody31](https://img.shields.io/badge/--twitter?label=Twitter&logo=Twitter&style=social))
  * Максим Мясоеденков([@tercerss](https://github.com/ternerss))
  * Максим Лицкевич([@Zzzabiyaka](https://github.com/Zzzabiyaka))

Лучший художник, ни разу не отказавший в помощи:

  * Ильина Елизавета

## Описание игры

Игроку предлагается 
побыть ученым будущего, создавшим машину времени-джетпак, который отправиться в 
прошлое в поисках легкого заработка. Чтобы добраться до сокровищ, ему придется 
воспользоваться смекалкой. Возможно, у него не будет получаться с первого раза, 
но с помощью своего изобретения он может попасть в один и тот же момент времени 
неограниченное количество раз.

В игре также присутствует возможность создавать свои уровни,
и тут же поиграть в них.

## Статус разработки

[![ci](https://github.com/tiliguzov/DimonRun/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/tiliguzov/DimonRun/actions/workflows/ci.yml)

Разработка игры была начата в марте 2022 года. Активная фаза завершилась в июне 2022.

## Установка игры

Если вы хотите собрать проект в среде разработки Clion, вам для начала необходимо клонировать репозиторий.

Далее для корректной работы скрипта необходимо указать путь к соответствующим библиотекам Qt на своем компьютере (установить соответствующее значение переменной CMAKE_PREFIX_PATH). Для этого в CMake-скрипте необходимо добавить строку set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake) на Linux или set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.13.0/5.13.0/mingw73_64/lib/cmake") на Windows.

После этого на Linux проект можно импортировать в CLion для дальнейшей работы. Под Windows необходимы дополнительные действия. Нужно указать в Path путь к компилятору в составе Qt, а также путь к библиотекам Qt. Например:

C:\Qt\Tools\mingw810_64\bin

C:\Qt\5.15.2\mingw81_64\bin

После внесения изменений в Path необходимо перезагрузить Windows. Далее рекомендуется выбрать в CLion компилятор, входящий в состав Qt (указать путь к нему в Settings - Toolchains). После этого можно работать с проектом.