-- funkcia na vypocet AC podla zadania2
-- ArmorClass = 10 + (Dexterity/2) + ClassArmorBonus
CREATE OR REPLACE FUNCTION f_calculate_armor_class(
    p_character_id INTEGER
) RETURNS NUMERIC AS $$
DECLARE
    v_dexterity INT;
    v_class_armor_bonus INT;
    v_armor_class NUMERIC;
BEGIN
    -- Získame dexterity
    SELECT value
    INTO v_dexterity
    FROM character_attributes
    WHERE character_id = p_character_id
      AND attribute_name = 'dexterity';

    -- Získame class armor bonus
    SELECT c.class_armor_bonus
    INTO v_class_armor_bonus
    FROM characters ch
    JOIN class c ON ch.class_id = c.id
    WHERE ch.id = p_character_id;

    -- Výpočet AC
    v_armor_class := 10 + (v_dexterity / 2.0) + v_class_armor_bonus;

    RETURN v_armor_class;
END;
$$ LANGUAGE plpgsql;
