-- maximalna hmotnost inventaru
CREATE OR REPLACE FUNCTION f_max_inventory_capacity(
    p_character_id INTEGER
) RETURNS NUMERIC AS $$
DECLARE
    v_strength INT;
    v_constitution INT;
    v_inventory_modifier INT;
    v_max_capacity NUMERIC;
BEGIN
    -- Získame Strength a Constitution
    SELECT value
    INTO v_strength
    FROM character_attributes
    WHERE character_id = p_character_id AND attribute_name = 'strength';

    SELECT value
    INTO v_constitution
    FROM character_attributes
    WHERE character_id = p_character_id AND attribute_name = 'constitution';

    -- Získame Inventory Modifier z class
    SELECT c.inventory_modifier
    INTO v_inventory_modifier
    FROM characters ch
    JOIN class c ON ch.class_id = c.id
    WHERE ch.id = p_character_id;

    -- Výpočet
    v_max_capacity := (v_strength + v_constitution) * v_inventory_modifier;

    RETURN v_max_capacity;
END;
$$ LANGUAGE plpgsql;