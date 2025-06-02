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
#include <limits> // Required for numeric_limits
#include <ctime>  // Required for time() for srand

using namespace std;

// --- Constants for Game Balance and Magic Numbers ---
const int HEALTH_POTION_HEAL_AMOUNT = 20;
const int ATTACK_BOOST_VALUE = 10;
const int POISON_DAMAGE_PER_TURN = 8;
const int BURN_DAMAGE_PER_TURN = 5;
const int DEFENSE_BONUS_FROM_DEFEND = 15;
const int DEFENSIVE_STANCE_BUFF_VALUE = 5; // Additional defense for Defensive Stance buff

const int WARRIOR_SPECIAL_COOLDOWN = 2;
const int MAGE_SPECIAL_COOLDOWN = 2;
const int MAGE_MANA_COST_FIREBALL = 30;
const int MAGE_MANA_REGEN_PER_TURN = 15;

const int PLAYER_INITIAL_HEALTH = 100;
const int PLAYER_INITIAL_ATTACK_POWER = 15;
const int PLAYER_INITIAL_XP_TO_LEVEL = 50;
const int XP_GAIN_PER_ENEMY = 20;
const int XP_INCREASE_PER_LEVEL = 20;
const int HEALTH_INCREASE_PER_LEVEL = 20;
const int ATTACK_INCREASE_PER_LEVEL = 5;

const size_t INVENTORY_MAX_ITEMS = 5;

// --- Utility Functions ---

void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // Assume POSIX or similar; works on Linux/macOS
    system("clear");
#endif
}

// This utility delays for styled borders
void printBorder(const string &title = "") {
    int width = 50;
    std::cout << string(width, '=') << std::endl;
    if (!title.empty()) {
        // Adjust padding for centering, ensuring title is not truncated
        int totalPadding = width - title.size() - 4; // 4 for "= " and " ="
        int leftPadding = totalPadding / 2;
        int rightPadding = totalPadding - leftPadding;
        std::cout << "= " << string(leftPadding, ' ') << title << string(rightPadding, ' ') << " =" << std::endl;
        std::cout << string(width, '=') << std::endl;
    }
}

// This utility styles text for better UI
void printStyled(const string &text, const string &style = "") {
    if (style == "bold") {
        std::cout << "\033[1m" << text << "\033[0m";
    } else if (style == "italic") {
        std::cout << "\033[3m" << text << "\033[0m";
    } else {
        std::cout << text;
    }
}

// we use this for sound effects (ASCII placeholder)
void playSoundEffect(const string &sound) {
    if (sound == "attack") {
        std::cout << "\U0001F5E1 Sword clashing sound effect\n";
    } else if (sound == "heal") {
        std::cout << "\U0001F3BC Healing sound effect\n";
    }
}

// --- Base Class: Character ---
class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int specialCooldown;
    int defense;
    vector<pair<string, int>> statusEffects; // New: Track status effects (name, duration)
    vector<pair<string, int>> temporaryBuffs; // New: Track temporary buffs (name, value)

