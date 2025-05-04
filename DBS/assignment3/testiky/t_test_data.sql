DO $$
DECLARE
    _combat_id INT;

    _swap_hero_id INT;
    _caster_hero_id INT;
    _tank_hero_id INT;
    _mage_hero_id INT;
    _warrior_hero_id INT;

    _item_id_1 INT;
    _item_id_2 INT;
    _item_id_3 INT;
    _item_id_4 INT;
    _item_id_5 INT;
    _spell_id_fireball INT;
BEGIN
    -- 1. Získaj alebo vytvor nový combat
    SELECT id INTO _combat_id FROM combats WHERE ended_at IS NULL LIMIT 1;
    IF _combat_id IS NULL THEN
        INSERT INTO combats (location, started_at)
        VALUES ('Test Arena', NOW())
        RETURNING id INTO _combat_id;
    END IF;

    -- 2. TestSwapHero
    INSERT INTO characters (name, class_id, max_hp, current_hp, max_ap, in_combat)
    VALUES ('TestSwapHero', 1, 30, 30, 6, TRUE)
    RETURNING id INTO _swap_hero_id;

    INSERT INTO character_attributes (character_id, attribute_name, value)
    VALUES
        (_swap_hero_id, 'strength', 10),
        (_swap_hero_id, 'dexterity', 10),
        (_swap_hero_id, 'intelligence', 10),
        (_swap_hero_id, 'constitution', 10),
        (_swap_hero_id, 'health', 30);

    INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
    VALUES (_combat_id, _swap_hero_id, 10, 'alive', 1);

    RAISE NOTICE '✅ % (ID: % | STR: % | DEX: % | INT: % | CON: % | HP: %)',
    'TestSwapHero', _swap_hero_id, 10, 10, 10, 10, 30;

    -- 3. TestCasterHero
    INSERT INTO characters (name, class_id, max_hp, current_hp, max_ap, in_combat)
    VALUES ('TestCasterHero', 2, 25, 25, 8, TRUE)
    RETURNING id INTO _caster_hero_id;

    INSERT INTO character_attributes (character_id, attribute_name, value)
    VALUES
        (_caster_hero_id, 'strength', 6),
        (_caster_hero_id, 'dexterity', 8),
        (_caster_hero_id, 'intelligence', 18),
        (_caster_hero_id, 'constitution', 8),
        (_caster_hero_id, 'health', 25);

    INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
    VALUES (_combat_id, _caster_hero_id, 8, 'alive', 2);

    RAISE NOTICE '✅ % (ID: % | STR: % | DEX: % | INT: % | CON: % | HP: %)',
    'TestCasterHero', _caster_hero_id, 6, 8, 18, 8, 25;

    -- 4. TestTankHero
    INSERT INTO characters (name, class_id, max_hp, current_hp, max_ap, in_combat)
    VALUES ('TestTankHero', 3, 50, 30, 5, TRUE)
    RETURNING id INTO _tank_hero_id;

    INSERT INTO character_attributes (character_id, attribute_name, value)
    VALUES
        (_tank_hero_id, 'strength', 14),
        (_tank_hero_id, 'dexterity', 6),
        (_tank_hero_id, 'intelligence', 5),
        (_tank_hero_id, 'constitution', 16),
        (_tank_hero_id, 'health', 50);

    INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
    VALUES (_combat_id, _tank_hero_id, 5, 'alive', 3);

    RAISE NOTICE '✅ % (ID: % | STR: % | DEX: % | INT: % | CON: % | HP: %)',
    'TestTankHero', _tank_hero_id, 14, 6, 5, 16, 50;

    -- 5. TestMageHero
    INSERT INTO characters (name, class_id, max_hp, current_hp, max_ap, in_combat)
    VALUES ('TestMageHero', 4, 20, 20, 8, TRUE)
    RETURNING id INTO _mage_hero_id;

    INSERT INTO character_attributes (character_id, attribute_name, value)
    VALUES
        (_mage_hero_id, 'strength', 10),
        (_mage_hero_id, 'dexterity', 20),
        (_mage_hero_id, 'intelligence', 15),
        (_mage_hero_id, 'constitution', 12),
        (_mage_hero_id, 'health', 30);

    INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
    VALUES (_combat_id, _mage_hero_id, 8, 'alive', 4);

    RAISE NOTICE '✅ % (ID: % | STR: % | DEX: % | INT: % | CON: % | HP: %)',
    'TestMageHero', _mage_hero_id, 10, 20, 15, 12, 30;

    -- 6. TestWarriorHero
    INSERT INTO characters (name, class_id, max_hp, current_hp, max_ap, in_combat)
    VALUES ('TestWarriorHero', 5, 35, 35, 6, TRUE)
    RETURNING id INTO _warrior_hero_id;

    INSERT INTO character_attributes (character_id, attribute_name, value)
    VALUES
        (_warrior_hero_id, 'strength', 20),
        (_warrior_hero_id, 'dexterity', 10),
        (_warrior_hero_id, 'intelligence', 5),
        (_warrior_hero_id, 'constitution', 10),
        (_warrior_hero_id, 'health', 35);

    INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
    VALUES (_combat_id, _warrior_hero_id, 6, 'alive', 5);

    RAISE NOTICE '✅ % (ID: % | STR: % | DEX: % | INT: % | CON: % | HP: %)',
    'TestWarriorHero', _warrior_hero_id, 20, 10, 5, 10, 35;

    -- 7. Vytvorenie itemov
    INSERT INTO items (name, weight, type) VALUES ('Heavy Shield', 12, 'armor') RETURNING id INTO _item_id_1;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_swap_hero_id, _item_id_1, 1, FALSE);

    INSERT INTO items (name, weight, type) VALUES ('Massive Hammer', 15, 'weapon') RETURNING id INTO _item_id_2;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_swap_hero_id, _item_id_2, 1, FALSE);

    INSERT INTO items (name, weight, type) VALUES ('Magic Scroll', 3, 'magic') RETURNING id INTO _item_id_3;
    INSERT INTO dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
    VALUES (_combat_id, _item_id_3, 1, NULL, NULL, 'Center Area');

    INSERT INTO items (name, weight, type) VALUES ('Tiny Feather', 2, 'trinket') RETURNING id INTO _item_id_4;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_tank_hero_id, _item_id_4, 1, FALSE);

    INSERT INTO items (name, weight, type) VALUES ('Steel Armor', 20, 'armor') RETURNING id INTO _item_id_5;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_tank_hero_id, _item_id_5, 1, TRUE);

    -- Items for TestCasterHero
    INSERT INTO items (name, weight, type) VALUES ('Magic Wand', 5, 'magic') RETURNING id INTO _item_id_1;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_caster_hero_id, _item_id_1, 1, TRUE);

    INSERT INTO items (name, weight, type) VALUES ('Enchanted Robe', 8, 'armor') RETURNING id INTO _item_id_2;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_caster_hero_id, _item_id_2, 1, FALSE);

    -- Items for TestTankHero
    INSERT INTO items (name, weight, type) VALUES ('Tower Shield', 18, 'armor') RETURNING id INTO _item_id_3;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_tank_hero_id, _item_id_3, 1, TRUE);

    INSERT INTO items (name, weight, type) VALUES ('Heavy Mace', 16, 'weapon') RETURNING id INTO _item_id_4;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_tank_hero_id, _item_id_4, 1, FALSE);

    -- Items for TestMageHero
    INSERT INTO items (name, weight, type) VALUES ('Spellbook', 6, 'magic') RETURNING id INTO _item_id_5;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_mage_hero_id, _item_id_5, 1, FALSE);

    INSERT INTO items (name, weight, type) VALUES ('Mystic Cloak', 5, 'armor') RETURNING id INTO _item_id_1;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_mage_hero_id, _item_id_1, 1, TRUE);

    -- Items for TestWarriorHero
    INSERT INTO items (name, weight, type) VALUES ('Great Sword', 14, 'weapon') RETURNING id INTO _item_id_2;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_warrior_hero_id, _item_id_2, 1, TRUE);

    INSERT INTO items (name, weight, type) VALUES ('Battle Armor', 18, 'armor') RETURNING id INTO _item_id_3;
    INSERT INTO inventory (character_id, item_id, quantity, equipped) VALUES (_warrior_hero_id, _item_id_3, 1, FALSE);

    RAISE NOTICE '🔥 Heroes and Items created successfully!';
END;
$$ LANGUAGE plpgsql;

-- Reset sekvencií
-- SELECT setval('characters_id_seq', (SELECT MAX(id) FROM characters));
-- SELECT setval('items_id_seq', (SELECT MAX(id) FROM items));
-- SELECT setval('spells_id_seq', (SELECT MAX(id) FROM spells));
