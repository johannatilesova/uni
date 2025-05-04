DO $$
DECLARE
    _character_id INT;
    _combat_id INT;
    _item_id INT;
    _item_weight INT;
    _max_capacity NUMERIC;
    _current_weight NUMERIC;
BEGIN
    -- ✅ 1. Vyberieme postavu, ktorá prežila combat
    SELECT cp.character_id, cp.combat_id
    INTO _character_id, _combat_id
    FROM combat_participants cp
    WHERE cp.status = 'alive'
    ORDER BY RANDOM()
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No alive character found in combat.';
    END IF;

    -- ✅ 2. Vyberieme náhodný predmet z bojiska
    SELECT di.item_id
    INTO _item_id
    FROM dropped_items di
    WHERE di.combat_id = _combat_id
    ORDER BY RANDOM()
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No available item on battlefield.';
    END IF;

    -- ✅ 3. Zistíme váhu itemu
    SELECT weight
    INTO _item_weight
    FROM items
    WHERE id = _item_id;

    -- ✅ 4. Zistíme kapacitu a aktuálnu váhu inventára
    _max_capacity := f_max_inventory_capacity(_character_id);
    _current_weight := f_current_inventory_weight(_character_id);

    RAISE NOTICE '🔍 Character % - Current weight: %, Max capacity: %, Item weight: %', _character_id, _current_weight, _max_capacity, _item_weight;

    -- ✅ 5. Pokusíme sa lootnúť podľa toho, či je miesto
    IF (_current_weight + _item_weight) <= _max_capacity THEN
        -- Máme miesto → lootneme a očakávame success
        BEGIN
            PERFORM sp_loot_item(
                _combat_id,
                _character_id,
                _item_id
            );
            RAISE NOTICE '✅ Test passed: Item looted successfully (inventory had space).';
        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION '❌ Test failed: Expected success but got error: %', SQLERRM;
        END;
    ELSE
        -- Nemáme miesto → očakávame chybu
        BEGIN
            PERFORM sp_loot_item(
                _combat_id,
                _character_id,
                _item_id
            );
            RAISE EXCEPTION '❌ Test failed: Expected failure (full inventory) but loot succeeded.';
        EXCEPTION WHEN OTHERS THEN
            RAISE NOTICE '✅ Test passed: Loot correctly failed due to full inventory. Error: %', SQLERRM;
        END;
    END IF;
END;
$$ LANGUAGE plpgsql;