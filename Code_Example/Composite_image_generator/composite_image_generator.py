import subprocess

EOF_RECORD = ":00000001FF"

BOOTLOADER_HEX = "Bootloader.hex"
APP_A_HEX      = "ApplicationA.hex"
APP_B_HEX      = "ApplicationB.hex"

OUTPUT_HEX     = "composite.hex"

OPENOCD_EXE = r"C:\IFX_EMBEDDED_SYSTEMS\softwares\openocd\bin\openocd.exe"

INTERFACE_CFG = r"C:\IFX_EMBEDDED_SYSTEMS\softwares\openocd\scripts\interface\kitprog3.cfg"

TARGET_CFG = r"C:\IFX_EMBEDDED_SYSTEMS\softwares\openocd\scripts\target\psoc4.cfg"


def read_hex_without_eof(filename):

    print(f"Reading {filename}")

    with open(filename, "r") as f:
        lines = f.readlines()

    result = []

    for line in lines:

        if line.strip() != EOF_RECORD:

            result.append(line)

    return result


def generate_composite_image():

    bootloader_lines = read_hex_without_eof(BOOTLOADER_HEX)
    app_a_lines      = read_hex_without_eof(APP_A_HEX)
    app_b_lines      = read_hex_without_eof(APP_B_HEX)

    print("\nGenerating Composite Image...\n")

    with open(OUTPUT_HEX, "w") as f:

        for line in bootloader_lines:
            f.write(line)

        for line in app_a_lines:
            f.write(line)

        for line in app_b_lines:
            f.write(line)

        f.write(EOF_RECORD + "\n")

    print(f"Composite Image Generated : {OUTPUT_HEX}")


def erase_device():

    print("\nErasing Device...\n")

    erase_cmd = [
        OPENOCD_EXE,
        "-f",
        INTERFACE_CFG,
        "-f",
        TARGET_CFG,
        "-c",
        "kitprog3 power_config on 3300; "
        "init; "
        "reset init; "
        "psoc4 mass_erase 0; "
        "shutdown; "
        "kitprog3 power_config off; "
        "exit"
    ]

    result = subprocess.run(erase_cmd)

    if result.returncode == 0:

        print("\nErase Successful\n")

    else:

        print("\nErase Failed\n")
        exit(1)


def flash_device():

    print("\nProgramming Device...\n")

    flash_cmd = [
        OPENOCD_EXE,
        "-f",
        INTERFACE_CFG,
        "-f",
        TARGET_CFG,
        "-c",
        f"program {OUTPUT_HEX} verify reset exit"
    ]

    result = subprocess.run(flash_cmd)

    if result.returncode == 0:

        print("\nProgramming Successful\n")

    else:

        print("\nProgramming Failed\n")
        exit(1)


def main():

    print("\n===================================")
    print(" Composite Image Generator ")
    print("===================================\n")

    generate_composite_image()

    print("\nComposite Image Ready")
    print(f"Output File : {OUTPUT_HEX}")

    option = input(
        "\nProceed with Device Erase ? (y/n): "
    )

    if option.lower() != "y":

        print("\nErase Cancelled\n")
        return

    erase_device()

    option = input(
        "\nProceed with Device Programming ? (y/n): "
    )

    if option.lower() != "y":

        print("\nProgramming Cancelled\n")
        return

    flash_device()

    print("\n===================================")
    print(" Process Completed Successfully ")
    print("===================================\n")


if __name__ == "__main__":
    main()