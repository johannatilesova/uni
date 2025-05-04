-- indexy na casto pouzivane foreign keys a joins pre tabulky:
-- combat_participants, combat_diary, inventory, dropped_items
CREATE INDEX idx_inventory_character_id ON inventory(character_id);
CREATE INDEX idx_inventory_item_id ON inventory(item_id);

CREATE INDEX idx_dropped_items_combat_id ON dropped_items(combat_id);
CREATE INDEX idx_dropped_items_item_id ON dropped_items(item_id);

CREATE INDEX idx_combat_participants_combat_id ON combat_participants(combat_id);
CREATE INDEX idx_combat_participants_character_id ON combat_participants(character_id);

-- indexovanie pre najviac filtrované stlpce vo WHERE
-- combat_participants.status, characters.in_combat, dropped_items.quantity
CREATE INDEX idx_combat_participants_status ON combat_participants(status);
CREATE INDEX idx_characters_in_combat ON characters(in_combat);
CREATE INDEX idx_dropped_items_quantity ON dropped_items(quantity);
