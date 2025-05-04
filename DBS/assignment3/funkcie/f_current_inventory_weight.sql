-- vypocet aktualnej hmotnosti inventaru
CREATE OR REPLACE FUNCTION f_current_inventory_weight(
    p_character_id INTEGER
) RETURNS NUMERIC AS $$
DECLARE
    v_total_weight NUMERIC;
BEGIN
    SELECT COALESCE(SUM(i.weight * inv.quantity), 0)
    INTO v_total_weight
    FROM inventory inv
    JOIN items i ON inv.item_id = i.id
    WHERE inv.character_id = p_character_id;

    RETURN v_total_weight;
END;
$$ LANGUAGE plpgsql;