public:
    Character(string n, int h, int a)
        : name(n), health(h), maxHealth(h), attackPower(a), specialCooldown(0), defense(0) {}

    // Add virtual destructor
    virtual ~Character() = default;

    void setHealth(int h) {
        health = h > 0 ? h : 0;
    }

    void setAttackPower(int a) {
        attackPower = a > 0 ? a : attackPower;
    }

    virtual void attack(Character &opponent) {
        int totalAttack = calculateTotalAttack();
        opponent.takeDamage(totalAttack);
        cout << name << " attacks " << opponent.getName() << " for " << totalAttack << " damage!" << endl;
        playSoundEffect("attack");
    }

    void takeDamage(int damage) {
        int totalDefense = calculateTotalDefense();
        int actualDamage = max(0, damage - totalDefense);
        setHealth(health - actualDamage);
        
        // Defense from defend() action persists until next turn
        // This is handled by clearing temporary buffs in updateEffects()
        // If 'defense' was a base stat, it wouldn't clear, but here it's more like a temporary shield.
        // If you want it to be a temporary shield that gets depleted, you'd adjust 'defense' here.
        // For simplicity, current 'defense' acts as a flat damage reduction for the turn, and resets via buffs.
    }

    void defend() {
        defense = DEFENSE_BONUS_FROM_DEFEND; // Increased from 10 for better defensive strategy
        addTemporaryBuff("Defensive Stance", DEFENSIVE_STANCE_BUFF_VALUE); // Additional defense bonus
        cout << name << " assumes a defensive stance, significantly reducing incoming damage!" << endl;
    }

    // New: Apply status effects
    void applyStatusEffect(const string &effect, int duration) {
        // Remove existing effect of same type if present to refresh duration
        removeStatusEffect(effect);
        statusEffects.emplace_back(effect, duration);
        cout << name << " is affected by " << effect << " for " << duration << " turns!" << endl;
    }

    // New: Remove a specific status effect
    void removeStatusEffect(const string &effect) {
        statusEffects.erase(
            remove_if(statusEffects.begin(), statusEffects.end(),
                [&effect](const auto &pair) { return pair.first == effect; }),
            statusEffects.end()
        );
    }

    // New: Add temporary buff
    void addTemporaryBuff(const string &buff, int value) {
        // Check if buff of the same type already exists, if so, update or add
        auto it = find_if(temporaryBuffs.begin(), temporaryBuffs.end(),
                          [&buff](const auto& p) { return p.first == buff; });
        if (it != temporaryBuffs.end()) {
            it->second = value; // Update value if buff already exists
        } else {
            temporaryBuffs.emplace_back(buff, value);
        }
    }

    // New: Update status effects and clear temporary buffs at end of turn
    void updateEffects() {
        // Process status effects
        for (auto it = statusEffects.begin(); it != statusEffects.end();) {
            if (it->first == "Poison") {
                takeDamage(POISON_DAMAGE_PER_TURN);
                cout << name << " takes " << POISON_DAMAGE_PER_TURN << " poison damage!" << endl;
            } else if (it->first == "Burn") {
                takeDamage(BURN_DAMAGE_PER_TURN);
                cout << name << " takes " << BURN_DAMAGE_PER_TURN << " burn damage!" << endl;
            }
            
            it->second--;
            if (it->second <= 0) {
                cout << name << " is no longer affected by " << it->first << "!" << endl;
                it = statusEffects.erase(it);
            } else {
                ++it;
            }
        }

        // Clear temporary buffs at the end of the turn
        temporaryBuffs.clear();
        defense = 0; // Reset base defense from defend() action
    }

    // New: Calculate total attack including buffs
    int calculateTotalAttack() const {
        int total = attackPower;
        for (const auto &buff : temporaryBuffs) {
            if (buff.first == "Attack Boost") {
                total += buff.second;
            }
        }
        return total;
    }

    // New: Calculate total defense including buffs
    int calculateTotalDefense() const {
        int total = defense;
        for (const auto &buff : temporaryBuffs) {
            if (buff.first == "Defensive Stance") {
                total += buff.second;
            }
        }
        return total;
    }

    // New: Get status effects for display
    string getStatusEffectsDisplay() const {
        if (statusEffects.empty()) {
            return "None";
        }
        string display;
        for (const auto &effect : statusEffects) {
            if (!display.empty()) display += ", ";
            display += effect.first + "(" + to_string(effect.second) + ")";
        }
        return display;
    }

    virtual void specialMove(Character &opponent) = 0;
    virtual void makeDecision(Character &player) = 0; // Make this pure virtual

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

    int getSpecialCooldown() const {
        return specialCooldown;
    }

    void setSpecialCooldown(int turns) {
        specialCooldown = turns;
    }
};

// --- Derived Class: Warrior ---
class Warrior : public Character {
public:
    Warrior(string n, int h, int a) : Character(n, h, a) {}

    void specialMove(Character &opponent) override {
        int damage = attackPower * 2;
        opponent.takeDamage(damage);
        std::cout << name << " performs a Heavy Strike on " << opponent.getName() << " for " << damage << " damage!" << std::endl;
        setSpecialCooldown(WARRIOR_SPECIAL_COOLDOWN); // the cooldown for special move is set 2 turns
    }

    void makeDecision(Character &player) override {
        // Warrior doesn't need AI decision making, but we need to implement the pure virtual function
        // This will never be called for the warrior character
    }
};

// --- Derived Class: Mage ---
class Mage : public Character {
private:
    int manaPoints;
    static const int MAX_MANA; // Just declare it here

public:
    Mage(string n, int h, int a) : Character(n, h, a), manaPoints(MAX_MANA) {}

