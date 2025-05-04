-- CHARACTER_ATTRIBUTES
insert into character_attributes (character_id, attribute_name, value) values (1, 'strength', 5);
insert into character_attributes (character_id, attribute_name, value) values (1, 'dexterity', 12);
insert into character_attributes (character_id, attribute_name, value) values (1, 'intelligence', 12);
insert into character_attributes (character_id, attribute_name, value) values (1, 'constitution', 5);
insert into character_attributes (character_id, attribute_name, value) values (1, 'health', 8);

insert into character_attributes (character_id, attribute_name, value) values (2, 'strength', 18);
insert into character_attributes (character_id, attribute_name, value) values (2, 'dexterity', 17);
insert into character_attributes (character_id, attribute_name, value) values (2, 'intelligence', 10);
insert into character_attributes (character_id, attribute_name, value) values (2, 'constitution', 10);
insert into character_attributes (character_id, attribute_name, value) values (2, 'health', 8);

insert into character_attributes (character_id, attribute_name, value) values (3, 'strength', 16);
insert into character_attributes (character_id, attribute_name, value) values (3, 'dexterity', 6);
insert into character_attributes (character_id, attribute_name, value) values (3, 'intelligence', 8);
insert into character_attributes (character_id, attribute_name, value) values (3, 'constitution', 18);
insert into character_attributes (character_id, attribute_name, value) values (3, 'health', 20);

insert into character_attributes (character_id, attribute_name, value) values (4, 'strength', 8);
insert into character_attributes (character_id, attribute_name, value) values (4, 'dexterity', 12);
insert into character_attributes (character_id, attribute_name, value) values (4, 'intelligence', 18);
insert into character_attributes (character_id, attribute_name, value) values (4, 'constitution', 10);
insert into character_attributes (character_id, attribute_name, value) values (4, 'health', 14);

insert into character_attributes (character_id, attribute_name, value) values (5, 'strength', 10);
insert into character_attributes (character_id, attribute_name, value) values (5, 'dexterity', 10);
insert into character_attributes (character_id, attribute_name, value) values (5, 'intelligence', 18);
insert into character_attributes (character_id, attribute_name, value) values (5, 'constitution', 12);
insert into character_attributes (character_id, attribute_name, value) values (5, 'health', 16);

-- CHARACTERS (15 rôznych)
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Thorin', 1, 30, 30, 6, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Fili', 1, 28, 28, 5, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Kili', 1, 26, 26, 6, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Aerin', 2, 20, 20, 8, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Lirael', 2, 22, 22, 9, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Sylva', 2, 18, 18, 7, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Brutus', 3, 50, 50, 4, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Grond', 3, 48, 48, 3, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Ulfric', 3, 52, 52, 3, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Selene', 4, 24, 24, 7, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Medea', 4, 25, 25, 8, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Rowena', 4, 23, 23, 7, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Galen', 5, 26, 26, 7, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Talon', 5, 25, 25, 6, false);
insert into characters (name, class_id, max_hp, current_hp, max_ap, in_combat) values ('Zephyr', 5, 27, 27, 6, false);

-- CLASS
insert into class (role, ap_modifier, inventory_modifier, class_armor_bonus) values ('trpaslik', 2, 3, 1);
insert into class (role, ap_modifier, inventory_modifier, class_armor_bonus) values ('vila', 4, 2, 2);
insert into class (role, ap_modifier, inventory_modifier, class_armor_bonus) values ('obor', 1, 1, 4);
insert into class (role, ap_modifier, inventory_modifier, class_armor_bonus) values ('carodejnica', 3, 2, 1);
insert into class (role, ap_modifier, inventory_modifier, class_armor_bonus) values ('kuzelnik', 3, 2, 1);

-- COMBAT_DIARY
-- Combat 3 - Round 1
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 1, 10, 11, 'spell', 1, null, 'intelligence', 5, 5, 17, true, 18, false, true, '2025-04-28 10:01:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 1, 11, 10, 'item', null, 3, 'strength', 3, 5, 12, false, 0, false, true, '2025-04-28 10:02:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 1, 12, 13, 'spell', 5, null, 'intelligence', 6, 4, 19, true, 22, false, true, '2025-04-28 10:03:00');

-- Combat 3 - Round 2
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 2, 13, 12, 'spell', 3, null, 'dexterity', 3, 7, 8, false, 0, false, true, '2025-04-28 10:06:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 2, 14, 10, 'item', null, 2, 'strength', 2, 8, 20, true, 15, false, true, '2025-04-28 10:07:00');

-- Combat 3 - Round 3
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 3, 10, 13, 'spell', 1, null, 'intelligence', 5, 0, 18, true, 20, true, false, '2025-04-28 10:11:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (3, 3, 12, 14, 'spell', 5, null, 'intelligence', 6, -2, 15, true, 25, false, true, '2025-04-28 10:13:00');

