# This script is used to setup the platformio environment for the MotorGo boards. This script is a temporary solution until the MotorGo boards are added to the platformio platform.

import os
import sys
import shutil
import glob

# Global variables
# Path to PIO files
PIO_PATH = "~/.platformio"
RELATIVE_PLATFORM_PATH = "platforms/espressif32/"
RELATIVE_PACKAGES_PATH = "packages/framework-arduinoespressif32"

BOARDS_TXT_PATH = "./platformio_board_defs/boards.txt"
BOARD_JSON_PATH = "./platformio_board_defs/board_json/*"
VARIANTS_PATH = "./motorgo_1.0/variants/*"


def main():
    # Get OS
    os_name = sys.platform

    pio_path = os.path.expanduser(PIO_PATH)
    print("PlatformIO path: " + pio_path)

    print("Copying...")

    packages_path = os.path.join(pio_path, RELATIVE_PACKAGES_PATH)
    platform_path = os.path.join(pio_path, RELATIVE_PLATFORM_PATH)
    variants_path = os.path.join(pio_path, RELATIVE_PACKAGES_PATH, "variants")
    boards_path = os.path.join(pio_path, RELATIVE_PLATFORM_PATH, "boards")


    # First, copy the boards.txt file to the platformio folder
    # boards.txt goes in the packges/framework-arduinoespressif32 folder
    # Write original boards.txt file to a backup file
    shutil.copy(os.path.join(packages_path, "boards.txt"), os.path.join(packages_path, "boards.txt.bak"))
    shutil.copy(BOARDS_TXT_PATH, os.path.join(packages_path, "boards.txt"))

    # Next copy the variants over, need to copy all the folders in the variants folder
    # The variants go in the packages/framework-arduinoespressif32/variants
    # Overwrite any matching existing variants
    for variant in glob.glob(VARIANTS_PATH):
        shutil.copytree(variant, os.path.join(variants_path, os.path.basename(variant)), dirs_exist_ok=True)


    # Last, copy the board json files to the platformio folder
    # The json files go in the platforms/espressif32/boards folder
    # Overwrite any matching existing json files
    for json_file in glob.glob(BOARD_JSON_PATH):
        shutil.copy(json_file, os.path.join(boards_path, os.path.basename(json_file)))


    print("Done!")



if __name__ == "__main__":
    main()
