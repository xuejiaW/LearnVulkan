function Compile-Shaders {
    param (
        [Parameter(Mandatory = $true)]
        [string]$DirectoryPath
    )

    # Check if directory exists
    if (-not (Test-Path -Path $DirectoryPath -PathType Container)) {
        Write-Error "Directory '$DirectoryPath' does not exist."
        return
    }

    # Check if glslc is available
    if (-not (Get-Command "glslc" -ErrorAction SilentlyContinue)) {
        Write-Error "glslc command not found. Please ensure the Vulkan SDK is installed and glslc is in your PATH."
        return
    }

    # Get all .vert and .frag files in the directory (including subdirectories)
    $shaderFiles = Get-ChildItem -Path $DirectoryPath -Include "*.vert", "*.frag" -Recurse

    foreach ($shaderFile in $shaderFiles) {
        $filePath = $shaderFile.FullName
        $fileName = $shaderFile.BaseName
        $fileExtension = $shaderFile.Extension.TrimStart(".")
        $fileDirectory = $shaderFile.DirectoryName

        # Define output file name with the convention (e.g., Triangle.vert -> TriangleVert.spv)
        $capitalizedExtension = (Get-Culture).TextInfo.ToTitleCase($fileExtension)
        $outputName = "$fileName$capitalizedExtension.spv"
        $outputPath = Join-Path -Path $fileDirectory -ChildPath $outputName

        # Check if file is UTF-8 without BOM and convert if necessary
        $content = [System.IO.File]::ReadAllBytes($filePath)
        $isUTF8WithBOM = $content.Length -ge 3 -and $content[0] -eq 0xEF -and $content[1] -eq 0xBB -and $content[2] -eq 0xBF
        
        if ($isUTF8WithBOM) {
            Write-Host "Converting $filePath to UTF-8 without BOM..."
            $text = [System.IO.File]::ReadAllText($filePath)
            [System.IO.File]::WriteAllText($filePath, $text, [System.Text.UTF8Encoding]::new($false))
        }

        # Compile shader
        Write-Host "Compiling $filePath to $outputPath..."
        & glslc $filePath -o $outputPath

        if ($LASTEXITCODE -eq 0) {
            Write-Host "Successfully compiled $filePath" -ForegroundColor Green
        } else {
            Write-Host "Failed to compile $filePath" -ForegroundColor Red
        }
    }

    Write-Host "Shader compilation complete." -ForegroundColor Green
}