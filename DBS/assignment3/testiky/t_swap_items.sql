DO $$
DECLARE
    _character_id INT;
    _combat_id INT;
    _item_id_pick INT;
    _item_weight_pick INT;
    _item_id_drop INT;
    _drop_item_weight INT;
    _max_capacity NUMERIC;
    _current_weight NUMERIC;
    _final_quantity INT;
BEGIN
    -- Vyberieme TestSwapHero
    SELECT cp.character_id, cp.combat_id
    INTO _character_id, _combat_id
    FROM combat_participants cp
    JOIN characters ch ON cp.character_id = ch.id
    WHERE ch.name = 'TestSwapHero'
    LIMIT 1;

    -- Vyberieme item na bojisku
    SELECT di.item_id
    INTO _item_id_pick
    FROM dropped_items di
    WHERE di.quantity > 0
      AND di.combat_id = _combat_id
    ORDER BY RANDOM()
    LIMIT 1;

    -- Váha itemu na zodvihnutie
    SELECT weight
    INTO _item_weight_pick
    FROM items
    WHERE id = _item_id_pick;

    -- Debug vybraný item
    RAISE NOTICE '🎯 Selected item to pick: ID %, Weight %', _item_id_pick, _item_weight_pick;

    -- Zistíme kapacitu a aktuálnu váhu
    _max_capacity := COALESCE(f_max_inventory_capacity(_character_id), 0);
    _current_weight := COALESCE(f_current_inventory_weight(_character_id), 0);

    -- Debug inventár
    RAISE NOTICE '🧳 Character inventory:';
    RAISE NOTICE '    Max capacity: %', _max_capacity;
    RAISE NOTICE '    Current weight: %', _current_weight;
    RAISE NOTICE '    Remaining capacity: %', _max_capacity - _current_weight;

    -- Ak je miesto, rovno lootni
    IF (_current_weight + _item_weight_pick) <= _max_capacity THEN
        PERFORM sp_loot_item(_combat_id, _character_id, _item_id_pick);
        RAISE NOTICE '✅ Test passed: Inventory had space, item looted.';
        RETURN;
    END IF;

    -- Smyčka vyprazdňovania inventára
    LOOP
        _current_weight := COALESCE(f_current_inventory_weight(_character_id), 0);

        RAISE NOTICE '🔁 Loop: current_weight=%, max_capacity=%, item_weight=%', _current_weight, _max_capacity, _item_weight_pick;

        IF (_current_weight + _item_weight_pick) <= _max_capacity THEN
            EXIT;
        END IF;

        IF _current_weight = 0 THEN
            RAISE EXCEPTION '❌ Test failed: Inventory empty but still cannot pick up item.';
        END IF;

        -- Vyber najľahší item na vyhodenie
        SELECT inv.item_id, it.weight
        INTO _item_id_drop, _drop_item_weight
        FROM inventory inv
        JOIN items it ON inv.item_id = it.id
        WHERE inv.character_id = _character_id
          AND inv.quantity > 0
        ORDER BY it.weight ASC
        LIMIT 1;

        RAISE NOTICE '🗑️ Dropping item ID % with weight %', _item_id_drop, _drop_item_weight;

        -- Vyhoď item
        UPDATE inventory
        SET quantity = quantity - 1
        WHERE character_id = _character_id
          AND item_id = _item_id_drop;

        DELETE FROM inventory
        WHERE character_id = _character_id
          AND item_id = _item_id_drop
          AND quantity <= 0;

        -- Zápis na bojisko
        INSERT INTO dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
        VALUES (_combat_id, _item_id_drop, 1, _character_id, NULL, 'Dropped during swap')
        ON CONFLICT (combat_id, item_id, character_id) DO UPDATE
        SET quantity = dropped_items.quantity + 1;
    END LOOP;

    -- Lootneme nový item
    PERFORM sp_loot_item(_combat_id, _character_id, _item_id_pick);

    -- Kontrola či nový item je v inventári
    SELECT quantity
    INTO _final_quantity
    FROM inventory
    WHERE character_id = _character_id
      AND item_id = _item_id_pick;

    IF _final_quantity IS NULL OR _final_quantity <= 0 THEN
        RAISE EXCEPTION '❌ Test failed: New item not found in inventory after swap.';
    ELSE
        RAISE NOTICE '✅ Test passed: New item correctly looted after swap.';
    END IF;

END;
$$ LANGUAGE plpgsql;
