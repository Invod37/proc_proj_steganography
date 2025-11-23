The current version contains a fully functional project application.

1. The directory structure has been simplified—all libraries and header files are now located in the src directory.
2. The steg_utils library has been created based on the latest versions of individual encoding and decoding applications, and the steg_encode and steg_decode libraries have been redesigned.
3. Wrapper functions steg_encode and steg_decode have been created, each with file-type parameters, for easy connection to the CLI shell.
4. A Makefile has been created.