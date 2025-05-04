create table class (
    id serial primary key,
    role varchar(50) not null,
    ap_modifier int not null,
    inventory_modifier int not null,
    class_armor_bonus int not null check (class_armor_bonus >= 0)
);

create table characters
(
    id serial primary key,
    name varchar(50) not null,
    class_id int references class (id),
    max_hp int not null check (max_hp > 0),
    current_hp int not null check (current_hp >= 0),
    max_ap int not null check (max_ap >= 0),
    in_combat boolean default false
);

CREATE TABLE character_attributes (
    character_id INT REFERENCES characters(id),
    attribute_name varchar(30) check (attribute_name in ('strength', 'dexterity', 'intelligence', 'constitution', 'health')),
    value INT NOT NULL CHECK (value >= 0),
    PRIMARY KEY (character_id, attribute_name)
);

create table items (
    id serial primary key,
    name varchar(50) not null,
    weight int not null check (weight >= 0),
    type varchar(30)
);

create table inventory (
    character_id int references characters(id),
    item_id int references items(id),
    quantity int not null check (quantity >= 0),
    equipped boolean default false,
    primary key (character_id, item_id)
);

create table spell_category (
    id serial primary key,
    name varchar(50) not null
);

create table spells (
    id serial primary key,
    name varchar(50) not null,
    description text,
    ap_cost int not null check (ap_cost >= 0),
    damage int not null check (damage >= -50 and damage <= 50),
    category_id int references spell_category(id)
);

create table spell_attribute_effects (
    spell_id int references spells(id),
    attribute_name varchar(30) check (attribute_name in ('strength', 'dexterity', 'intelligence', 'constitution', 'health')),
    effect_type varchar(30) check (effect_type in ('ap_cost', 'damage')),
    modifier_type varchar(30),
    multiplier float,
    primary key (spell_id, attribute_name, effect_type)
);

create table combats (
    id serial primary key,
    location varchar(100),
    started_at timestamp,
    ended_at timestamp
);

create table combat_participants (
    combat_id int references combats(id),
    character_id int references characters(id),
    current_ap int not null check (current_ap >= 0),
    status varchar(20),
    initiative_order int not null,
    primary key (combat_id, character_id)
);

create table combat_rounds (
    id serial primary key,
    combat_id int references combats(id),
    round_number int not null,
    started_at timestamp,
    ended_at timestamp
);

create table dropped_items (
    combat_id int references combats(id),
    item_id int references items(id),
    quantity int not null check (quantity >= 0),
    character_id int references characters(id),
    round_id int references combat_rounds(id),
    location varchar(100)
);

create table combat_diary (
    id serial primary key,
    combat_id int references combats(id),
    round_id int references combat_rounds(id),
    actor_id int references characters(id),
    target_id int references characters(id),
    action_type varchar(30),
    spell_id int references spells(id),
    item_id int references items(id),
    attribute_used varchar(30),
    ap_cost int not null,
    ap_remaining_after int,
    hit_roll int,
    success boolean,
    damage int,
    killed boolean,
    target_alive boolean,
    timestamp timestamp
);