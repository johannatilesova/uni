-- test na simuláciu combatu
-- v t_test_data sme si vytvorili postavy, ktore sa pridaju do combatu v tabulke combat_participants

-- tento test simuluje combat tak, ze postavy su na hracom poli a mozu postupne medzi sebou bojovat
-- - najprv jedna postava zautoci na druhu (to sa ulozi do tabulky combat diary, kde sa zapise, kto na koho utocil, kolko ap si postava minula,
-- o kolko hp postava prisla, aky item bol pouzity) - vypise sa debug hlaska o danych postavach a ich hodnotách (current_ap, current_hp)
-- takto combat pokracuje az kym jedna z postav nezomrie (ak postava zomrie, jej items ostanu na hracej ploche - to sa riesi uz v inom teste)
-- v combate moze bojovat niekolko postav naraz, combat sa ukonci vtedy, ked ostane posledna postava a uz nema na koho zautocit

DO $$
DECLARE
    _combat_id INT;
    _round_number INT := 1;
    _round_id INT;

    _actor_id INT;
    _actor_name VARCHAR;
    _actor_current_hp INT;
    _actor_current_ap INT;

    _target_id INT;
    _target_name VARCHAR;
    _target_current_ap INT;
    _target_current_hp INT;

    _spell_id INT;
    _spell_name VARCHAR;
    _spell_ap_cost INT;
    _attribute_name VARCHAR;
    _attribute_value INT;
    _effective_ap_cost NUMERIC;
    _attack_bonus INT;
    _hit_roll INT;
    _armor_class INT;
    _success BOOLEAN;
    _damage INT;
    _alive_count INT;
