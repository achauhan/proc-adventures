<#
Automated tests for hello_noinput.pc requirements (stubbed build).

Verifies:
- AC-NI-3: Stubbed build completes successfully
- AC-NI-1: Prints "Hello World" and exits 0
- AC-NI-2: Does not prompt / does not block for input (best-effort: run with stdin redirected)

Run:
  pwsh -NoProfile -ExecutionPolicy Bypass -File .\hello_noinput_pc_tests.ps1
#>

$ErrorActionPreference = 'Stop'

function Assert-True([bool]$cond, [string]$msg) {
  if (-not $cond) { throw "ASSERT FAILED: $msg" }
}

function Assert-Contains([string]$text, [string]$needle, [string]$msg) {
  if ($null -eq $text -or $text -notmatch [regex]::Escape($needle)) {
    throw "ASSERT FAILED: $msg`nExpected to find: '$needle'`nActual: '$text'"
  }
}

$root = $PSScriptRoot
$buildScript = Join-Path $root 'build_hello_stub.bat'
$exeNoInput = Join-Path $root 'bin\hello_noinput_stub.exe'

# Traceability inputs
$reqDoc = Join-Path $root 'HELLO_NOINPUT_PC_REQUIREMENTS.md'

# Report
$buildDir = Join-Path $root 'build'
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
$reportPath = Join-Path $buildDir 'hello_noinput_test_report.md'
$traceHtmlPath = Join-Path $buildDir 'hello_noinput_traceability_report.html'
$lines = @()
$lines += "# hello_noinput.pc - Test Report"
$lines += ""
$lines += ("Date: {0}" -f (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))
$lines += ("Host: {0}" -f $env:COMPUTERNAME)
$lines += ""

# Keep structured results so we can render a visual traceability grid
$results = @()

function Add-Result([string]$id, [string]$name, [bool]$pass, [string]$details) {
  $status = if ($pass) { 'PASS' } else { 'FAIL' }
  $script:lines += ("## {0}: {1} - {2}" -f $id, $name, $status)
  if ($details) {
    $script:lines += ""
    $fence = ('`' * 3)
    $script:lines += $fence
    $script:lines += $details
    $script:lines += $fence
  }
  $script:lines += ""

  $script:results += [pscustomobject]@{
    ReqId = $id
    Name = $name
    Status = $status
    Passed = [bool]$pass
    Details = $details
  }
}

function Html-Encode([string]$s) {
  if ($null -eq $s) { return '' }
  return ($s -replace '&','&amp;') -replace '<','&lt;' -replace '>','&gt;'
}

