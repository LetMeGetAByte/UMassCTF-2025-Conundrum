# UMassCTF 2025 Conundrum
### How to build (Only tested on linux)
gcc -o pack pack.c

gcc -o chall chall.c

./pack chall chall_enc

make

### Hardening Concepts and Documentation
This project was born from a concept I have been working on for several months now. The idea revolves around encrypted binary payloads that are managed by a launcher — in this case, another binary. The goal I am trying to achieve is the obfuscation of execution flow and decreasing the effectiveness of both static and dynamic analysis.

For this release, there are two binaries: a launcher and a payload. The launcher verifies the SHA-256 sum of the payload, making sure it has not been tampered with, and also verifies that it is not being debugged. The payload is encrypted with a basic XOR encryption, and the key is generated using a byte stream generator. The payload itself also checks if it is being debugged, but unlike the launcher, it does not terminate execution. Instead, the payload outputs the encrypted flag.

As for the functionality of both, the launcher essentially performs its integrity checks, decrypts the file with a hardcoded key (not recommended), and places it in the temp directory before launching it with `execl`. Then, the payload performs its integrity checks, loads the flag into a buffer, and redirects STDOUT to the write end of a pipe before using `puts` to store either the decrypted or encrypted flag in the pipe.

Before release, all the binaries were also stripped, and the launcher was packed with its UPX strings removed.

### Vulnerabilities and Shortcomings
The intended analysis route for this challenge was to find and disable all integrity checks, obtain the decrypted binary payload, and analyze the payload to disable the integrity checks. Doing this would have allowed for GDB to be used to trivially find the flag in memory.

However, after release, I realized that it would be trivial to reverse-engineer the byte generator used to encrypt the flag. This meant that to solve the challenge, you could rewrite the generator in another script and generate all possible keys for the flag. The seed for the key is only one byte, so this should have been fairly easy and quick.

Of course, I could have solved this by releasing the challenge as intended, with everything stored server-side. However, I was pressed for time and so opted to release it all as statically served files.

### Final Words
I have many more ideas for how to improve this concept in future challenges, so keep an eye out. Hopefully, I will have the opportunity to author another challenge of much higher quality than this one. I have a lot of ideas for how to solve different challenges I came across, but I will keep those to myself for the sake of any future releases.

Thank you to everyone who tried the challenge. I am sorry for any issues you might have experienced with it. I will do my best to improve future releases to be more consistent.
