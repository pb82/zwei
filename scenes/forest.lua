function setup()
    zwei.init()
    zwei.init_player()

    local x, y = zwei.pop_player_position()
    if not x then x, y = 2, 30 end

    zwei.set_player_position(x, y)
    zwei.load_map("Forest.json")
    zwei.set_game_state()
end

function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