function Write-TraceabilityHtml([string]$path, [string]$reqDocPath, [object[]]$rows) {
  $reqDocName = Split-Path -Leaf $reqDocPath

  $css = @'
:root{--bg:#0b1020;--card:#111a33;--text:#e9ecf1;--muted:#9aa4b2;--pass:#1fdd8a;--fail:#ff5d5d;--warn:#ffcc66;--line:rgba(255,255,255,.10)}
*{box-sizing:border-box}
body{margin:0;font-family:Segoe UI,Arial,sans-serif;background:linear-gradient(180deg,#0b1020 0%,#070a14 100%);color:var(--text)}
.header{padding:22px 26px;border-bottom:1px solid var(--line)}
.header h1{margin:0;font-size:20px}
.header .meta{margin-top:6px;color:var(--muted);font-size:12px}
.container{padding:18px 26px}
.card{background:var(--card);border:1px solid var(--line);border-radius:14px;overflow:hidden;box-shadow:0 10px 30px rgba(0,0,0,.35)}
.table{width:100%;border-collapse:collapse}
th,td{padding:12px 14px;border-bottom:1px solid var(--line);vertical-align:top}
th{color:#cbd5e1;text-align:left;font-size:12px;letter-spacing:.02em;text-transform:uppercase;background:rgba(255,255,255,.03)}
.badge{display:inline-block;padding:4px 10px;border-radius:999px;font-weight:700;font-size:12px}
.badge.pass{background:rgba(31,221,138,.15);color:var(--pass);border:1px solid rgba(31,221,138,.35)}
.badge.fail{background:rgba(255,93,93,.12);color:var(--fail);border:1px solid rgba(255,93,93,.35)}
pre{margin:0;white-space:pre-wrap;word-break:break-word;background:rgba(255,255,255,.03);border:1px solid var(--line);padding:10px 12px;border-radius:10px;color:#d7dde8}
.footer{margin-top:14px;color:var(--muted);font-size:12px}
a{color:#8ab4ff;text-decoration:none}
a:hover{text-decoration:underline}
'@

  $rowsHtml = @()
  foreach ($r in $rows) {
    $statusClass = if ($r.Passed) { 'pass' } else { 'fail' }
    $details = Html-Encode $r.Details
    $rowsHtml += "<tr>" +
      "<td><code>" + (Html-Encode $r.ReqId) + "</code></td>" +
      "<td>" + (Html-Encode $r.Name) + "</td>" +
      "<td><span class='badge $statusClass'>" + (Html-Encode $r.Status) + "</span></td>" +
      "<td><pre>" + $details + "</pre></td>" +
      "</tr>"
  }

  $overall = 'FAIL'
  if ($rows -and ($rows | Where-Object { -not $_.Passed } | Measure-Object).Count -eq 0) { $overall = 'PASS' }

  $html = "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>" +
    "<title>hello_noinput - Traceability Report</title><style>" + $css + "</style></head><body>" +
    "<div class='header'>" +
    "<h1>hello_noinput.pc - Visual Traceability Report</h1>" +
    "<div class='meta'>Generated: " + (Html-Encode (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')) +
    " | Host: " + (Html-Encode $env:COMPUTERNAME) +
    " | Requirements: <a href='../" + (Html-Encode $reqDocName) + "'>" + (Html-Encode $reqDocName) + "</a>" +
    " | Overall: <strong>" + (Html-Encode $overall) + "</strong></div>" +
    "</div>" +
    "<div class='container'><div class='card'>" +
    "<table class='table'><thead><tr><th>Requirement</th><th>Test / Check</th><th>Status</th><th>Evidence</th></tr></thead><tbody>" +
    ($rowsHtml -join "") +
    "</tbody></table></div>" +
    "<div class='footer'>This report is generated by <code>hello_noinput_pc_tests.ps1</code>.</div>" +
    "</div></body></html>"

  Set-Content -Path $path -Value $html -Encoding UTF8
}

$overallPass = $true

Write-Host "== Build (stubbed) ==" -ForegroundColor Cyan
Assert-True (Test-Path $buildScript) "Missing build script: $buildScript"

$buildLog = Join-Path $buildDir 'hello_noinput_build.log'
$buildExit = $null

# Builds hello_noinput_stub.exe (and possibly other stubs as well)
Push-Location $root
try {
  $cmdLine = "`"$buildScript`" 1> `"$buildLog`" 2>&1"
  & cmd.exe /c $cmdLine
  $buildExit = $LASTEXITCODE
} finally {
  Pop-Location
}

$buildOutput = Get-Content $buildLog -Raw -ErrorAction SilentlyContinue
$buildOk = (($buildExit -eq 0) -and (Test-Path $exeNoInput))
if (-not $buildOk) { $overallPass = $false }
Add-Result 'AC-NI-3' 'Stubbed build completes successfully' $buildOk ("ExitCode={0}`r`nExpectedExe={1}`r`n`r`n{2}" -f $buildExit, $exeNoInput, $buildOutput)

Assert-True (Test-Path $exeNoInput) "Expected executable not found: $exeNoInput"

Write-Host "== Run (stdin redirected to prevent prompts) ==" -ForegroundColor Cyan

$outFile = Join-Path $root 'build\hello_noinput_stdout.txt'
$errFile = Join-Path $root 'build\hello_noinput_stderr.txt'

# Provide empty stdin via a temp file to ensure the program does not wait for input.
$stdinFile = Join-Path $root 'build\hello_noinput_stdin.txt'
Set-Content -Path $stdinFile -Value "" -Encoding ASCII

$exitCode = $null
Push-Location $root
try {
  $cmdLine = "`"$exeNoInput`" < `"$stdinFile`" 1> `"$outFile`" 2> `"$errFile`""
  & cmd.exe /c $cmdLine
  $exitCode = $LASTEXITCODE
} finally {
  Pop-Location
}

$stdout = Get-Content $outFile -Raw -ErrorAction SilentlyContinue
$stderr = Get-Content $errFile -Raw -ErrorAction SilentlyContinue

$stdoutSafe = $stdout
if ($null -eq $stdoutSafe) { $stdoutSafe = "" }
$trimmed = $stdoutSafe.TrimEnd("`r", "`n")

$runOk = ($exitCode -eq 0)
if (-not $runOk) { $overallPass = $false }
Add-Result 'FR-NI-5' 'Exit code is 0' $runOk ("ExitCode={0}`r`nStderr={1}" -f $exitCode, $stderr)

$outOk = ($trimmed -eq 'Hello World')
if (-not $outOk) { $overallPass = $false }
Add-Result 'AC-NI-1' 'Prints exactly Hello World (with newline)' $outOk ("StdoutRaw:`r`n{0}" -f $stdoutSafe)

$noPromptsOk = (-not ($stdoutSafe -match '(?i)password|username|sid|database'))
if (-not $noPromptsOk) { $overallPass = $false }
Add-Result 'AC-NI-2' 'Does not prompt / does not block for input (best-effort)' $noPromptsOk ("StdoutRaw:`r`n{0}" -f $stdoutSafe)

# Always write reports
$lines += "---"
$overallText = 'FAIL'
if ($overallPass) { $overallText = 'PASS' }
$lines += ("Overall: {0}" -f $overallText)
Set-Content -Path $reportPath -Value ($lines -join "`r`n") -Encoding UTF8
Write-Host ("Report (Markdown): {0}" -f $reportPath) -ForegroundColor DarkCyan

# Visual traceability report
if (Test-Path $reqDoc) {
  Write-TraceabilityHtml -path $traceHtmlPath -reqDocPath $reqDoc -rows $results
  Write-Host ("Report (HTML): {0}" -f $traceHtmlPath) -ForegroundColor DarkCyan
} else {
  Write-Host ("WARNING: Requirements doc not found: {0}. Skipping HTML traceability report." -f $reqDoc) -ForegroundColor Yellow
}

Assert-True $overallPass "One or more checks failed. See report: $reportPath"
Write-Host "PASS" -ForegroundColor Green
