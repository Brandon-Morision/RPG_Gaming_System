
import React from 'react';

interface HealthBarProps {
  current: number;
  max: number;
  label?: string;
  color?: 'health' | 'mana' | 'xp';
  size?: 'sm' | 'md' | 'lg';
}

export const HealthBar: React.FC<HealthBarProps> = ({ 
  current, 
  max, 
  label, 
  color = 'health',
  size = 'md' 
}) => {
  const percentage = Math.max(0, Math.min(100, (current / max) * 100));
  
  const getColorClass = () => {
    switch (color) {
      case 'health':
        if (percentage > 60) return 'bg-green-500';
        if (percentage > 30) return 'bg-yellow-500';
        return 'bg-red-500';
      case 'mana':
        return 'bg-blue-500';
      case 'xp':
        return 'bg-purple-500';
      default:
        return 'bg-gray-500';
    }
  };

  const getSizeClass = () => {
    switch (size) {
      case 'sm': return 'h-2';
      case 'md': return 'h-4';
      case 'lg': return 'h-6';
      default: return 'h-4';
    }
  };

  return (
    <div className="w-full">
      {label && (
        <div className="flex justify-between text-sm font-medium mb-1">
          <span>{label}</span>
          <span>{current}/{max}</span>
        </div>
      )}
      <div className={`w-full ${getSizeClass()} bg-gray-200 rounded-full overflow-hidden`}>
        <div
          className={`${getSizeClass()} ${getColorClass()} transition-all duration-300 ease-in-out`}
          style={{ width: `${percentage}%` }}
        />
      </div>
    </div>
  );
};