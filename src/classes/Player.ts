import { Warrior } from './Warrior';
import { Inventory } from './Inventory';
import { GAME_CONSTANTS } from '../types/game';
import { Character } from './Character';

export class Player extends Warrior {
  private level: number;
  private experience: number;
  private experienceToNextLevel: number;
  private inventory: Inventory;

  constructor(name: string, health: number, attackPower: number) {
    super(name, health, attackPower);
    this.level = 1;
    this.experience = 0;
    this.experienceToNextLevel = GAME_CONSTANTS.PLAYER_INITIAL_XP_TO_LEVEL;
    this.inventory = new Inventory();
    this.initializeInventory();
  }

  makeDecision(player: Character): string {
    // Player makeDecision is not used in gameplay since player actions are controlled by user input
    // This implementation is just to satisfy the abstract method requirement
    return "Player decision - this should not be called in normal gameplay";
  }

  gainExperience(xp: number): { leveledUp: boolean; message: string } {
    this.experience += xp;
    const message = `${this.name} gains ${xp} XP!`;
    
    if (this.experience >= this.experienceToNextLevel) {
      const levelUpMessage = this.levelUp();
      return { leveledUp: true, message: message + ' ' + levelUpMessage };
    }
    
    return { leveledUp: false, message };
  }

  private levelUp(): string {
    this.level++;
    this.experience -= this.experienceToNextLevel;
    this.experienceToNextLevel += GAME_CONSTANTS.XP_INCREASE_PER_LEVEL;
    this.maxHealth += GAME_CONSTANTS.HEALTH_INCREASE_PER_LEVEL;
    this.setHealth(this.maxHealth);
    this.setAttackPower(this.getAttackPower() + GAME_CONSTANTS.ATTACK_INCREASE_PER_LEVEL);
    
    return `Congratulations! ${this.name} has leveled up to Level ${this.level}!`;
  }

  reset(): void {
    this.health = GAME_CONSTANTS.PLAYER_INITIAL_HEALTH;
    this.maxHealth = GAME_CONSTANTS.PLAYER_INITIAL_HEALTH;
    this.attackPower = GAME_CONSTANTS.PLAYER_INITIAL_ATTACK_POWER;
    this.specialCooldown = 0;
    this.defense = 0;
    this.statusEffects = [];
    this.temporaryBuffs = [];
    this.level = 1;
    this.experience = 0;
    this.experienceToNextLevel = GAME_CONSTANTS.PLAYER_INITIAL_XP_TO_LEVEL;
    this.initializeInventory();
  }

  private initializeInventory(): void {
    this.inventory.clear();
    this.inventory.addItem("Health Potion");
    this.inventory.addItem("Attack Boost");
  }

  saveProgress(): { success: boolean; message: string } {
    try {
      const saveData = {
        name: this.name,
        health: this.health,
        maxHealth: this.maxHealth,
        attackPower: this.attackPower,
        level: this.level,
        experience: this.experience,
        experienceToNextLevel: this.experienceToNextLevel,
        inventory: this.inventory.getItems()
      };
      
      localStorage.setItem('rpg_save', JSON.stringify(saveData));
      return { success: true, message: "Progress saved!" };
    } catch (error) {
      return { success: false, message: "Error saving progress!" };
    }
  }

  loadProgress(): { success: boolean; message: string } {
    try {
      const saveData = localStorage.getItem('rpg_save');
      if (!saveData) {
        return { success: false, message: "No save file found." };
      }

      const data = JSON.parse(saveData);
      
      this.name = data.name;
      this.health = data.health;
      this.maxHealth = data.maxHealth;
      this.attackPower = data.attackPower;
      this.level = data.level;
      this.experience = data.experience;
      this.experienceToNextLevel = data.experienceToNextLevel;
      
      this.inventory.clear();
      data.inventory.forEach((item: string) => this.inventory.addItem(item));
      
      return { success: true, message: "Progress loaded successfully!" };
    } catch (error) {
      return { success: false, message: "Error loading progress!" };
    }
  }

  getLevel(): number {
    return this.level;
  }

  getExperience(): number {
    return this.experience;
  }

  getExperienceToNextLevel(): number {
    return this.experienceToNextLevel;
  }

  getExperiencePercentage(): number {
    return (this.experience / this.experienceToNextLevel) * 100;
  }

  getInventory(): Inventory {
    return this.inventory;
  }
}