    void specialMove(Character &opponent) override {
        int damage = attackPower + 15;
        opponent.takeDamage(damage);
        opponent.applyStatusEffect("Burn", 2); // Fireball now applies burn
        manaPoints = max(0, manaPoints - MAGE_MANA_COST_FIREBALL);
        cout << name << " casts Fireball on " << opponent.getName() << " for " << damage << " damage and applies burn!" << endl;
        setSpecialCooldown(MAGE_SPECIAL_COOLDOWN); // Reduced from 3 to match Warrior
    }

    void makeDecision(Character &player) override {
        // Smarter AI decision making
        if (health < maxHealth / 3) {
            // Low health - prioritize defense
            if (rand() % 100 < 70) { // 70% chance to defend when low health
                defend();
                return;
            }
        }

        if (canUseSpecialMove() && manaPoints >= MAGE_MANA_COST_FIREBALL) {
            if (player.getHealth() <= (attackPower + 15) || (rand() % 100 < 40)) {
                // Use special if can kill player or 40% chance
                specialMove(player);
            } else {
                attack(player);
            }
        } else {
            if (rand() % 100 < 20) { // 20% chance to defend
                defend();
            } else {
                attack(player);
            }
        }

        // Regenerate some mana each turn
        manaPoints = min(MAX_MANA, manaPoints + MAGE_MANA_REGEN_PER_TURN);
    }

    int getManaPoints() const {
        return manaPoints;
    }

    string getManaDisplay() const {
        return "Mana: [" + string(manaPoints * 10 / MAX_MANA, '|') + string(10 - (manaPoints * 10 / MAX_MANA), ' ') + "] " + to_string(manaPoints) + "/" + to_string(MAX_MANA);
    }
};

// Add this before the main() function
const int Mage::MAX_MANA = 100;

// --- Inventory Management ---
class Inventory {
private:
    vector<string> items;

public:
    bool isEmpty() const {
        return items.empty();
    }

    void addItem(const string &item) {
        if (items.size() >= INVENTORY_MAX_ITEMS) {
            cout << "Inventory is full! Cannot add " << item << "." << endl;
            return;
        }
        items.push_back(item);
        cout << item << " added to inventory!" << endl;
    }

    void useItem(int choice, Character &player, Character &target) {
        try {
            if (choice <= 0 || choice > items.size()) {
                throw out_of_range("Invalid item selection!");
            }

            string item = items[choice - 1];
            bool itemUsed = false;

            if (item == "Health Potion") {
                int oldHealth = player.getHealth();
                player.setHealth(min(player.getHealth() + HEALTH_POTION_HEAL_AMOUNT, player.getMaxHealth()));
                int healthGained = player.getHealth() - oldHealth;
                items.erase(items.begin() + choice - 1);
                cout << "Used " << item << ". Health increased by " << healthGained << "!" << endl;
                playSoundEffect("heal");
                itemUsed = true;
            } else if (item == "Attack Boost") {
                player.addTemporaryBuff("Attack Boost", ATTACK_BOOST_VALUE);
                items.erase(items.begin() + choice - 1);
                cout << "Used " << item << ". Attack power increased by " << ATTACK_BOOST_VALUE << " for this turn!" << endl;
                itemUsed = true;
            } else if (item == "Poison Dart") {
                target.applyStatusEffect("Poison", 3);
                items.erase(items.begin() + choice - 1);
                cout << "Used " << item << ". Target is poisoned for 3 turns!" << endl;
                itemUsed = true;
            }

            if (!itemUsed) {
                throw invalid_argument("Invalid item effect!");
            }
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
            delay(1500);
        }
    }

    void showInventory() const {
        cout << "\nInventory (" << items.size() << "/" << INVENTORY_MAX_ITEMS << " items):" << endl;
        if (items.empty()) {
            cout << "  (Empty)" << endl;
            return;
        }
        for (size_t i = 0; i < items.size(); ++i) {
            cout << "  " << i + 1 << ") " << items[i] << endl;
        }
    }

    void clear() {
        items.clear();
    }

    vector<string> getItems() const {
        return items;
    }
};

// --- Player Class ---
class Player : public Warrior {
private:
    int level;
    int experience;
    int experienceToNextLevel;
    Inventory inventory;

public:
    Player(string n, int h, int a) : Warrior(n, h, a), level(1), experience(0), experienceToNextLevel(PLAYER_INITIAL_XP_TO_LEVEL) {
        initializeInventory();
    }

