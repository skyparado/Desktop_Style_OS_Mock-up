$ErrorActionPreference = 'Stop'
Push-Location (Split-Path $PSScriptRoot -Parent)
try {
    New-Item -ItemType Directory -Force .deps | Out-Null
    $downloads = @(
        @('https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b.tar.gz', 'imgui.tar.gz', '8E1BBC76C71D74FEF2FB85DB7E7CA8EBA13D6A86623C54992B60162DB554FFDB'),
        @('https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip', 'glfw.zip', '54EFA829400F2A0537F742B2B3BDD74E437BB4F2F048E4B7D3C5557D11A611E6'),
        @('https://github.com/skeeto/w64devkit/releases/download/v2.0.0/w64devkit-x64-2.0.0.exe', 'w64devkit.exe', 'CEA23FC56A5E61457492113A8377C8AB0C42ED82303FCC454CCD1963A46F8CE1')
    )
    foreach ($download in $downloads) {
        $destination = Join-Path '.deps' $download[1]
        if (!(Test-Path $destination)) {
            & curl.exe -fL --retry 2 $download[0] -o $destination
            if ($LASTEXITCODE -ne 0) { throw "Download failed: $($download[0])" }
        }
        if ((Get-FileHash $destination -Algorithm SHA256).Hash -ne $download[2]) {
            throw "Checksum mismatch: $destination"
        }
    }
    & tar -xf .deps/imgui.tar.gz -C .deps
    if ($LASTEXITCODE -ne 0) { throw 'ImGui extraction failed.' }
    Expand-Archive .deps/glfw.zip .deps -Force
    $extract = Start-Process -FilePath (Resolve-Path .deps/w64devkit.exe) -ArgumentList '-y','-o.deps' -WindowStyle Hidden -Wait -PassThru
    if ($extract.ExitCode -ne 0) { throw 'Compiler extraction failed.' }
    Write-Host 'Dependencies ready. Run scripts/build.ps1.'
} finally { Pop-Location }
