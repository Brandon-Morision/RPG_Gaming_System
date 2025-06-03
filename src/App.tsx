import React, { useState } from 'react';
import { Player } from './classes/Player';
import { MainMenu } from './components/MainMenu';
import { GameScreen } from './components/GameScreen';
import { Instructions } from './components/Instructions';
import { Toaster } from './components/ui/toaster';

type Screen = 'menu' | 'game' | 'instructions';

function App() {
  const [currentScreen, setCurrentScreen] = useState<Screen>('menu');
  const [player, setPlayer] = useState<Player | null>(null);

  const handleStartGame = (newPlayer: Player) => {
    setPlayer(newPlayer);
    setCurrentScreen('game');
  };

  const handleReturnToMenu = () => {
    setCurrentScreen('menu');
    setPlayer(null);
  };

  const handleShowInstructions = () => {
    setCurrentScreen('instructions');
  };

  const handleBackToMenu = () => {
    setCurrentScreen('menu');
  };

  return (
    <div className="min-h-screen">
      {currentScreen === 'menu' && (
        <MainMenu 
          onStartGame={handleStartGame}
          onShowInstructions={handleShowInstructions}
        />
      )}
      
      {currentScreen === 'game' && player && (
        <GameScreen 
          player={player}
          onReturnToMenu={handleReturnToMenu}
        />
      )}
      
      {currentScreen === 'instructions' && (
        <Instructions onBack={handleBackToMenu} />
      )}
      
      <Toaster />
    </div>
  );
}

export default App;