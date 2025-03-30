function Update-FileContent
{
    param (
        [Parameter(Mandatory = $true)]
        [string]$FilePath,
        [Parameter(Mandatory = $true)]
        [string]$OldProjectName,
        [Parameter(Mandatory = $true)]
        [string]$NewProjectName,
        [Parameter(Mandatory = $true)]
        [string]$NewGuid
    )

    if ($newName -match '\.(vcxproj|vcxproj.filters|h|hpp|c|cpp|txt|props|user)$')
    {
        $content = Get-Content -Path $FilePath -Raw
        $content = $content -replace [regex]::Escape($OldProjectName), $NewProjectName
        $content = $content -replace '<ProjectGuid>.*?</ProjectGuid>', "<ProjectGuid>$NewGuid</ProjectGuid>"
        Set-Content -Path $FilePath -Value $content -Encoding UTF8
        Write-Host "Update project name and guid：$FilePath"
    }
}

function Copy-VisualStudioProject
{
    param (
        [Parameter(Mandatory = $true)]
        [string]$SourceProjectName,

        [Parameter(Mandatory = $true)]
        [string]$DestinationProjectName
    )

    $solutionDir = Get-Location

    Write-Host "Solution Directory: $solutionDir"

    $sourceProjectDir = Join-Path $solutionDir $SourceProjectName
    $destProjectDir = Join-Path $solutionDir $DestinationProjectName

    if (-not (Test-Path $sourceProjectDir))
    {
        Write-Error "Source project not exists：$sourceProjectDir"
        return
    }

    Copy-Item -Path $sourceProjectDir -Destination $destProjectDir -Recurse

    Write-Host "Projects copied to：$destProjectDir"

    $newGuid = [guid]::NewGuid().ToString("B").ToUpper()
    $files = Get-ChildItem -Path $destProjectDir -Recurse | Where-Object { -not $_.PSIsContainer }

    foreach ($file in $files)
    {
        $filePath = $file.FullName
        if ($file.Name -match $SourceProjectName)
        {
            $newName = $file.Name -replace [regex]::Escape($SourceProjectName), $DestinationProjectName
            $newPath = Join-Path $file.DirectoryName $newName
            Rename-Item -Path $file.FullName -NewName $newName

            Write-Host "Rename file from$( $file.Name ) to $newName"

            Update-FileContent -FilePath $newPath -OldProjectName $SourceProjectName -NewProjectName $DestinationProjectName -NewGuid $newGuid
        }
        else
        {
            Update-FileContent -FilePath $filePath -OldProjectName $SourceProjectName -NewProjectName $DestinationProjectName -NewGuid $newGuid
        }
    }

    Write-Host "Done"
}