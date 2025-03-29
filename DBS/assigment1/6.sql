with zaklad as (
    select games.id as game_id,
           games.season_id,
           games.season_type,
           pr.player1_id as player_id,
           players.first_name,
           players.last_name
    from games

    join play_records pr on pr.game_id = games.id
    join players on pr.player1_id = players.id

    -- where season_type = 'Regular Season' and players.first_name = 'LeBron' and players.last_name = 'James'
    where season_type = 'Regular Season' and players.first_name = '{{first_name}}' and players.last_name = '{{last_name}}'
    ),
    pocet_zapasov as (
        select season_id, player_id,
               count(distinct game_id) as odohrane_hry
        from zaklad
        group by season_id, player_id
        having count(distinct game_id) >= 50
    ),
    pocet_striel as (
        select
            games.id as game_id,
            games.season_id,
            pr.player1_id,
            pr.event_msg_type
        from games

        join play_records pr on pr.game_id = games.id
        join pocet_zapasov as z on z.player_id = pr.player1_id
                                       and z.season_id = games.season_id

        where event_msg_type in ('FIELD_GOAL_MADE', 'FIELD_GOAL_MISSED')
    ),
    hody as (
        select season_id,
            game_id,
            sum(case when event_msg_type in ('FIELD_GOAL_MADE', 'FIELD_GOAL_MISSED') then 1 else 0 end) as pokusy,
            sum(case when event_msg_type in ('FIELD_GOAL_MADE') then 1 else 0 end) as kos
        from pocet_striel

        group by season_id, game_id
    ),
    uspesnost as (
        select
            season_id,
            game_id,
            round(kos * 1.0 / nullif(pokusy, 0), 4) as shooting_percentage
        from hody
    ),
    hody_s_rozdielmi as (
        select
            season_id,
            game_id,
            shooting_percentage,
            coalesce(
                abs(shooting_percentage - lag(shooting_percentage) over (partition by season_id order by game_id)),
                0
            ) as rozdiel
        from uspesnost
    ),
    stabilita as (
    select
        season_id,
        round(sum(rozdiel) * 100.0 / count(*), 2) as stability
    from hody_s_rozdielmi
    group by season_id
)
select distinct * from stabilita s
order by stability, s.season_id;
