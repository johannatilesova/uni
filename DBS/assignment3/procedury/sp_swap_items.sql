-- extra procedúra: sp_swap_items
-- procedúra sluzi na vymenu items, ak nastane prípad, ze charakter chce zobrat nejaky item z herneho pola,
-- ale jeho inventár je uz plny, tak má moznost vyhodit nejaky item zo svojho inventára, cim uvolni miesto pre novy item..
-- krok 1: najde item, porovna vahu, zisti, ze uz nema miesto
-- krol 2: vyhodí item zo svojho inventory, ktory má rovnaku alebo vaciu vahu ako item ktory chce zobrat
-- krok 3: opat prepocita miesto v inventory, ak má uz miesto (co by mal mat, ale double check what if)
-- krok 4: vezme novy item (vymaze sa z dropped items) a stary necha na hernom poli (zapise sa do dropped items)

CREATE OR REPLACE FUNCTION sp_swap_items(
    p_character_id INTEGER,
    p_item_id_drop INTEGER,
    p_item_id_pick INTEGER,
    p_combat_id INTEGER
) RETURNS VOID AS $$
DECLARE
    v_strength INT;
    v_constitution INT;
    v_inventory_modifier INT;
    v_max_inventory_weight INT;
    v_current_inventory_weight INT;
    v_drop_item_weight INT;
    v_pick_item_weight INT;
BEGIN
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

    -- Get weights of both items
    SELECT weight INTO v_drop_item_weight FROM items WHERE id = p_item_id_drop;
    SELECT weight INTO v_pick_item_weight FROM items WHERE id = p_item_id_pick;

    -- Check if the item to drop is heavy enough
    IF v_drop_item_weight < v_pick_item_weight THEN
        RAISE EXCEPTION 'Item to drop is lighter than item to pick, cannot swap.';
    END IF;

    -- Drop the item from inventory
    UPDATE inventory
    SET quantity = quantity - 1
    WHERE character_id = p_character_id
      AND item_id = p_item_id_drop;

    -- Remove completely if quantity drops to 0
    DELETE FROM inventory
    WHERE character_id = p_character_id
      AND item_id = p_item_id_drop
      AND quantity <= 0;

    -- Add the dropped item to dropped_items
    INSERT INTO dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
    VALUES (p_combat_id, p_item_id_drop, 1, p_character_id, NULL, 'Dropped during swap')
    ON CONFLICT (combat_id, item_id, character_id) DO UPDATE
    SET quantity = dropped_items.quantity + 1;

    -- Recalculate current inventory weight
    SELECT COALESCE(SUM(i.weight * inv.quantity), 0)
    INTO v_current_inventory_weight
    FROM inventory inv
    JOIN items i ON inv.item_id = i.id
    WHERE inv.character_id = p_character_id;

    -- Check again if the pick now fits
    IF (v_current_inventory_weight + v_pick_item_weight) <= v_max_inventory_weight THEN
        -- Add the picked item to inventory
        INSERT INTO inventory (character_id, item_id, quantity, equipped)
        VALUES (p_character_id, p_item_id_pick, 1, false)
        ON CONFLICT (character_id, item_id) DO UPDATE
        SET quantity = inventory.quantity + 1;

        -- Remove picked item from dropped_items
        UPDATE dropped_items
        SET quantity = quantity - 1
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id_pick;

        DELETE FROM dropped_items
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id_pick
          AND quantity <= 0;

    ELSE
        -- Something went wrong, can't pick the new item even after drop
        RAISE EXCEPTION 'Swap failed: not enough space even after dropping item.';
    END IF;
END;
$$ LANGUAGE plpgsql;

-- updated
CREATE OR REPLACE FUNCTION sp_swap_items(
    p_character_id INTEGER,
    p_item_id_drop INTEGER,
    p_item_id_pick INTEGER,
    p_combat_id INTEGER
) RETURNS VOID AS $$
DECLARE
    v_current_inventory_weight NUMERIC;
    v_max_inventory_weight NUMERIC;
    v_drop_item_weight INT;
    v_pick_item_weight INT;
