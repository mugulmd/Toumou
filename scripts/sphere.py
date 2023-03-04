from shooting import Shooting

import toumou as tm


class SphereShooting(Shooting):
	'''A simple shooting of a red sphere with varying roughness.
	'''

	def gen_cam(self, mode):
		cam = tm.Camera(36, 90, 0.1, 100)
		cam.move_to(tm.Vec3(0, 0, 5))
		yield cam

	def gen_env_light(self, mode):
		yield None

	def gen_lights(self, mode):
		light = tm.DirectionalLight(tm.Color(1, 1, 1), 1, tm.Vec3(1, 1, 1).normalized())
		yield [light]

	def gen_surfaces(self, mode):
		for i in range(1, 11):
			sphere = tm.Sphere(tm.Vec3(0, 0, 0), 1)
			sphere.material.set_solid_color(tm.Color.RED)
			sphere.material.roughness = 0.05 * i
			yield [sphere]

	def gen_render_params(self, mode):
		if mode == 0:
			params = {
				'width': 1280,
				'height': 720,
				'max_bounce': 0
			}
			yield params
		elif mode == 1:
			# Lower resolution and quality but faster
			params = {
				'width': 640,
				'height': 360,
				'pixel_sampling': 4,
				'max_bounce': 0
			}
			yield params
