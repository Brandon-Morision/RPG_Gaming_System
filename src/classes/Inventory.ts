import { Character } from './Character';
import { GAME_CONSTANTS } from '../types/game';

export class Inventory {
  private items: string[] = [];

  isEmpty(): boolean {
    return this.items.length === 0;
  }

  addItem(item: string): string {
    if (this.items.length >= GAME_CONSTANTS.INVENTORY_MAX_ITEMS) {
      return `Inventory is full! Cannot add ${item}.`;
    }
    this.items.push(item);
    return `${item} added to inventory!`;
  }

  useItem(index: number, player: Character, target: Character): { success: boolean; message: string } {
    if (index < 0 || index >= this.items.length) {
      return { success: false, message: "Invalid item selection!" };
    }

    const item = this.items[index];
    let message = "";

    switch (item) {
      case "Health Potion":
        const oldHealth = player.getHealth();
        player.setHealth(Math.min(player.getHealth() + GAME_CONSTANTS.HEALTH_POTION_HEAL_AMOUNT, player.getMaxHealth()));
        const healthGained = player.getHealth() - oldHealth;
        message = `Used ${item}. Health increased by ${healthGained}!`;
        break;
      
      case "Attack Boost":
        player.addTemporaryBuff("Attack Boost", GAME_CONSTANTS.ATTACK_BOOST_VALUE);
        message = `Used ${item}. Attack power increased by ${GAME_CONSTANTS.ATTACK_BOOST_VALUE} for this turn!`;
        break;
      
      case "Poison Dart":
        const poisonMessage = target.applyStatusEffect("Poison", 3);
        message = `Used ${item}. ${poisonMessage}`;
        break;
      
      default:
        return { success: false, message: "Invalid item effect!" };
    }

    this.items.splice(index, 1);
    return { success: true, message };
  }

  getItems(): string[] {
    return [...this.items];
  }

  clear(): void {
    this.items = [];
  }

  getSize(): number {
    return this.items.length;
  }

  getMaxSize(): number {
    return GAME_CONSTANTS.INVENTORY_MAX_ITEMS;
  }
}
