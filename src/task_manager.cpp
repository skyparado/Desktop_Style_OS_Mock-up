#include "task_manager.h"
#include "imgui.h"

void DrawTaskManager(bool& isOpen) {
    if (!isOpen) return;

    // set initial window size and position
    ImGui::SetNextWindowSize(ImVec2(480.0f, 300.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(150.0f, 150.0f), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Task Manager", &isOpen)) {
        
        // create the dummy processes table with 3 columns: Process Name, CPU Usage, Memory Usage
        if (ImGui::BeginTable("Processes", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            // setup columns and labels
            ImGui::TableSetupColumn("Process Name");
            ImGui::TableSetupColumn("CPU Usage");
            ImGui::TableSetupColumn("Memory Usage");
            ImGui::TableHeadersRow();

            // defined dummy/sample values as required
            const char* names[] = { "CSOPESY Desktop", "System Idle Process", "NotesApp.exe", "Calculator.exe", "Background Service" };
            const char* cpu[] = { "14.5%", "82.0%", "1.2%", "0.8%", "1.5%" };
            const char* mem[] = { "124.0 MB", "67.0 KB", "12.5 MB", "8.2 MB", "45.1 MB" };

            // populate the table with the defined dummy values
            for (int row = 0; row < 5; row++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%s", names[row]);
                ImGui::TableNextColumn(); ImGui::Text("%s", cpu[row]);
                ImGui::TableNextColumn(); ImGui::Text("%s", mem[row]);
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}