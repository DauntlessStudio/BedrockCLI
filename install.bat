@echo off
powershell -Command "& {$oldpath = (Get-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name PATH).path; $newpath = \"$oldpath;C:\Program Files (x86)\DauntlessStudio\BedrockCLI\"; if ($oldpath -like \"*BedrockCLI*\") {Write-Output 'Already in PATH'} else {Set-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name PATH -Value $newPath} }"
PAUSE