$width = 10
$height = 10
$content = "P3`n$width $height`n255`n"

for ($y = 0; $y -lt $height; $y++) {
    for ($x = 0; $x -lt $width; $x++) {
        if ($y -eq 9) {
            $content += "0 0 0 "
        } elseif ($x -eq 5 -and $y -eq 2) {
            $content += "255 0 0 "
        } else {
            $content += "255 255 255 "
        }
    }
    $content += "`n"
}

Set-Content -Path "assets\textures\test_map.ppm" -Value $content