    void makeDecision(Character &player) override {
        // Player doesn't need AI decision making, but we need to implement the pure virtual function
        // This will never be called for the player character
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
        experienceToNextLevel += XP_INCREASE_PER_LEVEL;
        maxHealth += HEALTH_INCREASE_PER_LEVEL;
        setHealth(maxHealth); // Heal to new max health
        setAttackPower(getAttackPower() + ATTACK_INCREASE_PER_LEVEL);
        cout << "\nCongratulations! " << name << " has leveled up to Level " << level << "!" << endl;
        // Consider adding new items or expanding inventory size at certain levels
    }

    void reset() {
        // Reset player stats to initial values
        name = "Player"; // Default name or ask again
        health = PLAYER_INITIAL_HEALTH;
        maxHealth = PLAYER_INITIAL_HEALTH;
        attackPower = PLAYER_INITIAL_ATTACK_POWER;
        specialCooldown = 0;
        defense = 0;
        statusEffects.clear();
        temporaryBuffs.clear();

        level = 1;
        experience = 0;
        experienceToNextLevel = PLAYER_INITIAL_XP_TO_LEVEL;
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

            saveFile << name << "\n" << health << "\n" << maxHealth << "\n"
                     << attackPower << "\n" << level << "\n" << experience << "\n"
                     << experienceToNextLevel << "\n";

            // Save inventory
            vector<string> items = inventory.getItems();
            saveFile << items.size() << "\n";
            for (const auto& item : items) {
                saveFile << item << "\n";
            }

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
                cout << "No save file found. Starting a new game." << endl;
                reset();
                return;
            }

            string loadedName;
            int loadedHealth, loadedMaxHealth, loadedAttackPower;
            int loadedLevel, loadedExperience, loadedExperienceToNextLevel;

            // Read each value with error checking
            if (!(saveFile >> loadedName >> loadedHealth >> loadedMaxHealth >> loadedAttackPower
                     >> loadedLevel >> loadedExperience >> loadedExperienceToNextLevel)) {
                throw runtime_error("Invalid save file format.");
            }
            saveFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline

            // Validate loaded values
            if (loadedHealth <= 0 || loadedMaxHealth <= 0 || loadedAttackPower <= 0 ||
                loadedLevel <= 0 || loadedExperience < 0 || loadedExperienceToNextLevel <= 0) {
                throw runtime_error("Invalid values in save file.");
            }

            // Apply loaded values
            name = loadedName;
            health = loadedHealth;
            maxHealth = loadedMaxHealth;
            attackPower = loadedAttackPower;
            level = loadedLevel;
            experience = loadedExperience;
            experienceToNextLevel = loadedExperienceToNextLevel;

            // Clear inventory and load saved items
            inventory.clear();
            size_t inventorySize;
            if (!(saveFile >> inventorySize)) {
                throw runtime_error("Invalid inventory size in save file.");
            }
            saveFile.ignore(numeric_limits<streamsize>::max(), '\n');

            for (size_t i = 0; i < inventorySize; ++i) {
                string item;
                if (!getline(saveFile, item) || item.empty()) {
                    throw runtime_error("Invalid item in save file.");
                }
                inventory.addItem(item);
            }

            specialCooldown = 0;
            defense = 0;
            statusEffects.clear();
            temporaryBuffs.clear();

            saveFile.close();
            cout << "Progress loaded successfully!" << endl;
        } catch (const exception &e) {
            cout << "Error loading progress: " << e.what() << endl;
            cout << "Starting a new game..." << endl;
            reset();
        }
    }

    int getLevel() const {
        return level;
    }
};

// --- Helper Function for Displaying Health Bars (Improvement) ---
void displayCharacterStats(const Character& character) {
    cout << "\n" << character.getName();
    // Only display level for Player characters
    if (const Player* player = dynamic_cast<const Player*>(&character)) {
        cout << " (Level " << player->getLevel() << ")";
    }
    cout << endl;

    cout << "Health: [";
    float healthPercent = (float)character.getHealth() / character.getMaxHealth();
    if (healthPercent > 0.6) cout << "\033[32m"; // Green
    else if (healthPercent > 0.3) cout << "\033[33m"; // Yellow
    else cout << "\033[31m"; // Red
    cout << string(character.getHealth() * 20 / character.getMaxHealth(), '#') 
         << "\033[0m" << string((character.getMaxHealth() - character.getHealth()) * 20 / character.getMaxHealth(), ' ') 
         << "] " << character.getHealth() << "/" << character.getMaxHealth() << endl;
    
    // Display mana for Mage characters
    if (const Mage* mage = dynamic_cast<const Mage*>(&character)) {
        cout << mage->getManaDisplay() << endl;
    }
}

