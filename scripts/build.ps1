param([switch]$Verify)
$ErrorActionPreference = 'Stop'
Push-Location (Split-Path $PSScriptRoot -Parent)
try {
    $compiler = Join-Path (Get-Location) '.deps/w64devkit/bin/g++.exe'
    if (!(Test-Path $compiler)) { throw 'Run scripts/setup.ps1 first.' }
    New-Item -ItemType Directory -Force build | Out-Null
    $imgui = '.deps/imgui-1.91.9b'
    $glfw = '.deps/glfw-3.4.bin.WIN64'
    $sources = @('src/main.cpp', 'src/desktop.cpp', "$imgui/imgui.cpp",
        "$imgui/imgui_draw.cpp", "$imgui/imgui_tables.cpp", "$imgui/imgui_widgets.cpp",
        "$imgui/backends/imgui_impl_glfw.cpp", "$imgui/backends/imgui_impl_opengl3.cpp")
    $options = @('-std=c++17', '-O2', '-Wall', '-Wextra', '-static',
        '-Isrc', "-I$imgui", "-I$imgui/backends", "-I$glfw/include")
    $output = 'build/desktop.exe'
    if ($Verify) {
        $options += @('-DDESKTOP_VERIFY', '-Itests')
        $sources += 'tests/verification.cpp'
        $output = 'build/desktop-verify.exe'
    } else { $options += '-mwindows' }
    & $compiler @options @sources "$glfw/lib-mingw-w64/libglfw3.a" '-lopengl32' '-lgdi32' '-o' $output
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed ($LASTEXITCODE)." }
    Write-Host "Built $output"
} finally { Pop-Location }
