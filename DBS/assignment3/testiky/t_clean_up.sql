DO $$
DECLARE
    _char_id INT;
    _combat_id INT;
BEGIN
    -- 1. Vymažeme závislé údaje pre všetkých testovacích hrdinov
    FOR _char_id IN
        SELECT id
        FROM characters
        WHERE name IN ('TestSwapHero', 'TestCasterHero', 'TestTankHero', 'TestMageHero', 'TestHero', 'TestWarriorHero')
    LOOP
        -- combat_diary (ako útočník alebo cieľ)
        DELETE FROM combat_diary WHERE actor_id = _char_id OR target_id = _char_id;

        -- dropped_items (položky ktoré pustili)
        DELETE FROM dropped_items WHERE character_id = _char_id;

        -- inventory
        DELETE FROM inventory WHERE character_id = _char_id;

        -- character_attributes
        DELETE FROM character_attributes WHERE character_id = _char_id;

        -- combat_participants
        DELETE FROM combat_participants WHERE character_id = _char_id;

        -- samotné characters
        DELETE FROM characters WHERE id = _char_id;

        RAISE NOTICE '✅ Vymazaná testovacia postava ID %.', _char_id;
    END LOOP;

    -- 2. Navyše vymažeme ešte inventár (pre istotu), kde zostali testovacie itemy
    DELETE FROM inventory
    WHERE item_id IN (
        SELECT id
        FROM items
        WHERE name IN ('Heavy Shield', 'Massive Hammer', 'Tiny Feather', 'Steel Armor', 'Magic Scroll')
    );

    -- 3. Vymažeme dropped_items pre tieto itemy
    DELETE FROM dropped_items
    WHERE item_id IN (
        SELECT id
        FROM items
        WHERE name IN ('Heavy Shield', 'Massive Hammer', 'Tiny Feather', 'Steel Armor', 'Magic Scroll')
    );

    -- 4. A teraz môžeme bezpečne vymazať samotné items
    DELETE FROM items
    WHERE name IN ('Heavy Shield', 'Massive Hammer', 'Tiny Feather', 'Steel Armor', 'Magic Scroll');

    -- 5. Vymažeme combaty bez účastníkov
    FOR _combat_id IN
        SELECT id
        FROM combats
        WHERE id NOT IN (SELECT DISTINCT combat_id FROM combat_participants)
    LOOP
        -- combat_rounds patria ku combate
        DELETE FROM combat_rounds WHERE combat_id = _combat_id;

        -- dropped_items patria ku combate
        DELETE FROM dropped_items WHERE combat_id = _combat_id;

        -- combat samotný
        DELETE FROM combats WHERE id = _combat_id;

        RAISE NOTICE '🧹 Vymazaný prázdny combat ID %.', _combat_id;
    END LOOP;

    -- 7. Reset všetkých sekvencií
    PERFORM setval('characters_id_seq', (SELECT COALESCE(MAX(id), 0) + 1 FROM characters), false);
    PERFORM setval('combat_diary_id_seq', (SELECT COALESCE(MAX(id), 0) + 1 FROM combat_diary), false);
    PERFORM setval('items_id_seq', (SELECT COALESCE(MAX(id), 0) + 1 FROM items), false);
    PERFORM setval('spells_id_seq', (SELECT COALESCE(MAX(id), 0) + 1 FROM spells), false);
    PERFORM setval('combats_id_seq', (SELECT COALESCE(MAX(id), 0) + 1 FROM combats), false);
    PERFORM setval('combat_rounds_id_seq', (SELECT COALESCE(MAX(id), 0) + 1 FROM combat_rounds), false);

    RAISE NOTICE '🎯 Kompletné čistenie testovacích dát bolo úspešne dokončené!';
END;
$$ LANGUAGE plpgsql;
