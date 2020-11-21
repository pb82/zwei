#include "Tweak.h"

#include <IMGUI/imgui.h>

#include "../../Rt.h"
#include "../../Debug.h"
#include "../Entity.h"
#include "../Acceleration.h"
#include "../Transform.h"

Tweak::Tweak(Entity &parent, std::shared_ptr<Entity> client)
        : Component(parent), client(client) {}

static void getWinSize(ImVec2 &s) {
    s.x = 0;
    s.y = 0;
}

static void getWinPos(ImVec2 &p) {
    p.x = 1;
    p.y = 1;
}

void Tweak::render() {
    ImVec2 size;
    getWinSize(size);

    ImVec2 position;
    getWinPos(position);

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);

    auto acceleration = client->getComponent<Acceleration>();
    auto transform = client->getComponent<Transform>();

    ImGui::Begin("Tweak", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::SliderFloat("Zoom", &RT_Camera.z, 1.0f, 10.0f);
    ImGui::Text("Pos. X: %02f", transform->p.x);
    ImGui::Text("Pos. Y: %02f", transform->p.y);
    ImGui::Checkbox("Draw bounding boxes", &Debug::drawBoundingBoxes);

    ImGui::End();
}