BEGIN
    -- ✅ 1. Nájdeme alebo vytvoríme aktívny combat
    SELECT id INTO _combat_id
    FROM combats
    WHERE ended_at IS NULL
    LIMIT 1;

    IF NOT FOUND THEN
        INSERT INTO combats (location, started_at)
        VALUES ('Combat Simulation Arena', NOW())
        RETURNING id INTO _combat_id;

        -- Pridáme viacero hrdinov
        INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
        SELECT _combat_id, id, max_ap, 'alive', ROW_NUMBER() OVER (ORDER BY id)
        FROM characters
        WHERE name IN ('TestSwapHero', 'TestAttackHero', 'TestTankHero', 'TestMageHero', 'TestWarriorHero');
    END IF;

    -- ✅ 2. Hlavný loop na kolá
    LOOP
        -- Každé nové kolo vytvoríme nový combat_rounds záznam
        INSERT INTO combat_rounds (combat_id, round_number, started_at)
        VALUES (_combat_id, _round_number, NOW())
        RETURNING id INTO _round_id;

        RAISE NOTICE '------------------------------------';
        RAISE NOTICE '🔄 Starting Round %', _round_number;

        -- ✅ 3. Slučka na akcie hráčov v rámci 1 kola
        LOOP
            -- 🛡️ Kontrola či niekto ešte môže útočiť
            IF NOT EXISTS (
                SELECT 1
                FROM combat_participants cp
                JOIN characters ch ON cp.character_id = ch.id
                JOIN (
                    SELECT s.id AS spell_id, s.ap_cost, sa.attribute_name, sa.effect_type
                    FROM spells s
                    LEFT JOIN spell_attribute_effects sa ON s.id = sa.spell_id
                    WHERE s.category_id = 1
                ) spell_info ON 1=1
                JOIN character_attributes ca ON ch.id = ca.character_id AND ca.attribute_name = spell_info.attribute_name
                WHERE cp.combat_id = _combat_id
                  AND cp.status = 'alive'
                  AND (cp.current_ap >= (spell_info.ap_cost * (1 - (ca.value/100.0))))
            ) THEN
                RAISE NOTICE '⚠️ No more actions possible in Round %, resetting round.', _round_number;
                -- Reset AP pre všetkých
                UPDATE combat_participants cp
                SET current_ap = ch.max_ap
                FROM characters ch
                WHERE cp.character_id = ch.id
                  AND cp.combat_id = _combat_id
                  AND cp.status = 'alive';

                -- Ukončíme aktuálne kolo
                UPDATE combat_rounds
                SET ended_at = NOW()
                WHERE id = _round_id;

                -- Zvýšime round counter
                _round_number := _round_number + 1;

                -- Začíname ďalšie kolo
                EXIT;
            END IF;

            -- ✅ Výber útočníka
            SELECT cp.character_id, ch.name, ch.current_hp, cp.current_ap
            INTO _actor_id, _actor_name, _actor_current_hp, _actor_current_ap
            FROM combat_participants cp
            JOIN characters ch ON cp.character_id = ch.id
            WHERE cp.combat_id = _combat_id
              AND cp.status = 'alive'
              AND cp.current_ap > 0
            ORDER BY RANDOM()
            LIMIT 1;

            -- Ak nikto nemá AP, kolo končí
            IF NOT FOUND THEN
                RAISE NOTICE '⚠️ No actor with AP left in Round %, ending round.', _round_number;
                UPDATE combat_rounds
                SET ended_at = NOW()
                WHERE id = _round_id;
                _round_number := _round_number + 1;
                EXIT;
            END IF;

            -- ✅ Výber cieľa (nie seba)
            SELECT cp.character_id, ch.name, ch.current_hp, cp.current_ap
            INTO _target_id, _target_name, _target_current_hp, _target_current_ap
            FROM combat_participants cp
            JOIN characters ch ON cp.character_id = ch.id
            WHERE cp.combat_id = _combat_id
              AND cp.status = 'alive'
              AND cp.character_id != _actor_id
            ORDER BY RANDOM()
            LIMIT 1;

            IF NOT FOUND THEN
                RAISE NOTICE '⚡ Only one character remains. Ending combat.';
                UPDATE combats SET ended_at = NOW() WHERE id = _combat_id;
                RETURN;
            END IF;

            -- Debug
            RAISE NOTICE '🎯 % (id % | HP: % | AP: %) attacks % (id % | HP: % | AP: %)',
                _actor_name, _actor_id,_actor_current_hp, _actor_current_ap, _target_name, _target_id, _target_current_hp, _target_current_ap;

            -- ✅ Vyberieme spell (attack)
            SELECT s.id, s.name, s.ap_cost, sa.attribute_name
            INTO _spell_id, _spell_name, _spell_ap_cost, _attribute_name
            FROM spells s
            JOIN spell_attribute_effects sa ON s.id = sa.spell_id
            WHERE s.category_id = 1
            ORDER BY RANDOM()
            LIMIT 1;

            SELECT value
            INTO _attribute_value
            FROM character_attributes
            WHERE character_id = _actor_id
              AND attribute_name = _attribute_name;

            _effective_ap_cost := _spell_ap_cost * (1 - (_attribute_value / 100.0));

            -- Ak nemáme dosť AP
            IF _actor_current_ap < CEIL(_effective_ap_cost) THEN
                RAISE NOTICE '⚡ Not enough AP to cast %, skipping.', _spell_name;
                CONTINUE;
            END IF;

            -- Hodíme kockou
            SELECT f_roll_d20() INTO _hit_roll;

            _attack_bonus := FLOOR(_attribute_value / 5);

            -- Výpočet armor class cieľa
            SELECT 10 + FLOOR((ca.value)/2) + cl.class_armor_bonus
            INTO _armor_class
            FROM characters t
            JOIN character_attributes ca ON t.id = ca.character_id AND ca.attribute_name = 'dexterity'
            JOIN class cl ON t.class_id = cl.id
            WHERE t.id = _target_id;

            -- Zasiahne?
            IF (_hit_roll + _attack_bonus) >= _armor_class THEN
                _success := TRUE;
                SELECT damage
                INTO _damage
                FROM spells
                WHERE id = _spell_id;

                -- Znížime HP cieľa
                UPDATE characters
                SET current_hp = GREATEST(current_hp - _damage, 0)
                WHERE id = _target_id;

                RAISE NOTICE '✅ Hit! Damage: %', _damage;
            ELSE
                _success := FALSE;
                _damage := 0;
                RAISE NOTICE '❌ Miss!';
            END IF;

            -- Znížime AP
            UPDATE combat_participants
            SET current_ap = GREATEST(current_ap - CEIL(_effective_ap_cost), 0)
            WHERE character_id = _actor_id
              AND combat_id = _combat_id;

            -- Záznam do combat_diary
            INSERT INTO combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
            VALUES (
                _combat_id,
                _round_id,
                _actor_id,
                _target_id,
                'spell',
                _spell_id,
                _attribute_name,
                CEIL(_effective_ap_cost),
                (SELECT current_ap FROM combat_participants WHERE character_id = _actor_id AND combat_id = _combat_id),
                _hit_roll,
                _success,
                _damage,
                CASE WHEN (SELECT current_hp FROM characters WHERE id = _target_id) = 0 THEN TRUE ELSE FALSE END,
                CASE WHEN (SELECT current_hp FROM characters WHERE id = _target_id) > 0 THEN TRUE ELSE FALSE END,
                NOW()
            );

            -- Ak cieľ zomrie
            IF (SELECT current_hp FROM characters WHERE id = _target_id) = 0 THEN
                UPDATE combat_participants
                SET status = 'killed'
                WHERE character_id = _target_id
                  AND combat_id = _combat_id;

                INSERT INTO dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
                SELECT _combat_id, inv.item_id, inv.quantity, _target_id, _round_id, 'Death Drop'
                FROM inventory inv
                WHERE inv.character_id = _target_id;

                DELETE FROM inventory
                WHERE character_id = _target_id;

                RAISE NOTICE '☠️ % (id %) died and dropped their loot!', _target_name, _target_id;
            END IF;

            -- ✅ Koľko hráčov ešte žije?
            SELECT COUNT(*)
            INTO _alive_count
            FROM combat_participants
            WHERE combat_id = _combat_id
              AND status = 'alive';

            IF _alive_count <= 1 THEN
                RAISE NOTICE '🏆 Combat finished. One survivor!';
                UPDATE combats
                SET ended_at = NOW()
                WHERE id = _combat_id;

                UPDATE combat_rounds
                SET ended_at = NOW()
                WHERE id = _round_id;
sx
                RETURN;
            END IF;

        END LOOP; -- Kolo
    END LOOP; -- Combat
END;
$$ LANGUAGE plpgsql;