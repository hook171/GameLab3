#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Player.h"
#include "Enemy.h"

using namespace std;

class Game {
public:
    Game() = default;

    //Функция load загружает информацию об уровне из текстового файла, заносит ее в массив _levelData
    //Затем инициализируются объекты игрока и врагов в зависимости от символов,содержащихся в _levelData
    //Сначала происходит очистка массива _levelData и инициализация переменной _currentLevel значением level.
    //Затем открывается файл, имя которого формируется на основе номера уровня,
    //и считываются строки из файла с помощью функции getline. Полученные строки добавляются в массив _levelData.
    void load(int level, Player & player) {
        _levelData.clear();
        _currentLevel = level;
        // Загрузка уровня с файла и занесение в массив строк
        ifstream file;
        string fileName = "C:/lab/roguelike-Rikucode-main/levels/Level" + to_string(_currentLevel) + ".txt";
        file.open(fileName);
        if (file.fail()) {
            perror(fileName.c_str());
            system("PAUSE");
            exit(-1);
        }

        string line;
        while (getline(file, line)) {
            _levelData.push_back(line);
        }

        file.close();
        _enemies.clear();
        // Пробег по карте для занесения информации о положении объектов
        unsigned char tile;
        for (int i = 0; i < _levelData.size(); ++i) {
            for (int j = 0; j < _levelData[i].size(); ++j) {
                tile = _levelData[i][j];
                switch (tile) {
                    // игрок
                    case '@' :
                        player.setPosition(j, i);
                        break;
                    // змея
                    case 's' :
                    case 'S' :
                        _enemies.push_back(Enemy("Snake", tile, 1, 5, 1, 10, 5, 3));
                        _enemies.back().setPosition(j, i);
                        break;
                    // гоблин
                    case 'g' :
                    case 'G' :
                        _enemies.push_back(Enemy("Goblin", tile, 2, 11, 3, 33, 15, 2));
                        _enemies.back().setPosition(j, i);
                        break;
                    // бандит
                    case 'b' :
                    case 'B' :
                        _enemies.push_back(Enemy("Bandit", tile, 3, 17, 11, 66, 100, 5));
                        _enemies.back().setPosition(j, i);
                        break;
                    // огр
                    case 'O':
                        _enemies.push_back(Enemy("Ogre", tile, 5, 40, 34, 150, 1500, 3));
                        _enemies.back().setPosition(j, i);
                        break;
                    // дракон
                    case 'D' :
                        _enemies.push_back(Enemy("Elder Dragon", tile, 100, 2000, 1500, 20000, 999999999, 0));
                        _enemies.back().setPosition(j, i);
                        break;
                }
            }
        }
    }

    void print() {
        for (int i = 0; i < _levelData.size(); ++i) {
            printf("%s", _levelData[i].c_str());
            printf("%s","\n");
        }
        printf("%s","\n");

    }
    // Функция отвечающая за перемещение игрока на игровом поле в зависимости от введенной пользователем команды.
    // помощью метода getPosition из класса Player получает текущие координаты игрока на игровом поле.
    // Затем в зависимости от введенного символа происходит вызов функции processPlayerMove,
    // которая обрабатывает передвижение игрока на новые координаты, а также вызывает функцию updateEnemies
    // для обновления состояния врагов на поле.
    void movePlayer(char input, Player &player) {

        int playerX;
        int playerY;

        player.getPosition(playerX, playerY);

        switch(input) {

                // move up
            case 'w':
            case 'W':
                processPlayerMove(player, playerX, playerY - 1);
                updateEnemies(player);
                break;

                // move down
            case 's':
            case 'S':
                processPlayerMove(player, playerX, playerY + 1);
                updateEnemies(player);
                break;

                // move right
            case 'd':
            case 'D':
                processPlayerMove(player, playerX + 1, playerY);
                updateEnemies(player);
                break;

                // move left
            case 'a':
            case 'A':
                processPlayerMove(player, playerX - 1, playerY);
                updateEnemies(player);
                break;

            default:
                printf("%s", "Invalid Input!\n\n");
                break;
        }
    }

    //Обновлении позиции врага на карте в зависимости от хода игрока.
    //Сначала функция получает позицию игрока, вызывая метод getPosition у объекта player,
    //передавая ему переменные playerX и playerY по ссылке. Затем функция перебирает всех врагов,
    //используя цикл for, и для каждого врага вызывает метод getMove, передавая ему позицию игрока playerX и playerY.
    //Метод getMove определяет, какой ход сделает враг, и возвращает символ, представляющий этот ход.
    void updateEnemies(Player &player) {

        char enemyMove;
        int playerX;
        int playerY;
        int enemyX;
        int enemyY;
        player.getPosition(playerX, playerY);

        for (int i = 0; i < _enemies.size(); ++i) {
            enemyMove = _enemies[i].getMove(playerX, playerY);
            _enemies[i].getPosition(enemyX, enemyY);
            switch (enemyMove) {
                // move up
                case 'w':
                    processEnemyMove(player, i, enemyX, enemyY - 1);
                    break;
                    // move down
                case 's':
                    processEnemyMove(player, i, enemyX, enemyY + 1);
                    break;
                    // move right
                case 'd':
                    processEnemyMove(player, i, enemyX + 1, enemyY);
                    break;
                    // move left
                case 'a':
                    processEnemyMove(player, i, enemyX - 1, enemyY);
                    break;
            }
        }
    }