BEGIN
    -- Get weights of both items
    SELECT weight INTO v_drop_item_weight FROM items WHERE id = p_item_id_drop;
    SELECT weight INTO v_pick_item_weight FROM items WHERE id = p_item_id_pick;

    -- Check if the item to drop is heavy enough
    IF v_drop_item_weight < v_pick_item_weight THEN
        RAISE EXCEPTION 'Item to drop is lighter than item to pick, cannot swap.';
    END IF;

    -- Drop the item from inventory
    UPDATE inventory
    SET quantity = quantity - 1
    WHERE character_id = p_character_id
      AND item_id = p_item_id_drop;

    -- Remove completely if quantity drops to 0
    DELETE FROM inventory
    WHERE character_id = p_character_id
      AND item_id = p_item_id_drop
      AND quantity <= 0;

    -- Add the dropped item to dropped_items
    INSERT INTO dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
    VALUES (p_combat_id, p_item_id_drop, 1, p_character_id, NULL, 'Dropped during swap')
    ON CONFLICT (combat_id, item_id, character_id) DO UPDATE
    SET quantity = dropped_items.quantity + 1;

    -- Use functions to recalculate current inventory weight and capacity
    v_current_inventory_weight := f_current_inventory_weight(p_character_id);
    v_max_inventory_weight := f_max_inventory_capacity(p_character_id);

    -- Check again if the picked item now fits
    IF (v_current_inventory_weight + v_pick_item_weight) <= v_max_inventory_weight THEN
        -- Add the picked item to inventory
        INSERT INTO inventory (character_id, item_id, quantity, equipped)
        VALUES (p_character_id, p_item_id_pick, 1, FALSE)
        ON CONFLICT (character_id, item_id) DO UPDATE
        SET quantity = inventory.quantity + 1;

        -- Remove picked item from dropped_items
        UPDATE dropped_items
        SET quantity = quantity - 1
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id_pick;

        DELETE FROM dropped_items
        WHERE combat_id = p_combat_id
          AND item_id = p_item_id_pick
          AND quantity <= 0;

    ELSE
        -- Something went wrong, can't pick the new item even after drop
        RAISE EXCEPTION 'Swap failed: not enough space even after dropping item.';
    END IF;
END;
$$ LANGUAGE plpgsql;

-- sp_reset_round
-- obnoví stav boja na zaciatku nového kola obnovením akcnych bodov
-- pre vsetky zucastnene postavy a aktualizáciou cisla kola

-- ak sa skonci kolo = to znamená, ze uz nie je mozné spravit ziadnu akciu alebo
-- ostane v boji iba jeden charakter a nemá na koho zautocit
CREATE OR REPLACE FUNCTION sp_reset_round(
    p_combat_id INTEGER
) RETURNS VOID AS $$
DECLARE
    v_alive_count INT;
    v_character_id INT;
    v_max_ap INT;
    v_new_round_number INT;
    v_new_round_id INT;
BEGIN
    -- Step 1: Check if more than one alive character remains
    SELECT COUNT(*)
    INTO v_alive_count
    FROM combat_participants
    WHERE combat_id = p_combat_id
      AND status = 'alive';

    IF v_alive_count <= 1 THEN
        RAISE EXCEPTION 'Combat cannot continue: Not enough alive participants.';
    END IF;

    -- Step 2: Reset action points for all alive participants
    FOR v_character_id IN
        SELECT character_id
        FROM combat_participants
        WHERE combat_id = p_combat_id
          AND status = 'alive'
    LOOP
        -- Get max_ap for each character
        SELECT max_ap INTO v_max_ap
        FROM characters
        WHERE id = v_character_id;

        -- Update their action points
        UPDATE combat_participants
        SET current_ap = v_max_ap
        WHERE combat_id = p_combat_id
          AND character_id = v_character_id;
    END LOOP;

    -- Step 3: Increment round counter
    SELECT COALESCE(MAX(round_number), 0) + 1
    INTO v_new_round_number
    FROM combat_rounds
    WHERE combat_id = p_combat_id;

    -- Insert new round record and get its ID
    INSERT INTO combat_rounds (combat_id, round_number, started_at)
    VALUES (p_combat_id, v_new_round_number, NOW())
    RETURNING id INTO v_new_round_id;

    -- Step 4: Log round reset into combat_diary
    INSERT INTO combat_diary (
        combat_id,
        round_id,
        actor_id,
        target_id,
        action_type,
        ap_cost,
        ap_remaining_after,
        hit_roll,
        success,
        damage,
        killed,
        target_alive,
        timestamp
    ) VALUES (
        p_combat_id,
        v_new_round_id,
        NULL,
        NULL,
        'round_reset', -- system event
        0,
        NULL,
        NULL,
        NULL,
        0,
        NULL,
        NULL,
        NOW()
    );
END;
$$ LANGUAGE plpgsql;