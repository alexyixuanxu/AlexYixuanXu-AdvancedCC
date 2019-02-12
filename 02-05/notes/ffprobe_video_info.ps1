#ffprobe is a utility that comes with ffmpeg. You can use it to get info on video files.
#Here we probe an input video and filter the result to show duration.
#we use the pipe | character to pass ffprobe into powershell's Select-String method
ffprobe -i INPUT_VIDEO_FILE -show_format -v quiet | Select-String -Pattern "duration=.*"

# ffmpeg -h encoder=libx264

# convert file type
# ffmpeg -i '.\Plane Crazy.mp4' output.avi

# convert video to images per second
# ffmpeg -i '.\Plane Crazy.mp4' -vf fps=1 image-%03d.png

# add all images together to make a film
# ffmpeg -i 'image-%03d.png' -c:v libx264 -pix_fmt yuv420p reconstruct.mp4

# rotate or flip video file (vertical/horizontal)
# ffmpeg -i 'reconstruct.mp4' -vf vflip -c:a copy vflipped.mp4
# ffmpeg -i 'reconstruct.mp4' -vf hflip -c:a copy hflipped.mp4

# -vf is the video filter

# overlayed video, blurring background
ffmpeg -i reconstruct.mp4 \
-lavfi "color=color=black@.5:size=720x960:d=1[dark]; \
[0:v]crop=720:960[blurbase]; \
[blurbase]boxblur=luma_radius='min(h,w)/20':luma_power=1:chroma_radius='min(cw,ch)/20':chroma_power=1[blurred]; \
[blurred][dark]overlay[darkened]; \
[darkened]scale=1920:1080[bg]; \
[0:v]scale=-1:360[fg]; \
[bg][fg]overlay=(W-w)/2:(H-h)/2" \
OUTPUT_VIDEO_PATH

#lavfi input using a mandlebrat test pattern
ffmpeg -f lavfi -i mandelbrot -t 30 -pix_fmt yuv422p mandle.mp4

#conways game of life; play around with the s and scale parameters for sizing
# 60 60 canvas
# 
ffmpeg -f lavfi -i 'life=s=60x60:death_color=#C83232:life_color=00ff00:ratio=.5:mold=10'\
  -sws_flags neighbor -vf scale=480:360,drawgrid=w=6:h=6 \
  -t 20 -pix_fmt yuv420p lifegame2.mp4

#cell automata
#Other interesting cellauto rule values: 
#9, 18, 22, 26, 30, 41, 45, 50, 54, 60, 62, 
#73, 75, 77, 82, 86, 89, 90, 97, 99, 101, 102, 
#105, 107, 109, ​110 (default), 124, 126, 129, 131, 
#133, 135, 137, 145, 146, 149, 150, 151, 153, 154, 
#161, 167, 169, 181, 182, 183, 193, 195, 210, 218, 225
#The -profileoption is specific to the prores encoder, 
#it specifies the ProRes profile quality: 0 is Proxy, 1 is LT, 2 is Normal, and 3 is HQ.
ffmpeg -f lavfi -i cellauto=rule=123 -t 30 -pix_fmt yuv422p10 -vcodec prores -profile 3 -vf scale=480:360 cellautoma.mov

ffmpeg -f lavfi -i cellauto=rule=123 -t 30 -pix_fmt yuv422p10 -vcodec prores -profile 3 -vf scale=480:360 cellautoma.mp4





ffmpeg -f lavfi -i "life=s=60x60:death_color='random':life_color='random':ratio=.5:mold=10"-sws_flags neighbor -vf scale=480:360,drawgrid=w=6:h=6 -t 20 -pix_fmt yuv420p lifegame.mp4


for 