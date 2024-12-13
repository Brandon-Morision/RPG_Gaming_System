RPG Battle Game 

Introduction
This is a command-line role-playing game (RPG) where the user controls a character and fights foes in turn-based combat. The options available to the player include attacking, employing special techniques, and using inventory items. Through the game's leveling system, players can advance in strength and experience.

Features
Combat is turn-based; you can strike, execute special techniques, and use objects to combat your adversaries.
Inventory System: Gather and utilize things such as attack enhancements and healing potions.
Leveling System: After every combat, get experience points (XP) to advance in level.
Special Moves: Following a cooldown, each character can use a different special move.
Dynamic AI: Enemies make decisions during combat, choosing between regular attacks and special moves.
Requirements
C++ compiler (e.g., g++)
C++11 or later
Compilation and Execution
Compile the code:
bash
Copy code
g++ -std=c++11 -o rpg_battle_game main.cpp
Run the game:
bash
Copy code
./rpg_battle_game
Game Mechanics
1. Character Classes
Warrior (Player): The player starts as a Warrior with basic attack and a powerful special move. The player can level up to increase health, attack power, and gain additional inventory items.
Mage (Enemy): Enemies are mages that use powerful magic attacks and special moves. The AI controls their actions during the battle.
2. Battle System
Each battle is between the player and a Mage.
The player has a turn to choose an action:
Regular attack.
Special move (after cooldown).
Use items from inventory (e.g., Health Potion, Attack Boost).
The Mage will choose its action based on a simple AI, using either an attack or a special move.
During the battle, a health bar shows the current health of the player and the enemy.
3. Inventory System
The player starts with a set of items in their inventory: Health Potion, Attack Boost, Shield, and Mana Potion.
Items can be used during battle to heal or boost attack power.
4. Leveling Up
After each battle, the player gains experience. Upon reaching a certain amount of XP, the player levels up, which increases health, and attack power, and replenishes the inventory.
5. Game Menu
Main Menu options:
Start Battle: Begin a battle with enemies.
View Inventory: See and manage the player's inventory.
Exit Game: Exit the game.
Example Gameplay
Start Battle: The player fights against an enemy Mage.
Use Special Move: The player can use a special move when it's available, dealing massive damage.
Level Up: After defeating enemies and gaining enough XP, the player levels up and gets stronger.
Inventory Management: The player can use items to heal or boost their stats during battle.
Game Controls
Main Menu:
Play instructions
Start new game
View Inventory
load saved game
Exit Game
Battle Options:
Choose between:
Regular Attack
Special Move
Use Item
Code Structure
Classes:
Character: Base class for all characters. Contains basic properties like health and attack power.
Warrior: Inherits from Character, representing the player character.
Mage: Inherits from Character, representing enemy mages with AI-controlled behavior.
Inventory: Manages items that the player can use.
Player: Inherits from Warrior, with additional features for leveling up and gaining experience.
Battle System: The main logic that controls the flow of battle between the player and enemies.
Contribution
If you'd like to contribute to the project, feel free to fork the repository, make changes, and submit a pull request.

License
This project is licensed under the MIT License - see the LICENSE file for details.# RPG_Gaming_System