    bool lookInventory(Player &player){
        printf("Your inventory:\n");
        string invStr;
        player.getInventory(invStr);
        for (int i = 0; i < invStr.size(); ++i) {
            string itemName;
            if(invStr[i] == 'H') {
                itemName = "Heal Potion";
            }
            if(invStr[i] == 'A') {
                itemName = "More Sharper sword";
            }
            if(invStr[i] == 'D') {
                itemName = "Stone Skin potion";
            }
            if(invStr[i] == 'X') {
                itemName = "EXP potion";
            }
            printf("%d)%s\n", i+1, itemName.c_str());
        }
        printf("Choose what you want to get by pressing a number, press something different to Return\n");
        int input;
        input = _getch();
        char item;
        switch (input) {
            case '1':
                if (invStr.empty()) {
                    return false;
                }
                item = invStr[0];
                useItem(item, player);
                invStr.erase(0,1);
                break;
            case '2':
                if (invStr.size() < 2) {
                    return false;
                }
                item = invStr[1];
                useItem(item, player);
                invStr.erase(1,1);
                break;
            case '3':
                if (invStr.size() < 3) {
                    return false;
                }
                item = invStr[2];
                useItem(item, player);
                invStr.erase(2,1);
                break;
            case '4':
                if (invStr.size() < 4) {
                    return false;
                }
                item = invStr[3];
                useItem(item, player);
                invStr.erase(3,1);
                break;
            case '5':
                if (invStr.size() < 5) {
                    return false;
                }
                item = invStr[4];
                useItem(item, player);
                invStr.erase(4,1);
                break;
            default:
                return false;
        }
        player.setInventory(invStr);
    }
    static void useItem(char ch, Player &player) {
        switch(ch) {
            case 'H':
                player.heal();
                printf("You drank a heal potion. Your HP has been restored!\n");
                break;
            case 'A':
                player.buffAttack();
                printf("You take a new, more sharper sword. Your attack has been increased!\n");
                break;
            case 'D':
                player.buffDefense();
                printf("You drank a stoneskin potion. Your defense has been increased!\n");
                break;
            case 'X':
                player.expBoost();
                printf("You drank an EXP potion. Your EXP has been increased!\n");
                break;

        }
    }

    //Возвращает тайл в зависимости от x,y
    char getTile (int x, int y) {
        return _levelData[y][x];
    };

    //Устанавливает tile по координатам
    void setTile (int x, int y, char tile) {
        _levelData[y][x] = tile;
    }

    static void clearScreen() {
        cout << string(40, '\n');
    }

private:

    //Функция которая обрабатывает движение игрока. Принимает ссылку на объект игрока,координаты целевой позиции, куда
    //игрок должен передвинуться.
    //Вычисляется позиция игрока, а также тайл, куда должен переместиться игрок
    //Потом запускается обработчик, где в зависимости от тайла выполняется действие.
    void processPlayerMove(Player &player, int targetX, int targetY) {
        int playerX;
        int playerY;
        player.getPosition(playerX, playerY);
        char nextTile = getTile(targetX, targetY);
        int random = rand() % 5;
        switch (nextTile) {
            case '#':
                break;
            case '.':
                player.setPosition(targetX, targetY);
                setTile(playerX, playerY, '.');
                setTile(targetX, targetY, '@');
                break;
            case 'H':
                printf("%s", "You have found a ladder to next floor\n");
                _currentLevel++;
                if (_currentLevel > 3) {
                    clearScreen();
                    printf("Congratulations! You have won!\n");
                    system("PAUSE");
                    exit(0);
                }
                load(_currentLevel, player);
                system("PAUSE");
                break;

            case 'T':
                if(random == 0) {
                    printf("You have found a heal potion!\n");
                    player.addToInventory('H');
                }
                if(random == 1) {
                    printf("You have found a more sharper sword!\n");
                    player.addToInventory('A');
                }
                if(random == 2) {
                    printf("You have found a stone skin potion!\n");
                    player.addToInventory('D');
                }
                if(random == 3) {
                    printf("You have found an EXP potion!\n");
                    player.addToInventory('X');
                }
                if(random == 4) {
                    printf("Sadly, the chest was already empty!\n");
                }
                player.setPosition(targetX, targetY);
                setTile(playerX, playerY, '.');
                setTile(targetX, targetY, '@');
                system("PAUSE");
                break;

            default:
                battleEnemy(player, true, targetX, targetY);
                break;
        }
    }

