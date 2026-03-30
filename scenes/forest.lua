function setup()
    local x, y = zwei.pop_player_position()
    if not x then x, y = 2, 30 end

    zwei.set_player_position(x, y)
    zwei.load_map("Forest.json")
    zwei.set_game_state()
    zwei.speech("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.")
end

function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
