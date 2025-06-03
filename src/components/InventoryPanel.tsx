
import React from 'react';
import { Inventory } from '../classes/Inventory';
import { Button } from './ui/button';
import { Package } from 'lucide-react';

interface InventoryPanelProps {
  inventory: Inventory;
  onUseItem: (index: number) => void;
  disabled?: boolean;
}

export const InventoryPanel: React.FC<InventoryPanelProps> = ({ 
  inventory, 
  onUseItem, 
  disabled = false 
}) => {
  const items = inventory.getItems();

  return (
    <div className="bg-amber-50 p-4 rounded-lg border border-amber-200">
      <div className="flex items-center space-x-2 mb-3">
        <Package className="w-5 h-5 text-amber-600" />
        <h3 className="text-lg font-bold text-amber-800">
          Inventory ({inventory.getSize()}/{inventory.getMaxSize()})
        </h3>
      </div>
      
      {items.length === 0 ? (
        <p className="text-amber-600 text-sm italic">Your inventory is empty</p>
      ) : (
        <div className="space-y-2">
          {items.map((item, index) => (
            <div key={index} className="flex items-center justify-between bg-white p-2 rounded border">
              <span className="text-sm font-medium">{item}</span>
              <Button
                size="sm"
                variant="outline"
                onClick={() => onUseItem(index)}
                disabled={disabled}
                className="text-xs"
              >
                Use
              </Button>
            </div>
          ))}
        </div>
      )}
    </div>
  );
};
