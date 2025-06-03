
import React from 'react';
import { Character } from '../classes/Character';
import { Player } from '../classes/Player';
import { Mage } from '../classes/Mage';
import { HealthBar } from './HealthBar';
import { Shield, Sword, Zap } from 'lucide-react';

interface CharacterCardProps {
  character: Character;
  isPlayer?: boolean;
}

export const CharacterCard: React.FC<CharacterCardProps> = ({ character, isPlayer = false }) => {
  const player = isPlayer ? character as Player : null;
  const mage = character instanceof Mage ? character as Mage : null;
  
  return (
    <div className={`p-6 rounded-lg border-2 ${isPlayer ? 'border-blue-500 bg-blue-50' : 'border-red-500 bg-red-50'} shadow-lg`}>
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-xl font-bold">
          {character.getName()}
          {player && ` (Level ${player.getLevel()})`}
        </h3>
        <div className="flex items-center space-x-2">
          <Sword className="w-5 h-5 text-orange-600" />
          <span className="font-semibold">{character.getAttackPower()}</span>
          {character.calculateTotalDefense() > 0 && (
            <>
              <Shield className="w-5 h-5 text-blue-600 ml-2" />
              <span className="font-semibold">{character.calculateTotalDefense()}</span>
            </>
          )}
        </div>
      </div>

      <div className="space-y-3">
        <HealthBar
          current={character.getHealth()}
          max={character.getMaxHealth()}
          label="Health"
          color="health"
          size="md"
        />

        {mage && (
          <HealthBar
            current={mage.getManaPoints()}
            max={100}
            label="Mana"
            color="mana"
            size="sm"
          />
        )}

        {player && (
          <HealthBar
            current={player.getExperience()}
            max={player.getExperienceToNextLevel()}
            label="Experience"
            color="xp"
            size="sm"
          />
        )}

        <div className="mt-4">
          <div className="text-sm font-medium mb-2">Status Effects:</div>
          <div className="text-sm">
            {character.getStatusEffectsDisplay() === "None" ? (
              <span className="text-gray-500">None</span>
            ) : (
              <span className="text-purple-600 font-medium">
                {character.getStatusEffectsDisplay()}
              </span>
            )}
          </div>
        </div>

        <div className="mt-3">
          <div className="flex items-center space-x-2">
            <Zap className="w-4 h-4 text-yellow-600" />
            <span className="text-sm font-medium">Special Move:</span>
            {character.canUseSpecialMove() ? (
              <span className="text-green-600 font-bold">READY!</span>
            ) : (
              <span className="text-red-600">{character.getSpecialCooldown()} turns</span>
            )}
          </div>
        </div>
      </div>
    </div>
  );
};