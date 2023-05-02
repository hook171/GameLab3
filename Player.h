#pragma once
#include <random>
#include <ctime>

using namespace std;

class Player {
public:

    Player() = default;

    //инициализация
    void init(int level, int health, int attack, int defense, int experience) {
        _level = level;
        _maxHealth = health;
        _health = _maxHealth;
        _attack = attack;
        _defense = defense;
        _experience = experience;
    }

    // TODO - рандом

    //метод возвращает урон который будет нанесен врагу.
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

    //Определяет урон полученный при атаке врага.
    int takeDamage(int damage) {
        damage -= _defense;
        if (damage > 0) {
            _health -= damage;
            return damage;
        }
        return 0;
    }

    //геттеры
    int getHP () {
        return _health;
    }

    int getMaxHP() {
        return _maxHealth;
    }

    int getEXP(){
        return _experience;
    }

    int getLevel() {
        return _level;
    }

    //метод восстановления здоровья
    void heal () {
        if (_health > _maxHealth * 0.5) {
            _health = _maxHealth;
        } else {
            _health += (int)(_maxHealth * 0.5);
        }
    }

    //атака под усилением
    void buffAttack() {
        _attack *= 1.5;
    }

    //увеличивает колво exp на 50 * _level
    void expBoost(){
        addExp(50*_level);
    }

    //Увеличивает защиту игрока в полтора раза.
    void buffDefense() {
        _defense *= 1.5;
    }

    //Увеличивает опыт игрока на заданное значение.
    //Если кол во опыта хватает на повышение лвл, то изменяем статы.
    void addExp(int experience) {
        _experience += experience;

        //level up
        if (_experience >= _level * 50) {
            printf("%s", "You have just leveled up! Stats increased!\n");
            _experience -= _level * 50;
            _attack += 10;
            _maxHealth += 20;
            _health = _maxHealth;
            _defense += 5;
            _level++;
            system("PAUSE");
        }
    }

    void setPosition(int x, int y) {
        _x = x;
        _y = y;
    }
    void getPosition(int &x, int &y) const {
        x = _x;
        y = _y;
    }

    void addToInventory(char ch) {
        if(inventory.size() > 5) {
            printf("You can't take it with you: your bag is already full!\n");
        }
        inventory.push_back(ch);
    }
    void setInventory(string str) {
        inventory = str;
    }
    void getInventory(string & str) {
        str = inventory;
    }

private:
    //Координаты
    int _x;
    int _y;

    //Свойства
    int _level;
    int _health;
    int _maxHealth;
    int _attack;
    int _defense;
    int _experience;
    //TODO
    string inventory;
};

