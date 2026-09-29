param(
    [string]$Action = "all"
)

$MAKE = "C:\MinGW\bin\mingw32-make.exe"

if ($Action -eq "clean") {
    & $MAKE clean
} else {
    & $MAKE
}
