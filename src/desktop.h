#pragma once

// Call once between ImGui::NewFrame() and your group's application windows.
// Returns true when the user activates PWR. The host owns shutdown/cleanup.
bool DrawDesktop();