// --- Pause Menu Function ---
void pauseMenu(Player &player, vector<Mage> &enemies) {
    cout << "\n--- Pause Menu ---\n";
    cout << "1) Save Progress\n2) Return to Main Menu\n3) Exit Game\n";
    int choice;
    try {
        cout << "Enter your choice: ";
        cin >> choice;
        // Clear buffer in case of non-integer input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Invalid input. Please enter a number.");
        }

        switch (choice) {
            case 1:
                player.saveProgress();
                break;
            case 2:
                cout << "Returning to the Main Menu...\n";
                // No need to exit(0) here, just return to main loop
                break;
            case 3:
                cout << "Exiting the game. Goodbye!\n";
                exit(0); // Exit the entire program
            default:
                throw out_of_range("Invalid menu choice! Please select a valid option.");
        }
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
        // Ensure input buffer is cleared if an exception occurs
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        delay(1500); // Pause to show error message
    }
}

// --- Battle Logic ---
bool battle(Player &player, vector<Mage> &enemies) {
    printBorder("Battle Begins!");
    
    for (Mage &enemy : enemies) {
        cout << player.getName() << " (Level " << player.getLevel() << ") vs. " << enemy.getName() << endl;
        printBorder();
        delay(1000); // Small delay before battle starts

        // Reset player's special cooldown at the start of each new battle
        player.setSpecialCooldown(0); 
        
        while (player.isAlive() && enemy.isAlive()) {
            clearScreen(); // Clear screen for cleaner display
            printBorder("Battle");
            
            // Display stats using the helper function
            displayCharacterStats(player);
            displayCharacterStats(enemy);

            // Display status effects with colors
            cout << "\nStatus Effects:" << endl;
            cout << player.getName() << ": ";
            string playerEffects = player.getStatusEffectsDisplay();
            if (playerEffects == "None") {
                cout << playerEffects << endl;
            } else {
                cout << "\033[35m" << playerEffects << "\033[0m" << endl; // Magenta for active effects
            }
            
            cout << enemy.getName() << ": ";
            string enemyEffects = enemy.getStatusEffectsDisplay();
            if (enemyEffects == "None") {
                cout << enemyEffects << endl;
            } else {
                cout << "\033[35m" << enemyEffects << "\033[0m" << endl;
            }

            // Display special move cooldown with color
            cout << "\nSpecial Move: ";
            if (player.getSpecialCooldown() > 0) {
                cout << "\033[31m" << player.getSpecialCooldown() << " turns remaining\033[0m" << endl;
            } else {
                cout << "\033[32mREADY!\033[0m" << endl;
            }
            
            cout << "\nYour turn! Choose an action:\n";
            cout << "1) Regular Attack\n";
            cout << "2) Special Move" << (player.canUseSpecialMove() ? " \033[32m(Ready!)\033[0m" : " \033[31m(Cooldown: " + to_string(player.getSpecialCooldown()) + ")\033[0m") << "\n";
            cout << "3) Defend\n";
            cout << "4) Use Item\n";
            cout << "5) Pause Game\n";
            
            int choice;
            bool turn_taken = false; // Flag to ensure a valid action was taken
            while (!turn_taken) {
                try {
                    cout << "\nEnter your choice (1-5): ";
                    cin >> choice;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("Invalid input. Please enter a number.");
                    }

                    switch (choice) {
                        case 1:
                            player.attack(enemy);
                            turn_taken = true;
                            break;
                        case 2:
                            if (player.canUseSpecialMove()) {
                                player.specialMove(enemy);
                                turn_taken = true;
                            } else {
                                cout << "\033[31mSpecial move is on cooldown for " << player.getSpecialCooldown() << " more turns!\033[0m" << endl;
                                delay(1500);
                            }
                            break;
                        case 3:
                            player.defend();
                            turn_taken = true;
                            break;
                        case 4:
                            if (player.getInventory().isEmpty()) {
                                cout << "Your inventory is empty!" << endl;
                                delay(1500);
                                continue; // Don't take a turn if inventory is empty
                            }
                            player.getInventory().showInventory();
                            cout << "\nSelect an item to use (0 to cancel): ";
                            int itemChoice;
                            if (!(cin >> itemChoice)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid input. Please enter a number." << endl;
                                delay(1500);
                                continue;
                            }
                            if (itemChoice == 0) {
                                continue; // Allow player to choose another action
                            }
                            player.getInventory().useItem(itemChoice, player, enemy);
                            turn_taken = true;
                            break;
                        case 5:
                            pauseMenu(player, enemies);
                            clearScreen();
                            return false;
                        default:
                            throw out_of_range("Invalid action! Please select a valid option.");
                    }
                } catch (const exception &e) {
                    cout << "\033[31mError: " << e.what() << "\033[0m" << endl;
                    delay(1500); // Pause to show the error
                    // Input stream already cleared by the catch block
                }
            }

            // Reduce special cooldown
            player.reduceCooldown();
            
            // Update effects at end of player's turn
            player.updateEffects();
            delay(1000); // Pause to show the results of player's action

            if (enemy.isAlive()) {
                cout << "\nEnemy's turn..." << endl;
                delay(1000);
                enemy.makeDecision(player);
                // Reduce enemy special cooldown (if they had one)
                enemy.reduceCooldown();
                // Update effects at end of enemy's turn
                enemy.updateEffects();
                delay(1500); // Pause to show enemy action results
            }
        }

        if (player.isAlive()) {
            printBorder("Victory!");
            cout << "\nYou defeated " << enemy.getName() << "!" << endl;
            player.gainExperience(XP_GAIN_PER_ENEMY);
            // Give bonus item after victory
            if (rand() % 2 == 0) { // 50% chance
                player.getInventory().addItem("Poison Dart");
                cout << "You found a Poison Dart on the defeated enemy!" << endl;
            }
            delay(2000); // Pause to celebrate victory
        } else {
            printBorder("Game Over");
            cout << "\n" << enemy.getName() << " has defeated you..." << endl;
            delay(2000);
            return false; // Player lost the game
        }
    }

    return true; // Player defeated all enemies
}

