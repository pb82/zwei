function setup()
    local x, y = zwei.pop_player_position()
    if not x then x, y = 2, 30 end

    print("entering scene")

    zwei.set_player_position(x, y)
    zwei.load_map("Forest.json")
    zwei.set_game_state()
    zwei.set_trigger(3, 19,
        function(angle)
            print("angle: ", angle)
            zwei.speech("You entered the zone!")
        end,
        function(angle)
            print("angle: ", angle)
        end,
        1.0  -- expand collider by 1 tile
    )

end

function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