-- Combat 1 - Round 1
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (1, 1, 1, 2, 'spell', 1, null, 'intelligence', 5, 5, 18, true, 20, false, true, '2025-04-27 14:01:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (1, 1, 2, 1, 'item', null, 1, 'strength', 3, 7, 13, false, 0, false, true, '2025-04-27 14:02:00');

-- Combat 1 - Round 2
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (1, 2, 3, 4, 'spell', 3, null, 'dexterity', 4, 6, 16, true, 18, false, true, '2025-04-27 14:06:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (1, 2, 4, 3, 'item', null, 2, 'strength', 2, 8, 9, false, 0, false, true, '2025-04-27 14:07:00');

-- Combat 4 - Round 1
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (4, 1, 15, 9, 'spell', 2, null, 'intelligence', 6, 4, 14, true, 22, false, true, '2025-04-29 11:01:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (4, 1, 9, 15, 'item', null, 2, 'strength', 2, 8, 11, false, 0, false, true, '2025-04-29 11:02:00');

-- Combat 4 - Round 2
insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (4, 2, 8, 7, 'spell', 5, null, 'dexterity', 5, 5, 17, true, 19, false, true, '2025-04-29 11:06:00');

insert into combat_diary (combat_id, round_id, actor_id, target_id, action_type, spell_id, item_id, attribute_used, ap_cost, ap_remaining_after, hit_roll, success, damage, killed, target_alive, timestamp)
values (4, 2, 7, 8, 'spell', 1, null, 'intelligence', 4, 6, 12, false, 0, false, true, '2025-04-29 11:07:00');


-- COMBAT_PARTICIPANTS
-- Combat 1: Dark Forest
insert into combat_participants (combat_id, character_id, current_ap, status, initiative_order)
values (1, 1, 10, 'alive', 1),
       (1, 2, 8, 'alive', 2),
       (1, 3, 9, 'alive', 3),
       (1, 4, 7, 'alive', 4),
       (1, 5, 10, 'alive', 5);

-- Combat 2: Crystal Lake
insert into combat_participants (combat_id, character_id, current_ap, status, initiative_order)
values (2, 6, 12, 'alive', 1),
       (2, 7, 11, 'alive', 2),
       (2, 8, 10, 'alive', 3),
       (2, 9, 9, 'alive', 4);

-- Combat 3: Misty Swamp
insert into combat_participants (combat_id, character_id, current_ap, status, initiative_order)
values (3, 10, 9, 'alive', 1),
       (3, 11, 8, 'alive', 2),
       (3, 12, 10, 'alive', 3),
       (3, 13, 7, 'alive', 4),
       (3, 14, 11, 'alive', 5);

-- Combat 4: Ancient Ruins
insert into combat_participants (combat_id, character_id, current_ap, status, initiative_order)
values (4, 2, 9, 'alive', 1),
       (4, 4, 10, 'alive', 2),
       (4, 6, 8, 'alive', 3),
       (4, 8, 7, 'alive', 4);

-- Combat 5: Enchanted Grove
insert into combat_participants (combat_id, character_id, current_ap, status, initiative_order)
values (5, 1, 10, 'alive', 1),
       (5, 3, 9, 'alive', 2),
       (5, 5, 11, 'alive', 3),
       (5, 7, 8, 'alive', 4),
       (5, 9, 7, 'alive', 5);

-- COMBAT_ROUNDS
-- Combat 3 - Round 1
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (3, 1, '2025-04-28 10:00:00', '2025-04-28 10:05:00');

-- Combat 3 - Round 2
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (3, 2, '2025-04-28 10:05:01', '2025-04-28 10:10:00');

-- Combat 3 - Round 3
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (3, 3, '2025-04-28 10:10:01', '2025-04-28 10:15:00');

-- Combat 1 - Round 1
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (1, 1, '2025-04-27 14:00:00', '2025-04-27 14:05:00');

-- Combat 1 - Round 2
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (1, 2, '2025-04-27 14:05:30', '2025-04-27 14:10:00');

-- Combat 4 - Round 1
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (4, 1, '2025-04-29 11:00:00', '2025-04-29 11:05:00');

-- Combat 4 - Round 2
insert into combat_rounds (combat_id, round_number, started_at, ended_at)
values (4, 2, '2025-04-29 11:05:30', '2025-04-29 11:10:00');

-- COMBATS
insert into combats (location, started_at, ended_at)
values ('Dark Forest', '2025-04-26 12:00:00', '2025-04-26 12:30:00');

insert into combats (location, started_at, ended_at)
values ('Crystal Lake', '2025-04-27 15:00:00', '2025-04-27 15:40:00');

insert into combats (location, started_at, ended_at)
values ('Misty Swamp', '2025-04-28 10:00:00', '2025-04-28 10:25:00');

insert into combats (location, started_at, ended_at)
values ('Ancient Ruins', '2025-04-28 16:00:00', '2025-04-28 16:50:00');

insert into combats (location, started_at, ended_at)
values ('Enchanted Grove', '2025-04-29 11:00:00', '2025-04-29 11:45:00');

-- DROPPED ITEMS
insert into dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
values (3, 4, 1, 13, 3, 'Misty Swamp - center');

insert into dropped_items (combat_id, item_id, quantity, character_id, round_id, location)
values (3, 5, 2, 13, 3, 'Misty Swamp - center');

-- INVENTORY
insert into inventory (character_id, item_id, quantity, equipped) values (1, 10, 2, false);
insert into inventory (character_id, item_id, quantity, equipped) values (1, 5, 3, true);

insert into inventory (character_id, item_id, quantity, equipped) values (2, 4, 1, false);
insert into inventory (character_id, item_id, quantity, equipped) values (2, 7, 1, true);
insert into inventory (character_id, item_id, quantity, equipped) values (2, 6, 3, false);
insert into inventory (character_id, item_id, quantity, equipped) values (2, 2, 1, false);

insert into inventory (character_id, item_id, quantity, equipped) values (3, 3, 2, true);
insert into inventory (character_id, item_id, quantity, equipped) values (3, 1, 1, false);
insert into inventory (character_id, item_id, quantity, equipped) values (3, 9, 2, true);
insert into inventory (character_id, item_id, quantity, equipped) values (3, 5, 1, true);

insert into inventory (character_id, item_id, quantity, equipped) values (4, 5, 2, false);
insert into inventory (character_id, item_id, quantity, equipped) values (4, 1, 3, true);
insert into inventory (character_id, item_id, quantity, equipped) values (4, 8, 3, true);

insert into inventory (character_id, item_id, quantity, equipped) values (5, 8, 1, true);
insert into inventory (character_id, item_id, quantity, equipped) values (5, 3, 2, false);

-- ITEMS
insert into items (id, name, weight, type) values (1, 'Iron Sword', 5, 'weapon');
insert into items (id, name, weight, type) values (2, 'Magic Wand', 2, 'weapon');
insert into items (id, name, weight, type) values (3, 'Healing Potion', 1, 'potion');
insert into items (id, name, weight, type) values (4, 'Mana Potion', 1, 'potion');
insert into items (id, name, weight, type) values (5, 'Shield', 6, 'armor');
insert into items (id, name, weight, type) values (6, 'Smoke Bomb', 1, 'tool');
insert into items (id, name, weight, type) values (7, 'Elixir of Strength', 2, 'potion');
insert into items (id, name, weight, type) values (8, 'Bow', 4, 'weapon');
insert into items (id, name, weight, type) values (9, 'Dagger', 2, 'weapon');
insert into items (id, name, weight, type) values (10, 'Scroll of Fire', 1, 'magic');

-- SPELL_ATTRIBUTE_EFFECTS
insert into spell_attribute_effects (spell_id, attribute_name, effect_type, modifier_type, multiplier)
values (1, 'intelligence', 'damage', 'increase', 0.2);

insert into spell_attribute_effects (spell_id, attribute_name, effect_type, modifier_type, multiplier)
values (2, 'constitution', 'ap_cost', 'decrease', 0.1);

insert into spell_attribute_effects (spell_id, attribute_name, effect_type, modifier_type, multiplier)
values (3, 'dexterity', 'ap_cost', 'decrease', 0.15);

insert into spell_attribute_effects (spell_id, attribute_name, effect_type, modifier_type, multiplier)
values (4, 'intelligence', 'damage', 'increase', 0.1);

insert into spell_attribute_effects (spell_id, attribute_name, effect_type, modifier_type, multiplier)
values (5, 'intelligence', 'damage', 'increase', 0.25);

-- SPELL_CATEGORY
insert into spell_category (id, name) values (1, 'Attack');
insert into spell_category (id, name) values (2, 'Defense');
insert into spell_category (id, name) values (3, 'Utility');

-- SPELLS
insert into spells (id, name, description, ap_cost, damage, category_id)
values (1, 'Fireball', 'Throws a fireball causing damage.', 5, 20, 1);

insert into spells (id, name, description, ap_cost, damage, category_id)
values (2, 'Shield', 'Creates a protective barrier.', 4, 0, 2);

insert into spells (id, name, description, ap_cost, damage, category_id)
values (3, 'Smoke Bomb', 'Obscures vision to cancel enemy action.', 3, 0, 3);

insert into spells (id, name, description, ap_cost, damage, category_id)
values (4, 'Healing Mist', 'Heals minor wounds.', 4, -15, 2);

insert into spells (id, name, description, ap_cost, damage, category_id)
values (5, 'Lightning Strike', 'Calls lightning from the sky.', 6, 25, 1);
