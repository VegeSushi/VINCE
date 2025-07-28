ffmpeg -i $1 -vf "scale=640:480" -an -c:v libtheora -q:v 7 $2
