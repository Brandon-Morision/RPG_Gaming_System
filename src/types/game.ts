// Game constants and types
export const GAME_CONSTANTS = {
  HEALTH_POTION_HEAL_AMOUNT: 20,
  ATTACK_BOOST_VALUE: 10,
  POISON_DAMAGE_PER_TURN: 8,
  BURN_DAMAGE_PER_TURN: 5,
  DEFENSE_BONUS_FROM_DEFEND: 15,
  DEFENSIVE_STANCE_BUFF_VALUE: 5,
  
  WARRIOR_SPECIAL_COOLDOWN: 2,
  MAGE_SPECIAL_COOLDOWN: 2,
  MAGE_MANA_COST_FIREBALL: 30,
  MAGE_MANA_REGEN_PER_TURN: 15,
  MAX_MANA: 100,
  
  PLAYER_INITIAL_HEALTH: 100,
  PLAYER_INITIAL_ATTACK_POWER: 15,
  PLAYER_INITIAL_XP_TO_LEVEL: 50,
  XP_GAIN_PER_ENEMY: 20,
  XP_INCREASE_PER_LEVEL: 20,
  HEALTH_INCREASE_PER_LEVEL: 20,
  ATTACK_INCREASE_PER_LEVEL: 5,
  
  INVENTORY_MAX_ITEMS: 5,
};

export interface StatusEffect {
  name: string;
  duration: number;
}

export interface TemporaryBuff {
  name: string;
  value: number;
}

export interface GameState {
  currentScreen: 'menu' | 'battle' | 'instructions' | 'pause';
  battleLog: string[];
  currentEnemyIndex: number;
  turnPhase: 'player' | 'enemy' | 'battle-end';
}