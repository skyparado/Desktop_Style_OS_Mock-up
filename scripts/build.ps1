param([switch]$Verify)
$ErrorActionPreference = 'Stop'
$originalPath = $env:PATH
Push-Location (Split-Path $PSScriptRoot -Parent)
try {
    $compiler = Join-Path (Get-Location) '.deps/w64devkit/bin/g++.exe'
    if (!(Test-Path $compiler)) { & "$PSScriptRoot/setup.ps1" }
    $env:PATH = (Split-Path $compiler -Parent) + ';' + $env:PATH
    New-Item -ItemType Directory -Force build | Out-Null
    $imgui = '.deps/imgui-1.91.9b'
    $glfw = '.deps/glfw-3.4.bin.WIN64'
    $sources = @('src/main.cpp', 'src/desktop.cpp', 'src/taskbar.cpp', 'src/app1.cpp', 'src/app2.cpp', 'src/task_manager.cpp', "$imgui/imgui.cpp",
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
    # skip the compile if nothing in src/ changed since the last build
    $newest = (Get-ChildItem src, scripts/build.ps1 -File | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime
    if (!$Verify -and (Test-Path $output) -and (Get-Item $output).LastWriteTime -gt $newest) {
        Write-Host "$output is up to date"
        return
    }
    & $compiler @options @sources "$glfw/lib-mingw-w64/libglfw3.a" '-lopengl32' '-lgdi32' '-o' $output
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed ($LASTEXITCODE)." }
    Write-Host "Built $output"
} finally { $env:PATH = $originalPath; Pop-Location }
