import React, { useState } from 'react';
import { Player } from '../classes/Player';
import { Button } from './ui/button';
import { Input } from './ui/input';
import { Card, CardContent, CardHeader, CardTitle } from './ui/card';
import { useToast } from './ui/use-toast';
import { Sword, BookOpen, Save, Play } from 'lucide-react';

interface MainMenuProps {
  onStartGame: (player: Player) => void;
  onShowInstructions: () => void;
}

export const MainMenu: React.FC<MainMenuProps> = ({ onStartGame, onShowInstructions }) => {
  const [playerName, setPlayerName] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const { toast } = useToast();

  const handleNewGame = () => {
    if (!playerName.trim()) {
      toast({
        title: "Name Required",
        description: "Please enter your character's name!",
        variant: "destructive"
      });
      return;
    }
    
    setIsLoading(true);
    const player = new Player(playerName.trim(), 100, 15);
    player.reset();
    
    setTimeout(() => {
      setIsLoading(false);
      onStartGame(player);
    }, 500);
  };

  const handleLoadGame = () => {
    setIsLoading(true);
    const player = new Player("Temp", 100, 15);
    const loadResult = player.loadProgress();
    
    setTimeout(() => {
      setIsLoading(false);
      if (loadResult.success) {
        toast({
          title: "Game Loaded",
          description: loadResult.message,
        });
        onStartGame(player);
      } else {
        toast({
          title: "Load Error",
          description: loadResult.message,
          variant: "destructive"
        });
      }
    }, 500);
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-purple-900 via-blue-900 to-indigo-900 flex items-center justify-center p-4">
      <div className="w-full max-w-md space-y-6">
        <div className="text-center">
          <h1 className="text-5xl font-bold text-white mb-2 bg-gradient-to-r from-yellow-400 to-orange-500 bg-clip-text text-transparent">
            RPG Quest
          </h1>
          <p className="text-blue-200 text-lg">Embark on an Epic Adventure</p>
        </div>

        <Card className="backdrop-blur-lg bg-white/10 border-white/20 text-white">
          <CardHeader>
            <CardTitle className="flex items-center space-x-2">
              <Sword className="w-6 h-6 text-yellow-400" />
              <span>Create Your Hero</span>
            </CardTitle>
          </CardHeader>
          <CardContent className="space-y-4">
            <div>
              <label htmlFor="playerName" className="block text-sm font-medium mb-2">
                Character Name
              </label>
              <Input
                id="playerName"
                type="text"
                placeholder="Enter your hero's name..."
                value={playerName}
                onChange={(e) => setPlayerName(e.target.value)}
                className="bg-white/20 border-white/30 text-white placeholder-white/70"
                onKeyPress={(e) => e.key === 'Enter' && handleNewGame()}
                maxLength={20}
              />
            </div>

            <div className="space-y-3">
              <Button
                onClick={handleNewGame}
                disabled={isLoading}
                className="w-full bg-gradient-to-r from-green-500 to-emerald-600 hover:from-green-600 hover:to-emerald-700 text-white font-semibold"
              >
                <Play className="w-4 h-4 mr-2" />
                {isLoading ? 'Starting...' : 'Start New Game'}
              </Button>

              <Button
                onClick={handleLoadGame}
                disabled={isLoading}
                variant="outline"
                className="w-full border-white/30 text-white hover:bg-white/20"
              >
                <Save className="w-4 h-4 mr-2" />
                {isLoading ? 'Loading...' : 'Load Saved Game'}
              </Button>

              <Button
                onClick={onShowInstructions}
                variant="outline"
                className="w-full border-white/30 text-white hover:bg-white/20"
              >
                <BookOpen className="w-4 h-4 mr-2" />
                Instructions
              </Button>
            </div>
          </CardContent>
        </Card>

        <div className="text-center text-blue-200 text-sm">
          <p>⚔️ Battle fierce enemies • 🧙‍♂️ Master magic and combat • 📈 Level up your hero</p>
        </div>
      </div>
    </div>
  );
};