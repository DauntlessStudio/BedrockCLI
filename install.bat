@echo off
mkdir "C:\BedrockCLI"
copy %~dp0bed.exe "C:\BedrockCLI\"
powershell -Command "& {$oldpath = (Get-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name PATH).path; $newpath = \"$oldpath;C:\BedrockCLI\"; if ($oldpath -like \"*Bedrock*\") {Write-Output 'Already in PATH'} else {Set-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name PATH -Value $newPath} }"
PAUSE