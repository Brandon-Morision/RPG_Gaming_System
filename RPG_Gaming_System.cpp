#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


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

    void useItem(const string &item, Character &player) {
        auto it = find(items.begin(), items.end(), item);
        if (it != items.end()) {
            if (item == "Health Potion") {
                player.setHealth(player.getHealth() + 20);
                items.erase(it);
                cout << "Used " << item << ". Health increased by 20!" << endl;
            } else if (item == "Attack Boost") {
                player.setAttackPower(player.getAttackPower() + 10);
                items.erase(it);
                cout << "Used " << item << ". Attack power increased by 10!" << endl;
            } else {
                cout << "Cannot use " << item << "!" << endl;
            }
        } else {
            cout << "Item not found in inventory!" << endl;
        }
    }

    void showInventory() const {
        cout << "Inventory: ";
        for (const auto &item : items) {
            cout << item << " ";
        }
        cout << endl;
    }

    vector<string> getItems() const {
        return items;
    }
};


void battle(Character &player, Character &enemy, Inventory &playerInventory) {
    cout << "Battle starts between " << player.getName() << " and " << enemy.getName() << "!" << endl;

    while (player.isAlive() && enemy.isAlive()) {
        cout << "\n" << player.getName() << " Health: " << player.getHealth()
             << " | " << enemy.getName() << " Health: " << enemy.getHealth() << endl;

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
                    cout << "Special move is on cooldown for " << player.getAttackPower() << " turns!" << endl;
                }
                break;
            case 3: {
                cout << "Choose an item to use: ";
                playerInventory.showInventory();
                string item;
                cin >> item;
                playerInventory.useItem(item, player);
                break;
            }
            default:
                cout << "Invalid choice! Turn skipped." << endl;
                break;
        }

        
        if (!enemy.isAlive()) {
            cout << enemy.getName() << " is defeated! You win the battle!" << endl;
            break;
        }

        
        enemy.attack(player);

        // Check if player is defeated
        if (!player.isAlive()) {
            cout << player.getName() << " is defeated! Game Over!" << endl;
            break;
        }

        
        player.reduceCooldown();
        enemy.reduceCooldown();
    }
}


int main() {
    cout << "Welcome to the RPG Gaming System!" << endl;

    Warrior player("Hero", 100, 15);
    Mage enemy("Dark Wizard", 80, 12);

    Inventory playerInventory;
    playerInventory.addItem("Sword");
    playerInventory.addItem("Health Potion");
    playerInventory.addItem("Attack Boost");

    battle(player, enemy, playerInventory);

    return 0;
}
