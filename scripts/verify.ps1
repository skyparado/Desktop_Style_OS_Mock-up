param([switch]$SkipBuild)
$ErrorActionPreference = 'Stop'
Push-Location (Split-Path $PSScriptRoot -Parent)
try {
    if (!$SkipBuild) { & "$PSScriptRoot/build.ps1" -Verify }
    New-Item -ItemType Directory -Force docs/ppt/screenshots | Out-Null
    & .\build\desktop-verify.exe
    $result = $LASTEXITCODE
    Add-Type -AssemblyName System.Drawing
    Get-ChildItem docs/ppt/screenshots -Filter '*.bmp' | ForEach-Object {
        $bitmap = [System.Drawing.Image]::FromFile($_.FullName)
        try { $bitmap.Save([IO.Path]::ChangeExtension($_.FullName, '.png'), [System.Drawing.Imaging.ImageFormat]::Png) }
        finally { $bitmap.Dispose() }
        Remove-Item -LiteralPath $_.FullName
    }
    Get-Content docs/ppt/test-results.txt
    if ($result -ne 0) { throw "Desktop verification failed ($result)." }
} finally { Pop-Location }
