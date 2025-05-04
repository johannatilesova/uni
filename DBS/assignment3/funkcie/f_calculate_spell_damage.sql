-- funkcia na vypocet damage podla zadania2
-- EffectiveDamage = BaseDamage * (1 + (ConfiguredAttribute / 20))
CREATE OR REPLACE FUNCTION f_calculate_spell_damage(
    p_spell_id INTEGER,
    p_caster_id INTEGER
) RETURNS NUMERIC AS $$
DECLARE
    v_base_damage INT;
    v_attribute_name VARCHAR(30);
    v_attribute_value INT;
    v_effective_damage NUMERIC;
BEGIN
    -- Základný damage kúzla
    SELECT damage
    INTO v_base_damage
    FROM spells
    WHERE id = p_spell_id;

    -- Zistíme, ktorý atribút ovplyvňuje damage
    SELECT sae.attribute_name
    INTO v_attribute_name
    FROM spell_attribute_effects sae
    WHERE sae.spell_id = p_spell_id
      AND sae.effect_type = 'damage'
    LIMIT 1;

    IF NOT FOUND THEN
        -- Ak kúzlo nemá atribút na damage (divné, ale fallback)
        v_effective_damage := v_base_damage;
    ELSE
        -- Získame hodnotu atribútu kúzelníka
        SELECT value
        INTO v_attribute_value
        FROM character_attributes
        WHERE character_id = p_caster_id
          AND attribute_name = v_attribute_name;

        -- Výpočet
        v_effective_damage := v_base_damage * (1 + (v_attribute_value / 20.0));
    END IF;

    RETURN v_effective_damage;
END;
$$ LANGUAGE plpgsql;
