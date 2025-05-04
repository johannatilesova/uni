-- f_effective_spell_cost
-- funckia na vypocet efektivnej ceny kuzla na zaklade atributov postavy a konfigurácie kuzla
-- funkciu som prisposobila svojmu modelu: v_effective_cost := v_base_cost * (1 - (v_attribute_value * v_multiplier))
-- kedze cena spellu zavisla aj od atributu a atributy pridavaju spellu rozne efekty, tak musi byt do vypoctu zahrnuty aj multiplier

CREATE OR REPLACE FUNCTION f_effective_spell_cost(
    p_spell_id INTEGER,
    p_caster_id INTEGER
) RETURNS NUMERIC AS $$
DECLARE
    v_base_cost NUMERIC;
    v_effective_cost NUMERIC;
    v_attribute_name VARCHAR(30);
    v_modifier_type VARCHAR(30);
    v_attribute_value NUMERIC;
    v_multiplier NUMERIC;
BEGIN
    -- 1. Base cost
    SELECT ap_cost
    INTO v_base_cost
    FROM spells
    WHERE id = p_spell_id;

    -- 2. Attribute, modifier type, multiplier
    SELECT sae.attribute_name, sae.modifier_type, sae.multiplier
    INTO v_attribute_name, v_modifier_type, v_multiplier
    FROM spell_attribute_effects sae
    WHERE sae.spell_id = p_spell_id
      AND sae.effect_type = 'ap_cost'
    LIMIT 1;

    -- 3. If no modifier, return base cost
    IF NOT FOUND THEN
        v_effective_cost := v_base_cost;
    ELSE
        -- 4. Get caster's attribute value
        SELECT value
        INTO v_attribute_value
        FROM character_attributes
        WHERE character_id = p_caster_id
          AND attribute_name = v_attribute_name;

        -- 5. Calculate effective cost based on modifier_type
        IF v_modifier_type = 'decrease' THEN
            v_effective_cost := v_base_cost * (1 - (v_attribute_value * v_multiplier));
        ELSIF v_modifier_type = 'increase' THEN
            v_effective_cost := v_base_cost * (1 + (v_attribute_value * v_multiplier));
        ELSE
            -- In case of unexpected modifier_type, fallback
            v_effective_cost := v_base_cost;
        END IF;

        -- 6. Ensure minimum cost 1
        IF v_effective_cost < 1 THEN
            v_effective_cost := 1;
        END IF;
    END IF;

    RETURN v_effective_cost;
END;
$$ LANGUAGE plpgsql;

/* skúška funkcie
select characters.id, spe.spell_id, spe.attribute_name, cha.value, sp.ap_cost, spe.modifier_type, spe.multiplier from characters
join character_attributes cha on characters.id = cha.character_id
join spell_attribute_effects spe on cha.attribute_name = spe.attribute_name
join spells sp on sp.id = spe.spell_id
where characters.id = 1 and spe.effect_type = 'ap_cost';

SELECT f_effective_spell_cost(2, 1); -- 3,8
SELECT f_effective_spell_cost(3, 1); -- 2,64
*/