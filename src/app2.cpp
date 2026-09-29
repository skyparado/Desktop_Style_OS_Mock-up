#include "app2.h"
#include "imgui.h"


// App 2: Simple Calculator
namespace {

//variables for numbers and result
double num1 = 0.0;
double num2 = 0.0;
double result = 0.0;

char operation = '+';

}


void DrawApp2(bool& isOpen) {
    
    if (!isOpen) return;

    //window size and position
    ImGui::SetNextWindowSize(ImVec2(440.0f, 320.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(90.0f, 120.0f), ImGuiCond_FirstUseEver);


    if (ImGui::Begin("Calculator - App 2", &isOpen)) {

        //inputs for num1 & num2
        ImGui::InputDouble("First number", &num1);
        ImGui::InputDouble("Second number", &num2);
        ImGui::Separator();


        //operations
        if (ImGui::Button("+")) {result = num1 + num2;}
        ImGui::SameLine();

        if (ImGui::Button("-")) {result = num1 - num2;}
        ImGui::SameLine();

        if (ImGui::Button("*")) {result = num1 * num2;}
        ImGui::SameLine();
        
        if (ImGui::Button("/")) {
            if (num2 != 0.0) {
                result = num1 / num2;}
        }

        ImGui::Separator();

        
        //display result and clear button
        ImGui::Text("Result: %.2f", result);
        if (ImGui::Button("Clear")) {
            num1 = 0.0;
            num2 = 0.0;
            result = 0.0;
        }
    }

    ImGui::End();
}