#instructions - 
# 1) modify image_dir 
#2) modify output_path
#3) open cmd prompt and verify blender --version
#3)a, if not recognized, add Blender path C:\Program Files\Blender Foundation\Blender 4.5\ to system:path (or user) environment variables
#3)b verify blender --version 
#4) navigate in cmd prompt to path of this python file
#5) type blender --background --python make_video.py and press Enter

import bpy
import os

# Paths
image_dir = r"C:\Users\snorm\source\repos\Computer graphics algos\Computer graphics algos\image file outputs"
output_path = r"C:\Users\snorm\Desktop\video.mp4"

# Reset Blender to empty scene
bpy.ops.wm.read_factory_settings(use_empty=True)

scene = bpy.context.scene
scene.sequence_editor_create()

# Collect PNG files sorted by name
files = sorted([f for f in os.listdir(image_dir) if f.lower().endswith(".png")])
if not files:
    raise RuntimeError(f"No PNG files found in {image_dir}")

# Settings
seconds_per_image = 1
fps = 30
frames_per_image = seconds_per_image * fps

scene.render.fps = fps

# Add each image as a separate strip
frame = 1
for i, f in enumerate(files):
    seq = scene.sequence_editor.sequences.new_image(
        name=f"Image_{i}",
        filepath=os.path.join(image_dir, f),
        channel=1,
        frame_start=frame
    )
    seq.frame_final_duration = frames_per_image
    frame += frames_per_image  # next strip starts after this one

# Set total animation length
scene.frame_start = 1
scene.frame_end = frame - 1

# Render output settings
scene.render.image_settings.file_format = "FFMPEG"
scene.render.ffmpeg.format = "MPEG4"
scene.render.ffmpeg.codec = "H264"
scene.render.ffmpeg.constant_rate_factor = "HIGH"
scene.render.ffmpeg.ffmpeg_preset = "GOOD"
scene.render.filepath = output_path

# Render the animation
bpy.ops.render.render(animation=True)
