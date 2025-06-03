
import React, { useEffect, useRef } from 'react';
import { ScrollArea } from './ui/scroll-area';

interface BattleLogProps {
  messages: string[];
}

export const BattleLog: React.FC<BattleLogProps> = ({ messages }) => {
  const bottomRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    bottomRef.current?.scrollIntoView({ behavior: 'smooth' });
  }, [messages]);

  return (
    <div className="bg-gray-900 text-green-400 p-4 rounded-lg border border-gray-700 h-48">
      <h3 className="text-lg font-bold mb-2 text-green-300">Battle Log</h3>
      <ScrollArea className="h-32">
        <div className="space-y-1 text-sm font-mono">
          {messages.map((message, index) => (
            <div key={index} className="animate-fadeIn">
              {message}
            </div>
          ))}
          <div ref={bottomRef} />
        </div>
      </ScrollArea>
    </div>
  );
};
