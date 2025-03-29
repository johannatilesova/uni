with aktivne_timy as (
    select
        th.team_id,
        th.city,
        th.nickname,
        th.city || ' ' || th.nickname as team_name,
        to_date(concat(th.year_founded, '-07-01'), 'YYYY-MM-DD') as platny_od,
        case
            when th.year_active_till = 2019 then to_date('9999-06-30', 'YYYY-MM-DD')
            else to_date(concat(th.year_active_till, '-06-30'), 'YYYY-MM-DD')
        end as platny_do
    from team_history th
),
zapasy as (
    select g.id game_id, g.home_team_id team_id, date(g.game_date) as game_date, 'home' as match_type from games g
    union
    select g.id game_id, g.away_team_id team_id, date(g.game_date) as game_date, 'away' as match_type from games g
),
filtrovane_zapasy as (
    select
        z.game_id,
        at.team_id,
        at.team_name,
        z.match_type,
        z.game_date
    from zapasy z
    join aktivne_timy at on z.team_id = at.team_id and z.game_date between at.platny_od and at.platny_do
),
statistiky as (
    select
        fz.team_id,
        fz.team_name,
        count(case when fz.match_type = 'home' then 1 end ) as number_home_matches,
        count(case when fz.match_type = 'away' then 1 end ) as number_away_matches,
        count(*)                                            as total_games
    from filtrovane_zapasy fz
    group by fz.team_id, fz.team_name
),
percentage as (
    select
        stats.team_id,
        stats.team_name,
        stats.number_away_matches,
        round(stats.number_away_matches * 100.0 / stats.total_games, 2) as percentage_away_matches,
        stats.number_home_matches,
        round(stats.number_home_matches * 100.0 / stats.total_games, 2) as percentage_home_matches,
        stats.total_games
    from statistiky stats
)
select
    per.team_id,
    per.team_name,
    per.number_away_matches,
    per.percentage_away_matches,
    per.number_home_matches,
    per.percentage_home_matches,
    per.total_games
from percentage per
order by per.team_id, per.team_name;