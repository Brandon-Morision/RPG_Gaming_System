
import React, { useState, useEffect } from 'react';
import { Player } from '../classes/Player';
import { Mage } from '../classes/Mage';
import { Character } from '../classes/Character';
import { CharacterCard } from './CharacterCard';
import { BattleLog } from './BattleLog';
import { InventoryPanel } from './InventoryPanel';
import { Button } from './ui/button';
import { useToast } from './ui/use-toast';
import { Sword, Shield, Zap, Package, Pause, Home } from 'lucide-react';

interface GameScreenProps {
  player: Player;
  onReturnToMenu: () => void;
}

export const GameScreen: React.FC<GameScreenProps> = ({ player, onReturnToMenu }) => {
  const [enemies] = useState<Mage[]>([
    new Mage("Dark Mage", 50, 10),
    new Mage("Shadow Wizard", 70, 12)
  ]);
  
  const [currentEnemyIndex, setCurrentEnemyIndex] = useState(0);
  const [battleLog, setBattleLog] = useState<string[]>(['Battle begins!']);
  const [turnPhase, setTurnPhase] = useState<'player' | 'enemy' | 'battle-end'>('player');
  const [gameOver, setGameOver] = useState(false);
  const [victory, setVictory] = useState(false);
  const { toast } = useToast();

  const currentEnemy = enemies[currentEnemyIndex];

  const addToBattleLog = (message: string) => {
    setBattleLog(prev => [...prev, message]);
  };

  const processEffects = (character: Character, label: string) => {
    const effectMessages = character.updateEffects();
    effectMessages.forEach(msg => addToBattleLog(msg));
    
    if (!character.isAlive() && label === "Enemy") {
      addToBattleLog(`${character.getName()} has been defeated!`);
    }
  };

  const handlePlayerAction = async (action: string, itemIndex?: number) => {
    if (turnPhase !== 'player' || gameOver) return;

    let actionMessage = '';
    let actionTaken = false;

    switch (action) {
      case 'attack':
        const attackResult = player.attack(currentEnemy);
        actionMessage = attackResult.message;
        actionTaken = true;
        break;

      case 'special':
        if (player.canUseSpecialMove()) {
          const specialResult = player.specialMove(currentEnemy);
          actionMessage = specialResult.message;
          actionTaken = true;
        } else {
          toast({
            title: "Special Move Unavailable",
            description: `Special move is on cooldown for ${player.getSpecialCooldown()} more turns!`,
            variant: "destructive"
          });
          return;
        }
        break;

      case 'defend':
        actionMessage = player.defend();
        actionTaken = true;
        break;

      case 'item':
        if (itemIndex !== undefined) {
          const result = player.getInventory().useItem(itemIndex, player, currentEnemy);
          if (result.success) {
            actionMessage = result.message;
            actionTaken = true;
          } else {
            toast({
              title: "Item Error",
              description: result.message,
              variant: "destructive"
            });
            return;
          }
        }
        break;
    }

    if (actionTaken) {
      addToBattleLog(actionMessage);
      player.reduceCooldown();
      processEffects(player, "Player");

      // Check if enemy is defeated
      if (!currentEnemy.isAlive()) {
        const xpResult = player.gainExperience(20);
        addToBattleLog(xpResult.message);
        
        if (xpResult.leveledUp) {
          toast({
            title: "Level Up!",
            description: `${player.getName()} has leveled up!`,
          });
        }

        // Bonus item chance
        if (Math.random() < 0.5) {
          const bonusMessage = player.getInventory().addItem("Poison Dart");
          addToBattleLog(bonusMessage);
        }

        // Check if all enemies defeated
        if (currentEnemyIndex >= enemies.length - 1) {
          setVictory(true);
          setGameOver(true);
          addToBattleLog("🎉 Victory! You have defeated all enemies!");
          return;
        } else {
          setCurrentEnemyIndex(prev => prev + 1);
          addToBattleLog(`Next enemy approaches: ${enemies[currentEnemyIndex + 1].getName()}`);
          setTurnPhase('player');
          return;
        }
      }

      setTurnPhase('enemy');
      
      // Enemy turn after delay
      setTimeout(() => {
        if (currentEnemy.isAlive()) {
          addToBattleLog("Enemy's turn...");
          
          setTimeout(() => {
            const enemyAction = currentEnemy.makeDecision!(player);
            addToBattleLog(enemyAction);
            
            if (currentEnemy instanceof Mage) {
              currentEnemy.regenerateMana();
            }
            
            currentEnemy.reduceCooldown();
            processEffects(currentEnemy, "Enemy");

            if (!player.isAlive()) {
              setGameOver(true);
              addToBattleLog("💀 Game Over! You have been defeated...");
              return;
            }

            setTurnPhase('player');
          }, 1000);
        }
      }, 1500);
    }
  };

  const handlePause = () => {
    const saveResult = player.saveProgress();
    toast({
      title: saveResult.success ? "Game Saved" : "Save Error",
      description: saveResult.message,
      variant: saveResult.success ? "default" : "destructive"
    });
  };

  if (gameOver) {
    return (
      <div className="min-h-screen bg-gradient-to-br from-purple-900 to-blue-900 flex items-center justify-center p-4">
        <div className="bg-white rounded-lg p-8 text-center max-w-md w-full">
          <h2 className={`text-3xl font-bold mb-4 ${victory ? 'text-green-600' : 'text-red-600'}`}>
            {victory ? '🎉 Victory!' : '💀 Game Over'}
          </h2>
          <p className="text-gray-600 mb-6">
            {victory 
              ? 'Congratulations! You have defeated all enemies and saved the realm!'
              : 'Your journey ends here, but you can try again!'
            }
          </p>
          <Button onClick={onReturnToMenu} className="w-full">
            Return to Main Menu
          </Button>
        </div>
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-900 to-slate-700 p-4">
      <div className="max-w-7xl mx-auto">
        <div className="flex justify-between items-center mb-6">
          <h1 className="text-3xl font-bold text-white">Battle Arena</h1>
          <div className="flex space-x-2">
            <Button variant="outline" size="sm" onClick={handlePause}>
              <Pause className="w-4 h-4 mr-2" />
              Save Game
            </Button>
            <Button variant="outline" size="sm" onClick={onReturnToMenu}>
              <Home className="w-4 h-4 mr-2" />
              Main Menu
            </Button>
          </div>
        </div>

        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          {/* Player Stats */}
          <div className="space-y-4">
            <CharacterCard character={player} isPlayer={true} />
            <InventoryPanel
              inventory={player.getInventory()}
              onUseItem={(index) => handlePlayerAction('item', index)}
              disabled={turnPhase !== 'player'}
            />
          </div>

          {/* Battle Area */}
          <div className="space-y-4">
            <CharacterCard character={currentEnemy} />
            
            <div className="bg-white p-4 rounded-lg">
              <h3 className="text-lg font-bold mb-4 text-center">Your Actions</h3>
              <div className="grid grid-cols-2 gap-3">
                <Button
                  onClick={() => handlePlayerAction('attack')}
                  disabled={turnPhase !== 'player'}
                  className="flex items-center justify-center space-x-2"
                >
                  <Sword className="w-4 h-4" />
                  <span>Attack</span>
                </Button>
                
                <Button
                  onClick={() => handlePlayerAction('special')}
                  disabled={turnPhase !== 'player' || !player.canUseSpecialMove()}
                  variant={player.canUseSpecialMove() ? "default" : "secondary"}
                  className="flex items-center justify-center space-x-2"
                >
                  <Zap className="w-4 h-4" />
                  <span>Special</span>
                </Button>
                
                <Button
                  onClick={() => handlePlayerAction('defend')}
                  disabled={turnPhase !== 'player'}
                  variant="outline"
                  className="flex items-center justify-center space-x-2"
                >
                  <Shield className="w-4 h-4" />
                  <span>Defend</span>
                </Button>
                
                <Button
                  disabled
                  variant="outline"
                  className="flex items-center justify-center space-x-2"
                >
                  <Package className="w-4 h-4" />
                  <span>Use Item</span>
                </Button>
              </div>
              
              <div className="mt-4 text-center">
                <div className={`text-sm font-medium ${
                  turnPhase === 'player' ? 'text-green-600' : 'text-orange-600'
                }`}>
                  {turnPhase === 'player' ? '⚔️ Your Turn' : '⏳ Enemy Turn'}
                </div>
              </div>
            </div>
          </div>

          {/* Battle Log */}
          <div>
            <BattleLog messages={battleLog} />
          </div>
        </div>
      </div>
    </div>
  );
};