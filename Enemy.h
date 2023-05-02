#pragma once

using namespace std;

class Enemy {
public:

    //Свойства врага.
    Enemy(string name, char tile, int level, int attack, int defense, int hp, int experience, int seekingRange) {
        _name = name;
        _tile = tile;
        _level = level;
        _attack = attack;
        _defense = defense;
        _health = hp;
        _experience = experience;
        _seekingRange = seekingRange;
    }

    //Атака
    int attack() {
        // 10% miss and 10% for critical attack!
        int random = rand() % 10;
        if (random == 0) return 0;
        if (random == 9) {
            printf("%s", "Critical hit! ");
            return _attack * 2;
        }

        return 1 + rand() % _attack;
    }

    //Метод получения урона.
    int takeDamage(int damage) {
        damage -= _defense;
        if (damage > 0) {
            _health -= damage;
            return damage;
        }
        return 0;
    }

    // Эта функция возвращает символ, представляющий направление движения врага к игроку или случайное направление,
    // если игрок находится вне зоны поиска врага
    char getMove(int playerX, int playerY) {

        int distance = abs(playerX - _x) + abs(playerY - _y);

        int dx = _x - playerX;
        int dy = _y - playerY;

        int adx = abs(dx);
        int ady = abs(dy);

        if (distance <= _seekingRange) {

            if (adx >= ady) {
                // движение по осе Х
                if (dx > 0) {
                    return 'a';
                } else {
                    return 'd';
                }
            } else {
               // движение по осе Y
               if (dy > 0) {
                   return 'w';
               } else {
                   return 's';
               }
            }
        }

        // 0 - 5
        int randomMove = rand() % 6;
        switch (randomMove) {
            case 0:
                return 'w';
            case 1:
                return 'a';
            case 2:
                return 's';
            case 3:
                return 'd';
            default:
                return '.';
        }
    }

    void setPosition(int x, int y) {
        _x = x;
        _y = y;
    }

    void getPosition(int &x, int &y) {
        x = _x;
        y = _y;
    }

    int getHP() {
        return _health;
    }

    int getEXP() {
        return _experience;
    }

    string getName() {
        return _name;
    }

    char getTile() {
        return _tile;
    }

private:
    // enemy info
    string _name;
    char _tile;
    int _x;
    int _y;

    // enemy stats
    int _level;
    int _attack;
    int _defense;
    int _health;
    int _experience;
    int _seekingRange;
};
