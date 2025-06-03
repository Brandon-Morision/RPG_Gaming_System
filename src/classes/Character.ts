
import { StatusEffect, TemporaryBuff, GAME_CONSTANTS } from '../types/game';

export abstract class Character {
  protected name: string;
  protected health: number;
  protected maxHealth: number;
  protected attackPower: number;
  protected specialCooldown: number;
  protected defense: number;
  protected statusEffects: StatusEffect[];
  protected temporaryBuffs: TemporaryBuff[];

  constructor(name: string, health: number, attackPower: number) {
    this.name = name;
    this.health = health;
    this.maxHealth = health;
    this.attackPower = attackPower;
    this.specialCooldown = 0;
    this.defense = 0;
    this.statusEffects = [];
    this.temporaryBuffs = [];
  }

  setHealth(h: number): void {
    this.health = h > 0 ? h : 0;
  }

  setAttackPower(a: number): void {
    this.attackPower = a > 0 ? a : this.attackPower;
  }

  attack(opponent: Character): { damage: number; message: string } {
    const totalAttack = this.calculateTotalAttack();
    const actualDamage = opponent.takeDamage(totalAttack);
    const message = `${this.name} attacks ${opponent.getName()} for ${actualDamage} damage!`;
    return { damage: actualDamage, message };
  }

  takeDamage(damage: number): number {
    const totalDefense = this.calculateTotalDefense();
    const actualDamage = Math.max(0, damage - totalDefense);
    this.setHealth(this.health - actualDamage);
    return actualDamage;
  }

  defend(): string {
    this.defense = GAME_CONSTANTS.DEFENSE_BONUS_FROM_DEFEND;
    this.addTemporaryBuff("Defensive Stance", GAME_CONSTANTS.DEFENSIVE_STANCE_BUFF_VALUE);
    return `${this.name} assumes a defensive stance, reducing incoming damage!`;
  }

  applyStatusEffect(effect: string, duration: number): string {
    this.removeStatusEffect(effect);
    this.statusEffects.push({ name: effect, duration });
    return `${this.name} is affected by ${effect} for ${duration} turns!`;
  }

  removeStatusEffect(effect: string): void {
    this.statusEffects = this.statusEffects.filter(e => e.name !== effect);
  }

  addTemporaryBuff(buff: string, value: number): void {
    const existing = this.temporaryBuffs.find(b => b.name === buff);
    if (existing) {
      existing.value = value;
    } else {
      this.temporaryBuffs.push({ name: buff, value });
    }
  }

  updateEffects(): string[] {
    const messages: string[] = [];

    // Process status effects
    this.statusEffects = this.statusEffects.filter(effect => {
      if (effect.name === "Poison") {
        this.takeDamage(GAME_CONSTANTS.POISON_DAMAGE_PER_TURN);
        messages.push(`${this.name} takes ${GAME_CONSTANTS.POISON_DAMAGE_PER_TURN} poison damage!`);
      } else if (effect.name === "Burn") {
        this.takeDamage(GAME_CONSTANTS.BURN_DAMAGE_PER_TURN);
        messages.push(`${this.name} takes ${GAME_CONSTANTS.BURN_DAMAGE_PER_TURN} burn damage!`);
      }
      
      effect.duration--;
      if (effect.duration <= 0) {
        messages.push(`${this.name} is no longer affected by ${effect.name}!`);
        return false;
      }
      return true;
    });

    // Clear temporary buffs
    this.temporaryBuffs = [];
    this.defense = 0;

    return messages;
  }

  calculateTotalAttack(): number {
    let total = this.attackPower;
    for (const buff of this.temporaryBuffs) {
      if (buff.name === "Attack Boost") {
        total += buff.value;
      }
    }
    return total;
  }

  calculateTotalDefense(): number {
    let total = this.defense;
    for (const buff of this.temporaryBuffs) {
      if (buff.name === "Defensive Stance") {
        total += buff.value;
      }
    }
    return total;
  }

  getStatusEffectsDisplay(): string {
    if (this.statusEffects.length === 0) return "None";
    return this.statusEffects.map(e => `${e.name}(${e.duration})`).join(", ");
  }

  abstract specialMove(opponent: Character): { damage: number; message: string; additionalEffects?: string[] };
  abstract makeDecision?(player: Character): string;

  isAlive(): boolean {
    return this.health > 0;
  }

  getName(): string {
    return this.name;
  }

  getHealth(): number {
    return this.health;
  }

  getAttackPower(): number {
    return this.attackPower;
  }

  getMaxHealth(): number {
    return this.maxHealth;
  }

  reduceCooldown(): void {
    if (this.specialCooldown > 0) {
      this.specialCooldown--;
    }
  }

  canUseSpecialMove(): boolean {
    return this.specialCooldown === 0;
  }

  getSpecialCooldown(): number {
    return this.specialCooldown;
  }

  setSpecialCooldown(turns: number): void {
    this.specialCooldown = turns;
  }

  getStatusEffects(): StatusEffect[] {
    return [...this.statusEffects];
  }

  getTemporaryBuffs(): TemporaryBuff[] {
    return [...this.temporaryBuffs];
  }
}