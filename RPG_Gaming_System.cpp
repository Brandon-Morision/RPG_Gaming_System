#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>
using namespace std;

void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}


void printBorder(const string &title = "") {
    int width = 50;
    cout << string(width, '=') << endl;
    if (!title.empty()) {
        cout << "= " << setw((width / 2) + (title.size() / 2)) << left << title << right << string(width - title.size() - 4, ' ') << "=" << endl;
        cout << string(width, '=') << endl;
    }
}


void printStyled(const string &text, const string &style = "") {
    if (style == "bold") {
        cout << "\033[1m" << text << "\033[0m";
    } else if (style == "italic") {
        cout << "\033[3m" << text << "\033[0m";
    } else {
        cout << text;
    }
}

class Character {
protected:
    string name;
    int health;
    int attackPower;
    int specialCooldown; 

public:
    Character(string n, int h, int a) : name(n), health(h), attackPower(a), specialCooldown(0) {}

    void setHealth(int h) {
        health = h > 0 ? h : 0; 
    }

    void setAttackPower(int a) {
        attackPower = a > 0 ? a : attackPower; 
    }

    virtual void attack(Character &opponent) {
        opponent.setHealth(opponent.getHealth() - attackPower);
        cout << name << " attacks " << opponent.getName() << " for " << attackPower << " damage!" << endl;
    }

    virtual void specialMove(Character &opponent) = 0;

    bool isAlive() const {
        return health > 0;
    }

    string getName() const {
        return name;
    }

    int getHealth() const {
        return health;
    }

    int getAttackPower() const {
        return attackPower;
    }

    void reduceCooldown() {
        if (specialCooldown > 0) {
            specialCooldown--;
        }
    }

    bool canUseSpecialMove() const {
        return specialCooldown == 0;
    }

    void setSpecialCooldown(int turns) {
        specialCooldown = turns;
    }
};

// Derived class: Warrior
class Warrior : public Character {
public:
    Warrior(string n, int h, int a) : Character(n, h, a) {}

    void specialMove(Character &opponent) override {
        int damage = attackPower * 2;
        opponent.setHealth(opponent.getHealth() - damage);
        cout << name << " performs a Heavy Strike on " << opponent.getName()
             << " for " << damage << " damage!" << endl;
        setSpecialCooldown(2); 
    }
};


class Mage : public Character {
public:
    Mage(string n, int h, int a) : Character(n, h, a) {}

    void specialMove(Character &opponent) override {
        int damage = attackPower + 10;
        opponent.setHealth(opponent.getHealth() - damage);
        cout << name << " casts Fireball on " << opponent.getName()
             << " for " << damage << " damage!" << endl;
        setSpecialCooldown(3); // Set cooldown for 3 turns
    }
};


class Inventory {
private:
    vector<string> items;

public:
    void addItem(const string &item) {
        items.push_back(item);
        cout << item << " added to inventory!" << endl;
    }

    void useItem(int choice, Character &player) {
        if (choice > 0 && choice <= items.size()) {
            string item = items[choice - 1];
            if (item == "Health Potion") {
                player.setHealth(player.getHealth() + 20);
                items.erase(items.begin() + choice - 1);
                cout << "Used " << item << ". Health increased by 20!" << endl;
            } else if (item == "Attack Boost") {
                player.setAttackPower(player.getAttackPower() + 10);
                items.erase(items.begin() + choice - 1);
                cout << "Used " << item << ". Attack power increased by 10!" << endl;
            } else {
                cout << "Cannot use " << item << "!" << endl;
            }
        } else {
            cout << "Invalid item selection!" << endl;
        }
    }

    void showInventory() const {
        cout << "Inventory:" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << "  " << i + 1 << ") " << items[i] << endl;
        }
    }

    vector<string> getItems() const {
        return items;
    }
};


class Player : public Warrior {
private:
    int level;
    int experience;
    int experienceToNextLevel;

public:
    Player(string n, int h, int a) : Warrior(n, h, a), level(1), experience(0), experienceToNextLevel(50) {}

    void gainExperience(int xp) {
        experience += xp;
        cout << name << " gains " << xp << " XP!" << endl;
        if (experience >= experienceToNextLevel) {
            levelUp();
        }
    }

    void levelUp() {
        level++;
        experience -= experienceToNextLevel;
        experienceToNextLevel += 20; 
        setHealth(getHealth() + 20); 
        setAttackPower(getAttackPower() + 5); 
        cout << "\nCongratulations! " << name << " has leveled up to Level " << level << "!" << endl;
    }

    int getLevel() const {
        return level;
    }

    int getExperience() const {
        return experience;
    }

    int getExperienceToNextLevel() const {
        return experienceToNextLevel;
    }
};


void battle(Player &player, vector<Mage> &enemies, Inventory &playerInventory) {
    printBorder("Battle Begins!");

    for (size_t i = 0; i < enemies.size(); ++i) {
        Mage &enemy = enemies[i];
        cout << player.getName() << " (Level " << player.getLevel() << ") vs. " << enemy.getName() << endl;
        printBorder();

        while (player.isAlive() && enemy.isAlive()) {
            
            cout << "\n";
            cout << left << setw(20) << player.getName() << "| Health: [" << string(player.getHealth() / 5, '#') << string((100 - player.getHealth()) / 5, ' ') << "] " << player.getHealth() << endl;
            cout << left << setw(20) << enemy.getName() << "| Health: [" << string(enemy.getHealth() / 5, '#') << string((100 - enemy.getHealth()) / 5, ' ') << "] " << enemy.getHealth() << endl;

            
            cout << "\nYour turn! Choose an action:\n1) Regular Attack\n2) Special Move\n3) Use Item\n";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    player.attack(enemy);
                    break;
                case 2:
                    if (player.canUseSpecialMove()) {
                        player.specialMove(enemy);
                    } else {
                        cout << "Special move is on cooldown!" << endl;
                    }
                    break;
                case 3: {
                    playerInventory.showInventory();
                    cout << "Choose an item number to use: ";
                    int itemChoice;
                    cin >> itemChoice;
                    playerInventory.useItem(itemChoice, player);
                    break;
                }
                default:
                    cout << "Invalid choice! Turn skipped." << endl;
                    break;
            }

            
            if (!enemy.isAlive()) {
                cout << enemy.getName() << " is defeated! You gain 20 XP!" << endl;
                player.gainExperience(20);
                break;
            }

            
            cout << "\nEnemy's turn!" << endl;
            enemy.attack(player);

            
            if (!player.isAlive()) {
                cout << player.getName() << " is defeated! Game Over!" << endl;
                return;
            }

            
            player.reduceCooldown();
            enemy.reduceCooldown();
        }
    }

    printBorder("Battle Over");
    cout << "You defeated all enemies! Congratulations, " << player.getName() << "!" << endl;
}


int main() {
    printBorder("Welcome to the RPG Gaming System!");

    Player player("Hero", 100, 15);
    vector<Mage> enemies = {
        Mage("Dark Wizard", 80, 12),
        Mage("Necromancer", 90, 14),
        Mage("Archmage", 100, 16)
    };

    Inventory playerInventory;
    playerInventory.addItem("Health Potion");
    playerInventory.addItem("Attack Boost");

    battle(player, enemies, playerInventory);

    return 0;
}
