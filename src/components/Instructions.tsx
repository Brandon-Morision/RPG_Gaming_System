
import React from 'react';
import { Button } from './ui/button';
import { Card, CardContent, CardHeader, CardTitle } from './ui/card';
import { ArrowLeft, Sword, Shield, Zap, Package, Heart, Star } from 'lucide-react';

interface InstructionsProps {
  onBack: () => void;
}

export const Instructions: React.FC<InstructionsProps> = ({ onBack }) => {
  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-900 to-slate-700 p-4">
      <div className="max-w-4xl mx-auto">
        <div className="flex items-center space-x-4 mb-6">
          <Button onClick={onBack} variant="outline" size="sm">
            <ArrowLeft className="w-4 h-4 mr-2" />
            Back to Menu
          </Button>
          <h1 className="text-3xl font-bold text-white">Game Instructions</h1>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          <Card>
            <CardHeader>
              <CardTitle className="flex items-center space-x-2">
                <Sword className="w-5 h-5 text-orange-500" />
                <span>Combat Actions</span>
              </CardTitle>
            </CardHeader>
            <CardContent className="space-y-3">
              <div className="flex items-start space-x-3">
                <Sword className="w-4 h-4 text-orange-500 mt-1" />
                <div>
                  <div className="font-semibold">Regular Attack</div>
                  <div className="text-sm text-gray-600">Deal damage based on your attack power</div>
                </div>
              </div>
              
              <div className="flex items-start space-x-3">
                <Zap className="w-4 h-4 text-yellow-500 mt-1" />
                <div>
                  <div className="font-semibold">Special Move</div>
                  <div className="text-sm text-gray-600">Powerful attack with 2-turn cooldown</div>
                </div>
              </div>
              
              <div className="flex items-start space-x-3">
                <Shield className="w-4 h-4 text-blue-500 mt-1" />
                <div>
                  <div className="font-semibold">Defend</div>
                  <div className="text-sm text-gray-600">Reduce incoming damage significantly</div>
                </div>
              </div>
            </CardContent>
          </Card>

          <Card>
            <CardHeader>
              <CardTitle className="flex items-center space-x-2">
                <Package className="w-5 h-5 text-amber-500" />
                <span>Items & Inventory</span>
              </CardTitle>
            </CardHeader>
            <CardContent className="space-y-3">
              <div className="flex items-start space-x-3">
                <Heart className="w-4 h-4 text-red-500 mt-1" />
                <div>
                  <div className="font-semibold">Health Potion</div>
                  <div className="text-sm text-gray-600">Restores 20 health points</div>
                </div>
              </div>
              
              <div className="flex items-start space-x-3">
                <Sword className="w-4 h-4 text-orange-500 mt-1" />
                <div>
                  <div className="font-semibold">Attack Boost</div>
                  <div className="text-sm text-gray-600">+10 attack power for one turn</div>
                </div>
              </div>
              
              <div className="flex items-start space-x-3">
                <div className="w-4 h-4 bg-purple-500 rounded-full mt-1" />
                <div>
                  <div className="font-semibold">Poison Dart</div>
                  <div className="text-sm text-gray-600">Poisons enemy for 3 turns</div>
                </div>
              </div>
            </CardContent>
          </Card>

          <Card>
            <CardHeader>
              <CardTitle className="flex items-center space-x-2">
                <Star className="w-5 h-5 text-purple-500" />
                <span>Character Progression</span>
              </CardTitle>
            </CardHeader>
            <CardContent className="space-y-3">
              <div>
                <div className="font-semibold mb-2">Level Up System</div>
                <div className="text-sm text-gray-600 space-y-1">
                  <div>• Gain 20 XP per defeated enemy</div>
                  <div>• Level up increases health and attack</div>
                  <div>• Full health restoration on level up</div>
                  <div>• XP requirement increases each level</div>
                </div>
              </div>
            </CardContent>
          </Card>

          <Card>
            <CardHeader>
              <CardTitle className="flex items-center space-x-2">
                <div className="w-5 h-5 bg-gradient-to-r from-red-500 to-purple-500 rounded" />
                <span>Status Effects</span>
              </CardTitle>
            </CardHeader>
            <CardContent className="space-y-3">
              <div>
                <div className="font-semibold text-purple-600">Poison</div>
                <div className="text-sm text-gray-600">Deals 8 damage per turn</div>
              </div>
              
              <div>
                <div className="font-semibold text-orange-600">Burn</div>
                <div className="text-sm text-gray-600">Deals 5 damage per turn</div>
              </div>
              
              <div>
                <div className="font-semibold text-blue-600">Defensive Stance</div>
                <div className="text-sm text-gray-600">Reduces incoming damage</div>
              </div>
            </CardContent>
          </Card>

          <Card className="md:col-span-2">
            <CardHeader>
              <CardTitle>Game Tips</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4 text-sm">
                <div>
                  <div className="font-semibold mb-2">Combat Strategy</div>
                  <div className="text-gray-600 space-y-1">
                    <div>• Use defend when low on health</div>
                    <div>• Save special moves for critical moments</div>
                    <div>• Items don't end your turn - use them wisely</div>
                    <div>• Status effects can turn the tide of battle</div>
                  </div>
                </div>
                
                <div>
                  <div className="font-semibold mb-2">Progression</div>
                  <div className="text-gray-600 space-y-1">
                    <div>• Save your progress frequently</div>
                    <div>• Inventory has a maximum of 5 items</div>
                    <div>• Defeated enemies may drop bonus items</div>
                    <div>• Each enemy type has unique AI behavior</div>
                  </div>
                </div>
              </div>
            </CardContent>
          </Card>
        </div>
      </div>
    </div>
  );
};