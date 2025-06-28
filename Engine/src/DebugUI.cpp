#include "DebugUI.hpp"
#include "Defines.hpp"
#include "imgui.h"

#include <cstddef>
#include <vector>

void DebugUI::draw()
{
    constexpr float scaleMax{1.f / 120.f}; // 120fps
    constexpr float scaleMin{1.f / 30.f};  // 60fps
    constexpr size_t seconds{3};
    const size_t sampleCount{seconds * m_state.maxFps};
    static std::vector<float> frametimes(sampleCount);

    static int offset{};
    frametimes[offset] = m_state.dt.asSeconds();
    offset = ++offset % sampleCount;

    ImGui::ShowDemoWindow();

    ImGui::Begin("Debug Stats");
    ImGui::SeparatorText("Performance");
    ImGui::Text("Delta Time: %.5f", m_state.dt.asSeconds());
    ImGui::Text("Frame Rate: %.5f", 1 / m_state.dt.asSeconds());
    ImGui::PlotLines("##Frame Time",
                     frametimes.data(),
                     sampleCount,
                     offset,
                     "Frame Time",
                     scaleMin,
                     scaleMax,
                     {0.f, 80.f});

    ImGui::SeparatorText("Window");
    ImGui::Text("Window size: %u, %u",
                m_state.window.getSize().x,
                m_state.window.getSize().y);
    ImGui::Text("View size: %.2f, %.2f",
                m_state.window.getView().getSize().x,
                m_state.window.getView().getSize().y);

    ImGui::SeparatorText("Assets");
    ImGui::Text("Textures Loaded: %zu",
                m_state.assets.getTextureList().size());

    ImGui::SeparatorText("ECS");
    auto entityCount = m_state.entities.getEntityCount();
    auto componentCount = m_state.entities.getComponentCount();
    ImGui::Text("Entities Alive: %u", entityCount);
    ImGui::ProgressBar(static_cast<float>(entityCount) / MAX_ENT);
    ImGui::Text("Components Active: %u", componentCount);
    ImGui::ProgressBar(static_cast<float>(componentCount) / MAX_COMP);

    ImGui::End();
}
