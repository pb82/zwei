function setup()
    local x, y = zwei.pop_player_position()
    if not x then x, y = 2, 30 end

    zwei.set_enable_lights(false)
    zwei.add_light(10, 24, 4000)

    zwei.set_player_position(x, y)
    zwei.load_map("Forest.json")
    zwei.set_game_state()
end

function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
