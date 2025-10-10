$failed = $false

& .\build.ps1 Console
if ($LASTEXITCODE -ne 0) {
	$failed = $true
}

& .\build.ps1 TestFixed
if ($LASTEXITCODE -ne 0) {
	$failed = $true
}

& .\build.ps1 TestORB
if ($LASTEXITCODE -ne 0) {
	$failed = $true
}

& .\build.ps1 TestSystem
if ($LASTEXITCODE -ne 0) {
	$failed = $true
}

if ($failed) {
	Write-Host "Failed"
	exit -1;
}
exit 0