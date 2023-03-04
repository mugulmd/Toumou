from sphere import SphereShooting

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('path')
parser.add_argument('--cam_mode', type=int, default=0)
parser.add_argument('--env_light_mode', type=int, default=0)
parser.add_argument('--lights_mode', type=int, default=0)
parser.add_argument('--surfaces_mode', type=int, default=0)
parser.add_argument('--render_params_mode', type=int, default=0)

args = parser.parse_args()

base_path = args.path
cam_mode = args.cam_mode
env_light_mode = args.env_light_mode
lights_mode = args.lights_mode
surfaces_mode = args.surfaces_mode
render_params_mode = args.render_params_mode

shooting = SphereShooting()
shooting.render(base_path, cam_mode, env_light_mode, lights_mode, surfaces_mode, render_params_mode)