// The main game's loop
void mainMenu() {
    printBorder("Welcome to the RPG Game");
    string playerName;

    cout << "Enter your desired name: ";
    // Use cin.ignore() to clear potential leftover newline from previous input if any
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, playerName);

    if (playerName.empty()) {
        playerName = "Hero";
    }

    Player player(playerName, PLAYER_INITIAL_HEALTH, PLAYER_INITIAL_ATTACK_POWER);
    
    while (true) {
        // Re-initialize enemies for each new game session or battle if needed,
        // or regenerate them as per game design (e.g., specific for each level).
        // For simplicity, we'll create them here and reset for new game.
        vector<Mage> enemies = {Mage("Dark Mage", 50, 10), Mage("Shadow Wizard", 70, 12)};

        cout << "\nMain Menu:\n1) Start New Game\n2) Load Game\n3) Instructions\n4) Exit\n";
        int choice;
        try {
            cout << "Enter your choice: ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid input. Please enter a number.");
            }

            switch (choice) {
                case 1:
                    player.reset(); // Reset player stats and inventory
                    enemies = {Mage("Dark Mage", 50, 10), Mage("Shadow Wizard", 70, 12)}; // Re-initialize enemies
                    if (battle(player, enemies)) {
                        cout << "Congratulations! You have defeated all enemies!" << endl;
                        delay(2000);
                    }
                    break;
                case 2:
                    player.loadProgress();
                    enemies = {Mage("Dark Mage", 50, 10), Mage("Shadow Wizard", 70, 12)}; // Re-initialize enemies for loaded game context
                    if (battle(player, enemies)) {
                        cout << "Congratulations! You have defeated all enemies!" << endl;
                        delay(2000);
                    }
                    break;
                case 3:
                    printBorder("Instructions");
                    cout << "1) Use options in battles to attack, defend, or use items.\n";
                    cout << "2) Earn XP to level up and enhance your abilities.\n";
                    cout << "3) Save progress to continue your journey later.\n";
                    cout << "4) Survive and defeat all enemies to win the game!\n";
                    delay(5000); // Show instructions for a longer period
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
            delay(1500);
        }
    }
}

int main() {
    // Seed the random number generator once at the beginning
    srand(static_cast<unsigned int>(time(0)));

    try {
        mainMenu();
    } catch (const exception &e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
    return 0;
}