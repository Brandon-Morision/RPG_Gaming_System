import { Character } from './Character';
import { GAME_CONSTANTS } from '../types/game';

export class Mage extends Character {
  private manaPoints: number;

  constructor(name: string, health: number, attackPower: number) {
    super(name, health, attackPower);
    this.manaPoints = GAME_CONSTANTS.MAX_MANA;
  }

  specialMove(opponent: Character): { damage: number; message: string; additionalEffects: string[] } {
    const damage = this.attackPower + 15;
    const actualDamage = opponent.takeDamage(damage);
    const burnMessage = opponent.applyStatusEffect("Burn", 2);
    this.manaPoints = Math.max(0, this.manaPoints - GAME_CONSTANTS.MAGE_MANA_COST_FIREBALL);
    this.setSpecialCooldown(GAME_CONSTANTS.MAGE_SPECIAL_COOLDOWN);
    
    return {
      damage: actualDamage,
      message: `${this.name} casts Fireball on ${opponent.getName()} for ${actualDamage} damage!`,
      additionalEffects: [burnMessage]
    };
  }

  makeDecision(player: Character): string {
    // AI decision making
    if (this.health < this.maxHealth / 3) {
      if (Math.random() < 0.7) {
        return this.defend();
      }
    }

    if (this.canUseSpecialMove() && this.manaPoints >= GAME_CONSTANTS.MAGE_MANA_COST_FIREBALL) {
      if (player.getHealth() <= (this.attackPower + 15) || Math.random() < 0.4) {
        const result = this.specialMove(player);
        return result.message + (result.additionalEffects ? ' ' + result.additionalEffects.join(' ') : '');
      } else {
        const result = this.attack(player);
        return result.message;
      }
    } else {
      if (Math.random() < 0.2) {
        return this.defend();
      } else {
        const result = this.attack(player);
        return result.message;
      }
    }
  }

  regenerateMana(): void {
    this.manaPoints = Math.min(GAME_CONSTANTS.MAX_MANA, this.manaPoints + GAME_CONSTANTS.MAGE_MANA_REGEN_PER_TURN);
  }

  getManaPoints(): number {
    return this.manaPoints;
  }

  getManaPercentage(): number {
    return (this.manaPoints / GAME_CONSTANTS.MAX_MANA) * 100;
  }
}