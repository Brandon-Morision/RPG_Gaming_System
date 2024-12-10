#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>
using namespace std;

// Utility for delays
void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

// Utility for styled borders
void printBorder(const string &title = "") {
    int width = 50;
    cout << string(width, '=') << endl;
    if (!title.empty()) {
        cout << "= " << setw((width / 2) + (title.size() / 2)) << left << title << right << string(width - title.size() - 4, ' ') << "=" << endl;
        cout << string(width, '=') << endl;
    }
}

// Utility for styled text
void printStyled(const string &text, const string &style = "") {
    if (style == "bold") {
        cout << "\033[1m" << text << "\033[0m";
    } else if (style == "italic") {
        cout << "\033[3m" << text << "\033[0m";
    } else {
        cout << text;
    }
}

// Base class: Character
class Character {
protected:
    string name;
    int health;
    int attackPower;
    int specialCooldown; // Tracks turns left until special move is available

public:
    Character(string n, int h, int a) : name(n), health(h), attackPower(a), specialCooldown(0) {}

    void setHealth(int h) {
        health = h > 0 ? h : 0; // Ensure health does not drop below zero
    }

    void setAttackPower(int a) {
        attackPower = a > 0 ? a : attackPower; // Prevent setting negative attack power
    }

    virtual void attack(Character &opponent) {
        opponent.setHealth(opponent.getHealth() - attackPower);
        cout << name << " attacks " << opponent.getName() << " for " << attackPower << " damage!" << endl;
    }

    virtual void specialMove(Character &opponent) = 0; // Pure virtual method

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
        setSpecialCooldown(2); // Set cooldown for 2 turns
    }
};

// Derived class: Mage
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

    void makeDecision(Character &player) {
        if (canUseSpecialMove() && (rand() % 2 == 0 || player.getHealth() < attackPower + 10)) {
            specialMove(player);
        } else {
            attack(player);
        }
    }
};

// Inventory class
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

// Player class with leveling system
class Player : public Warrior {
private:
    int level;
    int experience;
    int experienceToNextLevel;
    Inventory inventory;

public:
    Player(string n, int h, int a) : Warrior(n, h, a), level(1), experience(0), experienceToNextLevel(50) {
        // Initialize inventory with items
        inventory.addItem("Health Potion");
        inventory.addItem("Attack Boost");
        inventory.addItem("Shield");
        inventory.addItem("Mana Potion");
    }

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
        experienceToNextLevel += 20; // Increase XP needed for next level
        setHealth(getHealth() + 20); // Increase health
        setAttackPower(getAttackPower() + 5); // Increase attack power
        cout << "\nCongratulations! " << name << " has leveled up to Level " << level << "!" << endl;
        // Replenish inventory on level up
        inventory.addItem("Health Potion");
        inventory.addItem("Attack Boost");
        inventory.addItem("Shield");
        inventory.addItem("Mana Potion");
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

    Inventory& getInventory() {
        return inventory;
    }
};

// Battle System with dynamic AI
void battle(Player &player, vector<Mage> &enemies) {
    printBorder("Battle Begins!");

    for (size_t i = 0; i < enemies.size(); ++i) {
        Mage &enemy = enemies[i];
        cout << player.getName() << " (Level " << player.getLevel() << ") vs. " << enemy.getName() << endl;
        printBorder();

        while (player.isAlive() && enemy.isAlive()) {
            // Display health bars
            cout << "\n";
            cout << left << setw(20) << player.getName() << "| Health: [" << string(player.getHealth() / 5, '#') 
                 << string((100 - player.getHealth()) / 5, ' ') << "] " << player.getHealth() << endl;
            cout << left << setw(20) << enemy.getName() << "| Health: [" << string(enemy.getHealth() / 5, '#') 
                 << string((100 - enemy.getHealth()) / 5, ' ') << "] " << enemy.getHealth() << endl;

            // Player's turn
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
                    player.getInventory().showInventory();
                    cout << "Choose an item number to use: ";
                    int itemChoice;
                    cin >> itemChoice;
                    player.getInventory().useItem(itemChoice, player);
                    break;
                }
                default:
                    cout << "Invalid choice! Turn skipped." << endl;
                    break;
            }

            // Check if enemy is defeated
            if (!enemy.isAlive()) {
                cout << enemy.getName() << " is defeated! You gain 20 XP!" << endl;
                player.gainExperience(20);
                break;
            }

            // Enemy's turn
            cout << "\n" << enemy.getName() << "'s turn!" << endl;
            enemy.makeDecision(player);

            // Check if player is defeated
            if (!player.isAlive()) {
                cout << "\nYou were defeated by " << enemy.getName() << "!" << endl;
                return; // End battle if player is defeated
            }

            // Reduce cooldowns for both
            player.reduceCooldown();
            enemy.reduceCooldown();
        }

        if (!player.isAlive()) {
            break; // Stop further battles if player is defeated
        }
    }

    if (player.isAlive()) {
        cout << "\nCongratulations! You defeated all enemies!" << endl;
        printBorder("Victory");
    }
}

// Main function with game loop
int main() {
    string playerName;
    cout << "Enter your character's name: ";
    getline(cin, playerName);

    Player player(playerName, 100, 15);
    vector<Mage> enemies = {Mage("Fire Mage", 80, 12), Mage("Ice Mage", 90, 14)};

    bool running = true;

    while (running) {
        cout << "\nMain Menu:\n1) Start Battle\n2) View Inventory\n3) Exit Game\n";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                battle(player, enemies);
                break;
            case 2:
                player.getInventory().showInventory();
                break;
            case 3:
                cout << "Thank you for playing! Exiting the game.\n";
                running = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
