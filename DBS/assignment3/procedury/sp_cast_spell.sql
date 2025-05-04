-- 1. sp_cast_spell
-- simuluje zoslanie kuzla postavou na ciel v rámci aktívneho boja. Postup overuje akcne body,
-- vykonava hod d20 na urcenie zásahu, vypocitava poskodenie a zaznamenáva udalost.

-- krok1: skontroluje ci caster má dostatocne vela AP na spustenie kuzla/utoku
-- krok2: vypocita effective spell cost na základe character attributes
-- krok3: vypocita AP ktore caster potrebuje na útok
-- krok4: hodi kockou d20, co je ovplyvnene náhodou (čím vyššie číslo padne, tým väčšia šanca, že útok zasiahne cieľ)
-- k hodenej hodnote sa pripocita bonus (odvodený od konkrétneho atribútu postavy - definované v tabulke spells_attribute_effects)
-- krok5: porovna vysledok (hod kockou + bonus) s armor class cielovej postavy (musime vypocitat pomocou funkcie)
-- vypocet armor class: ArmorClass = 10 + (Dexterity/2) + ClassArmorBonus
-- Ak útočné skóre presiahne AC cieľa, kúzlo je úspešné. Potom sa vypočíta poškodenie, ktoré kúzlo spôsobí.
-- krok6: Každé kúzlo má definovanú základnú hodnotu damage v tabuľke spells, ktorá sa upravuje podľa nastaveného atribútu podľa vzorca:
-- damage = BaseDamage * (1 + (ConfiguredAttribute/20)) - vytvorime si funkciu
-- krok7: ak bola armor class zasiahnuta, tak sa vypocita poskodenie a updatne sa target´s attribute health
-- krok8: vsetky udaje o tejto akcii sa zaznamenaju v tabulke combat_diary

CREATE OR REPLACE FUNCTION sp_cast_spell(
    p_caster_id INTEGER,
    p_target_id INTEGER,
    p_spell_id INTEGER
) RETURNS VOID AS $$
DECLARE
    v_caster_current_ap INT;
    v_effective_ap_cost NUMERIC;
    v_roll_result INT;
    v_attack_score NUMERIC;
    v_target_ac NUMERIC;
    v_success BOOLEAN;
    v_damage NUMERIC;
    v_current_health INT;
    v_timestamp TIMESTAMP := NOW();
    v_attribute_name VARCHAR(30);
    v_attribute_bonus INT;
    v_combat_id INT;
BEGIN
    -- 1. Zistíme combat_id
    SELECT combat_id INTO v_combat_id
    FROM combat_participants
    WHERE character_id = p_caster_id
    LIMIT 1;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Caster is not participating in combat.';
    END IF;

    -- 2. Skontrolujeme current AP castera
    SELECT current_ap
    INTO v_caster_current_ap
    FROM combat_participants
    WHERE character_id = p_caster_id;

    -- 3. Vypocítame efektívne AP cost
    v_effective_ap_cost := f_effective_spell_cost(p_spell_id, p_caster_id);

    -- 4. Skontrolujeme, ci ma dosť AP
    IF v_caster_current_ap < CEIL(v_effective_ap_cost) THEN
        RAISE EXCEPTION 'Not enough AP to cast the spell.';
    END IF;

    -- 5. Zistíme base damage kúzla
    SELECT damage
    INTO v_damage
    FROM spells
    WHERE id = p_spell_id;

    -- ➡️ Healing kúzlo (damage < 0)
    IF v_damage < 0 THEN
        -- Healujeme rovno bez hodenia kockou
        UPDATE characters
        SET current_hp = LEAST(max_hp, current_hp + ABS(v_damage))
        WHERE id = p_target_id;

        -- Update AP
        UPDATE combat_participants
        SET current_ap = current_ap - CEIL(v_effective_ap_cost)
        WHERE character_id = p_caster_id;

        -- Log
        INSERT INTO combat_diary (
            combat_id, round_id, actor_id, target_id, action_type,
            spell_id, item_id, attribute_used, ap_cost,
            ap_remaining_after, hit_roll, success, damage,
            killed, target_alive, timestamp
        ) VALUES (
            v_combat_id, NULL, p_caster_id, p_target_id, 'spell',
            p_spell_id, NULL, NULL, CEIL(v_effective_ap_cost),
            (SELECT current_ap FROM combat_participants WHERE character_id = p_caster_id),
            NULL, TRUE, v_damage, FALSE, TRUE, v_timestamp
        );

        RETURN;
    END IF;

    -- ➡️ Útočné kúzlo (damage >= 0)

    -- 6. Hodíme kockou
    v_roll_result := f_roll_d20();

    -- 7. Získame atribút pre bonus
    SELECT sae.attribute_name
    INTO v_attribute_name
    FROM spell_attribute_effects sae
    WHERE sae.spell_id = p_spell_id
      AND sae.effect_type = 'ap_cost'
    LIMIT 1;

    IF FOUND THEN
        SELECT value
        INTO v_attribute_bonus
        FROM character_attributes
        WHERE character_id = p_caster_id
          AND attribute_name = v_attribute_name;
    ELSE
        v_attribute_bonus := 0;
    END IF;

    -- 8. Spočítame útocné skóre
    v_attack_score := v_roll_result + (v_attribute_bonus / 2.0);

    -- 9. Ziskáme AC cieľa
    v_target_ac := f_calculate_armor_class(p_target_id);

    -- 10. Overíme zásah
    IF v_attack_score >= v_target_ac THEN
        v_success := TRUE;

        -- Aktualizujeme HP cieľa
        UPDATE characters
        SET current_hp = GREATEST(current_hp - v_damage, 0)
        WHERE id = p_target_id;

    ELSE
        v_success := FALSE;
        v_damage := 0; -- Miss -> žiadne poškodenie
    END IF;

    -- 11. Update AP útočníka
    UPDATE combat_participants
    SET current_ap = current_ap - CEIL(v_effective_ap_cost)
    WHERE character_id = p_caster_id;

    -- 12. Zapiseme do combat_diary
    INSERT INTO combat_diary (
        combat_id, round_id, actor_id, target_id, action_type,
        spell_id, item_id, attribute_used, ap_cost,
        ap_remaining_after, hit_roll, success, damage,
        killed, target_alive, timestamp
    ) VALUES (
        v_combat_id, NULL, p_caster_id, p_target_id, 'spell',
        p_spell_id, NULL, v_attribute_name, CEIL(v_effective_ap_cost),
        (SELECT current_ap FROM combat_participants WHERE character_id = p_caster_id),
        v_roll_result, v_success, v_damage,
        CASE WHEN (SELECT current_hp FROM characters WHERE id = p_target_id) = 0 THEN TRUE ELSE FALSE END,
        CASE WHEN (SELECT current_hp FROM characters WHERE id = p_target_id) > 0 THEN TRUE ELSE FALSE END,
        v_timestamp
    );

END;
$$ LANGUAGE plpgsql;
