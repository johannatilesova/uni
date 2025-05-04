-- 2. sp_rest_character
-- Umoznuje postave odpocivat mimo boja, cim obnovuje zdravie.

CREATE OR REPLACE FUNCTION sp_rest_character(
    p_character_id INTEGER
) RETURNS VOID AS $$
BEGIN
    -- First, check if the character exists and is not in combat
    IF EXISTS (
        SELECT 1
        FROM characters
        WHERE id = p_character_id AND in_combat = false
    ) THEN
        -- Reset health to maximum
        UPDATE characters
        SET current_hp = max_hp
        WHERE id = p_character_id;

        -- Then, check if the character is present in combat_participants
        IF EXISTS (
            SELECT 1
            FROM combat_participants
            WHERE character_id = p_character_id
        ) THEN
            -- Reset action points to full capacity
            UPDATE combat_participants
            SET current_ap = (
                SELECT max_ap
                FROM characters
                WHERE id = p_character_id
            )
            WHERE character_id = p_character_id
              AND current_ap < (
                  SELECT max_ap
                  FROM characters
                  WHERE id = p_character_id
              );
        END IF;
    END IF;
END;
$$ LANGUAGE plpgsql;