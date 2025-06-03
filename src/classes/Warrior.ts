
import { Character } from './Character';
import { GAME_CONSTANTS } from '../types/game';

export class Warrior extends Character {
  specialMove(opponent: Character): { damage: number; message: string } {
    const damage = this.attackPower * 2;
    const actualDamage = opponent.takeDamage(damage);
    this.setSpecialCooldown(GAME_CONSTANTS.WARRIOR_SPECIAL_COOLDOWN);
    
    return {
      damage: actualDamage,
      message: `${this.name} performs a Heavy Strike on ${opponent.getName()} for ${actualDamage} damage!`
    };
  }

  makeDecision(player: Character): string {
    // Warrior AI decision making - simple but effective
    if (this.health < this.maxHealth / 3) {
      // Low health - prioritize defense
      if (Math.random() < 0.6) {
        return this.defend();
      }
    }

    if (this.canUseSpecialMove()) {
      if (player.getHealth() <= (this.attackPower * 2) || Math.random() < 0.3) {
        const result = this.specialMove(player);
        return result.message;
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
}