-- ked sa postava rozhodne pouzit kuzlo/zautocit, najprv systém overi, ci ma postava dost AP
-- potom hodi kockou - nahodne cislo od 1 po 20 (funckia f_roll_20) ktore urci pravepodobnost ci sa kuzlo vobec podari
-- pripocita sa bonus na základe atribútov - na vypocet AC mame uz procedúru
-- vlastne zistime armor class a ak sa do nej trafime tak zasiahnuty character príde o urcity pocet hp (podla toho aky damage malo kuzlo)
-- a utocnik príde o ap (podla toho kolko stálo kuzlo)

DO $$
DECLARE
    _actor_id INT;
    _target_id INT;
    _spell_id INT;
    _spell_name VARCHAR(50);
    _roll INT;
    _attack_bonus INT;
    _armor_class_target INT;
    _attribute_for_damage VARCHAR(30);
BEGIN
    -- Vyberieme náhodných dvoch živých hráčov v combatu
    SELECT cp1.character_id, cp2.character_id
    INTO _actor_id, _target_id
    FROM combat_participants cp1
    JOIN combat_participants cp2 ON cp1.combat_id = cp2.combat_id
    WHERE cp1.character_id != cp2.character_id
      AND cp1.status = 'alive'
      AND cp2.status = 'alive'
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No two living characters in combat.';
    END IF;

    -- Vyberieme náhodné kúzlo iba z kategórie Attack (category_id = 1)
    SELECT id, name
    INTO _spell_id, _spell_name
    FROM spells
    WHERE category_id = 1
    ORDER BY random()
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No attack spells available.';
    END IF;

    -- Hodíme kockou
    _roll := f_roll_d20();

    -- Zistíme, ktorý atribút sa používa pre damage
    SELECT attribute_name
    INTO _attribute_for_damage
    FROM spell_attribute_effects
    WHERE spell_id = _spell_id
      AND effect_type = 'damage'
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION '❌ Test failed: No damage attribute found for spell %.', _spell_id;
    END IF;

    -- Teraz podľa tohto atribútu zistíme hodnotu pre útočníka
    SELECT value
    INTO _attack_bonus
    FROM character_attributes
    WHERE character_id = _actor_id
      AND attribute_name = _attribute_for_damage;

    -- Spočítame Armor Class cieľa
    _armor_class_target := f_calculate_armor_class(_target_id);

    -- Debug výpisy
    RAISE NOTICE '🧙 Použité kúzlo: "%", ovplyvnené atribútom "%"', _spell_name, _attribute_for_damage;
    RAISE NOTICE '🎯 Útočník ID: %, Cieľ ID: %', _actor_id, _target_id;
    RAISE NOTICE '🎲 Hod kockou: %, Útočný bonus: %, Armor Class cieľa: %', _roll, _attack_bonus, _armor_class_target;
    RAISE NOTICE '💥 Útočné skóre: % (roll + bonus) vs AC: %', (_roll + _attack_bonus), _armor_class_target;

    -- Výsledok
    IF (_roll + _attack_bonus) >= _armor_class_target THEN
        RAISE NOTICE '✅ Útok úspešný! (zásah)';
    ELSE
        RAISE NOTICE '❌ Útok neúspešný! (miss)';
    END IF;

END;
$$ LANGUAGE plpgsql;
