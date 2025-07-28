set "input=%1"
set "output=%2"

ffmpeg -i "%input%" -vf "scale=640:480" -an -c:v libtheora -q:v 7 "%output%"