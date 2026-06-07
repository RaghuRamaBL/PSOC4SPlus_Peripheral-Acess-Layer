# Composite Image Generator

## Overview

This utility combines multiple Intel HEX files into a single composite HEX image and optionally programs the target device using OpenOCD.

The tool was created as part of a dual-application bootloader project where the firmware consisted of:

* Bootloader.hex
* ApplicationA.hex
* ApplicationB.hex

Instead of programming three separate images, a single composite image is generated and programmed in one step.

---

## Why Composite Images?

Consider the following Flash layout:

| Image         | Start Address |
| ------------- | ------------- |
| Bootloader    | 0x00000000    |
| Application A | 0x00008000    |
| Application B | 0x00014000    |

Each application is linked to a different Flash address and generates its own HEX file.

Programming each image separately works, but it requires multiple programming operations.

A composite image combines all firmware images into a single Intel HEX file, allowing the complete system to be programmed with a single flash operation.

---

## How It Works

The utility performs the following operations:

### Step 1: Read Input HEX Files

Input:

```text
Bootloader.hex
ApplicationA.hex
ApplicationB.hex
```

---

### Step 2: Remove Intermediate EOF Records

Each Intel HEX file ends with:

```text
:00000001FF
```

When combining multiple HEX files:

```text
Bootloader.hex
EOF

ApplicationA.hex
EOF

ApplicationB.hex
EOF
```

the intermediate EOF records must be removed.

Result:

```text
Bootloader.hex

ApplicationA.hex

ApplicationB.hex

EOF
```

Only a single EOF record is retained at the end of the composite image.

---

### Step 3: Generate Composite Image

Output:

```text
composite.hex
```

The generated file contains:

```text
Bootloader Records
+
Application A Records
+
Application B Records
+
Single EOF Record
```

---

### Step 4: Device Erase (Optional)

The utility prompts the user:

```text
Proceed with Device Erase ? (y/n)
```

If selected, OpenOCD performs a mass erase operation before programming.

---

### Step 5: Device Programming

The utility prompts the user:

```text
Proceed with Device Programming ? (y/n)
```

If selected, OpenOCD:

* Programs the composite image
* Verifies programmed contents
* Resets the device

---

## Input

Expected files:

```text
Bootloader.hex
ApplicationA.hex
ApplicationB.hex
```

Example Directory Structure:

```text
Composite_Image_Generator/

├── composite_image_generator.py
├── Bootloader.hex
├── ApplicationA.hex
├── ApplicationB.hex
```

---

## Output

Generated file:

```text
composite.hex
```

Example:

```text
Composite_Image_Generator/

├── composite_image_generator.py
├── Bootloader.hex
├── ApplicationA.hex
├── ApplicationB.hex
└── composite.hex
```

---

## Execution Flow

```text
Read Bootloader.hex
        ↓
Read ApplicationA.hex
        ↓
Read ApplicationB.hex
        ↓
Remove Intermediate EOF Records
        ↓
Generate composite.hex
        ↓
Ask for Device Erase
        ↓
Mass Erase Device
        ↓
Ask for Device Programming
        ↓
Program composite.hex
        ↓
Verify
        ↓
Reset Device
```

---

## Example Execution

```text
===================================
 Composite Image Generator
===================================

Reading Bootloader.hex
Reading ApplicationA.hex
Reading ApplicationB.hex

Generating Composite Image...

Composite Image Generated : composite.hex

Proceed with Device Erase ? (y/n): y

Erasing Device...

Erase Successful

Proceed with Device Programming ? (y/n): y

Programming Device...

Programming Successful

===================================
 Process Completed Successfully
===================================
```

---

## Important Note

While combining Intel HEX files, special records such as Extended Segment Address (ESA) records affect how subsequent addresses are interpreted.

For this reason, HEX files should be combined carefully while preserving the address information contained within the records.

Programming tools ultimately rely on the addresses encoded within the HEX records to determine where data is written in Flash.

---

## Reference

ARM Knowledge Article:

https://developer.arm.com/documentation/ka003292/1-0/

---

## Repository

Part of the PSoC 4S+ Peripheral Access Layer project.

Author: Raghu Rama
