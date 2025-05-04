-- 4. sp_loot_item
-- proces, pri ktorom postava zoberire predmet z bojovej oblasti.
-- zahrna kontrolu limitov inventára a aktualizáciu vlastníctva predmetu

CREATE OR REPLACE FUNCTION sp_loot_item(
    p_combat_id INTEGER,
    p_character_id INTEGER,
    p_item_id INTEGER
) RETURNS VOID AS $$
DECLARE
    v_strength INT;
    v_constitution INT;
    v_inventory_modifier INT;
    v_max_inventory_weight INT;
    v_current_inventory_weight INT;
    v_item_weight INT;
    v_quantity INT;
BEGIN
    -- Check if the item exists in dropped_items
    SELECT quantity INTO v_quantity
    FROM dropped_items
    WHERE combat_id = p_combat_id
      AND item_id = p_item_id
      AND quantity > 0
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Item not available for looting.';
    END IF;

    -- Get character attributes
    SELECT ca1.value, ca2.value
    INTO v_strength, v_constitution
    FROM character_attributes ca1
    JOIN character_attributes ca2 ON ca1.character_id = ca2.character_id
    WHERE ca1.character_id = p_character_id
      AND ca1.attribute_name = 'strength'
      AND ca2.attribute_name = 'constitution';

    -- Get class inventory modifier
    SELECT c.inventory_modifier
    INTO v_inventory_modifier
    FROM characters ch
    JOIN class c ON ch.class_id = c.id
    WHERE ch.id = p_character_id;

    -- Calculate max inventory weight
    v_max_inventory_weight := (v_strength + v_constitution) * v_inventory_modifier;

    -- Calculate current inventory weight
    SELECT COALESCE(SUM(i.weight * inv.quantity), 0)
    INTO v_current_inventory_weight
    FROM inventory inv
    JOIN items i ON inv.item_id = i.id
    WHERE inv.character_id = p_character_id;

    -- Get item weight
    SELECT weight
    INTO v_item_weight
    FROM items
    WHERE id = p_item_id;

    -- Check if item fits into inventory
    IF (v_current_inventory_weight + v_item_weight) <= v_max_inventory_weight THEN
        -- Add item to inventory
        INSERT INTO inventory (character_id, item_id, quantity, equipped)
        VALUES (p_character_id, p_item_id, 1, false)
        ON CONFLICT (character_id, item_id) DO UPDATE
        SET quantity = inventory.quantity + 1;

        -- Remove the item from dropped_items
        UPDATE dropped_items
        SET quantity = quantity - 1
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id;

        -- If quantity becomes zero, delete the row
        DELETE FROM dropped_items
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id
          AND quantity <= 0;

    ELSE
        -- Item is too heavy to carry
        RAISE EXCEPTION 'Item too heavy, cannot loot.';
    END IF;
END;
$$ LANGUAGE plpgsql;

-- update pomocou funkcii na vypocet inventaru
CREATE OR REPLACE FUNCTION sp_loot_item(
    p_combat_id INTEGER,
    p_character_id INTEGER,
    p_item_id INTEGER
) RETURNS VOID AS $$
DECLARE
    v_current_inventory_weight NUMERIC;
    v_max_inventory_weight NUMERIC;
    v_item_weight INT;
    v_quantity INT;
BEGIN
    -- Check if the item exists in dropped_items
    SELECT quantity
    INTO v_quantity
    FROM dropped_items
    WHERE combat_id = p_combat_id
      AND item_id = p_item_id
      AND quantity > 0
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Item not available for looting.';
    END IF;

    -- Get item weight
    SELECT weight
    INTO v_item_weight
    FROM items
    WHERE id = p_item_id;

    -- Use your new functions to get current weight and max weight
    v_current_inventory_weight := f_current_inventory_weight(p_character_id);
    v_max_inventory_weight := f_max_inventory_capacity(p_character_id);

    -- Check if item fits into inventory
    IF (v_current_inventory_weight + v_item_weight) <= v_max_inventory_weight THEN
        -- Add item to inventory
        INSERT INTO inventory (character_id, item_id, quantity, equipped)
        VALUES (p_character_id, p_item_id, 1, FALSE)
        ON CONFLICT (character_id, item_id) DO UPDATE
        SET quantity = inventory.quantity + 1;

        -- Remove the item from dropped_items
        UPDATE dropped_items
        SET quantity = quantity - 1
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id;

        -- If quantity becomes zero, delete the row
        DELETE FROM dropped_items
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id
          AND quantity <= 0;
    ELSE
        -- Item is too heavy to carry
        RAISE EXCEPTION 'Item too heavy, cannot loot.';
    END IF;
END;
$$ LANGUAGE plpgsql;