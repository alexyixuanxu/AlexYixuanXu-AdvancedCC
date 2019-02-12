ffmpeg -i tower.mp4 -vf reverse tower_reserve.mp4
ffmpeg -i tower_reserve.mp4 -i scream2.wav -c:v copy -map 0:v:0 -map 1:a:0 tower_reserve2.mp4
ffmpeg -i tower.mp4 -qscale:v 1 intermediate1.mpg
ffmpeg -i tower_reserve2.mp4 -qscale:v 1 intermediate2.mpg
ffmpeg -i concat:"intermediate1.mpg|intermediate2.mpg" -c copy intermediate_all.mpg
ffmpeg -i intermediate_all.mpg -qscale:v 2 output_scream.mp4