

echo "["
for x in $(seq 0 7); do 
    echo -ne "["    
    for y in $(seq 0 7); do 
        convert "$1" -resize 8x8 -quality 100 - |
        convert "-"[1x1+"$y"+"$x"] -format "\"%[hex:u]\"" info:
        if [ ! "$y" = 7 ]; then
            echo -ne ","
        fi
    done
    echo -ne "]"
    if [ ! "$x" = 7 ]; then
        echo ","
    else
        echo
    fi
done
echo "]"


