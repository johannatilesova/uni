DO $$
DECLARE
    _target_id INT;
    _caster_id INT;
    _spell_id INT;
    _spell_ap_cost INT;
    _healing_amount INT;
    _current_hp INT;
    _max_hp INT;
    _current_ap INT;
    _new_hp INT;
    _new_ap INT;
BEGIN
    -- 1. Nájdeme hráča, ktorý je zranený (current_hp < max_hp)
    SELECT id, current_hp, max_hp
    INTO _target_id, _current_hp, _max_hp
    FROM characters
    WHERE current_hp < max_hp
    ORDER BY RANDOM()
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No wounded character found.';
    END IF;

    -- 2. Nájdeme healera
    SELECT cp.character_id
    INTO _caster_id
    FROM combat_participants cp
    JOIN characters ch ON cp.character_id = ch.id
    WHERE cp.status = 'alive'
      AND ch.name = 'TestCasterHero'
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No healer (TestCasterHero) found.';
    END IF;

    -- 3. Healing spell
    SELECT id, ap_cost, damage
    INTO _spell_id, _spell_ap_cost, _healing_amount
    FROM spells
    WHERE damage < 0
      AND name = 'Healing Mist'
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No healing spell found.';
    END IF;

    -- 4. Zistíme AP healera
    SELECT current_ap
    INTO _current_ap
    FROM combat_participants
    WHERE character_id = _caster_id;

    RAISE NOTICE '🔍 Pred healingom: Target HP = %, Max HP = %, Healer AP = %', _current_hp, _max_hp, _current_ap;

    -- 5. Skontrolujeme či máme dosť AP
    IF _current_ap < _spell_ap_cost THEN
        RAISE EXCEPTION '❌ Test failed: Not enough AP to cast healing spell.';
    END IF;

    -- 6. Healneme
    PERFORM sp_cast_spell(_caster_id, _target_id, _spell_id);

    -- 7. Skontrolujeme výsledok
    SELECT current_hp
    INTO _new_hp
    FROM characters
    WHERE id = _target_id;

    SELECT current_ap
    INTO _new_ap
    FROM combat_participants
    WHERE character_id = _caster_id;

    RAISE NOTICE '🩹 Po healingu: Target HP = %, Healer AP = %', _new_hp, _new_ap;

    IF _new_hp <= _current_hp THEN
        RAISE EXCEPTION '❌ Test failed: Healing did not increase HP.';
    END IF;

    IF _new_hp > _max_hp THEN
        RAISE EXCEPTION '❌ Test failed: Healing exceeded max HP.';
    END IF;

    RAISE NOTICE '✅ Healing Test passed successfully!';
END;
$$ LANGUAGE plpgsql;
