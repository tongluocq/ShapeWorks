# -*- coding: utf-8 -*-

"""
Most of the following steps even though wrapped in python functions are using
the underlying c++ code, for which we need to call the source paths to the
binaries. This step should be common for any use of a function in ShapeWorks.
__This requires the full ShapeWorks to be correctly built/downloaded!__'

These following commands set the temporary environment variables to point to
shapeworks binaries and set the necessary library paths
"""
import os
import platform
from RunEllipsoid import *
from RunLeftAtrium import *
from RunFemur import *

# Path pre-setup
binpath = "../build/shapeworks/src/ShapeWorks-build/bin:../../bin"
if platform.system() == "Windows":
    binpath = "C:\\Program Files\ShapeWorks\\bin"
if platform.system() == "Darwin":
    binpath = binpath + ":/Applications/ShapeWorks/bin"

parser = argparse.ArgumentParser(description='Example ShapeWorks Pipeline')
parser.add_argument("--use_case", help="Specify which use case to run, either: ellipse, left_atrium, or femur.")
parser.add_argument("--interactive", help="Run in interactive mode", action="store_true")
parser.add_argument("--start_with_prepped_data", help="Start with already prepped data", action="store_true")
parser.add_argument("--start_with_image_and_segmentation_data", help = "use images and segmentations data for preprocessing", action="store_true")
parser.add_argument("--use_single_scale", help="Single scale or multi scale optimization", action="store_true")
parser.add_argument("--tiny_test", help="Run as a short test", action="store_true")
parser.add_argument("shapeworks_path", help="Path to ShapeWorks executables (default: "+binpath+")", nargs='?', type=str, default=binpath)
args = parser.parse_args()
binpath = args.shapeworks_path

# Path final
if platform.system() == "Darwin":
    items = binpath.split(os.pathsep)
    binpath = ""
    for item in items:
        binpath = binpath + os.pathsep + item \
            + os.pathsep + item + "/ShapeWorksStudio.app/Contents/MacOS"

os.environ["PATH"] = binpath + os.pathsep + os.environ["PATH"]
print(f"PATH = {os.environ['PATH']}")

try:
    if args.use_case == "ellipse":
        Run_Ellipsoid_Pipeline(args)
        print("\nShapeworks Pipeline Complete!")
    elif args.use_case == "left_atrium":
        Run_LeftAtrium_Pipline(args)
        print("\nShapeworks Pipeline Complete!")
    elif args.use_case == "femur":
        Run_Femur_Pipline(args)
        print("\nShapeworks Pipeline Complete!")
    else:
        print("Unrecognized use case, current supported use cases are: ellipse, left_atrium, and femur.")

except KeyboardInterrupt:
    print("KeyboardInterrupt exception caught")
    sys.exit(1)
except subprocess.CalledProcessError as e:
    print("General exception caught.\n\tReturncode: "+str(e.returncode)+"\n\tOutput: "+str(e.output))