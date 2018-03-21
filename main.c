#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    unsigned short int pixel[512][512][3];
    unsigned int w;
    unsigned int h;
} Image;


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int samePixel (Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}

Image greyScale(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2];
            media /= 3;
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }
    return img;
}

Image rotate90(Image img) {
  Image rotation;

  rotation.w = img.h;
  rotation.h = img.w;

  for (unsigned int i = 0, y = 0; i < rotation.h; ++i, ++y) {
    for (int j = rotation.w - 1, x = 0; j >= 0; --j, ++x) {
      rotation.pixel[i][j][0] = img.pixel[x][y][0];
      rotation.pixel[i][j][1] = img.pixel[x][y][1];
      rotation.pixel[i][j][2] = img.pixel[x][y][2];
    }
  }
  return rotation;
}

Image cropImage(Image img, int x, int y, int w, int h) {
    Image cropped;

    cropped.w = w;
    cropped.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cropped.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cropped.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cropped.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }
    return cropped;
}

void blur(unsigned int h, unsigned short int pixel[512][512][3], int T, unsigned int w) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (h - 1 > i + T/2) ? i + T/2 : h - 1;
            int min_w = (w - 1 > j + T/2) ? j + T/2 : w - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.r += pixel[x][y][0];
                    media.g += pixel[x][y][1];
                    media.b += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}

void colorInversion(unsigned short int pixel[512][512][3],
                    unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int nOptions;
    scanf("%d", &nOptions);

    for(int i = 0; i < nOptions; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                img = greyScale(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.h; ++x) {
                    for (unsigned int j = 0; j < img.w; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = img.pixel[x][j][0];
                        pixel[1] = img.pixel[x][j][1];
                        pixel[2] = img.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][0] = menor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][1] = menor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][2] = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                blur(img.h, img.pixel, size, img.w);
                break;
            }
            case 4: { // Rotacao
                int numRotations = 0;
                scanf("%d", &numRotations);
                numRotations %= 4;
                for (int j = 0; j < numRotations; ++j) {
                    img = rotate90(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.w, h = img.h;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.w - 1 - j;
                        else x = img.h - 1 - i2;

                        Pixel aux1;
                        aux1.r = img.pixel[i2][j][0];
                        aux1.g = img.pixel[i2][j][1];
                        aux1.b = img.pixel[i2][j][2];

                        img.pixel[i2][j][0] = img.pixel[x][y][0];
                        img.pixel[i2][j][1] = img.pixel[x][y][1];
                        img.pixel[i2][j][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.r;
                        img.pixel[x][y][1] = aux1.g;
                        img.pixel[x][y][2] = aux1.b;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                colorInversion(img.pixel, img.w, img.h);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = cropImage(img, x, y, w, h);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
