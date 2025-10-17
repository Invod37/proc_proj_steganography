# proc_proj_steganography

## Project Name
Steganography in BMP

## Short Description
This project is written in C and demonstrates the concept of steganography.  
The program allows hiding a text message inside a BMP image and then extracting it back.  
The goal is to study basic information security methods and show how data can be concealed within digital media.

## Project Modules
### Encoder (`steg_encode.c`)
**Purpose:** hides a text message inside a BMP image.  
**Functionality:**  
• Reads the input BMP file.  
• Reads the text message (for example, from `message.txt`).  
• Embeds the message bits into the least significant bits (LSB) of the image pixels.  
• Saves the new image file `encoded.bmp` containing the hidden message.  

### Decoder (`steg_decode.c`)
**Purpose:** extracts the hidden message from a BMP image.  
**Functionality:**  
• Reads the input BMP file (encoded image `encoded.bmp`).  
• Extracts message bits from the least significant bits of the pixels.  
• Restores the hidden text message.  
• Saves the result to an output file (for example, `decoded_message.txt`).  

### Main & Tests (`main.c`, `tests/tests.c`)
**Purpose:** coordinates program execution, processes arguments, and performs testing.  
**Functionality:**  
- Handles errors and validates arguments.  
- Calls the functions `encode()` and `decode()`.  
- Contains unit tests for functionality verification.  
- Includes a Makefile for automated build and testing.  
- Provides a command-line interface (CLI):  
  ```bash
  ./steg encode input.bmp message.txt encoded.bmp  
  ./steg decode encoded.bmp output.txt



### Structure
<img width="698" height="1280" alt="image" src="https://github.com/user-attachments/assets/5133036c-1ab3-4867-83fd-e15510cec860" />




## Third-Party Libraries

- `<stdio.h>` — file reading/writing.  
- `<stdlib.h>` — memory management.  
- `<string.h>` — string operations.  
- `<stdint.h>` — precise byte/bit-level types.  
- *(Optional)* `<assert.h>` — for simple tests.  



## Team Members
- Dudinov Oleh
- Liashko Yehor
- Volynchuk Pavlo
