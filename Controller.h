#pragma once

#include <conio.h>
//заголовочный файл для обработки нажатых клавиш с клавиатуры.
#include <thread>
#include <utility>
#include "Player.h"
#include "Game.h"
#include "Enemy.h"

using namespace std;

class Controller {
public:

    //Конструктор. создается объект игры _level с уровнем,
    //И объект игрока.
    Controller(int level) {
        _player.init(1, 100, 10, 10, 0);
        _level.load(level, _player);
    }

    //Метод отвечающий за процесс игры и запускается в бесконечном цикле, пока onRun = true,
    //На каждой итерации цикла метод выводит на экран игровое поле,здоровье, опыт игрока,
    //а затем ждет пользовательского ввода
    [[noreturn]] void playGame() {
        srand((unsigned)time(nullptr));
        bool onRun = true;
        cout << _title;
        system("PAUSE");

        while (onRun) {
            Game::clearScreen();

            printf("%s",_legend.c_str());
            _level.print();
            printf("Health: %d / %d\n", _player.getHP(), _player.getMaxHP());
            printf("EXP: %d / %d\n", _player.getEXP(), 50 * _player.getLevel());
            char input;
            input = _getch();

            if (input == 'w' || input == 'W' || input == 'a' || input == 'A' ||
            input == 's' || input == 'S' || input == 'd' || input == 'D'){
                playerMove(input);
            }

            if (input == 'i' || input == 'I') {
                Game::clearScreen();
                bool onInv = true;
                while (onInv) {
                    onInv = _level.lookInventory(_player);
                }
            }

            // задержка чтобы удерживая клавишу, все успевало прорисовываться плавнее
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    //Используется для перемещения игрока в зависимости отправленной команды.
    void playerMove(char command) {
        _level.movePlayer(command, _player);
    }

private:
    string _title = "$$$$$___$$$$___$$$$__$$__$$_$$$$$_________$$_____$$$$$$_$$__$$_$$$$$\n"
                   "$$__$$_$$__$$_$$_____$$__$$_$$____________$$_______$$___$$_$$__$$\n"
                   "$$$$$__$$__$$_$$_$$$_$$__$$_$$$$___$$$$$__$$_______$$___$$$$___$$$$\n"
                   "$$__$$_$$__$$_$$__$$_$$__$$_$$____________$$_______$$___$$_$$__$$\n"
                   "$$__$$__$$$$___$$$$___$$$$__$$$$$_________$$$$$$_$$$$$$_$$__$$_$$$$$\n\n";
    string _legend = "Use WASD for movement, I for Inventory \nEnemies:\nS - Snake   G - goblin  B - bandit  O - Ogre    D - Dragon\nOther stuff:\nH - Ladder  T - Treasure\n";
    Game _level;
    Player _player;
};