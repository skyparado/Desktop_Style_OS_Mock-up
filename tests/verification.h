#pragma once
struct GLFWwindow;
void VerificationBeforeFrame(GLFWwindow* window);
void VerificationWindow();
void VerificationAfterFrame(GLFWwindow* window);
int VerificationResult();