    //Функция, отвечающая за передвижение вражеских персонажей. Она принимает ссылку на объект класса игрока,
    //индекс врага, а также координаты куда должен переместиться враг.
    //Далее функция получает координаты игрока и врага, а также тип тайла, куда должен переместиться враг.
    //Если стенка то не проходит, если пустое место то перемещается, если игрок на вызывается функция боя.
    void processEnemyMove(Player &player, int enemyIndex, int targetX, int targetY) {

        int playerX;
        int playerY;
        int enemyX;
        int enemyY;

        _enemies[enemyIndex].getPosition(enemyX, enemyY);

        player.getPosition(playerX, playerY);

        char nextTile = getTile(targetX, targetY);

        switch (nextTile) {
            case '#':
                break;
            case '.':
                _enemies[enemyIndex].setPosition(targetX, targetY);
                setTile(enemyX, enemyY, '.');
                setTile(targetX, targetY, _enemies[enemyIndex].getTile());
                break;
            case '@':
                player.setPosition(enemyX, enemyY);
                battleEnemy(player, false, enemyX, enemyY);
                player.setPosition(targetX, targetY);
                break;
        }
    }

    //Функция атаки врага на игрока. Принимает ссылку на объект игрока, координаты цели, индекс врага в векторе
    //_enemies и флаг initiator.
    //Вызывает метод attack() у игрока, чтобы определить попадание.
    //Вызывается метод takeDamage для _enemies[i].
    //Если у врага осталось 0 хп, то он удаляется из вектора _enemies,
    //А также в зависимости от того кто инициатор, устанавливается тайл '.' в соответствующее место.
    bool playerAttack(Player &player, int targetX, int targetY, int i, bool initiator){
        int attackRoll;
        int attackResult;
        attackRoll = player.attack();
        if (attackRoll == 0) {
            printf("You attacked %s and missed! \n", _enemies[i].getName().c_str());
        } else {
            printf("You attacked %s with the roll of %d \n", _enemies[i].getName().c_str(), attackRoll);
        }
        attackResult = _enemies[i].takeDamage(attackRoll);
        printf("%s received %d damage!\n", _enemies[i].getName().c_str(), attackResult);
        if (_enemies[i].getHP() <= 0) {
            if (initiator) {
                setTile(targetX, targetY, '.');
            } else {
                int enemyX;
                int enemyY;
                _enemies[i].getPosition(enemyX, enemyY);
                setTile(enemyX, enemyY, '.');
            }
            printf("%s died. Player's victory!\n", _enemies[i].getName().c_str());
            player.addExp(_enemies[i].getEXP());
            _enemies.erase(_enemies.begin() + i);
            system("PAUSE");
            return true;
        }
        return false;
    }

    //Почти тоже самое что с playerattack
    void enemyAttack(Player &player, int playerX, int playerY, int i) {
        int attackRoll;
        int attackResult;
        attackRoll = _enemies[i].attack();
        if (attackRoll == 0) {
            printf("%s attacked you and missed! \n", _enemies[i].getName().c_str());
        } else {
            printf("%s attacked you with the roll of %d \n", _enemies[i].getName().c_str(), attackRoll);
        }
        attackResult = player.takeDamage(attackRoll);
        printf("You received %d damage!\n", attackResult);
        if (player.getHP() <= 0) {
            setTile(playerX, playerY, 'x');
            printf("You have been defeated!\n");
            system("PAUSE");
            clearScreen();
            printf("%s", "                 Game Over!");
            cout << string(10, '\n');
            system("PAUSE");
            exit(0);
        }
    }

    //Функция отвечающая за бой с врагами. На входе ссылка на объект класса Player, флаг initiator, координаты врага.
    //getPosition устанавливает координаты игрока в targetX и targetY.
    //Идет перебор вектора врагов, если i-ый _enemies совпадает, в зависимости от значения initiator,
    //ходит либо игрок либо враг. Если игрок то сначала атакует игрок, потом враг и наоборот.
    //Если в результате атаки врага или игрока здоровье становится меньше или равным нулю то враг умирает
    //и удаляется из _enemies
    void battleEnemy(Player &player, bool initiator, int targetX, int targetY) {

        int enemyX;
        int enemyY;
        int playerX;
        int playerY;

        player.getPosition(playerX, playerY);



        for (int i = 0; i < _enemies.size(); ++i) {
            _enemies[i].getPosition(enemyX, enemyY);
            if (targetX == enemyX && targetY == enemyY) {
                bool isVictory = false;
                // Battle starts here!
                if (initiator) {
                    // Player's turn
                    isVictory = playerAttack(player, targetX, targetY, i, true);
                    if (isVictory) return;
                    // Enemy turn
                    enemyAttack(player, playerX, playerY, i);
                } else {
                    // Enemy turn
                    enemyAttack(player, playerX, playerY, i);
                    // Player's turn
                    isVictory = playerAttack(player, targetX, targetY, i, true);
                    if (isVictory) return;
                }
                system("PAUSE");
                return;
            }
        }
    }
    vector<Enemy> _enemies;
    vector<string> _levelData;
    int _currentLevel;
};
