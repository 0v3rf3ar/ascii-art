#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

const char *ASCII_CHARS = " .:-=+*M%@";

char grayscaleToAscii(int gray) {
    int numChars = 10;
    int index = (gray * (numChars - 1)) / 255;
    return ASCII_CHARS[index];
}

int rgbToGray(unsigned char r, unsigned char g, unsigned char b) {
    return (int)(0.299*r + 0.587*g + 0.114*b);
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Usage: %s <image_path> <block_width>x<block_height>\n", argv[0]);
        printf("Example: %s picture.png 4x6\n", argv[0]);
        return 1;
    }

    const char *imagePath = argv[1];
    int blockWidth = 2, blockHeight = 2;

    if(sscanf(argv[2], "%dx%d", &blockWidth, &blockHeight) != 2) {
        printf("Invalid block size. Using default 2x2.\n");
        blockWidth = blockHeight = 2;
    }

    int width, height, channels;
    unsigned char *img = stbi_load(imagePath, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Failed to load image: %s\n", imagePath);
        return 1;
    }

    if(channels < 3) {
        printf("Image does not have enough color channels.\n");
        stbi_image_free(img);
        return 1;
    }

    for(int y = 0; y < height; y += blockHeight) {
        for(int x = 0; x < width; x += blockWidth) {
            int sumGray = 0;
            int sumAlpha = 0;
            int pixelsCounted = 0;

            for(int by = 0; by < blockHeight; by++) {
                for(int bx = 0; bx < blockWidth; bx++) {
                    int px = x + bx;
                    int py = y + by;
                    if(px < width && py < height) {
                        unsigned char *pixel = img + (py * width + px) * channels;
                        int r = pixel[0];
                        int g = pixel[1];
                        int b = pixel[2];
                        int a = (channels == 4) ? pixel[3] : 255; 
                        sumGray += rgbToGray(r, g, b);
                        sumAlpha += a;
                        pixelsCounted++;
                    }
                }
            }

            int avgGray = sumGray / pixelsCounted;
            int avgAlpha = sumAlpha / pixelsCounted;

            if(avgAlpha == 0) {
                printf(" "); // fully transparent block
            } else {
                char c = grayscaleToAscii(avgGray);
                printf("%c", c);
            }
        }
        printf("\n");
    }

    stbi_image_free(img);
    return 0;
}

