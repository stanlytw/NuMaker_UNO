# Compile all Arduino sketches found under ../libraries relative to this script.
#
# Expected layout:
#   <package-root>\tools\compile-libraries-relative.ps1
#   <package-root>\libraries\**\*.ino
#
# Default behavior:
# - Uses the parent directory of this script as the package root.
# - Recursively scans ../libraries for .ino files.
# - Treats each .ino parent directory as one sketch to compile.
#
# Optional behavior:
# - Use -Filter to limit compilation to sketch paths containing a keyword.
# - Use -LogFile to save the summary table to a text file.
# - Use -ShowCompileLog to print full arduino-cli output for each sketch.
param(
    [string]$Fqbn = "nuvoton:nuvoton_duo:nuvoton_m467sd",
    [string]$LibrariesPath = "",
    [string]$Filter = "",
    [string]$LogFile = "",
    [switch]$ShowCompileLog = $false
)

$scriptRoot = $PSScriptRoot
# When LibrariesPath is omitted, resolve ../libraries from the script location.
$defaultLibrariesPath = Join-Path (Split-Path -Parent $scriptRoot) "libraries"

if ([string]::IsNullOrWhiteSpace($LibrariesPath)) {
    $LibrariesPath = $defaultLibrariesPath
}

$arduinoCli = (Get-Command arduino-cli -ErrorAction SilentlyContinue).Source

if (-not $arduinoCli) {
    $candidates = @(
        "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe",
        "$env:ProgramFiles\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
    )

    $arduinoCli = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}

if (-not $arduinoCli) {
    Write-Error "arduino-cli.exe not found. Install Arduino CLI or Arduino IDE, or add arduino-cli to PATH."
    exit 1
}

if (-not (Test-Path $LibrariesPath)) {
    Write-Error "Libraries path not found: $LibrariesPath"
    exit 1
}

$librariesFullPath = (Resolve-Path $LibrariesPath).Path
$inoFiles = Get-ChildItem -Path $librariesFullPath -Recurse -Filter "*.ino" | Sort-Object FullName

if ($inoFiles.Count -eq 0) {
    Write-Warning "No .ino files found in $librariesFullPath"
    exit 1
}

# Use the .ino parent directory as the sketch root so each example/project is compiled once.
$sketchDirs = @{}
foreach ($inoFile in $inoFiles) {
    $sketchDir = $inoFile.Directory.FullName
    if ($sketchDirs.ContainsKey($sketchDir)) {
        continue
    }

    $relativePath = $sketchDir.Replace($librariesFullPath + '\', '')
    if (-not [string]::IsNullOrWhiteSpace($Filter) -and $relativePath -notlike "*$Filter*") {
        continue
    }

    $sketchDirs[$sketchDir] = [PSCustomObject]@{
        Path = $sketchDir
        RelativePath = $relativePath
        InoFile = $inoFile.Name
    }
}

if ($sketchDirs.Count -eq 0) {
    Write-Warning "No sketches matched the filter"
    exit 0
}

$results = @()
$successCount = 0
$failCount = 0
$totalCount = $sketchDirs.Count
$currentIndex = 0

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Compile sketches under ../libraries" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Script location: $scriptRoot"
Write-Host "Libraries path: $librariesFullPath"
Write-Host "FQBN: $Fqbn"
if (-not [string]::IsNullOrWhiteSpace($Filter)) {
    Write-Host "Filter keyword: $Filter" -ForegroundColor Yellow
}
Write-Host "Found $($inoFiles.Count) .ino files; $totalCount sketches after filtering`n" -ForegroundColor Yellow

foreach ($sketchInfo in $sketchDirs.Values | Sort-Object RelativePath) {
    $currentIndex++
    Write-Host "[$currentIndex/$totalCount] Compiling $($sketchInfo.RelativePath)..." -ForegroundColor Cyan -NoNewline

    $output = & $arduinoCli compile --fqbn $Fqbn $sketchInfo.Path 2>&1
    $exitCode = $LASTEXITCODE

    # Print raw arduino-cli output only when explicitly requested.
    if ($ShowCompileLog -and $output) {
        Write-Host ""
        Write-Host "----- compile log: $($sketchInfo.RelativePath) -----" -ForegroundColor DarkGray
        $output | Out-String | Write-Host
        Write-Host "----- end compile log -----" -ForegroundColor DarkGray
    }

    if ($exitCode -eq 0) {
        Write-Host " ✓ Success" -ForegroundColor Green
        $successCount++
        $sizeInfo = $output | Select-String -Pattern "Sketch uses (\d+) bytes" | ForEach-Object { $_.Matches.Groups[1].Value }
        $results += [PSCustomObject]@{
            Sketch = $sketchInfo.RelativePath
            File = $sketchInfo.InoFile
            Status = "Success"
            Size = if ($sizeInfo) { "$sizeInfo bytes" } else { "N/A" }
            Error = ""
        }
    }
    else {
        Write-Host " ✗ Failed" -ForegroundColor Red
        $failCount++
        $errorMsg = ($output | Select-String -Pattern "error:" | Select-Object -First 1).ToString()
        if ([string]::IsNullOrWhiteSpace($errorMsg)) {
            $errorMsg = "Compilation failed (exit code: $exitCode)"
        }

        $results += [PSCustomObject]@{
            Sketch = $sketchInfo.RelativePath
            File = $sketchInfo.InoFile
            Status = "Failed"
            Size = "N/A"
            Error = $errorMsg.Substring(0, [Math]::Min(120, $errorMsg.Length))
        }
    }
}

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "Compilation summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
$results | Format-Table -AutoSize -Wrap

Write-Host "Total: $totalCount sketches" -ForegroundColor White
Write-Host "Success: $successCount" -ForegroundColor Green
Write-Host "Failed: $failCount" -ForegroundColor $(if ($failCount -gt 0) { "Red" } else { "Green" })

if (-not [string]::IsNullOrWhiteSpace($LogFile)) {
    $logPath = [System.IO.Path]::GetFullPath($LogFile)
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"

    $logContent = @"
========================================
Compilation report
========================================
Time: $timestamp
Script location: $scriptRoot
Libraries path: $librariesFullPath
FQBN: $Fqbn
Total: $totalCount sketches
Success: $successCount
Failed: $failCount
========================================

Detailed results:
"@

    $logContent | Out-File -FilePath $logPath -Encoding UTF8
    $results | Format-Table -AutoSize | Out-File -FilePath $logPath -Append -Encoding UTF8
    Write-Host "`nResults saved to: $logPath" -ForegroundColor Cyan
}

if ($failCount -gt 0) {
    Write-Host "`n$failCount sketches failed to compile" -ForegroundColor Red
    exit 1
}

Write-Host "`nAll sketches compiled successfully!" -ForegroundColor Green
exit 0