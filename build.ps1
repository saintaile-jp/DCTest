param(
    [ValidateSet('build', 'clean', 'run', 'disc')]
    [string]$Target = 'build'
)

$ErrorActionPreference = 'Stop'
$runner = 'C:\DreamSDK\opt\dreamsdk\dreamsdk-runner.exe'

if (-not (Test-Path -LiteralPath $runner)) {
    throw "DreamSDK Runner was not found: $runner"
}

$makeTarget = if ($Target -eq 'build') { 'all' } else { $Target }

Push-Location $PSScriptRoot
try {
    & $runner make $makeTarget
    if ($LASTEXITCODE -ne 0) {
        throw "DreamSDK build failed (exit code: $LASTEXITCODE)."
    }
}
finally {
    Pop-Location
}
