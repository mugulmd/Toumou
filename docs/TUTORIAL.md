# Tutorial

## Importing the Lava Python bindings

There is currently no package for the Lava Python bindings, therefore to use it you need to add the installation folder to your Python path: 
```python
import sys
sys.path.append('/Lava/installation/folder')
```

Once this is done, simply import the `lava_python` module: 
```python
import lava_python as lava
print(lava.__doc__)
```

If you get an `ImportError: DLL load failed while importing lava_python`, you might need to provide the path to the OpenEXR, IMath and zlib DLLs to Python:
```python
import os
os.add_dll_directory('/path/to/OpenEXR/DLLs')
os.add_dll_directory('/path/to/Imath/DLLs')
os.add_dll_directory('/path/to/zlib/DLLs')
```

## Simple sphere

Here is how you can create and render a sphere with `lava_python`:
```python
cam = lava.make_camera(location=lava.Vec3(0, 0, 5))
sun = lava.make_sun_light(direction=lava.Vec3(1, 1, 1).normalized())

sphere = lava.make_sphere()
sphere.material.base_color = lava.Color(1, 0, 1)

scene = lava.Scene()
scene.set_camera(cam)
scene.add_light(sun)
scene.add_surface(sphere)

params = lava.RenderParams(4)
layers = lava.RenderLayers(600, 450)

def print_progress(progress):
	print(f'progress: {progress}')

lava.render(scene, params, layers, print_progress)

lava.write_EXR(layers, 'sphere.exr')
```

If everything goes well, you should get something like this:

![purple_sphere](assets/purple_sphere.jpg)

## Metaball animation

In this example, we create a metaball with two spheres and animate the positions of the spheres:
```python
cam = lava.make_camera(location=lava.Vec3(0, 0, 8))
sun = lava.make_sun_light(direction=lava.Vec3(1, 1, 1).normalized())

sphere_left = lava.make_sphere()
sphere_right = lava.make_sphere(radius=.8)
metaball = lava.make_metaball()
metaball.add_sphere(sphere_left)
metaball.add_sphere(sphere_right)
metaball.material.base_color = lava.Color(1, 0, 1)

scene = lava.Scene()
scene.set_camera(cam)
scene.add_light(sun)
scene.add_surface(metaball)

params = lava.RenderParams(4)
layers = lava.RenderLayers(640, 360)

def print_progress(progress):
	print(f'progress: {progress}')

n_frames = 10
for frame in range(n_frames):
	print(f'Frame {frame}')
	x = 2 * frame / n_frames
	sphere_left.center.x = -x
	sphere_right.center.x = x
	lava.render(scene, params, layers, print_progress)
	lava.write_EXR(layers, f'metaball/frame_{frame:04}.exr')
```

