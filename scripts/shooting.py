import toumou as tm

import sys


class Shooting:
	'''Shooting is an abstract class for automating the setup and rendering of multiple shots.
	'''

	@staticmethod
	def print_progress(progress):
		sys.stdout.write('\r')
		sys.stdout.write(f'{progress}%')
		if progress == 100:
			sys.stdout.write('\n')
		sys.stdout.flush()

	def gen_cam(self, mode):
		raise NotImplementedError('Shooting.gen_cam')

	def gen_env_light(self, mode):
		raise NotImplementedError('Shooting.gen_env_light')

	def gen_lights(self, mode):
		raise NotImplementedError('Shooting.gen_lights')

	def gen_surfaces(self, mode):
		raise NotImplementedError('Shooting.gen_surfaces')

	def gen_render_params(self, mode):
		raise NotImplementedError('Shooting.gen_render_params')

	def render(self, base_path, cam_mode, env_light_mode, ligths_mode, surfaces_mode, render_params_mode):
		n_shot = 0
		for cam in self.gen_cam(cam_mode):
			for env_light in self.gen_env_light(env_light_mode):
				for lights in self.gen_lights(ligths_mode):
					for surfaces in self.gen_surfaces(surfaces_mode):
						for render_params in self.gen_render_params(render_params_mode):
							print(f'Shot {n_shot}')

							scene = tm.Scene()

							scene.set_camera(cam)

							if env_light is not None:
								scene.set_env_light(env_light)

							for light in lights:
								scene.add_light(light)

							for surface in surfaces:
								scene.add_surface(surface)

							rt = tm.RayTracer(render_params['width'], render_params['height'])
							if 'pixel_sampling' in render_params:
								rt.pixel_sampling = render_params['pixel_sampling']
							if 'max_bounce' in render_params:
								rt.max_bounce = render_params['max_bounce']
							if 'rays_per_bounce' in render_params:
								rt.rays_per_bounce = render_params['rays_per_bounce']
							if 'env_sampling' in render_params:
								rt.env_sampling = render_params['env_sampling']

							rt.render(scene, Shooting.print_progress)

							filepath = f'{base_path}/{n_shot:04}.exr'
							tm.write_EXR(rt, filepath)

							n_shot += 1
