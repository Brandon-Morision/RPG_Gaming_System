#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <exception>
using namespace std;


void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

// This utility delays for styled borders
void printBorder(const string &title = "") {
    int width = 50;
    cout << string(width, '=') << endl;
    if (!title.empty()) {
        cout << "= " << setw((width / 2) + (title.size() / 2)) << left << title << right << string(width - title.size() - 4, ' ') << "=" << endl;
        cout << string(width, '=') << endl;
    }
}

// This utility  styles text for better UI 
void printStyled(const string &text, const string &style = "") {
    if (style == "bold") {
        cout << "\033[1m" << text << "\033[0m";
    } else if (style == "italic") {
        cout << "\033[3m" << text << "\033[0m";
    } else {
        cout << text;
    }
}

// we use this for sound effects (ASCII placeholder)
void playSoundEffect(const string &sound) {
    if (sound == "attack") {
        cout << "\U0001F5E1 Sword clashing sound effect\n";
    } else if (sound == "heal") {
        cout << "\U0001F3BC Healing sound effect\n";
    }
}

// This is the base class: Character
class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int specialCooldown; 
    int defense;         

public:
    Character(string n, int h, int a)
        : name(n), health(h), maxHealth(h), attackPower(a), specialCooldown(0), defense(0) {}

    void setHealth(int h) {
        health = h > 0 ? h : 0; 
    }

    void setAttackPower(int a) {
        attackPower = a > 0 ? a : attackPower;
    }

    virtual void attack(Character &opponent) {
        opponent.takeDamage(attackPower);
        cout << name << " attacks " << opponent.getName() << " for " << attackPower << " damage!" << endl;
        playSoundEffect("attack");
    }

    void takeDamage(int damage) {
        setHealth(health - max(0, damage - defense));
        defense = 0; 
    }

    void defend() {
        defense = 10;
        cout << name << " assumes a defensive stance, reducing incoming damage!" << endl;
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

    int getMaxHealth() const {
        return maxHealth;
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

// This is the derived class: Warrior
class Warrior : public Character {
public:
    Warrior(string n, int h, int a) : Character(n, h, a) {}

    void specialMove(Character &opponent) override {
        int damage = attackPower * 2;
        opponent.takeDamage(damage);
        cout << name << " performs a Heavy Strike on " << opponent.getName() << " for " << damage << " damage!" << endl;
        setSpecialCooldown(2); // the cooldown for special move is set 2 turns
    }
};

// Another Derived class: Mage
class Mage : public Character {
public:
    Mage(string n, int h, int a) : Character(n, h, a) {}

    void specialMove(Character &opponent) override {
        int damage = attackPower + 10;
        opponent.takeDamage(damage);
        cout << name << " casts Fireball on " << opponent.getName() << " for " << damage << " damage!" << endl;
        setSpecialCooldown(3);
    }

    void makeDecision(Character &player) {
        if (canUseSpecialMove() && player.getHealth() < attackPower + 10) {
            specialMove(player);
        } else if (canUseSpecialMove() && (rand() % 2 == 0)) {
            specialMove(player);
        } else {
            attack(player);
        }
    }
};

// Here the inventory is managed
class Inventory {
private:
    vector<string> items;

public:
    void addItem(const string &item) {
        items.push_back(item);
        cout << item << " added to inventory!" << endl;
    }

    void useItem(int choice, Character &player) {
        try {
            if (choice > 0 && choice <= items.size()) {
                string item = items[choice - 1];
                if (item == "Health Potion") {
                    player.setHealth(min(player.getHealth() + 20, player.getMaxHealth()));
                    items.erase(items.begin() + choice - 1);
                    cout << "Used " << item << ". Health increased by 20!" << endl;
                    playSoundEffect("heal");
                } else if (item == "Attack Boost") {
                    player.setAttackPower(player.getAttackPower() + 10);
                    items.erase(items.begin() + choice - 1);
                    cout << "Used " << item << ". Attack power increased by 10!" << endl;
                } else {
                    throw invalid_argument("Invalid item effect!");
                }
            } else {
                throw out_of_range("Invalid item selection!");
            }
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void showInventory() const {
        cout << "Inventory:" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << "  " << i + 1 << ") " << items[i] << endl;
        }
    }

    void clear() {
        items.clear();
    }
};

// The Player class
class Player : public Warrior {
private:
    int level;
    int experience;
    int experienceToNextLevel;
    Inventory inventory;

public:
    Player(string n, int h, int a) : Warrior(n, h, a), level(1), experience(0), experienceToNextLevel(50) {
        initializeInventory();
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
        experienceToNextLevel += 20;
        maxHealth += 20;
        setHealth(maxHealth);
        setAttackPower(getAttackPower() + 5);
        cout << "\nCongratulations! " << name << " has leveled up to Level " << level << "!" << endl;
        initializeInventory();
    }

    void reset() {
        setHealth(maxHealth);
        setAttackPower(15);
        level = 1;
        experience = 0;
        experienceToNextLevel = 50;
        initializeInventory();
    }

    void initializeInventory() {
        inventory.clear();
        inventory.addItem("Health Potion");
        inventory.addItem("Attack Boost");
    }

    Inventory &getInventory() {
        return inventory;
    }

    void saveProgress() {
        try {
            ofstream saveFile("savegame.txt");
            if (!saveFile.is_open()) {
                throw runtime_error("Unable to open save file.");
            }
            saveFile << name << "\n" << health << "\n" << attackPower << "\n" << level << "\n" << experience << "\n" << experienceToNextLevel << "\n";
            saveFile.close();
            cout << "Progress saved!" << endl;
        } catch (const exception &e) {
            cout << "Error saving progress: " << e.what() << endl;
        }
    }

    void loadProgress() {
        try {
            ifstream saveFile("savegame.txt");
            if (!saveFile.is_open()) {
                throw runtime_error("No save file found. Starting a new game.");
            }
            saveFile >> name >> health >> attackPower >> level >> experience >> experienceToNextLevel;
            saveFile.close();
            cout << "Progress loaded!" << endl;
        } catch (const exception &e) {
            cout << "Error loading progress: " << e.what() << endl;
        }
    }

    int getLevel() const {
        return level;
    }
};

// The pauseMenu function begins here
void pauseMenu(Player &player, vector<Mage> &enemies) {
    cout << "\n--- Pause Menu ---\n";
    cout << "1) Save Progress\n2) Return to Main Menu\n3) Exit Game\n";
    int choice;
    try {
        cin >> choice;
        if (cin.fail()) {
            throw invalid_argument("Invalid input. Please enter a number.");
        }

        switch (choice) {
        case 1:
            player.saveProgress();
            break;
        case 2:
            cout << "Returning to the Main Menu...\n";
            return;  
        case 3:
            cout << "Exiting the game. Goodbye!\n";
            exit(0);  
        default:
            throw out_of_range("Invalid menu choice! Please select a valid option.");
        }
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
// where Battle logic happens
void battle(Player &player, vector<Mage> &enemies) {
    printBorder("Battle Begins!");
    
    for (Mage &enemy : enemies) {
        cout << player.getName() << " (Level " << player.getLevel() << ") vs. " << enemy.getName() << endl;
        printBorder();
        
        while (player.isAlive() && enemy.isAlive()) {
            cout << "\n";
            cout << left << setw(20) << player.getName() << "| Health: [" 
                 << string(player.getHealth() * 20 / player.getMaxHealth(), '#') 
                 << string((player.getMaxHealth() - player.getHealth()) * 20 / player.getMaxHealth(), ' ') 
                 << "] " << player.getHealth() << "/" << player.getMaxHealth() << endl;
            cout << left << setw(20) << enemy.getName() << "| Health: [" 
                 << string(enemy.getHealth() * 20 / enemy.getMaxHealth(), '#') 
                 << string((enemy.getMaxHealth() - enemy.getHealth()) * 20 / enemy.getMaxHealth(), ' ') 
                 << "] " << enemy.getHealth() << "/" << enemy.getMaxHealth() << endl;

           
            cout << "\nYour turn! Choose an action:\n";
            cout << "1) Regular Attack\n2) Special Move\n3) Defend\n4) Use Item\n5) Pause Game\n";
            int choice;
            try {
                cin >> choice;
                if (cin.fail()) {
                    throw invalid_argument("Invalid input. Please enter a number.");
                }

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
                case 3:
                    player.defend();
                    break;
                case 4:
                    player.getInventory().showInventory();
                    cout << "Select an item to use: ";
                    int itemChoice;
                    cin >> itemChoice;
                    if (cin.fail()) {
                        throw invalid_argument("Invalid input. Please enter a valid item number.");
                    }
                    player.getInventory().useItem(itemChoice, player);
                    break;
                case 5:  
                    pauseMenu(player, enemies); 
                    return;  
                default:
                    throw out_of_range("Invalid action! Please select a valid option.");
                }
            } catch (const exception &e) {
                cout << "Error: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (enemy.isAlive()) {
                enemy.makeDecision(player);
            }
        }

        if (player.isAlive()) {
            cout << "\nYou defeated " << enemy.getName() << "!" << endl;
            player.gainExperience(20);
        } else {
            cout << "\n" << enemy.getName() << " has defeated you... Game Over!" << endl;
            break;
        }
    }
}

// The main game's loop
void mainMenu() {
    printBorder("Welcome to the RPG Game");
    string playerName;

    cout << "Enter your desired name: ";
    cin.ignore(); // Clear the input buffer in case of any leftover input
    getline(cin, playerName);

    if (playerName.empty()) {
        playerName = "Hero"; // Default name if none is provided
    }

    Player player(playerName, 100, 15);
    vector<Mage> enemies = {Mage("Dark Mage", 50, 10), Mage("Shadow Wizard", 70, 12)};

    while (true) {
        cout << "\nMain Menu:\n1) Start New Game\n2) Load Game\n3) Instructions\n4) Exit\n";
        int choice;
        try {
            cin >> choice;
            if (cin.fail()) {
                throw invalid_argument("Invalid input. Please enter a number.");
            }

            switch (choice) {
            case 1:
                player.reset();
                battle(player, enemies);
                if (player.isAlive()) {
                    cout << "Congratulations! You have defeated all enemies!" << endl;
                }
                break;
            case 2:
                player.loadProgress();
                battle(player, enemies);
                if (player.isAlive()) {
                    cout << "Congratulations! You have defeated all enemies!" << endl;
                }
                break;
            case 3:
                printBorder("Instructions");
                cout << "1) Use options in battles to attack, defend, or use items.\n";
                cout << "2) Earn XP to level up and enhance your abilities.\n";
                cout << "3) Save progress to continue your journey later.\n";
                cout << "4) Survive and defeat all enemies to win the game!\n";
                break;
            case 4:
                cout << "Exiting the game. Goodbye!" << endl;
                return;
            default:
                throw out_of_range("Invalid menu choice! Please select a valid option.");
            }
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    try {
        mainMenu();
    } catch (const exception &e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
    return 0;
}
 
